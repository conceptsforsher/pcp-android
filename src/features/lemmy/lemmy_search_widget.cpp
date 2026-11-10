#include "features/lemmy/lemmy_search_widget.hpp"
#include "app/application.hpp"
#include "lemmy/lemmy_session.hpp"
#include "lemmy/lemmy_session_holder.hpp"
#include "lemmy/lemmy_client.hpp"

#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QProgressBar>
#include <QLabel>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPainter>
#include <QPixmap>
#include <QDateTime>
#include <QFont>
#include <QJsonArray>
#include <QJsonValue>

static const char *kLemmySearchStyleSheet = R"(
    LemmySearchWidget {
        background-color: #1e1e2e;
    }
    LemmySearchWidget QLineEdit {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 8px;
        padding: 6px 12px;
        font-size: 13px;
    }
    LemmySearchWidget QLineEdit:focus {
        border-color: #cba6f7;
    }
    LemmySearchWidget QComboBox {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 10px;
        font-size: 12px;
        min-width: 80px;
    }
    LemmySearchWidget QComboBox::drop-down {
        border: none;
        width: 20px;
    }
    LemmySearchWidget QComboBox QAbstractItemView {
        background-color: #313244;
        color: #cdd6f4;
        selection-background-color: #45475a;
        border: 1px solid #45475a;
    }
    LemmySearchWidget QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 12px;
        font-size: 12px;
    }
    LemmySearchWidget QPushButton:hover {
        background-color: #45475a;
        border-color: #89b4fa;
    }
    LemmySearchWidget QPushButton#loadMoreBtn {
        background-color: #313244;
        color: #89b4fa;
        border: none;
        border-radius: 0px;
        padding: 8px;
        font-size: 12px;
    }
    LemmySearchWidget QPushButton#loadMoreBtn:hover {
        color: #b4d9ff;
        background-color: #2a2a3c;
    }
    LemmySearchWidget QListWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        border: none;
        outline: none;
    }
    LemmySearchWidget QListWidget::item {
        background-color: #1e1e2e;
        border-bottom: 1px solid #313244;
        padding: 0px;
    }
    LemmySearchWidget QListWidget::item:hover {
        background-color: #2a2a3c;
    }
    LemmySearchWidget QListWidget::item:selected {
        background-color: #2a2a3c;
    }
    LemmySearchWidget QProgressBar {
        background-color: #313244;
        border: none;
        border-radius: 4px;
        height: 4px;
        text-align: center;
        font-size: 11px;
        color: #6c7086;
    }
    LemmySearchWidget QProgressBar::chunk {
        background-color: #cba6f7;
        border-radius: 4px;
    }
)";

static QString relativeTime(qint64 unixSec)
{
    if (unixSec <= 0)
        return QStringLiteral("just now");

    qint64 now = QDateTime::currentSecsSinceEpoch();
    qint64 diff = now - unixSec;

    if (diff < 0) diff = 0;
    if (diff < 60)       return QStringLiteral("just now");
    if (diff < 3600)     return QStringLiteral("%1m").arg(diff / 60);
    if (diff < 86400)    return QStringLiteral("%1h").arg(diff / 3600);
    if (diff < 2592000)  return QStringLiteral("%1d").arg(diff / 86400);
    if (diff < 31536000) return QStringLiteral("%1mo").arg(diff / 2592000);

    return QStringLiteral("%1y").arg(diff / 31536000);
}

static qint64 parseTimestampSec(const QString &publishedStr)
{
    if (publishedStr.isEmpty())
        return 0;

    QDateTime dt = QDateTime::fromString(publishedStr, Qt::ISODate);
    if (!dt.isValid())
        dt = QDateTime::fromString(publishedStr, Qt::ISODateWithMs);

    return dt.isValid() ? dt.toSecsSinceEpoch() : 0;
}

static QPixmap makeAvatarPixmap(const QString &name, int size, const QColor &bg)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r(0, 0, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bg);
    painter.drawRoundedRect(r, size / 4, size / 4);

    QString letter;
    if (!name.isEmpty())
        letter = name.at(0).toUpper();
    else
        letter = QStringLiteral("?");

    QFont font;
    font.setPixelSize(size / 2 + 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(r, Qt::AlignCenter, letter);

    painter.end();
    return pix;
}

LemmySearchWidget::LemmySearchWidget(QWidget *parent)
    : QWidget(parent)
    , m_searchBar(nullptr)
    , m_typeCombo(nullptr)
    , m_sortCombo(nullptr)
    , m_resultsList(nullptr)
    , m_loadMoreButton(nullptr)
    , m_loadingBar(nullptr)
    , m_emptyLabel(nullptr)
    , m_debounceTimer(nullptr)
    , m_currentPage(1)
    , m_hasMorePages(false)
{
    m_currentSearchType = QStringLiteral("All");
    setStyleSheet(QString::fromLatin1(kLemmySearchStyleSheet));
    setupUi();
}

LemmySearchWidget::~LemmySearchWidget() = default;

void LemmySearchWidget::setInstanceUrl(const QString &url)
{
    m_instanceUrl = url;
}

void LemmySearchWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(8, 8, 8, 4);
    topBar->setSpacing(6);

    m_searchBar = new QLineEdit(this);
    m_searchBar->setPlaceholderText(tr("Search Lemmy..."));
    m_searchBar->setClearButtonEnabled(true);
    m_searchBar->setMinimumHeight(32);
    topBar->addWidget(m_searchBar, 2);

    m_typeCombo = new QComboBox(this);
    m_typeCombo->addItem(tr("All"),       QStringLiteral("All"));
    m_typeCombo->addItem(tr("Posts"),     QStringLiteral("Posts"));
    m_typeCombo->addItem(tr("Communities"), QStringLiteral("Communities"));
    m_typeCombo->addItem(tr("Users"),     QStringLiteral("Users"));
    m_typeCombo->addItem(tr("Comments"),  QStringLiteral("Comments"));
    m_typeCombo->setCurrentIndex(0);
    topBar->addWidget(m_typeCombo);

    m_sortCombo = new QComboBox(this);
    m_sortCombo->addItem(tr("Top All"),   QStringLiteral("TopAll"));
    m_sortCombo->addItem(tr("Top Year"),  QStringLiteral("TopYear"));
    m_sortCombo->addItem(tr("Top Month"), QStringLiteral("TopMonth"));
    m_sortCombo->addItem(tr("Top Week"),  QStringLiteral("TopWeek"));
    m_sortCombo->addItem(tr("Top Day"),   QStringLiteral("TopDay"));
    m_sortCombo->addItem(tr("Active"),    QStringLiteral("Active"));
    m_sortCombo->addItem(tr("New"),       QStringLiteral("New"));
    m_sortCombo->addItem(tr("Hot"),       QStringLiteral("Hot"));
    m_sortCombo->setCurrentIndex(0);
    topBar->addWidget(m_sortCombo);

    mainLayout->addLayout(topBar);

    m_loadingBar = new QProgressBar(this);
    m_loadingBar->setRange(0, 0);
    m_loadingBar->setFixedHeight(4);
    m_loadingBar->setVisible(false);
    mainLayout->addWidget(m_loadingBar);

    m_emptyLabel = new QLabel(tr("Search for posts, communities, users, and more"), this);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 14px; padding: 40px;"));
    mainLayout->addWidget(m_emptyLabel);

    m_resultsList = new QListWidget(this);
    m_resultsList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_resultsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_resultsList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_resultsList->setSpacing(4);
    m_resultsList->setVisible(false);
    mainLayout->addWidget(m_resultsList, 1);

    m_loadMoreButton = new QPushButton(tr("Load More"), this);
    m_loadMoreButton->setObjectName(QStringLiteral("loadMoreBtn"));
    m_loadMoreButton->setCursor(Qt::PointingHandCursor);
    m_loadMoreButton->setVisible(false);
    mainLayout->addWidget(m_loadMoreButton);

    m_debounceTimer = new QTimer(this);
    m_debounceTimer->setSingleShot(true);
    m_debounceTimer->setInterval(350);

    connect(m_searchBar, &QLineEdit::returnPressed,
            this, &LemmySearchWidget::onSearchTriggered);
    connect(m_searchBar, &QLineEdit::textChanged,
            this, &LemmySearchWidget::onSearchTextChanged);
    connect(m_typeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [this]() {
        m_currentSearchType = m_typeCombo->currentData().toString();
        if (!m_currentSearchQuery.isEmpty()) {
            performSearch(1);
        }
    });
    connect(m_resultsList, &QListWidget::itemActivated,
            this, &LemmySearchWidget::onResultItemActivated);
    connect(m_loadMoreButton, &QPushButton::clicked,
            this, &LemmySearchWidget::onLoadMoreClicked);
    connect(m_debounceTimer, &QTimer::timeout,
            this, [this]() {
        if (!m_searchBar->text().trimmed().isEmpty()) {
            onSearchTriggered();
        }
    });
}

void LemmySearchWidget::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text)
    m_debounceTimer->start();
}

void LemmySearchWidget::onSearchTriggered()
{
    m_debounceTimer->stop();

    QString query = m_searchBar->text().trimmed();
    if (query.isEmpty()) {
        clearResults();
        m_emptyLabel->setText(tr("Search for posts, communities, users, and more"));
        m_emptyLabel->setVisible(true);
        m_resultsList->setVisible(false);
        m_loadMoreButton->setVisible(false);
        return;
    }

    m_currentSearchQuery = query;
    m_currentSearchType = m_typeCombo->currentData().toString();
    performSearch(1);
}

void LemmySearchWidget::performSearch(int page)
{
    if (m_instanceUrl.isEmpty() || m_currentSearchQuery.isEmpty())
        return;

    showLoading(true);

    auto *holder = pcp::Application::instance().lemmySessionHolder();
    if (!holder) {
        showLoading(false);
        return;
    }

    LemmySession *session = holder->getSession(m_instanceUrl);
    if (!session) {
        showLoading(false);
        return;
    }

    LemmyClient *client = session->findChild<LemmyClient *>(QString(), Qt::FindDirectChildrenOnly);
    if (!client) {
        showLoading(false);
        return;
    }

    QJsonObject resp = client->search(m_currentSearchQuery, m_currentSearchType, page, 25);

    if (page == 1) {
        m_results.clear();
    }

    QVector<LemmySearchResult> newResults = parseSearchResponse(resp);

    if (page == 1) {
        m_results = newResults;
        m_currentPage = 1;
    } else {
        m_results.append(newResults);
        m_currentPage = page;
    }

    m_hasMorePages = (newResults.size() >= 25);

    clearResults();

    if (m_results.isEmpty()) {
        m_emptyLabel->setText(tr("No results found for \"%1\"").arg(m_currentSearchQuery));
        m_emptyLabel->setVisible(true);
        m_resultsList->setVisible(false);
        m_loadMoreButton->setVisible(false);
    } else {
        m_emptyLabel->setVisible(false);
        m_resultsList->setVisible(true);

        for (int i = 0; i < m_results.size(); ++i) {
            const auto &res = m_results[i];
            auto *item = new QListWidgetItem();
            item->setData(static_cast<int>(ResultIndexRole), i);

            QWidget *card = nullptr;
            switch (res.type) {
            case LemmySearchResult::Post:
                item->setSizeHint(QSize(0, 110));
                card = createPostResultWidget(res);
                break;
            case LemmySearchResult::Community:
                item->setSizeHint(QSize(0, 60));
                card = createCommunityResultWidget(res);
                break;
            case LemmySearchResult::User:
                item->setSizeHint(QSize(0, 60));
                card = createUserResultWidget(res);
                break;
            case LemmySearchResult::Comment:
                item->setSizeHint(QSize(0, 80));
                card = createCommentResultWidget(res);
                break;
            default:
                continue;
            }

            m_resultsList->addItem(item);
            m_resultsList->setItemWidget(item, card);
        }

        setLoadMoreVisible(m_hasMorePages);
    }

    showLoading(false);
}

QVector<LemmySearchResult> LemmySearchWidget::parseSearchResponse(const QJsonObject &json) const
{
    QVector<LemmySearchResult> results;

    QString type = m_currentSearchType;

    auto parsePost = [](const QJsonObject &obj) -> LemmySearchResult {
        LemmySearchResult r;
        r.type = LemmySearchResult::Post;
        QJsonObject pv = obj.value(QStringLiteral("post")).toObject();
        r.id = static_cast<qint64>(pv.value(QStringLiteral("id")).toInt());
        r.title = pv.value(QStringLiteral("name")).toString();
        r.body = pv.value(QStringLiteral("body")).toString();
        r.score = static_cast<qint64>(pv.value(QStringLiteral("score")).toInt());
        r.commentCount = static_cast<qint64>(pv.value(QStringLiteral("comments")).toInt());
        r.published = pv.value(QStringLiteral("published")).toString();
        QJsonObject community = obj.value(QStringLiteral("community")).toObject();
        r.communityName = community.value(QStringLiteral("name")).toString();
        r.communityId = static_cast<qint64>(community.value(QStringLiteral("id")).toInt());
        QJsonObject creator = obj.value(QStringLiteral("creator")).toObject();
        r.userName = creator.value(QStringLiteral("name")).toString();
        r.userId = static_cast<qint64>(creator.value(QStringLiteral("id")).toInt());
        return r;
    };

    auto parseCommunity = [](const QJsonObject &obj) -> LemmySearchResult {
        LemmySearchResult r;
        r.type = LemmySearchResult::Community;
        QJsonObject cv = obj.value(QStringLiteral("community")).toObject();
        r.id = static_cast<qint64>(cv.value(QStringLiteral("id")).toInt());
        r.title = cv.value(QStringLiteral("title")).toString();
        r.communityName = cv.value(QStringLiteral("name")).toString();
        r.body = cv.value(QStringLiteral("description")).toString();
        r.published = cv.value(QStringLiteral("published")).toString();
        QJsonObject counts = obj.value(QStringLiteral("counts")).toObject();
        r.subscriberCount = static_cast<qint64>(counts.value(QStringLiteral("subscribers")).toInt());
        r.commentCount = static_cast<qint64>(counts.value(QStringLiteral("posts")).toInt());
        return r;
    };

    auto parseUser = [](const QJsonObject &obj) -> LemmySearchResult {
        LemmySearchResult r;
        r.type = LemmySearchResult::User;
        QJsonObject pv = obj.value(QStringLiteral("person")).toObject();
        r.id = static_cast<qint64>(pv.value(QStringLiteral("id")).toInt());
        r.userName = pv.value(QStringLiteral("name")).toString();
        r.title = pv.value(QStringLiteral("display_name")).toString();
        r.avatarUrl = pv.value(QStringLiteral("avatar")).toString();
        r.published = pv.value(QStringLiteral("published")).toString();
        QJsonObject counts = obj.value(QStringLiteral("counts")).toObject();
        r.commentCount = static_cast<qint64>(counts.value(QStringLiteral("comment_count")).toInt());
        r.score = static_cast<qint64>(counts.value(QStringLiteral("post_count")).toInt());
        return r;
    };

    auto parseComment = [](const QJsonObject &obj) -> LemmySearchResult {
        LemmySearchResult r;
        r.type = LemmySearchResult::Comment;
        QJsonObject cv = obj.value(QStringLiteral("comment")).toObject();
        r.communityId = static_cast<qint64>(cv.value(QStringLiteral("id")).toInt());
        r.body = cv.value(QStringLiteral("content")).toString();
        r.score = static_cast<qint64>(cv.value(QStringLiteral("score")).toInt());
        r.published = cv.value(QStringLiteral("published")).toString();
        QJsonObject post = obj.value(QStringLiteral("post")).toObject();
        r.id = static_cast<qint64>(post.value(QStringLiteral("id")).toInt());
        r.title = post.value(QStringLiteral("name")).toString();
        r.communityName = post.value(QStringLiteral("community_name")).toString();
        QJsonObject creator = obj.value(QStringLiteral("creator")).toObject();
        r.userName = creator.value(QStringLiteral("name")).toString();
        return r;
    };

    if (type == QStringLiteral("All")) {
        for (const QString &key : {QStringLiteral("posts"), QStringLiteral("communities"),
                                    QStringLiteral("users"), QStringLiteral("comments")}) {
            QJsonArray arr = json.value(key).toArray();
            for (const QJsonValue &val : arr) {
                QJsonObject obj = val.toObject();
                if (key == QStringLiteral("posts"))
                    results.append(parsePost(obj));
                else if (key == QStringLiteral("communities"))
                    results.append(parseCommunity(obj));
                else if (key == QStringLiteral("users"))
                    results.append(parseUser(obj));
                else
                    results.append(parseComment(obj));
            }
        }
    } else if (type == QStringLiteral("Posts")) {
        QJsonArray arr = json.value(QStringLiteral("posts")).toArray();
        for (const QJsonValue &val : arr)
            results.append(parsePost(val.toObject()));
    } else if (type == QStringLiteral("Communities")) {
        QJsonArray arr = json.value(QStringLiteral("communities")).toArray();
        for (const QJsonValue &val : arr)
            results.append(parseCommunity(val.toObject()));
    } else if (type == QStringLiteral("Users")) {
        QJsonArray arr = json.value(QStringLiteral("users")).toArray();
        for (const QJsonValue &val : arr)
            results.append(parseUser(val.toObject()));
    } else if (type == QStringLiteral("Comments")) {
        QJsonArray arr = json.value(QStringLiteral("comments")).toArray();
        for (const QJsonValue &val : arr)
            results.append(parseComment(val.toObject()));
    }

    return results;
}

void LemmySearchWidget::clearResults()
{
    m_resultsList->clear();
}

void LemmySearchWidget::showLoading(bool loading)
{
    m_loadingBar->setVisible(loading);
}

void LemmySearchWidget::setLoadMoreVisible(bool visible)
{
    m_loadMoreButton->setVisible(visible);
}

void LemmySearchWidget::onResultItemActivated(QListWidgetItem *item)
{
    if (!item)
        return;

    bool ok = false;
    int idx = item->data(static_cast<int>(ResultIndexRole)).toInt(&ok);
    if (!ok || idx < 0 || idx >= m_results.size())
        return;

    const LemmySearchResult &res = m_results[idx];

    switch (res.type) {
    case LemmySearchResult::Post:
        emit postSelected(res.id);
        break;
    case LemmySearchResult::Community:
        emit communitySelected(res.id);
        break;
    case LemmySearchResult::User:
        emit userSelected(res.id);
        break;
    case LemmySearchResult::Comment:
        emit postSelected(res.id);
        break;
    default:
        break;
    }
}

void LemmySearchWidget::onLoadMoreClicked()
{
    if (m_currentSearchQuery.isEmpty())
        return;
    performSearch(m_currentPage + 1);
}

QWidget *LemmySearchWidget::createPostResultWidget(const LemmySearchResult &res) const
{
    auto *container = new QWidget();
    container->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *card = new QVBoxLayout(container);
    card->setContentsMargins(12, 8, 12, 8);
    card->setSpacing(4);

    auto *headerRow = new QHBoxLayout();
    headerRow->setSpacing(6);

    auto *communityLabel = new QLabel(
        QStringLiteral("c/") + res.communityName, container);
    communityLabel->setStyleSheet(QStringLiteral(
        "color: #89b4fa; font-weight: bold; font-size: 11px; background: transparent;"));
    headerRow->addWidget(communityLabel);

    auto *byLabel = new QLabel(
        tr("by ") + res.userName, container);
    byLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    headerRow->addWidget(byLabel);

    qint64 sec = parseTimestampSec(res.published);
    QString timeStr = relativeTime(sec);
    auto *timeLabel = new QLabel(timeStr, container);
    timeLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    headerRow->addWidget(timeLabel);

    headerRow->addStretch();

    auto *typeBadge = new QLabel(tr("post"), container);
    typeBadge->setStyleSheet(QStringLiteral(
        "background-color: #45475a; color: #a6e3a1; border-radius: 4px; "
        "padding: 1px 6px; font-size: 10px; font-weight: bold;"));
    headerRow->addWidget(typeBadge);

    card->addLayout(headerRow);

    auto *titleLabel = new QLabel(res.title, container);
    titleLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 14px; background: transparent;"));
    titleLabel->setWordWrap(true);
    titleLabel->setMaximumHeight(titleLabel->fontMetrics().lineSpacing() * 2 + 4);
    card->addWidget(titleLabel);

    auto *footerRow = new QHBoxLayout();
    footerRow->setSpacing(12);

    auto *scoreLabel = new QLabel(
        QStringLiteral("\xE2\x96\xB2 ") + QString::number(res.score) +
        QStringLiteral(" \xE2\x96\xBC"), container);
    scoreLabel->setStyleSheet(QStringLiteral(
        "color: #9399b2; font-size: 11px; background: transparent;"));
    footerRow->addWidget(scoreLabel);

    auto *commentLabel = new QLabel(
        QStringLiteral("\xF0\x9F\x92\xAC ") + QString::number(res.commentCount), container);
    commentLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    footerRow->addWidget(commentLabel);

    footerRow->addStretch();
    card->addLayout(footerRow);

    return container;
}

QWidget *LemmySearchWidget::createCommunityResultWidget(const LemmySearchResult &res) const
{
    auto *container = new QWidget();
    container->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *row = new QHBoxLayout(container);
    row->setContentsMargins(12, 8, 12, 8);
    row->setSpacing(10);

    QPixmap avatar = makeAvatarPixmap(res.communityName, 40, QColor(0xcba6f7));
    auto *avatarLabel = new QLabel(container);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setFixedSize(40, 40);
    row->addWidget(avatarLabel);

    auto *textCol = new QVBoxLayout();
    textCol->setSpacing(2);

    auto *nameLabel = new QLabel(
        QStringLiteral("c/") + res.communityName, container);
    nameLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 14px; background: transparent;"));
    textCol->addWidget(nameLabel);

    QString displayTitle = res.title.isEmpty() ? res.communityName : res.title;
    auto *titleLabel = new QLabel(displayTitle, container);
    titleLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 12px; background: transparent;"));
    textCol->addWidget(titleLabel);

    row->addLayout(textCol, 1);

    QString subText = QString::number(res.subscriberCount) + tr(" subscribers");
    auto *subLabel = new QLabel(subText, container);
    subLabel->setStyleSheet(QStringLiteral(
        "color: #9399b2; font-size: 11px; background: transparent;"));
    subLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row->addWidget(subLabel);

    auto *typeBadge = new QLabel(tr("community"), container);
    typeBadge->setStyleSheet(QStringLiteral(
        "background-color: #45475a; color: #cba6f7; border-radius: 4px; "
        "padding: 1px 6px; font-size: 10px; font-weight: bold;"));
    row->addWidget(typeBadge);

    return container;
}

QWidget *LemmySearchWidget::createUserResultWidget(const LemmySearchResult &res) const
{
    auto *container = new QWidget();
    container->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *row = new QHBoxLayout(container);
    row->setContentsMargins(12, 8, 12, 8);
    row->setSpacing(10);

    QPixmap avatar = makeAvatarPixmap(res.userName, 40, QColor(0x89b4fa));
    auto *avatarLabel = new QLabel(container);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setFixedSize(40, 40);
    row->addWidget(avatarLabel);

    auto *textCol = new QVBoxLayout();
    textCol->setSpacing(2);

    auto *nameLabel = new QLabel(
        QStringLiteral("@") + res.userName, container);
    nameLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 14px; background: transparent;"));
    textCol->addWidget(nameLabel);

    QString displayTitle = res.title.isEmpty() ? res.userName : res.title;
    auto *titleLabel = new QLabel(displayTitle, container);
    titleLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 12px; background: transparent;"));
    textCol->addWidget(titleLabel);

    row->addLayout(textCol, 1);

    QString statsText = QString::number(res.score) + tr(" posts, ") +
                        QString::number(res.commentCount) + tr(" comments");
    auto *statsLabel = new QLabel(statsText, container);
    statsLabel->setStyleSheet(QStringLiteral(
        "color: #9399b2; font-size: 11px; background: transparent;"));
    statsLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    row->addWidget(statsLabel);

    auto *typeBadge = new QLabel(tr("user"), container);
    typeBadge->setStyleSheet(QStringLiteral(
        "background-color: #45475a; color: #89b4fa; border-radius: 4px; "
        "padding: 1px 6px; font-size: 10px; font-weight: bold;"));
    row->addWidget(typeBadge);

    return container;
}

QWidget *LemmySearchWidget::createCommentResultWidget(const LemmySearchResult &res) const
{
    auto *container = new QWidget();
    container->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *card = new QVBoxLayout(container);
    card->setContentsMargins(12, 8, 12, 8);
    card->setSpacing(4);

    auto *headerRow = new QHBoxLayout();
    headerRow->setSpacing(6);

    auto *userLabel = new QLabel(
        res.userName, container);
    userLabel->setStyleSheet(QStringLiteral(
        "color: #89b4fa; font-weight: bold; font-size: 11px; background: transparent;"));
    headerRow->addWidget(userLabel);

    auto *inLabel = new QLabel(tr("in"), container);
    inLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    headerRow->addWidget(inLabel);

    auto *postLabel = new QLabel(res.title, container);
    postLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-size: 11px; background: transparent;"));
    postLabel->setWordWrap(true);
    postLabel->setMaximumHeight(postLabel->fontMetrics().lineSpacing() * 2);
    headerRow->addWidget(postLabel, 1);

    auto *typeBadge = new QLabel(tr("comment"), container);
    typeBadge->setStyleSheet(QStringLiteral(
        "background-color: #45475a; color: #f9e2af; border-radius: 4px; "
        "padding: 1px 6px; font-size: 10px; font-weight: bold;"));
    headerRow->addWidget(typeBadge);

    card->addLayout(headerRow);

    QString bodyPreview = res.body.left(200).replace(QChar('\n'), QChar(' '));
    auto *bodyLabel = new QLabel(bodyPreview, container);
    bodyLabel->setStyleSheet(QStringLiteral(
        "color: #9399b2; font-size: 12px; background: transparent;"));
    bodyLabel->setWordWrap(true);
    bodyLabel->setMaximumHeight(bodyLabel->fontMetrics().lineSpacing() * 2);
    card->addWidget(bodyLabel);

    auto *footerRow = new QHBoxLayout();
    footerRow->setSpacing(12);

    auto *scoreLabel = new QLabel(
        tr("Score: ") + QString::number(res.score), container);
    scoreLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    footerRow->addWidget(scoreLabel);

    qint64 sec = parseTimestampSec(res.published);
    QString timeStr = relativeTime(sec);
    auto *timeLabel = new QLabel(timeStr, container);
    timeLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px; background: transparent;"));
    footerRow->addWidget(timeLabel);

    footerRow->addStretch();
    card->addLayout(footerRow);

    return container;
}
