#include "features/matrix_chat/account_switcher.hpp"

#include <QApplication>
#include <QClipboard>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QPainter>
#include <QPixmap>
#include <QPushButton>
#include <QWidgetAction>
#include <QStyle>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "irc/irc_session.hpp"
#include "irc/irc_session_holder.hpp"
#include "lemmy/lemmy_session.hpp"
#include "lemmy/lemmy_session_holder.hpp"
#include "matrix/matrix_session.hpp"

namespace pcp {

// ─── Connection dot painter ──────────────────────────────────────────────────

namespace {

class ConnectionDotWidget : public QWidget
{
public:
    explicit ConnectionDotWidget(const QColor &color, QWidget *parent = nullptr)
        : QWidget(parent)
        , m_color(color)
    {
        setFixedSize(10, 10);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(m_color);
        p.setPen(Qt::NoPen);
        p.drawEllipse(rect().adjusted(1, 1, -1, -1));
    }

private:
    QColor m_color;
};

class SwitcherAvatarWidget : public QWidget
{
public:
    SwitcherAvatarWidget(const QString &name, const QColor &bg, int size, QWidget *parent = nullptr)
        : QWidget(parent)
        , m_name(name)
        , m_bg(bg)
        , m_size(size)
    {
        setFixedSize(size, size);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);
        p.setBrush(m_bg);
        p.setPen(Qt::NoPen);
        p.drawEllipse(rect().adjusted(1, 1, -1, -1));

        QFont f;
        f.setPixelSize(m_size / 3);
        f.setBold(true);
        p.setFont(f);
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, initialsFromName(m_name));
    }

private:
    static QString initialsFromName(const QString &name)
    {
        if (name.isEmpty())
            return QStringLiteral("?");

        const QStringList parts = name.split(QLatin1Char(' '), Qt::SkipEmptyParts);
        if (parts.size() >= 2)
            return QString(parts.at(0).at(0).toUpper()) + parts.at(1).at(0).toUpper();

        const QString &word = parts.at(0);
        if (word.length() >= 2)
            return QString(word.at(0).toUpper()) + word.at(1).toUpper();

        return QString(word.at(0).toUpper());
    }

    QString m_name;
    QColor m_bg;
    int m_size;
};

} // anonymous namespace

// ─── AccountSwitcher ─────────────────────────────────────────────────────────

AccountSwitcher::AccountSwitcher(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    refreshAccounts();
}

AccountSwitcher::~AccountSwitcher() = default;

// ─── Public API ──────────────────────────────────────────────────────────────

QVector<AccountEntry> AccountSwitcher::accounts() const
{
    return m_accounts;
}

AccountEntry AccountSwitcher::currentAccount() const
{
    for (const auto &entry : m_accounts) {
        if (entry.accountId == m_currentAccountId &&
            entry.protocolType == m_currentProtocolType)
            return entry;
    }
    return {};
}

void AccountSwitcher::setCurrentAccount(const QString &accountId, ProtocolType type)
{
    m_currentAccountId = accountId;
    m_currentProtocolType = type;
    updateToggleAppearance();
}

// ─── Slots ───────────────────────────────────────────────────────────────────

void AccountSwitcher::refreshAccounts()
{
    m_accounts.clear();

    Application &app = Application::instance();

    // Matrix session
    if (auto *mx = app.matrixSession()) {
        AccountEntry entry;
        entry.protocolType = ProtocolType::MATRIX;
        entry.accountId = mx->userId();
        entry.displayName = mx->displayName().isEmpty() ? mx->userId() : mx->displayName();
        entry.serverName = mx->homeserverUrl();
        entry.connectionState = mx->connectionState();
        m_accounts.append(entry);
    }

    // IRC sessions
    if (auto *ircHolder = app.ircSessionHolder()) {
        const auto ircSessions = ircHolder->getAllSessions();
        for (auto *irc : ircSessions) {
            AccountEntry entry;
            entry.protocolType = ProtocolType::IRC;
            entry.accountId = irc->userId();
            entry.displayName = irc->displayName().isEmpty() ? irc->userId() : irc->displayName();
            entry.serverName = irc->userId().split(QLatin1Char('!')).value(0);
            entry.connectionState = irc->connectionState();
            m_accounts.append(entry);
        }
    }

    // Lemmy sessions
    if (auto *lemmyHolder = app.lemmySessionHolder()) {
        const auto lemmySessions = lemmyHolder->getAllSessions();
        for (auto *lemmy : lemmySessions) {
            AccountEntry entry;
            entry.protocolType = ProtocolType::LEMMY;
            entry.accountId = lemmy->userId();
            entry.displayName = lemmy->displayName().isEmpty() ? lemmy->userId() : lemmy->displayName();
            // Extract instance url from userId or displayName
            entry.serverName = lemmy->userId();
            entry.connectionState = lemmy->connectionState();
            m_accounts.append(entry);
        }
    }

    // Default to first account if nothing selected
    if (!m_accounts.isEmpty() && m_currentAccountId.isEmpty()) {
        m_currentAccountId = m_accounts.at(0).accountId;
        m_currentProtocolType = m_accounts.at(0).protocolType;
    }

    rebuildMenu();
    updateToggleAppearance();
}

void AccountSwitcher::onConnectionStateChanged(ProtocolType type, ConnectionState state)
{
    for (auto &entry : m_accounts) {
        if (entry.protocolType == type) {
            entry.connectionState = state;
        }
    }
    rebuildMenu();
    updateToggleAppearance();
}

// ─── Private slots ───────────────────────────────────────────────────────────

void AccountSwitcher::onToggleClicked()
{
    rebuildMenu();

    const QPoint pos = m_toggleButton->mapToGlobal(
        QPoint(0, m_toggleButton->height() + 2));
    m_menu->setMinimumWidth(m_toggleButton->width());
    m_menu->exec(pos);
}

void AccountSwitcher::onAccountActionTriggered()
{
    auto *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    const QString accountId = action->data().toString();
    const ProtocolType type = static_cast<ProtocolType>(action->property("protocolType").toInt());

    if (accountId == m_currentAccountId && type == m_currentProtocolType)
        return;

    m_currentAccountId = accountId;
    m_currentProtocolType = type;
    updateToggleAppearance();
    emit accountSwitched(type, accountId);
}

void AccountSwitcher::onAddAccountTriggered()
{
    const QStringList protocols = {
        tr("Matrix"),
        tr("IRC"),
        tr("Lemmy"),
        tr("Telegram"),
        tr("DeltaChat")
    };

    bool ok = false;
    const QString choice = QInputDialog::getItem(
        this, tr("Add Account"),
        tr("Select protocol:"), protocols, 0, false, &ok);

    if (!ok || choice.isEmpty())
        return;

    ProtocolType pt = ProtocolType::MATRIX;
    if (choice == tr("Matrix"))    pt = ProtocolType::MATRIX;
    else if (choice == tr("IRC"))       pt = ProtocolType::IRC;
    else if (choice == tr("Lemmy"))     pt = ProtocolType::LEMMY;
    else if (choice == tr("Telegram"))  pt = ProtocolType::TELEGRAM;
    else if (choice == tr("DeltaChat")) pt = ProtocolType::DELTACHAT;

    emit addAccountRequested(pt);
}

void AccountSwitcher::onLogoutRequested()
{
    auto *action = qobject_cast<QAction *>(sender());
    if (!action)
        return;

    const QString accountId = action->data().toString();
    const ProtocolType type = static_cast<ProtocolType>(action->property("protocolType").toInt());

    const int ret = QMessageBox::question(
        this, tr("Confirm Logout"),
        tr("Are you sure you want to log out of %1?\n\n"
           "Your local data will be preserved.")
            .arg(accountId),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (ret == QMessageBox::Yes) {
        emit logoutRequested(type, accountId);
    }
}

void AccountSwitcher::onSetStatusTriggered()
{
    QStringList presences = {tr("Online"), tr("Away"), tr("Busy"), tr("Invisible")};
    bool ok = false;
    const QString choice = QInputDialog::getItem(
        this, tr("Set Status"),
        tr("Select presence:"), presences, 0, false, &ok);

    if (!ok)
        return;

    QString matrixPresence;
    if (choice == tr("Online"))       matrixPresence = QStringLiteral("online");
    else if (choice == tr("Away"))    matrixPresence = QStringLiteral("unavailable");
    else if (choice == tr("Busy"))    matrixPresence = QStringLiteral("busy");
    else if (choice == tr("Invisible")) matrixPresence = QStringLiteral("offline");
    else
        return;

    if (auto *mx = Application::instance().matrixSession()) {
        // Matrix presence API would be called here
        Q_UNUSED(matrixPresence);
        Q_UNUSED(mx);
    }
}

void AccountSwitcher::onEditProfileTriggered()
{
    emit profileEditRequested();
}

void AccountSwitcher::onViewProfileTriggered()
{
    emit profileEditRequested();
}

// ─── UI Setup ────────────────────────────────────────────────────────────────

void AccountSwitcher::setupUi()
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_toggleButton = new QPushButton(this);
    m_toggleButton->setCursor(Qt::PointingHandCursor);
    m_toggleButton->setStyleSheet(
        QStringLiteral("QPushButton { border: 1px solid #d1d5db; border-radius: 20px; "
                       "padding: 4px 10px 4px 6px; background: #ffffff; }"
                       "QPushButton:hover { background: #f3f4f6; border-color: #9ca3af; }"
                       "QPushButton:pressed { background: #e5e7eb; }"));

    layout->addWidget(m_toggleButton);

    connect(m_toggleButton, &QPushButton::clicked,
            this, &AccountSwitcher::onToggleClicked);

    m_menu = new QMenu(this);
    m_menu->setStyleSheet(
        QStringLiteral("QMenu { background: #ffffff; border: 1px solid #d1d5db; "
                       "border-radius: 8px; padding: 4px; }"
                       "QMenu::item { padding: 8px 32px 8px 12px; "
                       "border-radius: 4px; }"
                       "QMenu::item:selected { background: #f3f4f6; }"
                       "QMenu::separator { height: 1px; background: #e5e7eb; "
                       "margin: 4px 8px; }"));
}

void AccountSwitcher::rebuildMenu()
{
    m_menu->clear();

    // Section header
    auto *headerAction = m_menu->addAction(tr("Accounts"));
    headerAction->setEnabled(false);
    QFont headerFont = headerAction->font();
    headerFont.setBold(true);
    headerFont.setPointSize(headerFont.pointSize() - 1);
    headerAction->setFont(headerFont);

    for (const auto &entry : m_accounts) {
        QAction *act = createAccountAction(entry);
        m_menu->addAction(act);

        // Submenu for each account
        auto *subMenu = m_menu->addMenu(tr("  ⚙ Account options"));
        subMenu->setStyleSheet(m_menu->styleSheet());

        auto *logoutAction = subMenu->addAction(tr("Logout"));
        logoutAction->setData(entry.accountId);
        logoutAction->setProperty("protocolType", static_cast<int>(entry.protocolType));
        connect(logoutAction, &QAction::triggered,
                this, &AccountSwitcher::onLogoutRequested);
    }

    m_menu->addSeparator();

    // Add Account
    auto *addAction = m_menu->addAction(tr("+ Add Account"));
    addAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    connect(addAction, &QAction::triggered,
            this, &AccountSwitcher::onAddAccountTriggered);

    // Profile menu
    m_menu->addSeparator();
    m_menu->addMenu(createProfileMenu());
}

QMenu *AccountSwitcher::createProfileMenu()
{
    auto *menu = new QMenu(tr("Profile"), m_menu);

    auto *viewProfile = menu->addAction(tr("View Profile"));
    connect(viewProfile, &QAction::triggered,
            this, &AccountSwitcher::onViewProfileTriggered);

    auto *setStatus = menu->addAction(tr("Set Status"));
    connect(setStatus, &QAction::triggered,
            this, &AccountSwitcher::onSetStatusTriggered);

    menu->addSeparator();

    auto *editProfile = menu->addAction(tr("Edit Profile"));
    connect(editProfile, &QAction::triggered,
            this, &AccountSwitcher::onEditProfileTriggered);

    return menu;
}

QAction *AccountSwitcher::createAccountAction(const AccountEntry &entry)
{
    const QColor color = protocolColor(entry.protocolType);

    auto *widgetAction = new QWidgetAction(m_menu);
    widgetAction->setData(entry.accountId);
    widgetAction->setProperty("protocolType", static_cast<int>(entry.protocolType));

    // Build rich text label
    QString dotColor = statusDotColor(entry.connectionState).name();
    QString labelText = QStringLiteral(
        "<span style='color:%1; font-weight:bold;'>%2</span> &nbsp; "
        "<span style='color:#6b7280; font-size:10px;'>[%3]</span><br>"
        "<span style='color:#374151; font-size:11px;'>%4</span>")
        .arg(color.name(), protocolDisplayName(entry.protocolType),
             entry.serverName, entry.displayName);

    // Active account indicator
    bool isActive = (entry.accountId == m_currentAccountId &&
                     entry.protocolType == m_currentProtocolType);

    if (isActive) {
        labelText = QStringLiteral(
            "<span style='color:%1; font-size:11px;'>●</span> %2")
            .arg(statusDotColor(entry.connectionState).name(), labelText);
    }

    auto *widget = new QWidget;
    auto *layout = new QHBoxLayout(widget);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(8);

    const QColor avatarColor = color.lighter(150);
    auto *avatar = new SwitcherAvatarWidget(entry.displayName, avatarColor, 28, widget);
    layout->addWidget(avatar);

    auto *label = new QLabel(labelText, widget);
    label->setTextFormat(Qt::RichText);
    layout->addWidget(label, 1);

    auto *dot = new ConnectionDotWidget(statusDotColor(entry.connectionState), widget);
    layout->addWidget(dot);

    widgetAction->setDefaultWidget(widget);
    widgetAction->setData(entry.accountId);
    widgetAction->setProperty("protocolType", static_cast<int>(entry.protocolType));

    connect(widgetAction, &QAction::triggered,
            this, &AccountSwitcher::onAccountActionTriggered);

    return widgetAction;
}

void AccountSwitcher::updateToggleAppearance()
{
    QString accountName;
    QString serverName;
    ProtocolType proto = m_currentProtocolType;

    for (const auto &entry : m_accounts) {
        if (entry.accountId == m_currentAccountId &&
            entry.protocolType == m_currentProtocolType) {
            accountName = entry.displayName;
            serverName = entry.serverName;
            proto = entry.protocolType;
            break;
        }
    }

    const QColor color = protocolColor(proto);
    QString avatarInitials = initialsFromName(accountName);
    QPixmap avatar = makeAvatarPixmap(avatarInitials, 22, color);

    m_toggleButton->setIcon(QIcon(avatar));
    m_toggleButton->setIconSize(QSize(22, 22));

    QString btnText = accountName.isEmpty()
        ? tr("No Account")
        : QString(QChar(0x25BE)) + QLatin1Char(' ') + accountName;

    m_toggleButton->setText(btnText);
}

// ─── Static helpers ──────────────────────────────────────────────────────────

QColor AccountSwitcher::statusDotColor(ConnectionState state)
{
    switch (state) {
    case ConnectionState::CONNECTED:
    case ConnectionState::REGISTERED:
        return QColor(0x22, 0xC5, 0x5E);  // green
    case ConnectionState::CONNECTING:
        return QColor(0xF5, 0x9E, 0x0B);  // amber
    case ConnectionState::ERROR:
        return QColor(0xEF, 0x44, 0x44);  // red
    case ConnectionState::DISCONNECTED:
    default:
        return QColor(0x9C, 0xA3, 0xAF);  // grey
    }
}

QString AccountSwitcher::protocolDisplayName(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:    return QStringLiteral("Matrix");
    case ProtocolType::IRC:       return QStringLiteral("IRC");
    case ProtocolType::LEMMY:     return QStringLiteral("Lemmy");
    case ProtocolType::TELEGRAM:  return QStringLiteral("Telegram");
    case ProtocolType::DELTACHAT: return QStringLiteral("DeltaChat");
    }
    return QStringLiteral("Unknown");
}

QString AccountSwitcher::protocolIconChar(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:    return QStringLiteral("[m]");
    case ProtocolType::IRC:       return QStringLiteral("(#)");
    case ProtocolType::LEMMY:     return QStringLiteral("L");
    case ProtocolType::TELEGRAM:  return QStringLiteral("T");
    case ProtocolType::DELTACHAT: return QStringLiteral("D");
    }
    return QStringLiteral("?");
}

QColor AccountSwitcher::protocolColor(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:    return QColor(0x10, 0xb9, 0x81);
    case ProtocolType::IRC:       return QColor(0x63, 0x66, 0xf1);
    case ProtocolType::LEMMY:     return QColor(0xf5, 0x9e, 0x0b);
    case ProtocolType::TELEGRAM:  return QColor(0x0e, 0xa5, 0xe9);
    case ProtocolType::DELTACHAT: return QColor(0x8b, 0x5c, 0xf6);
    }
    return QColor(0x6b, 0x72, 0x80);
}

QString AccountSwitcher::initialsFromName(const QString &name)
{
    if (name.isEmpty())
        return QStringLiteral("?");

    const QStringList parts = name.split(QLatin1Char(' '), Qt::SkipEmptyParts);
    if (parts.size() >= 2)
        return QString(parts.at(0).at(0).toUpper()) + parts.at(1).at(0).toUpper();

    const QString &word = parts.at(0);
    if (word.length() >= 2)
        return QString(word.at(0).toUpper()) + word.at(1).toUpper();

    return QString(word.at(0).toUpper());
}

QPixmap AccountSwitcher::makeAvatarPixmap(const QString &name, int size, const QColor &bg)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter p(&pix);
    p.setRenderHint(QPainter::Antialiasing);
    p.setBrush(bg);
    p.setPen(Qt::NoPen);
    p.drawEllipse(1, 1, size - 2, size - 2);

    QFont f;
    f.setPixelSize(size / 3);
    f.setBold(true);
    p.setFont(f);
    p.setPen(Qt::white);
    p.drawText(QRect(0, 0, size, size), Qt::AlignCenter, initialsFromName(name));
    p.end();

    return pix;
}

} // namespace pcp
