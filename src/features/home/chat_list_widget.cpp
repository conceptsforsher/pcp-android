#include "chat_list_widget.hpp"
#include "../../protocol/protocol_manager.hpp"

#include <QIcon>
#include <QFont>

namespace pcp_chat {

ChatListWidget::ChatListWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
}

void ChatListWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Header with title and filter
    auto *headerLayout = new QHBoxLayout();
    auto *titleLabel = new QLabel("Chats");
    QFont titleFont;
    titleFont.setPixelSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    headerLayout->addWidget(titleLabel);
    headerLayout->addStretch();

    m_createRoomBtn = new QPushButton("+");
    m_createRoomBtn->setFixedSize(30, 30);
    m_createRoomBtn->setToolTip("Create a new room");
    m_joinRoomBtn = new QPushButton("→");
    m_joinRoomBtn->setFixedSize(30, 30);
    m_joinRoomBtn->setToolTip("Join a room");
    headerLayout->addWidget(m_createRoomBtn);
    headerLayout->addWidget(m_joinRoomBtn);
    layout->addLayout(headerLayout);

    // Search bar
    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText("Search rooms...");
    m_searchInput->setClearButtonEnabled(true);
    layout->addWidget(m_searchInput);

    // Filter
    m_filterCombo = new QComboBox();
    m_filterCombo->addItem("All", "all");
    m_filterCombo->addItem("Matrix", "matrix");
    m_filterCombo->addItem("IRC", "irc");
    m_filterCombo->addItem("Lemmy", "lemmy");
    m_filterCombo->addItem("Direct", "direct");
    m_filterCombo->addItem("Groups", "group");
    m_filterCombo->addItem("Unread", "unread");
    layout->addWidget(m_filterCombo);

    // Room list
    m_roomList = new QListWidget();
    m_roomList->setFrameShape(QFrame::NoFrame);
    m_roomList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_roomList->setIconSize(QSize(40, 40));
    layout->addWidget(m_roomList);

    // Connections
    connect(m_roomList, &QListWidget::itemClicked, this, &ChatListWidget::onRoomClicked);
    connect(m_filterCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ChatListWidget::onFilterChanged);
    connect(m_searchInput, &QLineEdit::textChanged, this, &ChatListWidget::onSearchTextChanged);
    connect(m_createRoomBtn, &QPushButton::clicked, this, &ChatListWidget::createRoomRequested);
    connect(m_joinRoomBtn, &QPushButton::clicked, this, &ChatListWidget::joinRoomRequested);
}

void ChatListWidget::setProtocolManager(ProtocolManager *manager)
{
    m_protocolManager = manager;
}

void ChatListWidget::addRoom(const QString &name, const QString &roomId,
                               int unread, const QString &lastMsg)
{
    auto *item = new QListWidgetItem();
    item->setText(name);
    item->setData(Qt::UserRole, roomId);
    item->setData(Qt::UserRole + 1, unread);
    if (unread > 0) {
        QFont boldFont = item->font();
        boldFont.setBold(true);
        item->setFont(boldFont);
    }
    m_roomList->addItem(item);
}

void ChatListWidget::removeRoom(const QString &roomId)
{
    for (int i = 0; i < m_roomList->count(); ++i) {
        if (m_roomList->item(i)->data(Qt::UserRole).toString() == roomId) {
            delete m_roomList->takeItem(i);
            break;
        }
    }
}

void ChatListWidget::updateRoom(const QString &roomId, const QString &name,
                                  int unread, const QString &lastMsg)
{
    for (int i = 0; i < m_roomList->count(); ++i) {
        auto *item = m_roomList->item(i);
        if (item->data(Qt::UserRole).toString() == roomId) {
            item->setText(name);
            item->setData(Qt::UserRole + 1, unread);
            QFont f = item->font();
            f.setBold(unread > 0);
            item->setFont(f);
            break;
        }
    }
}

void ChatListWidget::setActiveRoom(const QString &roomId)
{
    for (int i = 0; i < m_roomList->count(); ++i) {
        if (m_roomList->item(i)->data(Qt::UserRole).toString() == roomId) {
            m_roomList->setCurrentRow(i);
            break;
        }
    }
}

void ChatListWidget::filterByProtocol(const QString &protocol)
{
    m_filterCombo->setCurrentText(protocol);
}

void ChatListWidget::refresh()
{
    // Reload from protocol manager
}

void ChatListWidget::onRoomClicked(QListWidgetItem *item)
{
    QString roomId = item->data(Qt::UserRole).toString();
    emit roomSelected(roomId);
}

void ChatListWidget::onFilterChanged(int index)
{
    QString filter = m_filterCombo->itemData(index).toString();
    for (int i = 0; i < m_roomList->count(); ++i) {
        auto *item = m_roomList->item(i);
        bool visible = true;
        if (filter == "unread") {
            visible = item->data(Qt::UserRole + 1).toInt() > 0;
        }
        item->setHidden(!visible);
    }
}

void ChatListWidget::onSearchTextChanged(const QString &text)
{
    for (int i = 0; i < m_roomList->count(); ++i) {
        auto *item = m_roomList->item(i);
        bool match = text.isEmpty() ||
                     item->text().contains(text, Qt::CaseInsensitive);
        item->setHidden(!match);
    }
}

} // namespace pcp_chat
