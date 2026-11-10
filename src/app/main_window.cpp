#include "main_window.hpp"
#include "notification_manager.hpp"
#include "system_tray_manager.hpp"
#include "log_viewer_dialog.hpp"
#include "app_update_checker.hpp"
#include "../protocol/protocol_manager.hpp"
#include "../features/home/chat_list_widget.hpp"
#include "../features/home/chat_detail_widget.hpp"
#include "../features/home/welcome_widget.hpp"
#include "../features/home/status_message_editor.hpp"
#include "../features/matrix_chat/account_switcher.hpp"
#include "../features/matrix_chat/settings_dialog.hpp"
#include "../features/matrix_chat/command_palette.hpp"
#include "../features/matrix_chat/user_profile_dialog.hpp"
#include "../features/matrix_chat/proxy_config_dialog.hpp"
#include "../features/auth/matrix_auth_dialog.hpp"
#include "../ui/components/about_dialog.hpp"
#include "../ui/themes/theme_manager.hpp"

#include <QCloseEvent>
#include <QKeyEvent>
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QToolBar>
#include <QToolButton>
#include <QStatusBar>
#include <QSplitter>
#include <QSettings>
#include <QApplication>
#include <QMessageBox>
#include <QDialog>
#include <QTimer>

namespace pcp_chat {

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("PCP");
    setMinimumSize(900, 600);
    resize(1280, 800);
}

MainWindow::~MainWindow()
{
    saveWindowState();
    delete m_systemTray;
}

void MainWindow::setThemeManager(ThemeManager *manager) { m_themeManager = manager; }
void MainWindow::setNotificationManager(NotificationManager *manager) { m_notificationManager = manager; }
void MainWindow::setProtocolManager(ProtocolManager *manager) { m_protocolManager = manager; }

void MainWindow::initialize()
{
    setupMenuBar();
    setupToolBar();
    setupCentralArea();
    setupSidebar();
    setupStatusBar();
    setupDockWidgets();
    restoreWindowState();
    applyTheme();
}

void MainWindow::setupMenuBar()
{
    QMenuBar *mb = menuBar();

    // File menu
    QMenu *fileMenu = mb->addMenu(tr("&File"));
    QAction *loginAction = fileMenu->addAction(tr("&Add Account..."));
    connect(loginAction, &QAction::triggered, this, &MainWindow::showLoginDialog);

    QAction *accountAction = fileMenu->addAction(tr("&Switch Account..."));
    connect(accountAction, &QAction::triggered, this, &MainWindow::showAccountSwitcher);

    fileMenu->addSeparator();

    QAction *settingsAction = fileMenu->addAction(tr("&Settings..."));
    connect(settingsAction, &QAction::triggered, this, &MainWindow::showSettings);

    fileMenu->addSeparator();

    QAction *quitAction = fileMenu->addAction(tr("&Quit"));
    quitAction->setShortcut(QKeySequence::Quit);
    connect(quitAction, &QAction::triggered, this, &QWidget::close);

    // View menu
    QMenu *viewMenu = mb->addMenu(tr("&View"));
    QAction *toggleSidebar = viewMenu->addAction(tr("Toggle &Sidebar"));
    toggleSidebar->setShortcut(QKeySequence("Ctrl+\"));
    connect(toggleSidebar, &QAction::triggered, this, &MainWindow::toggleSidebar);

    QAction *compactAction = viewMenu->addAction(tr("&Compact Mode"));
    compactAction->setCheckable(true);
    compactAction->setShortcut(QKeySequence("Ctrl+Shift+C"));
    connect(compactAction, &QAction::toggled, this, [this](bool checked) {
        m_compactMode = checked;
    });

    viewMenu->addSeparator();
    QAction *zoomInAction = viewMenu->addAction(tr("Zoom &In"));
    zoomInAction->setShortcut(QKeySequence::ZoomIn);
    QAction *zoomOutAction = viewMenu->addAction(tr("Zoom &Out"));
    zoomOutAction->setShortcut(QKeySequence::ZoomOut);

    // Navigation menu
    QMenu *navMenu = mb->addMenu(tr("&Navigate"));
    QAction *cmdPalette = navMenu->addAction(tr("&Command Palette..."));
    cmdPalette->setShortcut(QKeySequence("Ctrl+K"));
    connect(cmdPalette, &QAction::triggered, this, &MainWindow::showCommandPalette);

    // Help menu
    QMenu *helpMenu = mb->addMenu(tr("&Help"));
    QAction *aboutAction = helpMenu->addAction(tr("&About PCP"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::showAboutDialog);

    QAction *logAction = helpMenu->addAction(tr("&Log Viewer..."));
    connect(logAction, &QAction::triggered, this, &MainWindow::showLogViewer);
}

void MainWindow::setupToolBar()
{
    m_mainToolBar = addToolBar(tr("Main"));
    m_mainToolBar->setMovable(false);
    m_mainToolBar->setIconSize(QSize(22, 22));

    QAction *homeAction = m_mainToolBar->addAction(tr("Home"));
    homeAction->setToolTip(tr("Home"));

    QAction *newChatAction = m_mainToolBar->addAction(tr("New Chat"));
    newChatAction->setToolTip(tr("Start a new chat"));

    QAction *searchAction = m_mainToolBar->addAction(tr("Search"));
    searchAction->setShortcut(QKeySequence("Ctrl+F"));
    searchAction->setToolTip(tr("Search messages"));

    m_mainToolBar->addSeparator();

    QAction *spacesAction = m_mainToolBar->addAction(tr("Spaces"));
    spacesAction->setToolTip(tr("Explore spaces"));

    m_mainToolBar->addSeparator();

    QAction *profileAction = m_mainToolBar->addAction(tr("Profile"));
    connect(profileAction, &QAction::triggered, this, &MainWindow::showUserProfile);
}

void MainWindow::setupCentralArea()
{
    m_centralStack = new QStackedWidget(this);

    // Welcome page
    m_welcomeWidget = new WelcomeWidget(this);
    m_centralStack->addWidget(m_welcomeWidget);

    // Chat split view
    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);

    m_chatList = new ChatListWidget(splitter);
    m_chatDetail = new ChatDetailWidget(splitter);

    splitter->addWidget(m_chatList);
    splitter->addWidget(m_chatDetail);
    splitter->setStretchFactor(0, 1);
    splitter->setStretchFactor(1, 3);

    m_centralStack->addWidget(splitter);
    m_mainSplitter = splitter;

    // Connect chat list to detail
    connect(m_chatList, &ChatListWidget::roomSelected, this, &MainWindow::navigateToRoom);
    connect(m_chatList, &ChatListWidget::roomSelected,
            m_chatDetail, &ChatDetailWidget::loadRoom);

    setCentralWidget(m_centralStack);
}

void MainWindow::setupSidebar()
{
    // Sidebar already integrated via splitter layout
}

void MainWindow::setupStatusBar()
{
    QStatusBar *sb = statusBar();

    m_statusEditor = new StatusMessageEditor(this);
    sb->addPermanentWidget(m_statusEditor);

    QLabel *connectionLabel = new QLabel("Disconnected", this);
    sb->addPermanentWidget(connectionLabel);

    connect(this, &MainWindow::roomSelected, this, [this](const QString &roomId) {
        m_currentRoomId = roomId;
        statusBar()->showMessage("Room: " + roomId, 3000);
    });
}

void MainWindow::setupDockWidgets()
{
    // Right-side info panel (can be toggled)
    QDockWidget *infoDock = new QDockWidget(tr("Room Info"), this);
    QWidget *infoPanel = new QWidget(infoDock);
    infoPanel->setMinimumWidth(200);
    infoDock->setWidget(infoPanel);
    addDockWidget(Qt::RightDockWidgetArea, infoDock);
    infoDock->hide();
}

void MainWindow::setupSystemTray(NotificationManager *notifManager)
{
    if (QSystemTrayIcon::isSystemTrayAvailable()) {
        m_systemTray = new SystemTrayManager(this, notifManager);
        m_systemTray->initialize();
    }
}

void MainWindow::applyTheme()
{
    if (m_themeManager) {
        QString themeName = m_themeManager->activeThemeName();
        // Apply to all widgets...
    }
}

void MainWindow::showSettings()
{
    SettingsDialog dialog(m_protocolManager, this);
    dialog.exec();
}

void MainWindow::showAboutDialog()
{
    AboutDialog dialog(this);
    dialog.exec();
}

void MainWindow::showCommandPalette()
{
    CommandPalette palette(m_protocolManager, this);
    palette.exec();
}

void MainWindow::showLogViewer()
{
    LogViewerDialog dialog(this);
    dialog.exec();
}

void MainWindow::showUserProfile()
{
    UserProfileDialog dialog(m_protocolManager, this);
    dialog.exec();
}

void MainWindow::showLoginDialog()
{
    MatrixAuthDialog dialog(m_protocolManager, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_centralStack->setCurrentIndex(1);
    }
}

void MainWindow::showAccountSwitcher()
{
    AccountSwitcher switcher(m_protocolManager, this);
    switcher.exec();
}

void MainWindow::toggleSidebar()
{
    m_sidebarVisible = !m_sidebarVisible;
    if (m_chatList)
        m_chatList->setVisible(m_sidebarVisible);
}

void MainWindow::toggleCompactMode()
{
    m_compactMode = !m_compactMode;
}

void MainWindow::navigateToRoom(const QString &roomId)
{
    m_currentRoomId = roomId;
    m_centralStack->setCurrentIndex(1);
    emit roomSelected(roomId);
}

void MainWindow::updateConnectionStatus(const QString &status, bool connected)
{
    statusBar()->showMessage(status);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_systemTray && m_systemTray->isVisible()) {
        hide();
        event->ignore();
    } else {
        saveWindowState();
        event->accept();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        if (m_chatDetail && !m_chatDetail->hasActiveSearch())
            m_chatDetail->clearFocus();
    }
    QMainWindow::keyPressEvent(event);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);
}

void MainWindow::saveWindowState()
{
    QSettings settings("PCP", "pcp_chat");
    settings.setValue("mainwindow/geometry", saveGeometry());
    settings.setValue("mainwindow/state", saveState());
    settings.setValue("mainwindow/compact", m_compactMode);
    settings.setValue("mainwindow/sidebar", m_sidebarVisible);
}

void MainWindow::restoreWindowState()
{
    QSettings settings("PCP", "pcp_chat");
    if (settings.contains("mainwindow/geometry"))
        restoreGeometry(settings.value("mainwindow/geometry").toByteArray());
    if (settings.contains("mainwindow/state"))
        restoreState(settings.value("mainwindow/state").toByteArray());
    m_compactMode = settings.value("mainwindow/compact", false).toBool();
    m_sidebarVisible = settings.value("mainwindow/sidebar", true).toBool();
}

} // namespace pcp_chat
