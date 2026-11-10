#pragma once

#include <QMainWindow>

class QTabWidget;
class QStackedWidget;
class QListWidget;
class QTextBrowser;
class QLineEdit;
class QLabel;
class QToolBar;
class QStatusBar;
class QAction;
class QPushButton;
class QSplitter;
class QListView;
class QStringListModel;

namespace pcp {
class Application;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

public slots:
    void updateConnectionStates();

private slots:
    void onSettingsTriggered();
    void onQuitTriggered();
    void onAddMatrixAccount();
    void onAddIrcServer();
    void onAddLemmyInstance();
    void onNewChat();
    void onJoinRoom();
    void onRoomSelected(const QModelIndex &index);
    void onProtocolTabChanged(int index);
    void onSendMessage();

private:
    void setupUi();
    void setupMenuBar();
    void setupToolBar();
    void setupStatusBar();
    void setupCentralWidget();
    void connectSignals();

    QLabel *createConnectionIndicator(const QString &name);

    pcp::Application &m_app;

    QTabWidget *m_protocolTabs;

    QStackedWidget *m_centralStack;

    QWidget *m_chatListPage;
    QSplitter *m_chatListSplitter;
    QListView *m_chatListView;
    QStringListModel *m_chatListModel;

    QWidget *m_chatDetailPage;
    QTextBrowser *m_messageArea;
    QLineEdit *m_messageInput;
    QPushButton *m_sendButton;
    QPushButton *m_backButton;

    QWidget *m_settingsPage;

    QToolBar *m_toolBar;
    QAction *m_newChatAction;
    QAction *m_joinRoomAction;

    QLabel *m_matrixStatus;
    QLabel *m_ircStatus;
    QLabel *m_lemmyStatus;

    QAction *m_settingsAction;
    QAction *m_quitAction;
    QAction *m_addMatrixAction;
    QAction *m_addIrcAction;
    QAction *m_addLemmyAction;
};
