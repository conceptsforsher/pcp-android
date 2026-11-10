#pragma once

#include <QMainWindow>
#include <QString>
#include <memory>
#include <QStackedWidget>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QToolBar>
#include <QMenuBar>
#include <QStatusBar>
#include <QDockWidget>
#include <QListView>
#include <QHash>

namespace pcp_chat {

class ThemeManager;
class NotificationManager;
class ProtocolManager;
class ChatListWidget;
class ChatDetailWidget;
class StatusMessageEditor;
class AccountSwitcher;
class WelcomeWidget;
class SystemTrayManager;

// Main application window — hosts all UI panels, menus, toolbars
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void setThemeManager(ThemeManager *manager);
    void setNotificationManager(NotificationManager *manager);
    void setProtocolManager(ProtocolManager *manager);
    void initialize();
    void setupSystemTray(NotificationManager *notifManager);

public slots:
    void showSettings();
    void showAboutDialog();
    void showCommandPalette();
    void showLogViewer();
    void showUserProfile();
    void toggleSidebar();
    void toggleCompactMode();
    void navigateToRoom(const QString &roomId);
    void showLoginDialog();
    void showAccountSwitcher();
    void updateConnectionStatus(const QString &status, bool connected);

signals:
    void roomSelected(const QString &roomId);
    void logoutRequested();
    void themeChanged(const QString &themeName);

protected:
    void closeEvent(QCloseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void setupMenuBar();
    void setupToolBar();
    void setupCentralArea();
    void setupSidebar();
    void setupStatusBar();
    void setupDockWidgets();
    void applyTheme();
    void saveWindowState();
    void restoreWindowState();

    // UI components
    QToolBar *m_mainToolBar = nullptr;
    QSplitter *m_mainSplitter = nullptr;
    QStackedWidget *m_centralStack = nullptr;
    ChatListWidget *m_chatList = nullptr;
    ChatDetailWidget *m_chatDetail = nullptr;
    StatusMessageEditor *m_statusEditor = nullptr;
    AccountSwitcher *m_accountSwitcher = nullptr;
    WelcomeWidget *m_welcomeWidget = nullptr;
    SystemTrayManager *m_systemTray = nullptr;

    // Subsystem references
    ThemeManager *m_themeManager = nullptr;
    NotificationManager *m_notificationManager = nullptr;
    ProtocolManager *m_protocolManager = nullptr;

    // State
    bool m_compactMode = false;
    bool m_sidebarVisible = true;
    QString m_currentRoomId;
};

} // namespace pcp_chat
