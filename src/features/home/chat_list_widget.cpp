#include "features/home/chat_list_widget.hpp"
#include "app/application.hpp"
#include "protocol/protocol_manager.hpp"

#include <QListWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>
#include <QDateTime>
#include <QFont>

static const char *kDarkStyleSheet = R"(
    ChatListWidget {
        background-color: #1e1e2e;
    }
    ChatListWidget QLineEdit {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 8px;
        padding: 6px 12px;
        font-size: 13px;
    }
    ChatListWidget QLineEdit:focus {
        border-color: #89b4fa;
    }
    ChatListWidget QComboBox {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 10px;
        font-size: 12px;
        min-width: 80px;
    }
    ChatListWidget QComboBox::drop-down {
        border: none;
        width: 20px;
    }
    ChatListWidget QComboBox QAbstractItemView {
        background-color: #313244;
        color: #cdd6f4;
        selection-background-color: #45475a;
        border: 1px solid #45475a;
    }
    ChatListWidget QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 4px 12px;
        font-size: 12px;
    }
    ChatListWidget QPushButton:hover {
        background-color: #45475a;
        border-color: #89b4fa;
    }
    ChatListWidget QListWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        border: none;
        outline: none;
    }
    ChatListWidget QListWidget::item {
        background-color: #1e1e2e;
        border-bottom: 1px solid #313244;
        padding: 8px 12px;
    }
    ChatListWidget QListWidget::item:hover {
        background-color: #2a2a3c;
    }
    ChatListWidget QListWidget::item:selected {
        background-color: #45475a;
    }
)";

ChatListWidget::ChatListWidget(QWidget *parent)
    : QWidget(parent)
    , m_searchEdit(nullptr)
    , m_protocolFilter(nullptr)
    , m_refreshButton(nullptr)
    , m_roomList(nullptr)
{
    setStyleSheet(QString::fromLatin1(kDarkStyleSheet));
    setupUi();
}

ChatListWidget::~ChatListWidget() = default;

void ChatListWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(8, 8, 8, 8);
    topBar->setSpacing(6);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search rooms..."));
    m_searchEdit->setClearButtonEnabled(true);
    topBar->addWidget(m_searchEdit, 1);

    m_protocolFilter = new QComboBox(this);
    m_protocolFilter->addItem(tr("All"));
    m_protocolFilter->addItem(tr("Matrix"));
    m_protocolFilter->addItem(tr("IRC"));
    m_protocolFilter->addItem(tr("Lemmy"));
    topBar->addWidget(m_protocolFilter);

    m_refreshButton = new QPushButton(tr("Refresh"), this);
    topBar->addWidget(m_refreshButton);

    mainLayout->addLayout(topBar);

    m_roomList = new QListWidget(this);
    m_roomList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_roomList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_roomList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_roomList->setIconSize(QSize(44, 44));
    mainLayout->addWidget(m_roomList, 1);

    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &ChatListWidget::onFilterChanged);
    connect(m_protocolFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChatListWidget::onFilterChanged);
    connect(m_refreshButton, &QPushButton::clicked,
            this, &ChatListWidget::refreshRoomList);
    connect(m_roomList, &QListWidget::itemActivated,
            this, &ChatListWidget::onItemActivated);
    connect(m_roomList, &QListWidget::itemDoubleClicked,
            this, &ChatListWidget::onItemActivated);
}

void ChatListWidget::refreshRoomList()
{
    auto *pm = pcp::Application::instance().protocolManager();
    if (!pm)
        return;

    m_rooms = pm->getAllRooms();

    std::sort(m_rooms.begin(), m_rooms.end(),
              [](const ProtocolRoom &a, const ProtocolRoom &b) {
                  return a.lastMessageTimestamp > b.lastMessageTimestamp;
              });

    applyFilter();
}

void ChatListWidget::onMessageReceived(ProtocolMessage message)
{
    for (int i = 0; i < m_rooms.size(); ++i) {
        if (m_rooms[i].id == message.roomId) {
            m_rooms[i].lastMessagePreview = message.text;
            m_rooms[i].lastMessageTimestamp = message.timestamp;

            std::sort(m_rooms.begin(), m_rooms.end(),
                      [](const ProtocolRoom &a, const ProtocolRoom &b) {
                          return a.lastMessageTimestamp > b.lastMessageTimestamp;
                      });

            applyFilter();
            return;
        }
    }
}

void ChatListWidget::onFilterChanged()
{
    applyFilter();
}

void ChatListWidget::onItemActivated(QListWidgetItem *item)
{
    if (!item)
        return;

    bool ok = false;
    int idx = item->data(static_cast<int>(RoomIndexRole)).toInt(&ok);
    if (ok && idx >= 0 && idx < m_rooms.size())
        emit roomSelected(m_rooms[idx]);
}

void ChatListWidget::applyFilter()
{
    const QString searchText = m_searchEdit ? m_searchEdit->text().trimmed() : QString();
    const int filterIndex = m_protocolFilter ? m_protocolFilter->currentIndex() : 0;

    ProtocolType targetType = ProtocolType::MATRIX;
    switch (filterIndex) {
    case 1: targetType = ProtocolType::MATRIX; break;
    case 2: targetType = ProtocolType::IRC;    break;
    case 3: targetType = ProtocolType::LEMMY;  break;
    default: targetType = ProtocolType::MATRIX; break;
    }

    m_roomList->clear();

    for (int i = 0; i < m_rooms.size(); ++i) {
        const auto &room = m_rooms[i];

        if (filterIndex != 0 && room.protocolType != targetType)
            continue;

        if (!searchText.isEmpty() &&
            !room.name.contains(searchText, Qt::CaseInsensitive))
            continue;

        auto *item = new QListWidgetItem();
        item->setData(static_cast<int>(RoomIndexRole), i);
        item->setSizeHint(QSize(0, 64));
        populateItem(room, item);
        m_roomList->addItem(item);
    }
}

void ChatListWidget::populateItem(const ProtocolRoom &room, QListWidgetItem *item)
{
    const int avatarSize = 44;
    const int dotSize = 10;

    auto *widget = new QWidget();
    widget->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(0, 0, 8, 0);
    layout->setSpacing(10);

    // --- Avatar with protocol dot overlay ---
    QColor bgColor = protocolColor(room.protocolType);
    QPixmap avatar = makeAvatarPixmap(room.name, avatarSize, bgColor);

    auto *avatarLabel = new QLabel();
    avatarLabel->setFixedSize(avatarSize + 4, avatarSize + 4);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(avatarLabel);

    // --- Center: name + preview ---
    auto *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(2);

    auto *nameLabel = new QLabel(room.name);
    nameLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 13px; background: transparent;"));
    nameLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    centerLayout->addWidget(nameLabel);

    QString preview = room.lastMessagePreview;
    if (preview.isEmpty())
        preview = tr("No messages yet");
    else if (preview.length() > 60)
        preview = preview.left(57) + QStringLiteral("...");

    auto *previewLabel = new QLabel(preview);
    previewLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 12px; background: transparent;"));
    previewLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    centerLayout->addWidget(previewLabel);

    layout->addLayout(centerLayout, 1);

    // --- Right: timestamp + unread badge + protocol dot ---
    auto *rightLayout = new QVBoxLayout();
    rightLayout->setSpacing(4);
    rightLayout->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    if (room.lastMessageTimestamp > 0) {
        auto *timeLabel = new QLabel(formatTimestamp(room.lastMessageTimestamp));
        timeLabel->setStyleSheet(QStringLiteral(
            "color: #6c7086; font-size: 11px; background: transparent;"));
        timeLabel->setAlignment(Qt::AlignRight);
        rightLayout->addWidget(timeLabel);
    }

    if (room.unreadCount > 0) {
        auto *badge = new QLabel(QString::number(room.unreadCount));
        badge->setAlignment(Qt::AlignCenter);
        badge->setFixedSize(24, 20);
        badge->setStyleSheet(QStringLiteral(
            "background-color: #cba6f7; color: #1e1e2e;"
            "border-radius: 10px; font-size: 11px; font-weight: bold;"));
        rightLayout->addWidget(badge);
    }

    auto *dotLabel = new QLabel();
    QPixmap dot = makeProtocolDot(dotSize, protocolColor(room.protocolType));
    dotLabel->setPixmap(dot);
    dotLabel->setFixedSize(dotSize, dotSize);
    dotLabel->setToolTip(protocolName(room.protocolType));
    rightLayout->addWidget(dotLabel);

    rightLayout->addStretch();
    layout->addLayout(rightLayout);

    widget->setLayout(layout);
    m_roomList->setItemWidget(item, widget);
}

// --- Static helpers ---

QColor ChatListWidget::protocolColor(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:   return QColor(0x89, 0xb4, 0xfa); // blue
    case ProtocolType::IRC:      return QColor(0xa6, 0xe3, 0xa1); // green
    case ProtocolType::LEMMY:    return QColor(0xfa, 0xb3, 0x87); // orange
    default:                     return QColor(0x6c, 0x70, 0x86); // gray
    }
}

QString ChatListWidget::protocolName(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX: return QStringLiteral("Matrix");
    case ProtocolType::IRC:    return QStringLiteral("IRC");
    case ProtocolType::LEMMY:  return QStringLiteral("Lemmy");
    default:                   return QStringLiteral("Unknown");
    }
}

QString ChatListWidget::formatTimestamp(qint64 timestampMs)
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

QPixmap ChatListWidget::makeAvatarPixmap(const QString &name, int size, const QColor &bgColor)
{
    QPixmap pix(size + 4, size + 4);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect circleRect(2, 2, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawEllipse(circleRect);

    QString letter;
    if (!name.isEmpty()) {
        QChar first = name.at(0);
        if (name.length() > 1) {
            int spaceIdx = name.indexOf(QChar::Space);
            if (spaceIdx != -1 && spaceIdx + 1 < name.length())
                letter = name.at(0).toUpper() + QString(name.at(spaceIdx + 1).toUpper());
            else
                letter = first.toUpper();
        } else {
            letter = first.toUpper();
        }
    } else {
        letter = QStringLiteral("?");
    }

    if (letter.length() > 2)
        letter = letter.left(2);

    QFont font;
    font.setPixelSize(size / 2 + 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(circleRect, Qt::AlignCenter, letter);

    painter.end();
    return pix;
}

QPixmap ChatListWidget::makeProtocolDot(int size, const QColor &color)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);
    painter.setBrush(color);
    painter.drawEllipse(0, 0, size, size);
    painter.end();

    return pix;
}
