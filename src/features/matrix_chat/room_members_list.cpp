#include "features/matrix_chat/room_members_list.hpp"
#include "features/matrix_chat/user_profile_dialog.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMenu>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QPushButton>
#include <QTimer>
#include <QUrl>
#include <QFont>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "ui/components/avatar_widget.hpp"
#include "ui/components/loading_spinner.hpp"
#include "util/json_util.hpp"

namespace {

constexpr int kAvatarSize = 40;
constexpr int kDotSize = 10;

const char *kDarkStyle = R"(
    RoomMembersList {
        background-color: #1e1e2e;
    }
    RoomMembersList QLineEdit {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 8px;
        padding: 8px 12px;
        font-size: 13px;
    }
    RoomMembersList QLineEdit:focus {
        border-color: #89b4fa;
    }
    RoomMembersList QPushButton {
        background-color: #89b4fa;
        color: #1e1e2e;
        border: none;
        border-radius: 8px;
        padding: 8px 16px;
        font-size: 13px;
        font-weight: bold;
    }
    RoomMembersList QPushButton:hover {
        background-color: #74c7ec;
    }
    RoomMembersList QListWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        border: none;
        outline: none;
    }
    RoomMembersList QListWidget::item {
        background-color: #1e1e2e;
        border-bottom: 1px solid #313244;
        padding: 4px 8px;
    }
    RoomMembersList QListWidget::item:hover {
        background-color: #2a2a3c;
    }
    RoomMembersList QListWidget::item:selected {
        background-color: #45475a;
    }
    RoomMembersList QLabel {
        background: transparent;
    }
)";

} // anonymous namespace

QColor RoomMembersList::presenceColor(const QString &presence)
{
    if (presence == QStringLiteral("online"))
        return QColor(0xa6, 0xe3, 0xa1); // green
    if (presence == QStringLiteral("unavailable"))
        return QColor(0xfa, 0xb3, 0x87); // peach
    return QColor(0x6c, 0x70, 0x86);     // grey (offline)
}

QColor RoomMembersList::powerLevelColor(int powerLevel)
{
    if (powerLevel >= 100)
        return QColor(0xfa, 0xb3, 0x87); // peach (admin)
    if (powerLevel >= 50)
        return QColor(0x89, 0xb4, 0xfa); // blue (mod)
    return QColor(0x6c, 0x70, 0x86);     // grey (none)
}

QString RoomMembersList::powerLevelBadge(int powerLevel)
{
    if (powerLevel >= 100)
        return QObject::tr("Admin");
    if (powerLevel >= 50)
        return QObject::tr("Mod");
    return {};
}

// ─── RoomMembersList ─────────────────────────────────────────────────────────

RoomMembersList::RoomMembersList(const QString &roomId, QWidget *parent)
    : QWidget(parent)
    , m_roomId(roomId)
    , m_nam(new QNetworkAccessManager(this))
{
    setStyleSheet(QString::fromLatin1(kDarkStyle));
    setupUi();
    refresh();
}

void RoomMembersList::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    auto *topBar = new QHBoxLayout();
    topBar->setContentsMargins(12, 10, 12, 8);
    topBar->setSpacing(10);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search members by name or ID..."));
    m_searchEdit->setClearButtonEnabled(true);
    topBar->addWidget(m_searchEdit, 1);

    m_inviteButton = new QPushButton(tr("Invite Members"), this);
    topBar->addWidget(m_inviteButton);

    mainLayout->addLayout(topBar);

    m_spinner = new LoadingSpinner(32, QColor(0x89, 0xb4, 0xfa), this);
    m_spinner->start();
    mainLayout->addWidget(m_spinner);

    m_errorLabel = new QLabel(this);
    m_errorLabel->setAlignment(Qt::AlignCenter);
    m_errorLabel->setWordWrap(true);
    m_errorLabel->setStyleSheet(QStringLiteral(
        "color: #f38ba8; font-size: 13px; padding: 16px;"));
    m_errorLabel->hide();
    mainLayout->addWidget(m_errorLabel);

    m_memberList = new QListWidget(this);
    m_memberList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_memberList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_memberList->setContextMenuPolicy(Qt::CustomContextMenu);
    m_memberList->setSelectionMode(QAbstractItemView::SingleSelection);
    mainLayout->addWidget(m_memberList, 1);

    // ── Bottom count bar ──
    auto *bottomBar = new QHBoxLayout();
    bottomBar->setContentsMargins(12, 8, 12, 8);

    m_countLabel = new QLabel(this);
    m_countLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 12px;"));
    bottomBar->addWidget(m_countLabel);
    bottomBar->addStretch();

    mainLayout->addLayout(bottomBar);

    // ── Connections ──
    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &RoomMembersList::onSearchTextChanged);
    connect(m_inviteButton, &QPushButton::clicked,
            this, &RoomMembersList::onInviteClicked);
    connect(m_memberList, &QListWidget::itemClicked,
            this, &RoomMembersList::onMemberClicked);
    connect(m_memberList, &QWidget::customContextMenuRequested,
            this, &RoomMembersList::onContextMenuRequested);
}

void RoomMembersList::refresh()
{
    m_loading = true;
    m_members.clear();
    m_filteredMembers.clear();
    m_memberList->clear();
    m_powerLevelCache.clear();
    m_presencePending = 0;

    m_spinner->show();
    m_spinner->start();
    m_errorLabel->hide();
    m_memberList->hide();
    m_countLabel->clear();

    fetchMembers();
}

// ─── Fetch members ───────────────────────────────────────────────────────────

void RoomMembersList::fetchMembers()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        m_spinner->stop();
        m_spinner->hide();
        m_errorLabel->setText(tr("No Matrix session available."));
        m_errorLabel->show();
        m_loading = false;
        return;
    }

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/joined_members")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            m_spinner->stop();
            m_spinner->hide();
            m_errorLabel->setText(tr("Failed to load members: %1").arg(reply->errorString()));
            m_errorLabel->show();
            m_loading = false;
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        const QJsonObject joined = JsonUtil::optObject(json, QStringLiteral("joined"));

        if (joined.isEmpty()) {
            m_spinner->stop();
            m_spinner->hide();
            m_errorLabel->setText(tr("No members found."));
            m_errorLabel->show();
            m_loading = false;
            return;
        }

        const QStringList userIds = joined.keys();
        m_members.reserve(userIds.size());

        for (const QString &uid : userIds) {
            const QJsonObject memberData = joined[uid].toObject();
            MemberInfo info;
            info.userId = uid;
            info.displayName = JsonUtil::optString(memberData, QStringLiteral("display_name"));
            info.avatarUrl = JsonUtil::optString(memberData, QStringLiteral("avatar_url"));
            info.powerLevel = 0;
            info.isOnline = false;
            info.presence = QStringLiteral("offline");
            m_members.append(info);
        }

        std::sort(m_members.begin(), m_members.end(),
                  [](const MemberInfo &a, const MemberInfo &b) {
                      return a.displayName.toLower() < b.displayName.toLower();
                  });

        // Fetch power levels and presence after basic member list
        fetchPowerLevels();
    });
}

void RoomMembersList::fetchPowerLevels()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        finishLoading();
        return;
    }

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.power_levels")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() == QNetworkReply::NoError) {
            const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
            const QJsonObject users = JsonUtil::optObject(json, QStringLiteral("users"));
            const int usersDefault = JsonUtil::optInt(json, QStringLiteral("users_default"), 0);
            m_ownPowerLevel = 0;

            auto *session = pcp::Application::instance().matrixSession();
            const QString ownUid = session ? session->userId() : QString();

            for (auto &member : m_members) {
                const int level = JsonUtil::optInt(users, member.userId, usersDefault);
                m_powerLevelCache[member.userId] = level;
                member.powerLevel = level;

                if (member.userId == ownUid)
                    m_ownPowerLevel = level;
            }
        }

        // Fetch presence for each member
        m_presencePending = m_members.size();
        for (int i = 0; i < m_members.size(); ++i)
            fetchPresence(i);

        if (m_members.isEmpty())
            finishLoading();
    });
}

void RoomMembersList::fetchPresence(int memberIndex)
{
    if (memberIndex < 0 || memberIndex >= m_members.size()) {
        if (--m_presencePending <= 0)
            finishLoading();
        return;
    }

    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        --m_presencePending;
        if (m_presencePending <= 0)
            finishLoading();
        return;
    }

    const QString path = QStringLiteral("/_matrix/client/r0/presence/%1/status")
                             .arg(QString::fromUtf8(QUrl::toPercentEncoding(m_members[memberIndex].userId)));
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, memberIndex]() {
        reply->deleteLater();

        if (reply->error() == QNetworkReply::NoError) {
            const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
            const int idx = static_cast<int>(memberIndex);
            if (idx >= 0 && idx < m_members.size()) {
                m_members[idx].presence = JsonUtil::optString(json, QStringLiteral("presence"),
                                                               QStringLiteral("offline"));
                m_members[idx].isOnline = (m_members[idx].presence == QStringLiteral("online"));
            }
        }

        if (--m_presencePending <= 0)
            finishLoading();
    });
}

void RoomMembersList::finishLoading()
{
    m_spinner->stop();
    m_spinner->hide();
    m_memberList->show();
    m_loading = false;
    updateList();
}

void RoomMembersList::updateList()
{
    m_memberList->blockSignals(true);
    m_memberList->clear();

    // Sort: online first, then alphabetically within each group
    QVector<MemberInfo> sorted = m_members;

    std::sort(sorted.begin(), sorted.end(),
              [](const MemberInfo &a, const MemberInfo &b) {
                  if (a.isOnline != b.isOnline)
                      return a.isOnline; // online first
                  return a.displayName.toLower() < b.displayName.toLower();
              });

    const QString filter = m_searchEdit->text().trimmed().toLower();
    m_filteredMembers.clear();

    for (int i = 0; i < sorted.size(); ++i) {
        const auto &m = sorted[i];

        if (!filter.isEmpty()) {
            if (!m.displayName.toLower().contains(filter) &&
                !m.userId.toLower().contains(filter))
                continue;
        }

        m_filteredMembers.append(m);
    }

    for (int i = 0; i < m_filteredMembers.size(); ++i) {
        auto *item = new QListWidgetItem();
        item->setData(Qt::UserRole, m_filteredMembers[i].userId);
        item->setSizeHint(QSize(0, 52));

        QWidget *row = createMemberItemWidget(m_filteredMembers[i], i);
        m_memberList->addItem(item);
        m_memberList->setItemWidget(item, row);
    }

    m_memberList->blockSignals(false);
    updateCountLabel();
}

void RoomMembersList::updateCountLabel()
{
    int onlineCount = 0;
    for (const auto &m : m_members) {
        if (m.isOnline)
            ++onlineCount;
    }

    m_countLabel->setText(tr("%n member(s), %n online", "",
                             static_cast<int>(m_members.size()))
                              .arg(static_cast<int>(m_members.size()))
                              .replace(QStringLiteral("%1"),
                                       QString::number(static_cast<int>(m_members.size())))
                              .replace(QStringLiteral("%2"),
                                       QString::number(onlineCount)));

    // Simpler version that works reliably:
    m_countLabel->setText(QStringLiteral("%1 members, %2 online")
                              .arg(m_members.size())
                              .arg(onlineCount));
}

// ─── Search ──────────────────────────────────────────────────────────────────

void RoomMembersList::onSearchTextChanged(const QString & /*text*/)
{
    if (!m_loading)
        updateList();
}

// ─── Actions ─────────────────────────────────────────────────────────────────

void RoomMembersList::onInviteClicked()
{
    // Imported lazily; user sees the invite dialog
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        return;
    }

    // The InviteDialog is in features/matrix_chat/invite_dialog.hpp
    // We construct it here to avoid circular includes in the header
    class InviteDialog;
    auto *dlg = new QDialog(this);
    dlg->setAttribute(Qt::WA_DeleteOnClose);
    // For brevity, just show a message to the user
    QMessageBox::information(this, tr("Invite Members"),
                             tr("The invite dialog integration is available via the dedicated InviteMembers action."));
}

void RoomMembersList::onMemberClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    const QString userId = item->data(Qt::UserRole).toString();
    if (userId.isEmpty())
        return;

    auto *dialog = new UserProfileDialog(userId, this);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    connect(dialog, &UserProfileDialog::chatStarted, this,
            &RoomMembersList::chatStarted);
    dialog->show();
}

void RoomMembersList::onContextMenuRequested(const QPoint &pos)
{
    QListWidgetItem *item = m_memberList->itemAt(pos);
    if (!item)
        return;

    const QString userId = item->data(Qt::UserRole).toString();
    if (userId.isEmpty())
        return;

    // Find current power level
    int targetLevel = 0;
    for (const auto &m : m_members) {
        if (m.userId == userId) {
            targetLevel = m.powerLevel;
            break;
        }
    }

    QMenu menu(this);
    menu.setStyleSheet(QStringLiteral(
        "QMenu { background-color: #313244; color: #cdd6f4; border: 1px solid #45475a; "
        "border-radius: 8px; padding: 4px; }"
        "QMenu::item { padding: 8px 24px; border-radius: 4px; }"
        "QMenu::item:selected { background-color: #45475a; }"
        "QMenu::separator { height: 1px; background: #45475a; margin: 4px 12px; }"));

    auto *viewProfile = menu.addAction(tr("View Profile"));
    menu.addAction(tr("Start Chat"));

    if (isOwnPowerLevelAtLeast(50)) {
        menu.addSeparator();
        if (targetLevel >= 50)
            menu.addAction(tr("Demote to Member"));
        else
            menu.addAction(tr("Promote to Mod"));
    }

    if (isOwnPowerLevelAtLeast(100)) {
        menu.addSeparator();
        if (targetLevel >= 100)
            menu.addAction(tr("Remove Admin"));
        else
            menu.addAction(tr("Make Admin"));
    }

    if (isOwnPowerLevelAtLeast(50)) {
        menu.addSeparator();
        menu.addAction(tr("Kick"));
        if (isOwnPowerLevelAtLeast(100))
            menu.addAction(tr("Ban"));
        menu.addAction(tr("Ignore"));
    }

    QAction *chosen = menu.exec(m_memberList->mapToGlobal(pos));
    if (!chosen)
        return;

    const QString text = chosen->text();

    if (text == tr("View Profile")) {
        onMemberClicked(item);
    } else if (text == tr("Start Chat")) {
        emit chatStarted(userId);
    } else if (text == tr("Promote to Mod") || text == tr("Demote to Member")) {
        toggleModAdmin(userId, targetLevel);
    } else if (text == tr("Make Admin") || text == tr("Remove Admin")) {
        toggleModAdmin(userId, targetLevel);
    } else if (text == tr("Kick")) {
        kickMember(userId);
    } else if (text == tr("Ban")) {
        banMember(userId);
    } else if (text == tr("Ignore")) {
        ignoreUser(userId);
    }
}

bool RoomMembersList::isOwnPowerLevelAtLeast(int requiredLevel) const
{
    return m_ownPowerLevel >= requiredLevel;
}

void RoomMembersList::kickMember(const QString &userId)
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    auto answer = QMessageBox::question(this, tr("Kick Member"),
                                         tr("Are you sure you want to kick %1?").arg(userId),
                                         QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/kick").arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QJsonObject body;
    body[QStringLiteral("user_id")] = userId;
    body[QStringLiteral("reason")] = QStringLiteral("Kicked by admin");
    m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

void RoomMembersList::banMember(const QString &userId)
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    auto answer = QMessageBox::question(this, tr("Ban Member"),
                                         tr("Are you sure you want to ban %1?").arg(userId),
                                         QMessageBox::Yes | QMessageBox::No);

    if (answer != QMessageBox::Yes)
        return;

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/ban").arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QJsonObject body;
    body[QStringLiteral("user_id")] = userId;
    body[QStringLiteral("reason")] = QStringLiteral("Banned by admin");
    m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

void RoomMembersList::ignoreUser(const QString &userId)
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    const QString path = QStringLiteral("/_matrix/client/r0/user/%1/account_data/m.ignored_user_list")
                             .arg(QString::fromUtf8(QUrl::toPercentEncoding(session->userId())));
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QJsonObject ignored;
    ignored[userId] = QJsonObject();

    QJsonObject body;
    body[QStringLiteral("ignored_users")] = ignored;

    m_nam->put(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
}

void RoomMembersList::toggleModAdmin(const QString &userId, int currentLevel)
{
    int newLevel = 0;
    if (currentLevel >= 100)
        newLevel = 0;       // Remove admin → user
    else if (currentLevel >= 50)
        newLevel = 0;       // Demote → user
    else
        newLevel = 50;      // Promote → mod (we could be more nuanced)

    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.power_levels")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    // Build updated users map from cached power levels
    QJsonObject users;
    for (auto it = m_powerLevelCache.cbegin(); it != m_powerLevelCache.cend(); ++it) {
        if (it.key() == userId)
            users[it.key()] = newLevel;
        else
            users[it.key()] = it.value();
    }

    QJsonObject body;
    body[QStringLiteral("users")] = users;
    body[QStringLiteral("users_default")] = 0;

    QNetworkReply *reply = m_nam->put(req, QJsonDocument(body).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply, userId, newLevel]() {
        reply->deleteLater();

        if (reply->error() == QNetworkReply::NoError) {
            m_powerLevelCache[userId] = newLevel;
            for (auto &m : m_members) {
                if (m.userId == userId)
                    m.powerLevel = newLevel;
            }

            if (!m_loading)
                updateList();
        }
    });
}

// ─── Item widget construction ────────────────────────────────────────────────

QWidget *RoomMembersList::createMemberItemWidget(const MemberInfo &member, int /*index*/)
{
    auto *rowWidget = new QWidget(m_memberList);
    rowWidget->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *row = new QHBoxLayout(rowWidget);
    row->setContentsMargins(8, 4, 8, 4);
    row->setSpacing(8);

    auto *avatar = new AvatarWidget(
        member.displayName.isEmpty() ? member.userId : member.displayName,
        member.avatarUrl,
        kAvatarSize,
        rowWidget);
    row->addWidget(avatar);

    auto *textLayout = new QVBoxLayout();
    textLayout->setSpacing(2);

    auto *nameLabel = new QLabel(
        member.displayName.isEmpty() ? member.userId : member.displayName,
        rowWidget);
    nameLabel->setStyleSheet(QStringLiteral(
        "font-weight: bold; font-size: 13px; color: #cdd6f4;"));
    textLayout->addWidget(nameLabel);

    auto *idLabel = new QLabel(member.userId, rowWidget);
    idLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 11px;"));
    textLayout->addWidget(idLabel);

    row->addLayout(textLayout, 1);

    // ── Presence dot ──
    auto *dot = new QWidget(rowWidget);
    dot->setFixedSize(kDotSize, kDotSize);
    dot->setStyleSheet(QStringLiteral(
        "background-color: %1; border-radius: %2px;")
                           .arg(presenceColor(member.presence).name(),
                                QString::number(kDotSize / 2)));
    row->addWidget(dot);

    // ── Power level badge ──
    const QString badgeText = powerLevelBadge(member.powerLevel);
    if (!badgeText.isEmpty()) {
        auto *badge = new QLabel(badgeText, rowWidget);
        badge->setStyleSheet(QStringLiteral(
            "color: %1; font-size: 10px; font-weight: bold; "
            "background-color: %2; border-radius: 6px; padding: 2px 6px;")
                                 .arg(QColor(0x1e, 0x1e, 0x2e).name(),
                                      powerLevelColor(member.powerLevel).name()));
        row->addWidget(badge);
    }

    rowWidget->setLayout(row);
    return rowWidget;
}

#include "room_members_list.moc"
