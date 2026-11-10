#include "system_tray_manager.hpp"

#include <QApplication>
#include <QEvent>
#include <QFont>
#include <QMenu>
#include <QPainter>
#include <QPixmap>

SystemTrayManager* SystemTrayManager::s_instance = nullptr;

SystemTrayManager* SystemTrayManager::instance()
{
    if (!s_instance) {
        s_instance = new SystemTrayManager(qApp);
    }
    return s_instance;
}

SystemTrayManager::SystemTrayManager(QObject* parent)
    : QObject(parent)
{
    m_trayAvailable = QSystemTrayIcon::isSystemTrayAvailable();

    m_protocolStatus[ProtocolType::Matrix] = false;
    m_protocolStatus[ProtocolType::IRC] = false;
    m_protocolStatus[ProtocolType::Lemmy] = false;

    if (m_trayAvailable) {
        createTrayIcon();
        createContextMenu();
        m_trayIcon->setContextMenu(m_menu);
        m_trayIcon->show();
    }
}

SystemTrayManager::~SystemTrayManager()
{
    s_instance = nullptr;
}

bool SystemTrayManager::eventFilter(QObject* obj, QEvent* event)
{
    if (obj == m_window && (event->type() == QEvent::Show || event->type() == QEvent::Hide)) {
        updateShowHideAction();
    }
    return QObject::eventFilter(obj, event);
}

void SystemTrayManager::setWindow(QWidget* window)
{
    if (m_window) {
        m_window->removeEventFilter(this);
    }

    m_window = window;

    if (m_window) {
        m_window->installEventFilter(this);
    }

    updateShowHideAction();
}

void SystemTrayManager::setConnected(bool connected, ProtocolType protocol)
{
    m_protocolStatus[protocol] = connected;
    updateStatusTexts();
}

void SystemTrayManager::setUnreadCount(int count)
{
    m_unreadCount = count;
    updateUnreadAction();
}

void SystemTrayManager::showBalloon(const QString& title, const QString& message)
{
    if (m_trayAvailable && m_trayIcon) {
        m_trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 5000);
    }
}

void SystemTrayManager::onActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        onShowHideWindow();
        break;
    case QSystemTrayIcon::MiddleClick:
        break;
    case QSystemTrayIcon::Context:
        break;
    case QSystemTrayIcon::Unknown:
        break;
    }
}

void SystemTrayManager::onShowHideWindow()
{
    if (!m_window) {
        return;
    }

    if (m_window->isVisible()) {
        emit hideWindowRequested();
    } else {
        emit showWindowRequested();
    }
}

void SystemTrayManager::createTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    m_trayIcon->setIcon(generateTrayIcon());
    m_trayIcon->setToolTip(QStringLiteral("PCP"));

    connect(m_trayIcon, &QSystemTrayIcon::activated,
            this, &SystemTrayManager::onActivated);
}

void SystemTrayManager::createContextMenu()
{
    m_menu = new QMenu();

    m_showHideAction = m_menu->addAction(QString());
    connect(m_showHideAction, &QAction::triggered,
            this, &SystemTrayManager::onShowHideWindow);

    m_menu->addSeparator();

    updateStatusTexts();

    m_menu->addSeparator();

    m_unreadAction = m_menu->addAction(QString());
    m_unreadAction->setEnabled(false);

    m_menu->addSeparator();

    m_settingsAction = m_menu->addAction(QStringLiteral("Settings"));
    connect(m_settingsAction, &QAction::triggered, this, [this]() {
        emit showWindowRequested();
    });

    m_quitAction = m_menu->addAction(QStringLiteral("Quit"));
    connect(m_quitAction, &QAction::triggered, this, [this]() {
        emit quitRequested();
    });

    updateShowHideAction();
    updateUnreadAction();
}

QIcon SystemTrayManager::generateTrayIcon() const
{
    constexpr int size = 32;
    QPixmap pixmap(size, size);
    pixmap.fill(QColor(59, 130, 246));

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);

    QFont font(QStringLiteral("sans-serif"), 12, QFont::Bold);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(pixmap.rect(), Qt::AlignCenter, QStringLiteral("PC"));
    painter.end();

    return QIcon(pixmap);
}

QIcon SystemTrayManager::generateDotIcon(const QColor& color)
{
    constexpr int size = 16;
    QPixmap pixmap(size, size);
    pixmap.fill(Qt::transparent);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(color);
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(2, 2, size - 4, size - 4);
    painter.end();

    return QIcon(pixmap);
}

void SystemTrayManager::updateShowHideAction()
{
    if (!m_showHideAction) {
        return;
    }

    bool visible = m_window && m_window->isVisible();
    m_showHideAction->setText(visible ? QStringLiteral("Hide Window")
                                      : QStringLiteral("Show Window"));
}

void SystemTrayManager::updateStatusTexts()
{
    if (!m_menu) {
        return;
    }

    // Clean up old protocol actions and the old status action
    if (m_statusAction) {
        m_menu->removeAction(m_statusAction);
        m_statusAction->deleteLater();
        m_statusAction = nullptr;
    }
    for (auto* action : m_protocolActions) {
        m_menu->removeAction(action);
        action->deleteLater();
    }
    m_protocolActions.clear();

    updateOverallStatus();

    // Rebuild status section between the two separators is complex because
    // the separators have been added. Instead, clear and recreate the whole menu.
    // A simpler approach: store the separators and re-insert.

    // Reconstruct from scratch for simplicity and correctness.
    m_menu->clear();

    m_showHideAction = m_menu->addAction(QString());
    connect(m_showHideAction, &QAction::triggered,
            this, &SystemTrayManager::onShowHideWindow);
    updateShowHideAction();

    m_menu->addSeparator();

    m_statusAction = m_menu->addAction(QString());
    m_statusAction->setEnabled(false);

    static const QList<ProtocolType> protocolOrder = {
        ProtocolType::Matrix,
        ProtocolType::IRC,
        ProtocolType::Lemmy
    };
    static const QMap<ProtocolType, QString> protocolNames = {
        {ProtocolType::Matrix, QStringLiteral("Matrix")},
        {ProtocolType::IRC, QStringLiteral("IRC")},
        {ProtocolType::Lemmy, QStringLiteral("Lemmy")}
    };

    for (ProtocolType proto : protocolOrder) {
        bool conn = m_protocolStatus.value(proto, false);
        QIcon dot = generateDotIcon(conn ? QColor(34, 197, 94) : QColor(239, 68, 68));
        QAction* action = m_menu->addAction(dot, QStringLiteral("  %1").arg(protocolNames.value(proto)));
        action->setEnabled(false);
        m_protocolActions[proto] = action;
    }

    m_menu->addSeparator();

    m_unreadAction = m_menu->addAction(QString());
    m_unreadAction->setEnabled(false);

    m_menu->addSeparator();

    m_settingsAction = m_menu->addAction(QStringLiteral("Settings"));
    connect(m_settingsAction, &QAction::triggered, this, [this]() {
        emit showWindowRequested();
    });

    m_quitAction = m_menu->addAction(QStringLiteral("Quit"));
    connect(m_quitAction, &QAction::triggered, this, [this]() {
        emit quitRequested();
    });

    updateUnreadAction();
}

void SystemTrayManager::updateUnreadAction()
{
    if (!m_unreadAction) {
        return;
    }
    m_unreadAction->setText(QStringLiteral("Unread: %1 message%2")
                                .arg(m_unreadCount)
                                .arg(m_unreadCount == 1 ? QString() : QStringLiteral("s")));
}

void SystemTrayManager::updateOverallStatus()
{
    if (!m_statusAction) {
        return;
    }

    bool anyConnected = false;
    for (auto it = m_protocolStatus.cbegin(); it != m_protocolStatus.cend(); ++it) {
        if (it.value()) {
            anyConnected = true;
            break;
        }
    }

    m_statusAction->setText(anyConnected ? QStringLiteral("Status: Connected")
                                         : QStringLiteral("Status: Disconnected"));
}
