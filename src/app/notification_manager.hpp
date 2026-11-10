#pragma once

#include <QObject>
#include <QString>
#include <QHash>
#include <memory>

namespace pcp_chat {

class NotificationManager : public QObject
{
    Q_OBJECT

public:
    explicit NotificationManager(QObject *parent = nullptr);
    ~NotificationManager() override;

    enum class Priority { Low, Normal, High, Critical };

    void showNotification(const QString &title, const QString &body,
                          const QString &roomId = "", Priority priority = Priority::Normal);
    void showMessageNotification(const QString &sender, const QString &message,
                                  const QString &roomId, Priority priority = Priority::Normal);
    void clearRoomNotifications(const QString &roomId);
    void clearAllNotifications();
    int unreadCount() const;

    void setEnabled(bool enabled);
    void setSoundEnabled(bool enabled);

signals:
    void notificationClicked(const QString &roomId);
    void notificationDismissed(const QString &roomId);
    void unreadCountChanged(int count);

private:
    struct ActiveNotification {
        QString roomId;
        uint id = 0;
    };

    QHash<QString, ActiveNotification> m_activeNotifications;
    bool m_enabled = true;
    bool m_soundEnabled = true;
    int m_totalUnread = 0;
};

} // namespace pcp_chat
