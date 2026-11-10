#include "features/matrix_chat/direct_chat_dialog.hpp"

#include <QCheckBox>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QProgressBar>
#include <QPushButton>
#include <QRegularExpression>
#include <QSettings>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

#include <algorithm>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

namespace {

constexpr int kSearchDebounceMs = 300;
constexpr int kMaxRecentUsers = 20;
constexpr int kAvatarSize = 40;

QColor avatarColorFromNameLocal(const QString &name)
{
    if (name.isEmpty())
        return QColor(0x6B, 0x72, 0x80);

    const uint hash = qHash(name);
    const int hue = static_cast<int>(hash % 360);
    return QColor::fromHsl(hue, 180, 160);
}

QPixmap makeAvatarPixmapLocal(const QString &name, int size)
{
    const QColor bg = avatarColorFromNameLocal(name);

    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect r(0, 0, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bg);
    painter.drawRoundedRect(r, size / 4, size / 4);

    QString letter;
    if (!name.isEmpty())
        letter = name.at(0).toUpper();
    else
        letter = QStringLiteral("?");

    QFont font;
    font.setPixelSize(size / 2 + 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(r, Qt::AlignCenter, letter);

    painter.end();
    return pix;
}

} // anonymous namespace

DirectChatDialog::DirectChatDialog(const QString &preSelectedUserId, QWidget *parent)
    : QDialog(parent)
    , m_preSelectedUserId(preSelectedUserId)
    , m_nam(new QNetworkAccessManager(this))
    , m_searchTimer(new QTimer(this))
{
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(kSearchDebounceMs);

    auto *session = pcp::Application::instance().matrixSession();
    if (session) {
        m_homeserverUrl = session->homeserverUrl();
        m_userId = session->userId();
        m_accessToken = session->accessToken();
    }

    connect(m_searchTimer, &QTimer::timeout,
            this, &DirectChatDialog::onSearchDebounceTimeout);

    setupUi();
    loadExistingDMs();
    loadRecentUsers();
    showRecentUsers();

    if (!m_preSelectedUserId.isEmpty()) {
        m_searchEdit->setText(m_preSelectedUserId);
        m_searchTimer->start();
    }
}

DirectChatDialog::~DirectChatDialog() = default;

void DirectChatDialog::setupUi()
{
    setWindowTitle(tr("Direct Chat"));
    setMinimumSize(500, 580);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(10);

    auto *titleLabel = new QLabel(tr("Start a Direct Chat"), this);
    titleLabel->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 15px;"));
    mainLayout->addWidget(titleLabel);

    m_searchHeaderLabel = new QLabel(tr("Search for users to chat with:"), this);
    m_searchHeaderLabel->setStyleSheet(QStringLiteral("font-size: 12px; color: #6b7280;"));
    mainLayout->addWidget(m_searchHeaderLabel);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search by name or Matrix ID (@user:server)..."));
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->setMinimumHeight(34);
    mainLayout->addWidget(m_searchEdit);

    m_recentHeaderLabel = new QLabel(tr("Recent"), this);
    m_recentHeaderLabel->setStyleSheet(QStringLiteral(
        "color: #6b7280; font-size: 11px; font-weight: bold; margin-top: 4px;"));
    m_recentHeaderLabel->setVisible(false);
    mainLayout->addWidget(m_recentHeaderLabel);

    m_resultsList = new QListWidget(this);
    m_resultsList->setSelectionMode(QAbstractItemView::MultiSelection);
    m_resultsList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_resultsList->setStyleSheet(QStringLiteral(
        "QListWidget { border: 1px solid #d1d5db; border-radius: 6px; }"
        "QListWidget::item { border-bottom: 1px solid #e5e7eb; padding: 4px; }"
        "QListWidget::item:selected { background-color: #e0e7ff; }"));
    mainLayout->addWidget(m_resultsList, 1);

    auto *roomNameRow = new QHBoxLayout();
    roomNameRow->setSpacing(8);

    auto *roomNameLabel = new QLabel(tr("Room name (optional):"), this);
    roomNameLabel->setStyleSheet(QStringLiteral("font-size: 12px;"));
    roomNameRow->addWidget(roomNameLabel);

    m_roomNameEdit = new QLineEdit(this);
    m_roomNameEdit->setPlaceholderText(tr("Group DM name (leave empty for default)"));
    m_roomNameEdit->setMinimumHeight(30);
    roomNameRow->addWidget(m_roomNameEdit, 1);

    mainLayout->addLayout(roomNameRow);

    m_encryptionToggle = new QCheckBox(tr("Enable end-to-end encryption"), this);
    m_encryptionToggle->setChecked(true);
    m_encryptionToggle->setStyleSheet(QStringLiteral("font-size: 12px;"));
    mainLayout->addWidget(m_encryptionToggle);

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

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_actionButton = new QPushButton(tr("Create DM"), this);
    m_actionButton->setDefault(true);
    m_actionButton->setEnabled(false);
    m_actionButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #4f46e5; color: white; border: none; "
        "border-radius: 6px; padding: 8px 20px; font-weight: bold; }"
        "QPushButton:hover { background-color: #4338ca; }"
        "QPushButton:disabled { background-color: #9ca3af; }"));

    buttonLayout->addWidget(m_cancelButton);
    buttonLayout->addWidget(m_actionButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &DirectChatDialog::onSearchTextChanged);
    connect(m_resultsList, &QListWidget::itemClicked,
            this, &DirectChatDialog::onResultItemToggled);
    connect(m_actionButton, &QPushButton::clicked,
            this, &DirectChatDialog::onCreateOrOpenClicked);
    connect(m_cancelButton, &QPushButton::clicked,
            this, &QDialog::reject);
}

void DirectChatDialog::loadExistingDMs()
{
    if (m_accessToken.isEmpty() || m_homeserverUrl.isEmpty() || m_userId.isEmpty())
        return;

    QString encodedUserId = QString::fromUtf8(QUrl::toPercentEncoding(m_userId));
    QUrl url(m_homeserverUrl + QStringLiteral("/_matrix/client/r0/user/")
             + encodedUserId + QStringLiteral("/account_data/m.direct"));

    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
            return;

        const QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        const QJsonObject content = doc.object();

        m_directMap.clear();
        for (auto it = content.constBegin(); it != content.constEnd(); ++it) {
            const QJsonArray arr = it.value().toArray();
            QStringList roomIds;
            for (const QJsonValue &val : arr)
                roomIds.append(val.toString());
            if (!roomIds.isEmpty())
                m_directMap.insert(it.key(), roomIds);
        }
    });
}

bool DirectChatDialog::isExistingDM(const QString &userId) const
{
    return m_directMap.contains(userId) && !m_directMap.value(userId).isEmpty();
}

QString DirectChatDialog::existingDMRoomId(const QString &userId) const
{
    if (!m_directMap.contains(userId))
        return QString();

    const QStringList &rooms = m_directMap.value(userId);
    return rooms.isEmpty() ? QString() : rooms.first();
}

void DirectChatDialog::loadRecentUsers()
{
    QSettings settings(QStringLiteral("PCP"), QStringLiteral("PCP"));
    const QByteArray raw = settings.value(QStringLiteral("recentDirectChatUsers")).toByteArray();

    if (raw.isEmpty())
        return;

    const QJsonDocument doc = QJsonDocument::fromJson(raw);
    if (!doc.isArray())
        return;

    m_recentUsers.clear();
    const QJsonArray arr = doc.array();
    for (const auto &val : arr) {
        const QJsonObject obj = val.toObject();
        DirectChatUserInfo user;
        user.userId = JsonUtil::optString(obj, QStringLiteral("userId"));
        user.displayName = JsonUtil::optString(obj, QStringLiteral("displayName"));
        user.avatarUrl = JsonUtil::optString(obj, QStringLiteral("avatarUrl"));
        if (!user.userId.isEmpty() && isValidMatrixId(user.userId))
            m_recentUsers.append(user);
    }
}

static void saveRecentUser(const DirectChatUserInfo &user)
{
    if (user.userId.isEmpty())
        return;

    QSettings settings(QStringLiteral("PCP"), QStringLiteral("PCP"));
    const QByteArray raw = settings.value(QStringLiteral("recentDirectChatUsers")).toByteArray();

    QVector<DirectChatUserInfo> recent;
    if (!raw.isEmpty()) {
        const QJsonDocument doc = QJsonDocument::fromJson(raw);
        if (doc.isArray()) {
            const QJsonArray arr = doc.array();
            for (const auto &val : arr) {
                const QJsonObject obj = val.toObject();
                DirectChatUserInfo u;
                u.userId = JsonUtil::optString(obj, QStringLiteral("userId"));
                u.displayName = JsonUtil::optString(obj, QStringLiteral("displayName"));
                u.avatarUrl = JsonUtil::optString(obj, QStringLiteral("avatarUrl"));
                if (!u.userId.isEmpty())
                    recent.append(u);
            }
        }
    }

    auto it = std::find_if(recent.begin(), recent.end(),
        [&user](const DirectChatUserInfo &u) { return u.userId == user.userId; });
    if (it != recent.end())
        recent.erase(it);

    recent.prepend(user);

    while (recent.size() > kMaxRecentUsers)
        recent.removeLast();

    QJsonArray arr;
    for (const auto &u : recent) {
        QJsonObject obj;
        obj[QStringLiteral("userId")] = u.userId;
        obj[QStringLiteral("displayName")] = u.displayName;
        obj[QStringLiteral("avatarUrl")] = u.avatarUrl;
        arr.append(obj);
    }

    settings.setValue(QStringLiteral("recentDirectChatUsers"),
                      QString::fromUtf8(QJsonDocument(arr).toJson(QJsonDocument::Compact)));
}

void DirectChatDialog::showRecentUsers()
{
    clearResultsList();

    if (m_recentUsers.isEmpty()) {
        m_recentHeaderLabel->setVisible(false);
        return;
    }

    m_recentHeaderLabel->setVisible(true);

    for (const auto &user : std::as_const(m_recentUsers)) {
        addSearchResultItem(user);
    }
}

void DirectChatDialog::onSearchTextChanged(const QString &text)
{
    Q_UNUSED(text)
    m_searchTimer->start();
}

void DirectChatDialog::onSearchDebounceTimeout()
{
    const QString query = m_searchEdit->text().trimmed();
    if (query.isEmpty()) {
        m_searchHeaderLabel->setText(tr("Search for users to chat with:"));
        showRecentUsers();
        return;
    }
    m_searchHeaderLabel->setText(tr("Search results:"));
    performSearch(query);
}

void DirectChatDialog::performSearch(const QString &query)
{
    if (m_accessToken.isEmpty() || m_homeserverUrl.isEmpty()) {
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

    QUrl url(m_homeserverUrl + QStringLiteral("/_matrix/client/r0/user_directory/search"));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());

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
            DirectChatUserInfo user;
            user.userId = JsonUtil::optString(entry, QStringLiteral("user_id"));
            user.displayName = JsonUtil::optString(entry, QStringLiteral("display_name"));
            user.avatarUrl = JsonUtil::optString(entry, QStringLiteral("avatar_url"));

            if (!user.userId.isEmpty() && user.userId != m_userId)
                addSearchResultItem(user);
        }

        updateCreateButton();
    });
}

void DirectChatDialog::addSearchResultItem(const DirectChatUserInfo &user)
{
    auto *item = new QListWidgetItem();
    item->setData(Qt::UserRole, user.userId);
    item->setData(Qt::UserRole + 1, user.displayName);
    item->setData(Qt::UserRole + 2, user.avatarUrl);
    item->setSizeHint(QSize(0, kAvatarSize + 16));

    if (user.userId == m_preSelectedUserId) {
        item->setSelected(true);
    }

    auto *rowWidget = new QWidget(m_resultsList);
    auto *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(10, 6, 10, 6);
    rowLayout->setSpacing(10);

    QPixmap avatar = makeAvatarPixmapLocal(
        user.displayName.isEmpty() ? user.userId : user.displayName, kAvatarSize);
    auto *avatarLabel = new QLabel(rowWidget);
    avatarLabel->setPixmap(avatar);
    avatarLabel->setFixedSize(kAvatarSize, kAvatarSize);
    rowLayout->addWidget(avatarLabel);

    auto *textLayout = new QVBoxLayout;
    textLayout->setSpacing(0);

    if (!user.displayName.isEmpty()) {
        auto *nameLabel = new QLabel(user.displayName, rowWidget);
        nameLabel->setStyleSheet(QStringLiteral(
            "font-weight: bold; font-size: 13px; background: transparent;"));
        textLayout->addWidget(nameLabel);

        auto *idLabel = new QLabel(user.userId, rowWidget);
        idLabel->setStyleSheet(QStringLiteral(
            "color: #6b7280; font-size: 11px; background: transparent;"));
        textLayout->addWidget(idLabel);
    } else {
        auto *nameLabel = new QLabel(user.userId, rowWidget);
        nameLabel->setStyleSheet(QStringLiteral(
            "font-weight: bold; font-size: 13px; background: transparent;"));
        textLayout->addWidget(nameLabel);
    }

    rowLayout->addLayout(textLayout, 1);

    if (isExistingDM(user.userId)) {
        auto *openBadge = new QLabel(tr("Open"), rowWidget);
        openBadge->setStyleSheet(QStringLiteral(
            "background-color: #d1fae5; color: #059669; font-size: 11px; "
            "border-radius: 4px; padding: 2px 8px; font-weight: bold;"));
        rowLayout->addWidget(openBadge);
    }

    m_resultsList->addItem(item);
    m_resultsList->setItemWidget(item, rowWidget);
}

void DirectChatDialog::clearResultsList()
{
    m_resultsList->blockSignals(true);
    m_resultsList->clear();
    m_resultsList->blockSignals(false);
}

void DirectChatDialog::onResultItemToggled(QListWidgetItem *item)
{
    Q_UNUSED(item)
    updateCreateButton();
}

void DirectChatDialog::updateCreateButton()
{
    m_selectedUsers.clear();

    for (int i = 0; i < m_resultsList->count(); ++i) {
        auto *item = m_resultsList->item(i);
        if (item && item->isSelected()) {
            DirectChatUserInfo user;
            user.userId = item->data(Qt::UserRole).toString();
            user.displayName = item->data(Qt::UserRole + 1).toString();
            user.avatarUrl = item->data(Qt::UserRole + 2).toString();
            if (!user.userId.isEmpty())
                m_selectedUsers.append(user);
        }
    }

    const int count = m_selectedUsers.size();
    m_actionButton->setEnabled(count > 0);

    if (count == 0) {
        m_actionButton->setText(tr("Create DM"));
    } else if (count == 1 && isExistingDM(m_selectedUsers.first().userId)) {
        m_actionButton->setText(tr("Open Chat"));
    } else if (count > 1) {
        m_actionButton->setText(tr("Create Group DM (%1)").arg(count));
    } else {
        m_actionButton->setText(tr("Create DM"));
    }
}

void DirectChatDialog::onCreateOrOpenClicked()
{
    if (m_selectedUsers.isEmpty()) {
        m_statusLabel->setText(tr("Please select at least one user."));
        m_statusLabel->setVisible(true);
        return;
    }

    if (m_selectedUsers.size() == 1 && isExistingDM(m_selectedUsers.first().userId)) {
        const QString roomId = existingDMRoomId(m_selectedUsers.first().userId);
        saveRecentUser(m_selectedUsers.first());
        emit dmOpened(roomId);
        accept();
        return;
    }

    if (m_accessToken.isEmpty() || m_homeserverUrl.isEmpty()) {
        m_statusLabel->setText(tr("No Matrix session available."));
        m_statusLabel->setVisible(true);
        return;
    }

    m_actionButton->setEnabled(false);
    m_progressBar->setVisible(true);
    m_statusLabel->setText(tr("Creating direct chat..."));
    m_statusLabel->setVisible(true);

    QJsonObject body;

    const QString roomName = m_roomNameEdit->text().trimmed();
    bool isGroup = m_selectedUsers.size() > 1;

    if (!roomName.isEmpty()) {
        body[QStringLiteral("name")] = roomName;
    } else if (isGroup) {
        QStringList displayNames;
        for (const auto &u : m_selectedUsers) {
            displayNames.append(u.displayName.isEmpty() ? u.userId : u.displayName);
        }
        body[QStringLiteral("name")] = displayNames.join(QStringLiteral(", "));
    }

    body[QStringLiteral("is_direct")] = !isGroup;
    body[QStringLiteral("visibility")] = QStringLiteral("private");
    body[QStringLiteral("preset")] = isGroup
        ? QStringLiteral("private_chat")
        : QStringLiteral("trusted_private_chat");

    QJsonArray inviteArray;
    for (const auto &u : m_selectedUsers) {
        inviteArray.append(u.userId);
    }
    body[QStringLiteral("invite")] = inviteArray;

    if (m_encryptionToggle->isChecked()) {
        QJsonObject encContent;
        encContent[QStringLiteral("algorithm")] = QStringLiteral("m.megolm.v1.aes-sha2");

        QJsonObject encEvent;
        encEvent[QStringLiteral("type")] = QStringLiteral("m.room.encryption");
        encEvent[QStringLiteral("state_key")] = QStringLiteral("");
        encEvent[QStringLiteral("content")] = encContent;

        QJsonArray initialState;
        initialState.append(encEvent);
        body[QStringLiteral("initial_state")] = initialState;
    }

    QUrl url(m_homeserverUrl + QStringLiteral("/_matrix/client/r0/createRoom"));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + m_accessToken).toUtf8());

    QNetworkReply *reply = m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        m_actionButton->setEnabled(true);
        m_progressBar->setVisible(false);

        if (reply->error() != QNetworkReply::NoError) {
            const QJsonObject errJson = JsonUtil::parseObject(
                QString::fromUtf8(reply->readAll()));
            const QString errorMsg = JsonUtil::optString(errJson, QStringLiteral("error"),
                                                         reply->errorString());
            m_statusLabel->setText(tr("Error: %1").arg(errorMsg));
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        const QString roomId = JsonUtil::optString(json, QStringLiteral("room_id"));

        if (roomId.isEmpty()) {
            m_statusLabel->setText(tr("Error: No room ID returned."));
            return;
        }

        if (m_selectedUsers.size() == 1) {
            const QString &targetUserId = m_selectedUsers.first().userId;
            QJsonObject directBody;
            QJsonArray roomArray;
            roomArray.append(roomId);
            directBody[targetUserId] = roomArray;

            QString encodedUserId = QString::fromUtf8(
                QUrl::toPercentEncoding(m_userId));
            QUrl directUrl(m_homeserverUrl
                + QStringLiteral("/_matrix/client/r0/user/")
                + encodedUserId
                + QStringLiteral("/account_data/m.direct"));

            QNetworkRequest directReq(directUrl);
            directReq.setHeader(QNetworkRequest::ContentTypeHeader,
                                QStringLiteral("application/json"));
            directReq.setRawHeader("Authorization",
                                   ("Bearer " + m_accessToken).toUtf8());

            QNetworkReply *directReply = m_nam->put(directReq,
                QJsonDocument(directBody).toJson(QJsonDocument::Compact));

            connect(directReply, &QNetworkReply::finished, this,
                    [this, directReply, roomId]() {
                directReply->deleteLater();
            });
        }

        m_statusLabel->setText(tr("Direct chat created! Room ID: %1").arg(roomId));

        for (const auto &u : m_selectedUsers)
            saveRecentUser(u);

        emit dmCreated(roomId);
        accept();
    });
}

bool DirectChatDialog::isValidMatrixId(const QString &id)
{
    if (id.isEmpty())
        return false;

    static const QRegularExpression re(
        QStringLiteral(R"(^@[a-zA-Z0-9._=\-/]+:[a-zA-Z0-9.\-]+(?::\d+)?$)"));
    return re.match(id).hasMatch();
}

QColor DirectChatDialog::avatarColorFromName(const QString &name)
{
    return avatarColorFromNameLocal(name);
}
