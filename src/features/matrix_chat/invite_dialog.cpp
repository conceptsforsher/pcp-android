#include "features/matrix_chat/invite_dialog.hpp"

#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QRegularExpression>
#include <QScrollArea>
#include <QSettings>
#include <QStackedWidget>
#include <QTcpSocket>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "irc/irc_session.hpp"
#include "irc/irc_session_holder.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

namespace {

constexpr int kSearchDebounceMs = 300;
constexpr int kMaxRecentContacts = 20;
constexpr int kAvatarSize = 36;

// ─── WrapLayout: simple wrapping layout for chip badges ──────────────────────

class WrapLayout : public QLayout
{
    Q_OBJECT
public:
    explicit WrapLayout(QWidget *parent = nullptr)
        : QLayout(parent)
    {
    }

    ~WrapLayout() override
    {
        while (auto *item = takeAt(0))
            delete item;
    }

    void addItem(QLayoutItem *item) override
    {
        m_items.append(item);
    }

    [[nodiscard]] int count() const override
    {
        return m_items.size();
    }

    [[nodiscard]] QLayoutItem *itemAt(int index) const override
    {
        if (index < 0 || index >= m_items.size())
            return nullptr;
        return m_items.at(index);
    }

    QLayoutItem *takeAt(int index) override
    {
        if (index < 0 || index >= m_items.size())
            return nullptr;
        return m_items.takeAt(index);
    }

    [[nodiscard]] QSize sizeHint() const override
    {
        return minimumSize();
    }

    [[nodiscard]] QSize minimumSize() const override
    {
        QSize size;
        for (const auto *item : m_items)
            size = size.expandedTo(item->minimumSize());
        const QMargins m = contentsMargins();
        size += QSize(m.left() + m.right(), m.top() + m.bottom());
        return size;
    }

    void setGeometry(const QRect &rect) override
    {
        QLayout::setGeometry(rect);

        if (m_items.isEmpty())
            return;

        const QMargins m = contentsMargins();
        const int maxWidth = rect.width() - m.left() - m.right();
        int x = 0;
        int y = 0;
        int rowHeight = 0;

        for (auto *item : std::as_const(m_items)) {
            const QSize sz = item->sizeHint();

            if (x > 0 && x + sz.width() > maxWidth) {
                x = 0;
                y += rowHeight + 4;
                rowHeight = 0;
            }

            item->setGeometry(QRect(m.left() + x, m.top() + y, sz.width(), sz.height()));
            x += sz.width() + 4;
            rowHeight = std::max(rowHeight, sz.height());
        }
    }

private:
    QVector<QLayoutItem *> m_items;
};

// ─── AvatarWidget: simple circular avatar with initials or solid color ───────

class MiniAvatarWidget : public QWidget
{
public:
    MiniAvatarWidget(const QString &name, QWidget *parent = nullptr)
        : QWidget(parent)
        , m_name(name)
        , m_color(InviteDialog::avatarColorFromName(name))
    {
        setFixedSize(kAvatarSize, kAvatarSize);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        p.setBrush(m_color);
        p.setPen(Qt::NoPen);
        p.drawEllipse(rect().adjusted(1, 1, -1, -1));

        QFont font;
        font.setPixelSize(14);
        font.setBold(true);
        p.setFont(font);
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, InviteDialog::initialsFromName(m_name));
    }

private:
    QString m_name;
    QColor m_color;
};

// ─── ChipWidget: removable badge showing a selected user ─────────────────────

class ChipWidget : public QFrame
{
    Q_OBJECT

public:
    ChipWidget(const UserInfo &user, QWidget *parent = nullptr)
        : QFrame(parent)
        , m_userId(user.userId)
    {
        setFrameShape(QFrame::StyledPanel);
        setStyleSheet(QStringLiteral(
            "ChipWidget { background: #e0e7ff; border: 1px solid #a5b4fc; "
            "border-radius: 12px; padding: 2px; }"));

        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(6, 2, 4, 2);
        layout->setSpacing(4);

        auto *nameLabel = new QLabel(user.displayName.isEmpty() ? user.userId : user.displayName, this);
        nameLabel->setStyleSheet(QStringLiteral("color: #3730a3; font-size: 12px; border: none; background: transparent;"));
        layout->addWidget(nameLabel);

        auto *removeButton = new QPushButton(QStringLiteral("\u00d7"), this);
        removeButton->setFixedSize(18, 18);
        removeButton->setStyleSheet(QStringLiteral(
            "QPushButton { background: transparent; border: none; color: #6366f1; "
            "font-size: 14px; font-weight: bold; }"
            "QPushButton:hover { color: #dc2626; }"));
        removeButton->setCursor(Qt::PointingHandCursor);
        layout->addWidget(removeButton);

        connect(removeButton, &QPushButton::clicked, this, [this]() {
            emit removeRequested(m_userId);
        });

        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    }

    [[nodiscard]] QString userId() const { return m_userId; }

signals:
    void removeRequested(const QString &userId);

private:
    QString m_userId;
};

} // anonymous namespace

// ─── InviteDialog implementation ─────────────────────────────────────────────

InviteDialog::InviteDialog(const QString &roomId, ProtocolType protocolType, QWidget *parent)
    : QDialog(parent)
    , m_roomId(roomId)
    , m_protocolType(protocolType)
    , m_nam(new QNetworkAccessManager(this))
    , m_searchTimer(new QTimer(this))
{
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(kSearchDebounceMs);
    connect(m_searchTimer, &QTimer::timeout, this, &InviteDialog::onSearchDebounceTimeout);

    setupUi();

    if (m_protocolType == ProtocolType::MATRIX) {
        loadRecentContacts();
    }
}

void InviteDialog::setupUi()
{
    setWindowTitle(tr("Invite Users"));
    setMinimumSize(480, 520);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(10);

    m_contentStack = new QStackedWidget(this);
    m_contentStack->addWidget(createMatrixInvitePage());
    m_contentStack->addWidget(createIrcInvitePage());
    m_contentStack->addWidget(createLemmyInvitePage());
    mainLayout->addWidget(m_contentStack, 1);

    switch (m_protocolType) {
    case ProtocolType::IRC:
        m_contentStack->setCurrentIndex(1);
        break;
    case ProtocolType::LEMMY:
        m_contentStack->setCurrentIndex(2);
        break;
    default:
        m_contentStack->setCurrentIndex(0);
        break;
    }

    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    m_progressBar->setMaximumHeight(6);
    m_progressBar->setTextVisible(false);
    mainLayout->addWidget(m_progressBar);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setVisible(false);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 12px;"));
    mainLayout->addWidget(m_statusLabel);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    auto *cancelButton = new QPushButton(tr("Cancel"), this);
    m_inviteButton = new QPushButton(tr("Invite"), this);
    m_inviteButton->setDefault(true);
    m_inviteButton->setEnabled(false);

    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(m_inviteButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_inviteButton, &QPushButton::clicked, this, &InviteDialog::onInviteClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

QWidget *InviteDialog::createMatrixInvitePage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(8);

    auto *headerLabel = new QLabel(tr("Search for users to invite to this room:"), page);
    headerLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
    layout->addWidget(headerLabel);

    m_searchEdit = new QLineEdit(page);
    m_searchEdit->setPlaceholderText(tr("Search by name or Matrix ID (@user:server)..."));
    m_searchEdit->setClearButtonEnabled(true);
    layout->addWidget(m_searchEdit);

    m_recentHeaderLabel = new QLabel(tr("Recent Contacts"), page);
    m_recentHeaderLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px; font-weight: bold;"));
    m_recentHeaderLabel->setVisible(false);
    layout->addWidget(m_recentHeaderLabel);

    m_resultsList = new QListWidget(page);
    m_resultsList->setSelectionMode(QAbstractItemView::NoSelection);
    m_resultsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_resultsList->setStyleSheet(QStringLiteral(
        "QListWidget { border: 1px solid #d1d5db; border-radius: 6px; }"
        "QListWidget::item { border-bottom: 1px solid #e5e7eb; padding: 4px; }"));
    layout->addWidget(m_resultsList, 1);

    auto *selectedLabel = new QLabel(tr("Selected:"), page);
    selectedLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px; font-weight: bold;"));
    layout->addWidget(selectedLabel);

    auto *chipScrollArea = new QScrollArea(page);
    chipScrollArea->setWidgetResizable(true);
    chipScrollArea->setMaximumHeight(80);
    chipScrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    chipScrollArea->setFrameShape(QFrame::NoFrame);

    m_chipContainer = new QWidget(chipScrollArea);
    m_chipContainer->setLayout(new WrapLayout);
    chipScrollArea->setWidget(m_chipContainer);
    layout->addWidget(chipScrollArea);

    connect(m_searchEdit, &QLineEdit::textChanged, this, &InviteDialog::onSearchTextChanged);
    connect(m_resultsList, &QListWidget::itemChanged, this, &InviteDialog::onResultItemChanged);

    return page;
}

QWidget *InviteDialog::createIrcInvitePage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    auto *headerLabel = new QLabel(tr("Invite a user to this IRC channel by nickname:"), page);
    headerLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
    layout->addWidget(headerLabel);

    auto *infoLabel = new QLabel(
        tr("Enter the nickname of the user you want to invite to %1.").arg(m_roomId),
        page);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 12px;"));
    layout->addWidget(infoLabel);

    m_ircNickEdit = new QLineEdit(page);
    m_ircNickEdit->setPlaceholderText(tr("e.g. someuser"));
    layout->addWidget(m_ircNickEdit);

    m_ircNotifyCheck = new QCheckBox(tr("Notify user via private message about the invitation"), page);
    m_ircNotifyCheck->setChecked(true);
    layout->addWidget(m_ircNotifyCheck);

    layout->addStretch();

    connect(m_ircNickEdit, &QLineEdit::textChanged, this, [this]() {
        updateSelectedCount();
    });

    return page;
}

QWidget *InviteDialog::createLemmyInvitePage()
{
    auto *page = new QWidget(this);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(10);

    auto *headerLabel = new QLabel(tr("Invite Users"), page);
    headerLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 14px;"));
    layout->addWidget(headerLabel);

    auto *infoLabel = new QLabel(
        tr("Lemmy communities do not support direct user invitations. "
           "Users can discover and join communities through the Lemmy "
           "interface or via federation."),
        page);
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 12px; padding: 12px;"));
    layout->addWidget(infoLabel);

    layout->addStretch();

    return page;
}

// ─── Search ──────────────────────────────────────────────────────────────────

void InviteDialog::onSearchTextChanged(const QString &text)
{
    m_searchTimer->start();
}

void InviteDialog::onSearchDebounceTimeout()
{
    const QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty()) {
        showRecentContacts();
        return;
    }
    performSearch(query);
}

void InviteDialog::performSearch(const QString &query)
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        m_statusLabel->setText(tr("No Matrix session available."));
        m_statusLabel->setVisible(true);
        return;
    }

    m_progressBar->setVisible(true);
    m_statusLabel->setVisible(false);
    clearResultsList();
    m_recentHeaderLabel->setVisible(false);

    QJsonObject body;
    body[QStringLiteral("search_term")] = query;
    body[QStringLiteral("limit")] = 20;

    QUrl url(session->homeserverUrl() + QStringLiteral("/_matrix/client/r0/user_directory/search"));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        m_progressBar->setVisible(false);

        if (reply->error() != QNetworkReply::NoError) {
            m_statusLabel->setText(tr("Search error: %1").arg(reply->errorString()));
            m_statusLabel->setVisible(true);
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        const QJsonArray results = JsonUtil::optArray(json, QStringLiteral("results"));

        if (results.isEmpty()) {
            m_statusLabel->setText(tr("No users found matching \"%1\".").arg(
                m_searchEdit->text().trimmed()));
            m_statusLabel->setVisible(true);
            return;
        }

        for (const auto &val : results) {
            const QJsonObject entry = val.toObject();
            UserInfo user;
            user.userId = JsonUtil::optString(entry, QStringLiteral("user_id"));
            user.displayName = JsonUtil::optString(entry, QStringLiteral("display_name"));
            user.avatarUrl = JsonUtil::optString(entry, QStringLiteral("avatar_url"));

            if (!user.userId.isEmpty())
                addResultItem(user);
        }

        if (m_resultsList->count() == 0 && results.isEmpty()) {
            m_statusLabel->setText(tr("No users found matching \"%1\".").arg(
                m_searchEdit->text().trimmed()));
            m_statusLabel->setVisible(true);
        }
    });
}

// ─── Recent Contacts ─────────────────────────────────────────────────────────

void InviteDialog::loadRecentContacts()
{
    QSettings settings(QStringLiteral("PCP"), QStringLiteral("PCP"));
    const QByteArray raw = settings.value(QStringLiteral("recentContacts")).toByteArray();

    if (raw.isEmpty())
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isArray())
        return;

    m_recentContacts.clear();
    const QJsonArray arr = doc.array();
    for (const auto &val : arr) {
        const QJsonObject obj = val.toObject();
        UserInfo user;
        user.userId = JsonUtil::optString(obj, QStringLiteral("userId"));
        user.displayName = JsonUtil::optString(obj, QStringLiteral("displayName"));
        user.avatarUrl = JsonUtil::optString(obj, QStringLiteral("avatarUrl"));
        if (!user.userId.isEmpty() && isValidMatrixId(user.userId))
            m_recentContacts.append(user);
    }
}

void InviteDialog::showRecentContacts()
{
    clearResultsList();

    if (m_recentContacts.isEmpty()) {
        m_recentHeaderLabel->setVisible(false);
        m_statusLabel->setText(tr("No recent contacts. Type to search for users."));
        m_statusLabel->setVisible(true);
        return;
    }

    m_recentHeaderLabel->setVisible(true);
    m_statusLabel->setVisible(false);

    for (const auto &user : std::as_const(m_recentContacts)) {
        UserInfo u = user;
        u.isRecent = true;
        addResultItem(u);
    }
}

void InviteDialog::saveRecentContact(const UserInfo &user)
{
    if (!isValidMatrixId(user.userId))
        return;

    // Remove existing entry if present
    auto it = std::find_if(m_recentContacts.begin(), m_recentContacts.end(),
        [&user](const UserInfo &u) { return u.userId == user.userId; });
    if (it != m_recentContacts.end())
        m_recentContacts.erase(it);

    // Insert at front
    m_recentContacts.prepend(user);

    // Trim to max
    while (m_recentContacts.size() > kMaxRecentContacts)
        m_recentContacts.removeLast();

    // Persist
    QJsonArray arr;
    for (const auto &u : std::as_const(m_recentContacts)) {
        QJsonObject obj;
        obj[QStringLiteral("userId")] = u.userId;
        obj[QStringLiteral("displayName")] = u.displayName;
        obj[QStringLiteral("avatarUrl")] = u.avatarUrl;
        arr.append(obj);
    }

    QSettings settings(QStringLiteral("PCP"), QStringLiteral("PCP"));
    settings.setValue(QStringLiteral("recentContacts"), QString::fromUtf8(
        QJsonDocument(arr).toJson(QJsonDocument::Compact)));
}

// ─── Result list management ──────────────────────────────────────────────────

void InviteDialog::clearResultsList()
{
    // Block signals to avoid triggering itemChanged during clear
    m_resultsList->blockSignals(true);
    m_resultsList->clear();
    m_resultsList->blockSignals(false);
}

void InviteDialog::addResultItem(const UserInfo &user)
{
    auto *item = new QListWidgetItem();
    item->setData(Qt::UserRole, user.userId);
    item->setData(Qt::UserRole + 1, user.displayName);
    item->setData(Qt::UserRole + 2, user.avatarUrl);

    const bool alreadySelected = std::any_of(m_selectedUsers.cbegin(), m_selectedUsers.cend(),
        [&user](const UserInfo &u) { return u.userId == user.userId; });

    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(alreadySelected ? Qt::Checked : Qt::Unchecked);

    item->setSizeHint(QSize(0, kAvatarSize + 12));

    // Build row widget
    auto *rowWidget = new QWidget(m_resultsList);
    auto *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(8, 4, 8, 4);
    rowLayout->setSpacing(10);

    auto *avatar = new MiniAvatarWidget(
        user.displayName.isEmpty() ? user.userId : user.displayName, rowWidget);
    rowLayout->addWidget(avatar);

    auto *textLayout = new QVBoxLayout;
    textLayout->setSpacing(0);

    if (!user.displayName.isEmpty()) {
        auto *nameLabel = new QLabel(user.displayName, rowWidget);
        nameLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
        textLayout->addWidget(nameLabel);

        auto *idLabel = new QLabel(user.userId, rowWidget);
        idLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px;"));
        textLayout->addWidget(idLabel);
    } else {
        auto *nameLabel = new QLabel(user.userId, rowWidget);
        nameLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 13px;"));
        textLayout->addWidget(nameLabel);
    }

    rowLayout->addLayout(textLayout, 1);

    if (user.isRecent) {
        auto *recentBadge = new QLabel(tr("recent"), rowWidget);
        recentBadge->setStyleSheet(QStringLiteral(
            "color: #059669; font-size: 10px; background: #d1fae5; "
            "border-radius: 4px; padding: 1px 4px;"));
        rowLayout->addWidget(recentBadge);
    }

    m_resultsList->addItem(item);
    m_resultsList->setItemWidget(item, rowWidget);
}

void InviteDialog::onResultItemChanged(QListWidgetItem *item)
{
    if (!item)
        return;

    const QString userId = item->data(Qt::UserRole).toString();
    const QString displayName = item->data(Qt::UserRole + 1).toString();
    const QString avatarUrl = item->data(Qt::UserRole + 2).toString();

    auto it = std::find_if(m_selectedUsers.begin(), m_selectedUsers.end(),
        [&userId](const UserInfo &u) { return u.userId == userId; });

    if (item->checkState() == Qt::Checked) {
        if (it == m_selectedUsers.end()) {
            UserInfo user{userId, displayName, avatarUrl};
            m_selectedUsers.append(user);
            addChip(user);
        }
    } else {
        if (it != m_selectedUsers.end()) {
            m_selectedUsers.erase(it);
            removeChip(userId);
        }
    }

    updateSelectedCount();
}

// ─── Chip management ─────────────────────────────────────────────────────────

void InviteDialog::addChip(const UserInfo &user)
{
    auto *chip = new ChipWidget(user, m_chipContainer);
    m_chipContainer->setObjectName(QStringLiteral("chipContainer"));

    connect(chip, &ChipWidget::removeRequested, this, [this](const QString &userId) {
        removeChip(userId);

        auto it = std::find_if(m_selectedUsers.begin(), m_selectedUsers.end(),
            [&userId](const UserInfo &u) { return u.userId == userId; });
        if (it != m_selectedUsers.end())
            m_selectedUsers.erase(it);

        // Uncheck in list
        for (int i = 0; i < m_resultsList->count(); ++i) {
            auto *listItem = m_resultsList->item(i);
            if (listItem && listItem->data(Qt::UserRole).toString() == userId) {
                m_resultsList->blockSignals(true);
                listItem->setCheckState(Qt::Unchecked);
                m_resultsList->blockSignals(false);
                break;
            }
        }
        updateSelectedCount();
    });

    auto *wrapLayout = qobject_cast<WrapLayout *>(m_chipContainer->layout());
    if (wrapLayout)
        wrapLayout->addWidget(chip);

    m_chipContainer->adjustSize();
}

void InviteDialog::removeChip(const QString &userId)
{
    auto *wrapLayout = qobject_cast<WrapLayout *>(m_chipContainer->layout());
    if (!wrapLayout)
        return;

    for (int i = 0; i < wrapLayout->count(); ++i) {
        auto *item = wrapLayout->itemAt(i);
        if (!item)
            continue;
        auto *chip = qobject_cast<ChipWidget *>(item->widget());
        if (chip && chip->userId() == userId) {
            wrapLayout->takeAt(i);
            chip->deleteLater();
            break;
        }
    }

    m_chipContainer->adjustSize();
}

void InviteDialog::updateSelectedCount()
{
    const int count = m_selectedUsers.size();
    if (m_protocolType == ProtocolType::MATRIX) {
        m_inviteButton->setEnabled(count > 0);
        m_inviteButton->setText(count > 0 ? tr("Invite (%1)").arg(count) : tr("Invite"));
    } else if (m_protocolType == ProtocolType::IRC) {
        const QString nick = m_ircNickEdit ? m_ircNickEdit->text().trimmed() : QString();
        m_inviteButton->setEnabled(!nick.isEmpty());
        m_inviteButton->setText(tr("Invite by Nick"));
    }
}

// ─── Invite action ───────────────────────────────────────────────────────────

void InviteDialog::onInviteClicked()
{
    switch (m_protocolType) {
    case ProtocolType::MATRIX:
        inviteNextMatrixUser();
        break;
    case ProtocolType::IRC: {
        const QString nick = m_ircNickEdit ? m_ircNickEdit->text().trimmed() : QString();
        if (nick.isEmpty()) {
            QMessageBox::warning(this, tr("No Nickname"),
                                 tr("Please enter a nickname to invite."));
            m_ircNickEdit->setFocus();
            return;
        }

        auto *holder = pcp::Application::instance().ircSessionHolder();
        if (!holder) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("IRC session holder not available."));
            return;
        }

        auto sessions = holder->getAllSessions();
        if (sessions.isEmpty()) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("No active IRC sessions."));
            return;
        }

        // Use the first connected IRC session
        IrcSession *irc = nullptr;
        for (auto *s : sessions) {
            if (s->connectionState() == ConnectionState::REGISTERED) {
                irc = s;
                break;
            }
        }
        if (!irc) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("No connected IRC session available."));
            return;
        }

        auto *socket = irc->findChild<QTcpSocket *>();
        if (!socket || !socket->isOpen()) {
            QMessageBox::warning(this, tr("Error"),
                                 tr("IRC socket is not available."));
            return;
        }

        const QString cmd = QStringLiteral("INVITE %1 %2\r\n").arg(nick, m_roomId);
        socket->write(cmd.toUtf8());

        m_statusLabel->setText(tr("Invite sent for %1 to %2").arg(nick, m_roomId));
        m_statusLabel->setVisible(true);

        if (m_ircNotifyCheck && m_ircNotifyCheck->isChecked()) {
            const QString msg = QStringLiteral(
                "NOTICE %1 :You have been invited to %2\r\n").arg(nick, m_roomId);
            socket->write(msg.toUtf8());
        }

        QMessageBox::information(this, tr("Invite Sent"),
                                 tr("Invite sent for %1 to %2.").arg(nick, m_roomId));
        accept();
        break;
    }
    case ProtocolType::LEMMY:
    default:
        m_statusLabel->setText(tr("Invites are not supported for this protocol."));
        m_statusLabel->setVisible(true);
        break;
    }
}

void InviteDialog::inviteNextMatrixUser()
{
    if (m_currentInviteIndex >= m_selectedUsers.size()) {
        // All done
        m_inviteButton->setEnabled(true);
        m_progressBar->setVisible(false);

        QString msg;
        if (m_failCount == 0) {
            msg = tr("All %n user(s) invited successfully.", "", m_successCount);
            for (const auto &user : std::as_const(m_selectedUsers))
                saveRecentContact(user);
            QMessageBox::information(this, tr("Invites Sent"), msg);
            accept();
        } else {
            msg = tr("%1 succeeded, %2 failed.").arg(m_successCount).arg(m_failCount);
            QMessageBox::warning(this, tr("Invite Results"), msg);
        }
        return;
    }

    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        m_statusLabel->setText(tr("No Matrix session available."));
        m_statusLabel->setVisible(true);
        return;
    }

    const UserInfo &user = m_selectedUsers.at(m_currentInviteIndex);

    if (!isValidMatrixId(user.userId)) {
        m_statusLabel->setText(tr("Invalid Matrix ID format: %1").arg(user.userId));
        m_statusLabel->setVisible(true);
        ++m_currentInviteIndex;
        ++m_failCount;
        inviteNextMatrixUser();
        return;
    }

    m_inviteButton->setEnabled(false);
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Inviting %1...").arg(user.displayName.isEmpty() ? user.userId : user.displayName));
    m_statusLabel->setVisible(true);

    QJsonObject body;
    body[QStringLiteral("user_id")] = user.userId;

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/invite").arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply, user]() {
        reply->deleteLater();

        if (reply->error() == QNetworkReply::NoError) {
            ++m_successCount;
            saveRecentContact(user);
        } else {
            ++m_failCount;
            const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
            const QString errorMsg = JsonUtil::optString(json, QStringLiteral("error"),
                                                         reply->errorString());
            m_statusLabel->setText(
                tr("Failed to invite %1: %2").arg(user.userId, errorMsg));
        }

        ++m_currentInviteIndex;
        inviteNextMatrixUser();
    });
}

// ─── Validation & utilities ──────────────────────────────────────────────────

bool InviteDialog::isValidMatrixId(const QString &id)
{
    if (id.isEmpty())
        return false;

    static const QRegularExpression re(
        QStringLiteral(R"(^@[a-zA-Z0-9._=\-/]+:[a-zA-Z0-9.\-]+(?::\d+)?$)"));
    return re.match(id).hasMatch();
}

QString InviteDialog::initialsFromName(const QString &name)
{
    if (name.isEmpty())
        return QStringLiteral("?");

    const QStringList parts = name.split(QRegularExpression(QStringLiteral(R"(\s+)")),
                                         Qt::SkipEmptyParts);
    if (parts.size() >= 2)
        return QString(parts.at(0).at(0).toUpper()) + parts.at(1).at(0).toUpper();

    return parts.at(0).at(0).toUpper();
}

QColor InviteDialog::avatarColorFromName(const QString &name)
{
    if (name.isEmpty())
        return QColor(0x6B, 0x72, 0x80);

    const uint hash = qHash(name);
    const int hue = static_cast<int>(hash % 360);
    return QColor::fromHsl(hue, 180, 160);
}

#include "invite_dialog.moc"
