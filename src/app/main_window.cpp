#include "app/main_window.hpp"

#include "app/application.hpp"
#include "protocol/protocol_manager.hpp"

#include <QTabWidget>
#include <QStackedWidget>
#include <QListView>
#include <QStringListModel>
#include <QTextBrowser>
#include <QLineEdit>
#include <QLabel>
#include <QToolBar>
#include <QStatusBar>
#include <QAction>
#include <QPushButton>
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QApplication>
#include <QPixmap>
#include <QPainter>
#include <QMessageBox>

using namespace pcp;

static QPixmap makeDot(QColor color, int size = 10)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(0, 0, size, size);
    painter.end();
    return pix;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_app(Application::instance())
    , m_protocolTabs(nullptr)
    , m_centralStack(nullptr)
    , m_chatListPage(nullptr)
    , m_chatListSplitter(nullptr)
    , m_chatListView(nullptr)
    , m_chatListModel(nullptr)
    , m_chatDetailPage(nullptr)
    , m_messageArea(nullptr)
    , m_messageInput(nullptr)
    , m_sendButton(nullptr)
    , m_backButton(nullptr)
    , m_settingsPage(nullptr)
    , m_toolBar(nullptr)
    , m_newChatAction(nullptr)
    , m_joinRoomAction(nullptr)
    , m_matrixStatus(nullptr)
    , m_ircStatus(nullptr)
    , m_lemmyStatus(nullptr)
    , m_settingsAction(nullptr)
    , m_quitAction(nullptr)
    , m_addMatrixAction(nullptr)
    , m_addIrcAction(nullptr)
    , m_addLemmyAction(nullptr)
{
    setWindowTitle("PCP Concept");
    resize(1200, 800);

    setupUi();
    connectSignals();
}

MainWindow::~MainWindow() = default;

void MainWindow::setupUi()
{
    setupMenuBar();
    setupToolBar();
    setupStatusBar();
    setupCentralWidget();
}

void MainWindow::setupMenuBar()
{
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));

    m_settingsAction = new QAction(tr("&Settings..."), this);
    m_settingsAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Comma));
    fileMenu->addAction(m_settingsAction);

    fileMenu->addSeparator();

    m_quitAction = new QAction(tr("&Quit"), this);
    m_quitAction->setShortcut(QKeySequence::Quit);
    fileMenu->addAction(m_quitAction);

    QMenu *protocolsMenu = menuBar()->addMenu(tr("&Protocols"));

    m_addMatrixAction = new QAction(tr("Add &Matrix Account..."), this);
    protocolsMenu->addAction(m_addMatrixAction);

    m_addIrcAction = new QAction(tr("Add &IRC Server..."), this);
    protocolsMenu->addAction(m_addIrcAction);

    m_addLemmyAction = new QAction(tr("Add &Lemmy Instance..."), this);
    protocolsMenu->addAction(m_addLemmyAction);
}

void MainWindow::setupToolBar()
{
    m_toolBar = addToolBar(tr("Main"));

    m_newChatAction = new QAction(tr("New Chat"), this);
    m_newChatAction->setStatusTip(tr("Start a new chat"));
    m_toolBar->addAction(m_newChatAction);

    m_joinRoomAction = new QAction(tr("Join Room"), this);
    m_joinRoomAction->setStatusTip(tr("Join an existing room or channel"));
    m_toolBar->addAction(m_joinRoomAction);
}

void MainWindow::setupStatusBar()
{
    m_matrixStatus = createConnectionIndicator(tr("Matrix"));
    m_ircStatus = createConnectionIndicator(tr("IRC"));
    m_lemmyStatus = createConnectionIndicator(tr("Lemmy"));

    statusBar()->addPermanentWidget(m_matrixStatus);
    statusBar()->addPermanentWidget(m_ircStatus);
    statusBar()->addPermanentWidget(m_lemmyStatus);
}

QLabel *MainWindow::createConnectionIndicator(const QString &name)
{
    auto *label = new QLabel(this);
    label->setPixmap(makeDot(Qt::red));
    label->setToolTip(tr("%1: Disconnected").arg(name));
    label->setContentsMargins(4, 0, 10, 0);
    return label;
}

void MainWindow::setupCentralWidget()
{
    m_protocolTabs = new QTabWidget(this);

    // --- Chat List Page (page 0) ---
    m_chatListPage = new QWidget(this);
    auto *page0Layout = new QVBoxLayout(m_chatListPage);
    page0Layout->setContentsMargins(0, 0, 0, 0);

    m_chatListSplitter = new QSplitter(Qt::Horizontal, m_chatListPage);
    m_chatListView = new QListView(m_chatListSplitter);
    m_chatListModel = new QStringListModel(m_chatListView);
    m_chatListView->setModel(m_chatListModel);
    m_chatListView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    auto *infoPlaceholder = new QLabel(
        tr("Select a room to start chatting"), m_chatListSplitter);
    infoPlaceholder->setAlignment(Qt::AlignCenter);
    infoPlaceholder->setStyleSheet("color: #888; font-size: 14px;");

    m_chatListSplitter->addWidget(m_chatListView);
    m_chatListSplitter->addWidget(infoPlaceholder);
    m_chatListSplitter->setStretchFactor(0, 1);
    m_chatListSplitter->setStretchFactor(1, 3);

    page0Layout->addWidget(m_chatListSplitter);

    // --- Chat Detail Page (page 1) ---
    m_chatDetailPage = new QWidget(this);
    auto *page1Layout = new QVBoxLayout(m_chatDetailPage);
    page1Layout->setContentsMargins(4, 4, 4, 4);

    auto *backBar = new QHBoxLayout();
    m_backButton = new QPushButton(tr("< Back"), m_chatDetailPage);
    backBar->addWidget(m_backButton);
    backBar->addStretch();
    page1Layout->addLayout(backBar);

    m_messageArea = new QTextBrowser(m_chatDetailPage);
    m_messageArea->setOpenExternalLinks(true);
    page1Layout->addWidget(m_messageArea);

    auto *inputBar = new QHBoxLayout();
    m_messageInput = new QLineEdit(m_chatDetailPage);
    m_messageInput->setPlaceholderText(tr("Type a message..."));
    m_sendButton = new QPushButton(tr("Send"), m_chatDetailPage);
    inputBar->addWidget(m_messageInput);
    inputBar->addWidget(m_sendButton);
    page1Layout->addLayout(inputBar);

    // --- Settings Page (page 2) ---
    m_settingsPage = new QWidget(this);
    auto *page2Layout = new QVBoxLayout(m_settingsPage);
    page2Layout->setContentsMargins(12, 12, 12, 12);
    auto *settingsLabel = new QLabel(tr("Settings"), m_settingsPage);
    settingsLabel->setStyleSheet("font-size: 18px; font-weight: bold;");
    page2Layout->addWidget(settingsLabel);
    page2Layout->addStretch();

    // --- Stacked Widget ---
    m_centralStack = new QStackedWidget(this);
    m_centralStack->addWidget(m_chatListPage);   // index 0
    m_centralStack->addWidget(m_chatDetailPage); // index 1
    m_centralStack->addWidget(m_settingsPage);   // index 2
    m_centralStack->setCurrentIndex(0);

    // Protocol tabs wrap the central stack
    m_protocolTabs->addTab(m_centralStack, tr("Matrix"));
    m_protocolTabs->addTab(new QWidget(this), tr("IRC"));
    m_protocolTabs->addTab(new QWidget(this), tr("Lemmy"));

    setCentralWidget(m_protocolTabs);
}

void MainWindow::connectSignals()
{
    // Menu actions
    QObject::connect(m_settingsAction, &QAction::triggered,
                     this, &MainWindow::onSettingsTriggered);
    QObject::connect(m_quitAction, &QAction::triggered,
                     this, &MainWindow::onQuitTriggered);
    QObject::connect(m_addMatrixAction, &QAction::triggered,
                     this, &MainWindow::onAddMatrixAccount);
    QObject::connect(m_addIrcAction, &QAction::triggered,
                     this, &MainWindow::onAddIrcServer);
    QObject::connect(m_addLemmyAction, &QAction::triggered,
                     this, &MainWindow::onAddLemmyInstance);

    // Toolbar actions
    QObject::connect(m_newChatAction, &QAction::triggered,
                     this, &MainWindow::onNewChat);
    QObject::connect(m_joinRoomAction, &QAction::triggered,
                     this, &MainWindow::onJoinRoom);

    // Chat list selection
    QObject::connect(m_chatListView, &QListView::activated,
                     this, &MainWindow::onRoomSelected);
    QObject::connect(m_chatListView, &QListView::clicked,
                     this, &MainWindow::onRoomSelected);

    // Protocol tabs
    QObject::connect(m_protocolTabs, &QTabWidget::currentChanged,
                     this, &MainWindow::onProtocolTabChanged);

    // Send message
    QObject::connect(m_sendButton, &QPushButton::clicked,
                     this, &MainWindow::onSendMessage);
    QObject::connect(m_messageInput, &QLineEdit::returnPressed,
                     this, &MainWindow::onSendMessage);

    // Back button from chat detail
    QObject::connect(m_backButton, &QPushButton::clicked, this, [this]() {
        m_centralStack->setCurrentIndex(0);
    });

    // Application connection state updates
    QObject::connect(&m_app, &Application::connectionStateChanged,
                     this, &MainWindow::updateConnectionStates);
}

// --- Slots ---

void MainWindow::onSettingsTriggered()
{
    m_centralStack->setCurrentIndex(2);
}

void MainWindow::onQuitTriggered()
{
    QApplication::quit();
}

void MainWindow::onAddMatrixAccount()
{
    QMessageBox::information(this, tr("Add Matrix Account"),
                             tr("Matrix account dialog placeholder."));
}

void MainWindow::onAddIrcServer()
{
    QMessageBox::information(this, tr("Add IRC Server"),
                             tr("IRC server dialog placeholder."));
}

void MainWindow::onAddLemmyInstance()
{
    QMessageBox::information(this, tr("Add Lemmy Instance"),
                             tr("Lemmy instance dialog placeholder."));
}

void MainWindow::onNewChat()
{
    m_protocolTabs->setCurrentIndex(0);
    m_centralStack->setCurrentIndex(0);

    QMessageBox::information(this, tr("New Chat"),
                             tr("New chat dialog placeholder."));
}

void MainWindow::onJoinRoom()
{
    m_protocolTabs->setCurrentIndex(0);
    m_centralStack->setCurrentIndex(0);

    QMessageBox::information(this, tr("Join Room"),
                             tr("Join room dialog placeholder."));
}

void MainWindow::onRoomSelected(const QModelIndex &index)
{
    if (!index.isValid())
        return;

    const QString roomName = m_chatListModel->data(index, Qt::DisplayRole).toString();
    m_messageArea->clear();
    m_messageArea->append(
        QStringLiteral("<b>%1</b>").arg(roomName.toHtmlEscaped()));
    m_messageInput->clear();
    m_messageInput->setFocus();

    m_centralStack->setCurrentIndex(1);
}

void MainWindow::onProtocolTabChanged(int index)
{
    if (m_centralStack->parent() == m_protocolTabs) {
        Q_UNUSED(index);
        m_centralStack->setCurrentIndex(0);
    }
}

void MainWindow::onSendMessage()
{
    const QString text = m_messageInput->text().trimmed();
    if (text.isEmpty())
        return;

    m_messageArea->append(
        QStringLiteral("<span style='color:#2b579a;'><b>You:</b> %1</span>")
            .arg(text.toHtmlEscaped()));

    m_messageInput->clear();
    m_messageInput->setFocus();
}

void MainWindow::updateConnectionStates()
{
    const auto &pm = m_app.protocolManager();

    auto setDot = [](QLabel *label, bool connected) {
        label->setPixmap(makeDot(connected ? Qt::green : Qt::red));
    };

    setDot(m_matrixStatus, pm->overallConnectionState() == ConnectionState::CONNECTED);
    setDot(m_ircStatus, false);
    setDot(m_lemmyStatus, false);
}
