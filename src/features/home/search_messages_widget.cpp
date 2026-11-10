#include "features/home/search_messages_widget.hpp"
#include "app/application.hpp"
#include "protocol/protocol_manager.hpp"
#include "ui/components/loading_spinner.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <QShortcut>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QDateTime>
#include <QFont>
#include <QSettings>
#include <QRegularExpression>
#include <QApplication>
#include <QKeySequence>

static const char *kDarkStyleSheet = R"(
    SearchMessagesWidget {
        background-color: #1e1e2e;
    }
    SearchMessagesWidget QLineEdit {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 8px;
        padding: 6px 12px;
        font-size: 13px;
    }
    SearchMessagesWidget QLineEdit:focus {
        border-color: #89b4fa;
    }
    SearchMessagesWidget QComboBox {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 10px;
        font-size: 12px;
        min-width: 80px;
    }
    SearchMessagesWidget QComboBox::drop-down {
        border: none;
        width: 20px;
    }
    SearchMessagesWidget QComboBox QAbstractItemView {
        background-color: #313244;
        color: #cdd6f4;
        selection-background-color: #45475a;
        border: 1px solid #45475a;
    }
    SearchMessagesWidget QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 6px 16px;
        font-size: 12px;
    }
    SearchMessagesWidget QPushButton:hover {
        background-color: #45475a;
        border-color: #89b4fa;
    }
    SearchMessagesWidget QListWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        border: none;
        outline: none;
    }
    SearchMessagesWidget QListWidget::item {
        background-color: #1e1e2e;
        border-bottom: 1px solid #313244;
        padding: 6px 12px;
    }
    SearchMessagesWidget QListWidget::item:hover {
        background-color: #2a2a3c;
    }
    SearchMessagesWidget QListWidget::item:selected {
        background-color: #45475a;
    }
    SearchMessagesWidget QLabel {
        background: transparent;
    }
)";

static const char *kRecentItemStyle = R"(
    QListWidget::item {
        padding: 4px 12px;
        border-bottom: none;
    }
)";

SearchMessagesWidget::SearchMessagesWidget(QWidget *parent)
    : QWidget(parent)
{
    setStyleSheet(QString::fromLatin1(kDarkStyleSheet));
    setupUi();

    m_debounceTimer = new QTimer(this);
    m_debounceTimer->setSingleShot(true);
    m_debounceTimer->setInterval(kDebounceMs);
    connect(m_debounceTimer, &QTimer::timeout,
            this, &SearchMessagesWidget::onDebounceTimeout);

    m_shortcut = new QShortcut(QKeySequence(QStringLiteral("Ctrl+F")), this);
    m_shortcut->setContext(Qt::ApplicationShortcut);
    connect(m_shortcut, &QShortcut::activated,
            this, &SearchMessagesWidget::openAndFocus);

    showRecentSearchesPanel(true);
    showNoResultsPlaceholder(false);
    showSearchingIndicator(false);
}

SearchMessagesWidget::~SearchMessagesWidget() = default;

void SearchMessagesWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // ── Top bar: search edit + protocol filter ──
    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(8, 8, 8, 4);
    topBar->setSpacing(6);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search messages..."));
    m_searchEdit->setClearButtonEnabled(true);
    topBar->addWidget(m_searchEdit, 1);

    m_protocolFilter = new QComboBox(this);
    m_protocolFilter->addItem(tr("All"));
    m_protocolFilter->addItem(tr("Matrix"));
    m_protocolFilter->addItem(tr("IRC"));
    m_protocolFilter->addItem(tr("Lemmy"));
    topBar->addWidget(m_protocolFilter);

    mainLayout->addLayout(topBar);

    // ── Room filter row ──
    auto *roomBar = new QHBoxLayout();
    roomBar->setContentsMargins(8, 0, 8, 4);
    roomBar->setSpacing(6);

    m_roomFilter = new QComboBox(this);
    m_roomFilter->setMinimumWidth(120);
    m_roomFilter->addItem(tr("All rooms"));
    m_roomFilterIds.append(QString());
    roomBar->addWidget(m_roomFilter, 1);

    mainLayout->addLayout(roomBar);

    // ── Searching indicator ──
    m_searchingContainer = new QWidget(this);
    auto *searchingLayout = new QHBoxLayout(m_searchingContainer);
    searchingLayout->setContentsMargins(16, 12, 16, 12);
    searchingLayout->setSpacing(8);

    m_spinner = new LoadingSpinner(20, QColor(0x89, 0xb4, 0xfa), m_searchingContainer);
    m_spinner->start();
    searchingLayout->addWidget(m_spinner);

    m_searchingLabel = new QLabel(tr("Searching..."), m_searchingContainer);
    m_searchingLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 13px; background: transparent;"));
    searchingLayout->addWidget(m_searchingLabel);
    searchingLayout->addStretch();

    m_searchingContainer->setLayout(searchingLayout);
    m_searchingContainer->hide();
    mainLayout->addWidget(m_searchingContainer);

    // ── No results placeholder ──
    m_noResultsLabel = new QLabel(tr("No results found"), this);
    m_noResultsLabel->setAlignment(Qt::AlignCenter);
    m_noResultsLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 14px; padding: 32px; background: transparent;"));
    m_noResultsLabel->hide();
    mainLayout->addWidget(m_noResultsLabel);

    // ── Recent searches panel ──
    m_recentLabel = new QLabel(tr("Recent searches"), this);
    m_recentLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; font-weight: bold; "
        "padding: 4px 12px; background: transparent;"));
    mainLayout->addWidget(m_recentLabel);

    m_recentList = new QListWidget(this);
    m_recentList->setStyleSheet(QString::fromLatin1(kRecentItemStyle));
    m_recentList->setMaximumHeight(10 * 28);
    m_recentList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_recentList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    mainLayout->addWidget(m_recentList);

    // ── Results list ──
    m_resultsList = new QListWidget(this);
    m_resultsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_resultsList->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(m_resultsList, 1);

    // ── Load more button ──
    m_loadMoreButton = new QPushButton(tr("Load more results"), this);
    m_loadMoreButton->setFlat(true);
    m_loadMoreButton->setStyleSheet(QStringLiteral(
        "QPushButton { color: #89b4fa; border: none; padding: 8px; "
        "font-size: 12px; background: transparent; } "
        "QPushButton:hover { color: #b4d0fb; }"));
    m_loadMoreButton->hide();
    mainLayout->addWidget(m_loadMoreButton);

    // ── Connections ──
    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &SearchMessagesWidget::onSearchTextChanged);
    connect(m_searchEdit, &QLineEdit::returnPressed,
            this, &SearchMessagesWidget::onSearchReturnPressed);
    connect(m_protocolFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SearchMessagesWidget::onProtocolFilterChanged);
    connect(m_roomFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &SearchMessagesWidget::onRoomFilterChanged);
    connect(m_resultsList, &QListWidget::itemClicked,
            this, &SearchMessagesWidget::onResultClicked);
    connect(m_loadMoreButton, &QPushButton::clicked,
            this, &SearchMessagesWidget::onLoadMoreClicked);
    connect(m_recentList, &QListWidget::itemClicked,
            this, &SearchMessagesWidget::onRecentSearchClicked);
}

// ── Public slots ──

void SearchMessagesWidget::openAndFocus()
{
    show();
    raise();
    m_searchEdit->setFocus();
    m_searchEdit->selectAll();

    if (m_searchEdit->text().isEmpty())
        showRecentSearchesPanel(true);
}

// ── Private slots ──

void SearchMessagesWidget::onSearchTextChanged()
{
    const QString text = m_searchEdit->text().trimmed();
    if (text.isEmpty()) {
        m_debounceTimer->stop();
        clearResults();
        showNoResultsPlaceholder(false);
        showSearchingIndicator(false);
        showRecentSearchesPanel(true);
        return;
    }
    m_debounceTimer->start(kDebounceMs);
}

void SearchMessagesWidget::onSearchReturnPressed()
{
    m_debounceTimer->stop();
    triggerSearch();
}

void SearchMessagesWidget::onDebounceTimeout()
{
    triggerSearch();
}

void SearchMessagesWidget::onProtocolFilterChanged(int index)
{
    switch (index) {
    case 0: m_currentProtocolFilter.reset();       break;
    case 1: m_currentProtocolFilter = ProtocolType::MATRIX; break;
    case 2: m_currentProtocolFilter = ProtocolType::IRC;    break;
    case 3: m_currentProtocolFilter = ProtocolType::LEMMY;  break;
    default: m_currentProtocolFilter.reset(); break;
    }

    updateRoomFilter();

    if (!m_currentQuery.isEmpty())
        triggerSearch();
}

void SearchMessagesWidget::onRoomFilterChanged(int index)
{
    if (index >= 0 && index < m_roomFilterIds.size())
        m_currentRoomFilter = m_roomFilterIds.at(index);
    else
        m_currentRoomFilter.clear();

    if (!m_currentQuery.isEmpty())
        triggerSearch();
}

void SearchMessagesWidget::onResultClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    const QString roomId = item->data(static_cast<int>(RoomIdRole)).toString();
    const QString messageId = item->data(static_cast<int>(MessageIdRole)).toString();
    if (!roomId.isEmpty())
        emit resultSelected(roomId, messageId);
}

void SearchMessagesWidget::onLoadMoreClicked()
{
    displayResultsPage();
}

void SearchMessagesWidget::onRecentSearchClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    const QString query = item->text();
    m_searchEdit->setText(query);
    triggerSearch();
}

// ── Search logic ──

void SearchMessagesWidget::triggerSearch()
{
    m_currentQuery = m_searchEdit->text().trimmed();

    if (m_currentQuery.isEmpty()) {
        clearResults();
        showNoResultsPlaceholder(false);
        showSearchingIndicator(false);
        showRecentSearchesPanel(true);
        return;
    }

    saveRecentSearch(m_currentQuery);
    refreshRecentSearchesList();
    showRecentSearchesPanel(false);
    performSearch();
}

void SearchMessagesWidget::performSearch()
{
    clearResults();
    showNoResultsPlaceholder(false);
    showSearchingIndicator(true);

    m_allResults.clear();
    m_displayedCount = 0;

    auto *pm = pcp::Application::instance().protocolManager();
    if (!pm) {
        showSearchingIndicator(false);
        showNoResultsPlaceholder(true);
        return;
    }

    const auto allRooms = pm->getAllRooms();

    for (const auto &room : allRooms) {
        if (m_currentProtocolFilter.has_value() &&
            room.protocolType != m_currentProtocolFilter.value())
            continue;

        if (!m_currentRoomFilter.isEmpty() && room.id != m_currentRoomFilter)
            continue;

        auto messages = pm->getAllMessages(room.id);
        if (messages.size() > kMessagesPerRoom)
            messages = messages.mid(messages.size() - kMessagesPerRoom);

        for (const auto &msg : messages) {
            if (msg.text.contains(m_currentQuery, Qt::CaseInsensitive))
                m_allResults.append({msg, room.name});
        }
    }

    std::sort(m_allResults.begin(), m_allResults.end(),
              [](const SearchResult &a, const SearchResult &b) {
                  return a.message.timestamp > b.message.timestamp;
              });

    showSearchingIndicator(false);

    if (m_allResults.isEmpty()) {
        showNoResultsPlaceholder(true);
    } else {
        displayResultsPage();
    }
}

void SearchMessagesWidget::displayResultsPage()
{
    const int start = m_displayedCount;
    const int end = std::min(start + kPageSize, static_cast<int>(m_allResults.size()));

    for (int i = start; i < end; ++i) {
        auto *item = new QListWidgetItem();
        item->setData(static_cast<int>(RoomIdRole), m_allResults[i].message.roomId);
        item->setData(static_cast<int>(MessageIdRole),
                      QString::number(m_allResults[i].message.id));
        item->setSizeHint(QSize(0, 60));
        m_resultsList->addItem(item);
        m_resultsList->setItemWidget(item, createResultItemWidget(m_allResults[i]));
    }

    m_displayedCount = end;
    m_loadMoreButton->setVisible(m_displayedCount < static_cast<int>(m_allResults.size()));
}

void SearchMessagesWidget::clearResults()
{
    m_resultsList->clear();
    m_allResults.clear();
    m_displayedCount = 0;
    m_loadMoreButton->hide();
}

void SearchMessagesWidget::updateRoomFilter()
{
    const QString previousRoomId = m_roomFilterIds.value(m_roomFilter->currentIndex());

    m_roomFilter->blockSignals(true);
    m_roomFilter->clear();
    m_roomFilterIds.clear();

    m_roomFilter->addItem(tr("All rooms"));
    m_roomFilterIds.append(QString());

    auto *pm = pcp::Application::instance().protocolManager();
    if (pm) {
        const auto allRooms = pm->getAllRooms();
        for (const auto &room : allRooms) {
            if (m_currentProtocolFilter.has_value() &&
                room.protocolType != m_currentProtocolFilter.value())
                continue;

            m_roomFilter->addItem(room.name);
            m_roomFilterIds.append(room.id);
        }
    }

    int restoreIdx = 0;
    if (!previousRoomId.isEmpty()) {
        for (int i = 0; i < m_roomFilterIds.size(); ++i) {
            if (m_roomFilterIds[i] == previousRoomId) {
                restoreIdx = i;
                break;
            }
        }
    }
    m_roomFilter->setCurrentIndex(restoreIdx);
    m_currentRoomFilter = m_roomFilterIds.value(restoreIdx);

    m_roomFilter->blockSignals(false);
}

// ── Visibility helpers ──

void SearchMessagesWidget::showNoResultsPlaceholder(bool show)
{
    m_noResultsLabel->setVisible(show);
    m_resultsList->setVisible(!show);
}

void SearchMessagesWidget::showSearchingIndicator(bool show)
{
    m_searchingContainer->setVisible(show);
    if (show)
        m_spinner->start();
    else
        m_spinner->stop();
}

void SearchMessagesWidget::showRecentSearchesPanel(bool show)
{
    m_recentLabel->setVisible(show);
    m_recentList->setVisible(show);
    m_resultsList->setVisible(!show);
    m_loadMoreButton->setVisible(false);

    if (show)
        refreshRecentSearchesList();
}

// ── Recent searches ──

void SearchMessagesWidget::refreshRecentSearchesList()
{
    m_recentList->clear();

    const QStringList recents = loadRecentSearches();
    for (const QString &q : recents) {
        auto *item = new QListWidgetItem(q);
        item->setSizeHint(QSize(0, 28));
        m_recentList->addItem(item);
    }
}

void SearchMessagesWidget::saveRecentSearch(const QString &query)
{
    if (query.isEmpty())
        return;

    QSettings settings;
    QStringList recents = settings.value(
        QStringLiteral("SearchMessagesWidget/recentSearches")).toStringList();

    recents.removeAll(query);
    recents.prepend(query);

    while (recents.size() > kMaxRecentSearches)
        recents.removeLast();

    settings.setValue(QStringLiteral("SearchMessagesWidget/recentSearches"), recents);
}

QStringList SearchMessagesWidget::loadRecentSearches() const
{
    QSettings settings;
    return settings.value(
        QStringLiteral("SearchMessagesWidget/recentSearches")).toStringList();
}

// ── Result item rendering ──

QWidget *SearchMessagesWidget::createResultItemWidget(const SearchResult &result)
{
    auto *widget = new QWidget();
    widget->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *row = new QHBoxLayout(widget);
    row->setContentsMargins(4, 4, 8, 4);
    row->setSpacing(8);

    // Avatar
    constexpr int kAvatarSize = 28;
    QColor bgColor = protocolColor(result.message.protocolType);
    QPixmap avatar = makeAvatarPixmap(result.message.senderName, kAvatarSize, bgColor);

    auto *avatarLabel = new QLabel();
    avatarLabel->setFixedSize(kAvatarSize, kAvatarSize);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setAlignment(Qt::AlignCenter);
    row->addWidget(avatarLabel);

    // Center: sender + room name, then preview
    auto *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(2);

    QString headerText = QStringLiteral(
        "<span style='color: #cdd6f4; font-weight: bold; font-size: 12px;'>%1</span>"
        "<span style='color: #6c7086; font-size: 11px;'> &mdash; %2</span>")
        .arg(result.message.senderName.toHtmlEscaped(),
             result.roomName.toHtmlEscaped());

    auto *headerLabel = new QLabel(headerText);
    headerLabel->setTextFormat(Qt::RichText);
    headerLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    centerLayout->addWidget(headerLabel);

    QString previewText = result.message.text;
    if (previewText.length() > 120)
        previewText = previewText.left(117) + QStringLiteral("...");

    QString highlighted = highlightMatch(previewText);

    auto *previewLabel = new QLabel(highlighted);
    previewLabel->setTextFormat(Qt::RichText);
    previewLabel->setWordWrap(true);
    previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    previewLabel->setStyleSheet(QStringLiteral(
        "color: #a6adc8; font-size: 11px; background: transparent;"));
    centerLayout->addWidget(previewLabel);

    row->addLayout(centerLayout, 1);

    // Timestamp
    auto *timeLabel = new QLabel(formatTimestamp(result.message.timestamp));
    timeLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 10px; background: transparent;"));
    timeLabel->setAlignment(Qt::AlignRight | Qt::AlignTop);
    row->addWidget(timeLabel);

    widget->setLayout(row);
    return widget;
}

QString SearchMessagesWidget::highlightMatch(const QString &text) const
{
    if (m_currentQuery.isEmpty())
        return text.toHtmlEscaped();

    QString escapedText = text.toHtmlEscaped();
    QString escapedQuery = QRegularExpression::escape(m_currentQuery);

    QRegularExpression re(escapedQuery, QRegularExpression::CaseInsensitiveOption);
    escapedText.replace(re, QStringLiteral(
        "<b style='background-color: #f9e2af; color: #1e1e2e;'>\\0</b>"));

    return escapedText;
}

// ── Static helpers ──

QColor SearchMessagesWidget::protocolColor(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX: return QColor(0x89, 0xb4, 0xfa);
    case ProtocolType::IRC:    return QColor(0xa6, 0xe3, 0xa1);
    case ProtocolType::LEMMY:  return QColor(0xfa, 0xb3, 0x87);
    default:                   return QColor(0x6c, 0x70, 0x86);
    }
}

QString SearchMessagesWidget::protocolName(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX: return QStringLiteral("Matrix");
    case ProtocolType::IRC:    return QStringLiteral("IRC");
    case ProtocolType::LEMMY:  return QStringLiteral("Lemmy");
    default:                   return QStringLiteral("Unknown");
    }
}

QString SearchMessagesWidget::formatTimestamp(qint64 timestampMs)
{
    if (timestampMs <= 0)
        return {};

    QDateTime dt = QDateTime::fromMSecsSinceEpoch(timestampMs);
    QDateTime now = QDateTime::currentDateTime();

    if (dt.date() == now.date())
        return dt.toString(QStringLiteral("HH:mm"));

    if (dt.date().year() == now.date().year())
        return dt.toString(QStringLiteral("MMM d"));

    return dt.toString(QStringLiteral("yyyy-MM-dd"));
}

QPixmap SearchMessagesWidget::makeAvatarPixmap(const QString &name, int size,
                                                const QColor &bgColor)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect circleRect(0, 0, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawEllipse(circleRect);

    QString letters = initialsFromName(name);

    QFont font;
    font.setPixelSize(size / 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(circleRect, Qt::AlignCenter, letters);

    painter.end();
    return pix;
}

QString SearchMessagesWidget::initialsFromName(const QString &name)
{
    if (name.isEmpty())
        return QStringLiteral("?");

    QStringList parts = name.split(QChar::Space, Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return QStringLiteral("?");

    QString result;
    result += parts.first().at(0).toUpper();
    if (parts.size() >= 2)
        result += parts.last().at(0).toUpper();

    return result;
}
