#pragma once

#include <QObject>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QString>

namespace pcp_chat {

class NotificationManager;

class SystemTrayManager : public QObject
{
    Q_OBJECT

public:
    explicit SystemTrayManager(QObject *parent, NotificationManager *notifManager);
    ~SystemTrayManager() override;

    void initialize();
    bool isVisible() const;
    void showMessage(const QString &title, const QString &message);
    void updateUnreadBadge(int count);
    void toggleMuteNotifications();

signals:
    void showMainWindow();
    void quitRequested();
    void notificationMuteToggled(bool muted);

private:
    void setupMenu();
    void setupIcon();

    QSystemTrayIcon *m_trayIcon = nullptr;
    QMenu *m_trayMenu = nullptr;
    NotificationManager *m_notificationManager = nullptr;
    bool m_muted = false;
};

} // namespace pcp_chat
