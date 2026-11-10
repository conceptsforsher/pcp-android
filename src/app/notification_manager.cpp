#include "notification_manager.hpp"

#include <QApplication>
#include <QSystemTrayIcon>
#include <QDebug>

namespace pcp_chat {

NotificationManager::NotificationManager(QObject *parent)
    : QObject(parent)
{
}

NotificationManager::~NotificationManager()
{
    clearAllNotifications();
}

void NotificationManager::showNotification(const QString &title, const QString &body,
                                             const QString &roomId, Priority priority)
{
    if (!m_enabled) return;

    m_totalUnread++;
    emit unreadCountChanged(m_totalUnread);

    if (QSystemTrayIcon::supportsMessages()) {
        // System tray notifications are handled by SystemTrayManager
    }

    qDebug() << "[Notification]" << title << "-" << body;
}

void NotificationManager::showMessageNotification(const QString &sender,
                                                     const QString &message,
                                                     const QString &roomId,
                                                     Priority priority)
{
    showNotification(sender, message, roomId, priority);
}

void NotificationManager::clearRoomNotifications(const QString &roomId)
{
    m_activeNotifications.remove(roomId);
}

void NotificationManager::clearAllNotifications()
{
    m_activeNotifications.clear();
    m_totalUnread = 0;
    emit unreadCountChanged(0);
}

int NotificationManager::unreadCount() const { return m_totalUnread; }

void NotificationManager::setEnabled(bool enabled) { m_enabled = enabled; }
void NotificationManager::setSoundEnabled(bool enabled) { m_soundEnabled = enabled; }

} // namespace pcp_chat
