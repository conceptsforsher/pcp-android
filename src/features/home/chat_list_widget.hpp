#pragma once

#include <QWidget>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QListWidgetItem>

namespace pcp_chat {

class ProtocolManager;

class ChatListWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatListWidget(QWidget *parent = nullptr);
    void setProtocolManager(ProtocolManager *manager);

    void addRoom(const QString &name, const QString &roomId, int unread = 0,
                 const QString &lastMsg = "");
    void removeRoom(const QString &roomId);
    void updateRoom(const QString &roomId, const QString &name, int unread = 0,
                    const QString &lastMsg = "");
    void setActiveRoom(const QString &roomId);
    void filterByProtocol(const QString &protocol);
    void refresh();

signals:
    void roomSelected(const QString &roomId);
    void createRoomRequested();
    void joinRoomRequested();

private:
    void setupUi();
    void onRoomClicked(QListWidgetItem *item);
    void onFilterChanged(int index);
    void onSearchTextChanged(const QString &text);

    QListWidget *m_roomList;
    QLineEdit *m_searchInput;
    QComboBox *m_filterCombo;
    QPushButton *m_createRoomBtn;
    QPushButton *m_joinRoomBtn;
    ProtocolManager *m_protocolManager = nullptr;
};

} // namespace pcp_chat
