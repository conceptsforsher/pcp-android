#include "system_tray_manager.hpp"
#include "notification_manager.hpp"
#include <QApplication>
#include <QStyle>
#include <QAction>
#include <QPainter>
#include <QPixmap>

namespace pcp_chat {

SystemTrayManager::SystemTrayManager(QObject *parent, NotificationManager *notifManager)
    : QObject(parent)
    , m_notificationManager(notifManager)
{
}

SystemTrayManager::~SystemTrayManager()
{
    delete m_trayMenu;
    if (m_trayIcon) {
        m_trayIcon->hide();
        delete m_trayIcon;
    }
}

void SystemTrayManager::initialize()
{
    m_trayIcon = new QSystemTrayIcon(this);
    setupIcon();
    setupMenu();
    m_trayIcon->show();

    connect(m_trayIcon, &QSystemTrayIcon::activated, this,
            [this](QSystemTrayIcon::ActivationReason reason) {
        if (reason == QSystemTrayIcon::DoubleClick || reason == QSystemTrayIcon::Trigger) {
            emit showMainWindow();
        }
    });
}

void SystemTrayManager::setupMenu()
{
    m_trayMenu = new QMenu();

    QAction *showAction = m_trayMenu->addAction("Show");
    connect(showAction, &QAction::triggered, this, &SystemTrayManager::showMainWindow);

    m_trayMenu->addSeparator();

    QAction *muteAction = m_trayMenu->addAction("Mute Notifications");
    muteAction->setCheckable(true);
    connect(muteAction, &QAction::toggled, this, [this](bool muted) {
        m_muted = muted;
        if (m_notificationManager) {
            m_notificationManager->setEnabled(!muted);
            m_notificationManager->setSoundEnabled(!muted);
        }
        emit notificationMuteToggled(muted);
    });

    m_trayMenu->addSeparator();

    QAction *quitAction = m_trayMenu->addAction("Quit");
    connect(quitAction, &QAction::triggered, this, &SystemTrayManager::quitRequested);

    m_trayIcon->setContextMenu(m_trayMenu);
}

void SystemTrayManager::setupIcon()
{
    // Create a default tray icon programmatically
    QPixmap pixmap(64, 64);
    pixmap.fill(Qt::transparent);
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor("#4CAF50"));
    painter.setPen(Qt::NoPen);
    painter.drawRoundedRect(8, 8, 48, 48, 10, 10);
    painter.setPen(Qt::white);
    QFont font;
    font.setBold(true);
    font.setPixelSize(28);
    painter.setFont(font);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, "P");
    painter.end();

    m_trayIcon->setIcon(QIcon(pixmap));
    m_trayIcon->setToolTip("PCP");
}

bool SystemTrayManager::isVisible() const
{
    return m_trayIcon && m_trayIcon->isVisible();
}

void SystemTrayManager::showMessage(const QString &title, const QString &message)
{
    if (m_trayIcon && !m_muted)
        m_trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 5000);
}

void SystemTrayManager::updateUnreadBadge(int count)
{
    Q_UNUSED(count);
    // Update tray icon with badge overlay
}

void SystemTrayManager::toggleMuteNotifications()
{
    m_muted = !m_muted;
    emit notificationMuteToggled(m_muted);
}

} // namespace pcp_chat
