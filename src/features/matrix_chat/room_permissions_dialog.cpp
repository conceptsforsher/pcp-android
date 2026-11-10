#include "features/matrix_chat/room_permissions_dialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

namespace {

constexpr int kStateEventCount = 4;

QString joinRuleDescription(int index)
{
    switch (index) {
    case 0: return QStringLiteral("Anyone can join the room without an invitation.");
    case 1: return QStringLiteral("Users can request to join. A moderator must approve.");
    case 2: return QStringLiteral("Only invited users can join.");
    case 3: return QStringLiteral("Room is not listed in the directory.");
    default: return {};
    }
}

QString guestAccessDescription(int index)
{
    switch (index) {
    case 0: return QStringLiteral("Guest users can join and view messages.");
    case 1: return QStringLiteral("Guest users are not allowed.");
    default: return {};
    }
}

QString historyVisibilityDescription(int index)
{
    switch (index) {
    case 0: return QStringLiteral("Anyone can see the full message history, including before they joined.");
    case 1: return QStringLiteral("Members can see history from when they were invited.");
    case 2: return QStringLiteral("Members can see history from when they joined.");
    case 3: return QStringLiteral("Anyone can read the room history without joining (world readable).");
    default: return {};
    }
}

} // anonymous namespace

RoomPermissionsDialog::RoomPermissionsDialog(const QString &roomId, QWidget *parent)
    : QDialog(parent)
    , m_roomId(roomId)
    , m_nam(new QNetworkAccessManager(this))
    , m_joinRuleCombo(nullptr)
    , m_roomAliasEdit(nullptr)
    , m_guestAccessCombo(nullptr)
    , m_historyVisibilityCombo(nullptr)
    , m_encryptionCheck(nullptr)
    , m_saveBtn(nullptr)
    , m_statusLabel(nullptr)
    , m_changesIndicator(nullptr)
{
    setupUi();
    fetchCurrentState();
}

void RoomPermissionsDialog::setupUi()
{
    setWindowTitle(tr("Room Permissions"));
    setMinimumSize(520, 480);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    // ─── Join Rules ──────────────────────────────────────────────

    auto *joinGroup = new QGroupBox(tr("Join Rules"), this);
    auto *joinLayout = new QFormLayout(joinGroup);
    joinLayout->setSpacing(8);

    m_joinRuleCombo = new QComboBox(joinGroup);
    m_joinRuleCombo->addItem(tr("Public"), QStringLiteral("public"));
    m_joinRuleCombo->addItem(tr("Knock"), QStringLiteral("knock"));
    m_joinRuleCombo->addItem(tr("Invite"), QStringLiteral("invite"));
    m_joinRuleCombo->addItem(tr("Private"), QStringLiteral("private"));

    m_joinRuleCombo->setItemData(0, tr("Anyone can join the room without an invitation."), Qt::ToolTipRole);
    m_joinRuleCombo->setItemData(1, tr("Users can request to join. A moderator must approve."), Qt::ToolTipRole);
    m_joinRuleCombo->setItemData(2, tr("Only invited users can join."), Qt::ToolTipRole);
    m_joinRuleCombo->setItemData(3, tr("Room is not listed in the public directory."), Qt::ToolTipRole);

    joinLayout->addRow(tr("Who can join:"), m_joinRuleCombo);

    auto *joinDesc = new QLabel(joinGroup);
    joinDesc->setWordWrap(true);
    joinDesc->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px;"));
    joinLayout->addRow(joinDesc);

    m_roomAliasEdit = new QLineEdit(joinGroup);
    m_roomAliasEdit->setPlaceholderText(tr("#room-alias:server.org (for public rooms)"));
    joinLayout->addRow(tr("Room Address:"), m_roomAliasEdit);

    mainLayout->addWidget(joinGroup);

    // ─── Guest Access ────────────────────────────────────────────

    auto *guestGroup = new QGroupBox(tr("Guest Access"), this);
    auto *guestLayout = new QFormLayout(guestGroup);
    guestLayout->setSpacing(8);

    m_guestAccessCombo = new QComboBox(guestGroup);
    m_guestAccessCombo->addItem(tr("Allowed"), QStringLiteral("can_join"));
    m_guestAccessCombo->addItem(tr("Forbidden"), QStringLiteral("forbidden"));
    m_guestAccessCombo->setItemData(0,
        tr("Guest users can join and view messages."), Qt::ToolTipRole);
    m_guestAccessCombo->setItemData(1,
        tr("Guest users are not allowed to join."), Qt::ToolTipRole);

    guestLayout->addRow(tr("Guest access:"), m_guestAccessCombo);

    auto *guestDesc = new QLabel(guestGroup);
    guestDesc->setWordWrap(true);
    guestDesc->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px;"));
    guestLayout->addRow(guestDesc);

    mainLayout->addWidget(guestGroup);

    // ─── History Visibility ──────────────────────────────────────

    auto *historyGroup = new QGroupBox(tr("History Visibility"), this);
    auto *historyLayout = new QFormLayout(historyGroup);
    historyLayout->setSpacing(8);

    m_historyVisibilityCombo = new QComboBox(historyGroup);
    m_historyVisibilityCombo->addItem(tr("Shared"), QStringLiteral("shared"));
    m_historyVisibilityCombo->addItem(tr("Invited"), QStringLiteral("invited"));
    m_historyVisibilityCombo->addItem(tr("Joined"), QStringLiteral("joined"));
    m_historyVisibilityCombo->addItem(tr("World Readable"), QStringLiteral("world_readable"));
    m_historyVisibilityCombo->setItemData(0,
        tr("Anyone can see the full message history."), Qt::ToolTipRole);
    m_historyVisibilityCombo->setItemData(1,
        tr("Members can see history from when they were invited."), Qt::ToolTipRole);
    m_historyVisibilityCombo->setItemData(2,
        tr("Members can see history from when they joined."), Qt::ToolTipRole);
    m_historyVisibilityCombo->setItemData(3,
        tr("Anyone can read the room history without joining."), Qt::ToolTipRole);

    historyLayout->addRow(tr("Who can see history:"), m_historyVisibilityCombo);

    auto *historyDesc = new QLabel(historyGroup);
    historyDesc->setWordWrap(true);
    historyDesc->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px;"));
    historyLayout->addRow(historyDesc);

    mainLayout->addWidget(historyGroup);

    // ─── Encryption ──────────────────────────────────────────────

    auto *encGroup = new QGroupBox(tr("Encryption"), this);
    auto *encLayout = new QVBoxLayout(encGroup);
    encLayout->setSpacing(8);

    m_encryptionCheck = new QCheckBox(tr("Enable End-to-End Encryption"), encGroup);
    m_encryptionCheck->setToolTip(tr("Once enabled, encryption cannot be disabled for this room."));
    encLayout->addWidget(m_encryptionCheck);

    auto *encWarning = new QLabel(
        tr("\u26a0\ufe0f  Enabling encryption cannot be undone. "
           "All messages will be encrypted end-to-end."),
        encGroup);
    encWarning->setWordWrap(true);
    encWarning->setStyleSheet(QStringLiteral(
        "color: #92400e; background: #fef3c7; border: 1px solid #fcd34d; "
        "border-radius: 4px; padding: 6px; font-size: 11px;"));
    encLayout->addWidget(encWarning);

    mainLayout->addWidget(encGroup);

    // ─── Status & Changes ────────────────────────────────────────

    m_changesIndicator = new QLabel(this);
    m_changesIndicator->setAlignment(Qt::AlignCenter);
    m_changesIndicator->setVisible(false);
    m_changesIndicator->setStyleSheet(QStringLiteral(
        "color: #059669; font-weight: bold; padding: 4px; font-size: 12px;"));
    mainLayout->addWidget(m_changesIndicator);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setVisible(false);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 12px;"));
    mainLayout->addWidget(m_statusLabel);

    // ─── Buttons ─────────────────────────────────────────────────

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_saveBtn = new QPushButton(tr("Save Changes"), this);
    m_saveBtn->setDefault(true);
    m_saveBtn->setEnabled(false);

    auto *cancelBtn = new QPushButton(tr("Cancel"), this);

    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(m_saveBtn);
    mainLayout->addLayout(buttonLayout);

    connect(m_saveBtn, &QPushButton::clicked, this, &RoomPermissionsDialog::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    connect(m_joinRuleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RoomPermissionsDialog::onJoinRuleChanged);
    connect(m_joinRuleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [joinDesc](int index) {
                joinDesc->setText(joinRuleDescription(index));
            });
    connect(m_guestAccessCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [guestDesc](int index) {
                guestDesc->setText(guestAccessDescription(index));
            });
    connect(m_historyVisibilityCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [historyDesc](int index) {
                historyDesc->setText(historyVisibilityDescription(index));
            });

    connect(m_joinRuleCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RoomPermissionsDialog::onValueChanged);
    connect(m_guestAccessCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RoomPermissionsDialog::onValueChanged);
    connect(m_historyVisibilityCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RoomPermissionsDialog::onValueChanged);
    connect(m_encryptionCheck, &QCheckBox::toggled,
            this, &RoomPermissionsDialog::onValueChanged);
}

void RoomPermissionsDialog::fetchCurrentState()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    m_statusLabel->setText(tr("Loading current permissions..."));
    m_statusLabel->setVisible(true);

    const auto fetchState = [this, session](const QString &eventType) {
        const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/%2")
                                 .arg(m_roomId, eventType);
        QUrl url(session->homeserverUrl() + path);
        QNetworkRequest req(url);
        req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

        QNetworkReply *reply = m_nam->get(req);

        connect(reply, &QNetworkReply::finished, this, [this, reply, eventType]() {
            reply->deleteLater();

            if (reply->error() != QNetworkReply::NoError)
                return;

            const QJsonObject json = JsonUtil::parseObject(
                QString::fromUtf8(reply->readAll()));
            const QJsonObject content = JsonUtil::optObject(json, QStringLiteral("content"));

            if (eventType == QStringLiteral("m.room.join_rules")) {
                m_originalJoinRules = content;
                populateFromState();
            } else if (eventType == QStringLiteral("m.room.guest_access")) {
                m_originalGuestAccess = content;
                populateFromState();
            } else if (eventType == QStringLiteral("m.room.history_visibility")) {
                m_originalHistoryVisibility = content;
                populateFromState();
            } else if (eventType == QStringLiteral("m.room.encryption")) {
                m_originalEncryption = content;
                populateFromState();
            }
        });
    };

    fetchState(QStringLiteral("m.room.join_rules"));
    fetchState(QStringLiteral("m.room.guest_access"));
    fetchState(QStringLiteral("m.room.history_visibility"));
    fetchState(QStringLiteral("m.room.encryption"));
}

void RoomPermissionsDialog::populateFromState()
{
    if (!m_originalJoinRules.isEmpty()) {
        const QString rule = JsonUtil::optString(m_originalJoinRules, QStringLiteral("join_rule"),
                                                  QStringLiteral("invite"));
        for (int i = 0; i < m_joinRuleCombo->count(); ++i) {
            if (m_joinRuleCombo->itemData(i).toString() == rule) {
                m_joinRuleCombo->blockSignals(true);
                m_joinRuleCombo->setCurrentIndex(i);
                m_joinRuleCombo->blockSignals(false);
                break;
            }
        }
    }

    if (!m_originalGuestAccess.isEmpty()) {
        const QString access = JsonUtil::optString(m_originalGuestAccess,
                                                    QStringLiteral("guest_access"),
                                                    QStringLiteral("forbidden"));
        for (int i = 0; i < m_guestAccessCombo->count(); ++i) {
            if (m_guestAccessCombo->itemData(i).toString() == access) {
                m_guestAccessCombo->blockSignals(true);
                m_guestAccessCombo->setCurrentIndex(i);
                m_guestAccessCombo->blockSignals(false);
                break;
            }
        }
    }

    if (!m_originalHistoryVisibility.isEmpty()) {
        const QString vis = JsonUtil::optString(m_originalHistoryVisibility,
                                                 QStringLiteral("history_visibility"),
                                                 QStringLiteral("shared"));
        for (int i = 0; i < m_historyVisibilityCombo->count(); ++i) {
            if (m_historyVisibilityCombo->itemData(i).toString() == vis) {
                m_historyVisibilityCombo->blockSignals(true);
                m_historyVisibilityCombo->setCurrentIndex(i);
                m_historyVisibilityCombo->blockSignals(false);
                break;
            }
        }
    }

    if (!m_originalEncryption.isEmpty()) {
        m_encryptionCheck->blockSignals(true);
        m_encryptionCheck->setChecked(true);
        m_encryptionCheck->setEnabled(false);
        m_encryptionCheck->blockSignals(false);
    }

    m_statusLabel->setVisible(false);
    updateChangesIndicator();
}

void RoomPermissionsDialog::onJoinRuleChanged(int index)
{
    const QString rule = m_joinRuleCombo->itemData(index).toString();
    m_roomAliasEdit->setEnabled(rule == QStringLiteral("public"));
}

void RoomPermissionsDialog::onValueChanged()
{
    setModified();
}

void RoomPermissionsDialog::setModified()
{
    m_modified = true;
    updateChangesIndicator();
}

bool RoomPermissionsDialog::hasChanges() const
{
    if (m_originalJoinRules.isEmpty() && m_originalGuestAccess.isEmpty()
        && m_originalHistoryVisibility.isEmpty() && m_originalEncryption.isEmpty())
        return true;

    const QString currentRule = m_joinRuleCombo->currentData().toString();
    const QString origRule = JsonUtil::optString(m_originalJoinRules,
                                                  QStringLiteral("join_rule"),
                                                  QStringLiteral("invite"));
    if (currentRule != origRule)
        return true;

    const QString currentGuest = m_guestAccessCombo->currentData().toString();
    const QString origGuest = JsonUtil::optString(m_originalGuestAccess,
                                                   QStringLiteral("guest_access"),
                                                   QStringLiteral("forbidden"));
    if (currentGuest != origGuest)
        return true;

    const QString currentVis = m_historyVisibilityCombo->currentData().toString();
    const QString origVis = JsonUtil::optString(m_originalHistoryVisibility,
                                                 QStringLiteral("history_visibility"),
                                                 QStringLiteral("shared"));
    if (currentVis != origVis)
        return true;

    const bool encEnabled = m_encryptionCheck->isChecked();
    const bool origEnc = !m_originalEncryption.isEmpty();
    if (encEnabled != origEnc)
        return true;

    return false;
}

void RoomPermissionsDialog::updateChangesIndicator()
{
    if (hasChanges()) {
        m_saveBtn->setEnabled(true);
        m_changesIndicator->setText(tr("Changes pending \u2014 click Save to apply"));
        m_changesIndicator->setVisible(true);
    } else {
        m_saveBtn->setEnabled(false);
        m_changesIndicator->setText(tr("No changes"));
        m_changesIndicator->setVisible(true);
    }
}

void RoomPermissionsDialog::onSaveClicked()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        return;
    }

    m_saveBtn->setEnabled(false);
    m_statusLabel->setText(tr("Saving permissions..."));
    m_statusLabel->setVisible(true);

    const auto putState = [this, session](const QString &eventType,
                                          const QString &stateKey,
                                          const QJsonObject &content,
                                          const QString &label) {
        const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/%2/%3")
                                 .arg(m_roomId, eventType, stateKey);
        QUrl url(session->homeserverUrl() + path);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
        req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

        QNetworkReply *reply = m_nam->put(req,
            QJsonDocument(content).toJson(QJsonDocument::Compact));

        connect(reply, &QNetworkReply::finished, this, [this, reply, label]() {
            reply->deleteLater();

            if (reply->error() != QNetworkReply::NoError) {
                const QJsonObject json = JsonUtil::parseObject(
                    QString::fromUtf8(reply->readAll()));
                const QString errorMsg = JsonUtil::optString(json, QStringLiteral("error"),
                                                             reply->errorString());
                m_statusLabel->setText(tr("Error saving %1: %2").arg(label, errorMsg));
                return;
            }

            m_statusLabel->setText(tr("%1 saved successfully.").arg(label));
            QTimer::singleShot(1500, this, [this]() {
                m_statusLabel->setVisible(false);
            });
        });
    };

    const QString currentRule = m_joinRuleCombo->currentData().toString();
    const QString origRule = JsonUtil::optString(m_originalJoinRules,
                                                  QStringLiteral("join_rule"),
                                                  QStringLiteral("invite"));
    if (currentRule != origRule) {
        QJsonObject content;
        content[QStringLiteral("join_rule")] = currentRule;
        putState(QStringLiteral("m.room.join_rules"), QString(),
                 content, tr("Join rules"));
    }

    const QString currentGuest = m_guestAccessCombo->currentData().toString();
    const QString origGuest = JsonUtil::optString(m_originalGuestAccess,
                                                   QStringLiteral("guest_access"),
                                                   QStringLiteral("forbidden"));
    if (currentGuest != origGuest) {
        QJsonObject content;
        content[QStringLiteral("guest_access")] = currentGuest;
        putState(QStringLiteral("m.room.guest_access"), QString(),
                 content, tr("Guest access"));
    }

    const QString currentVis = m_historyVisibilityCombo->currentData().toString();
    const QString origVis = JsonUtil::optString(m_originalHistoryVisibility,
                                                 QStringLiteral("history_visibility"),
                                                 QStringLiteral("shared"));
    if (currentVis != origVis) {
        QJsonObject content;
        content[QStringLiteral("history_visibility")] = currentVis;
        putState(QStringLiteral("m.room.history_visibility"), QString(),
                 content, tr("History visibility"));
    }

    const bool encEnabled = m_encryptionCheck->isChecked();
    const bool origEnc = !m_originalEncryption.isEmpty();
    if (encEnabled && !origEnc) {
        QJsonObject content;
        content[QStringLiteral("algorithm")] = QStringLiteral("m.megolm.v1.aes-sha2");
        putState(QStringLiteral("m.room.encryption"), QString(),
                 content, tr("Encryption"));
    }

    m_modified = false;
    updateChangesIndicator();
}

#include "room_permissions_dialog.moc"
