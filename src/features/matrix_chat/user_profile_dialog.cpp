#include "features/matrix_chat/user_profile_dialog.hpp"

#include <QDateTime>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "ui/components/avatar_widget.hpp"

UserProfileDialog::UserProfileDialog(const QString &userId, QWidget *parent)
    : QDialog(parent)
    , m_userId(userId)
    , m_avatar(nullptr)
    , m_displayNameLabel(nullptr)
    , m_displayNameEdit(nullptr)
    , m_userIdLabel(nullptr)
    , m_presenceDot(nullptr)
    , m_presenceLabel(nullptr)
    , m_statusMessageLabel(nullptr)
    , m_statusMessageEdit(nullptr)
    , m_startChatButton(nullptr)
    , m_inviteButton(nullptr)
    , m_blockButton(nullptr)
    , m_encryptionKeysButton(nullptr)
    , m_editButton(nullptr)
    , m_saveButton(nullptr)
    , m_cancelEditButton(nullptr)
    , m_joinedLabel(nullptr)
    , m_lastSeenLabel(nullptr)
    , m_deviceCountLabel(nullptr)
{
    auto *session = pcp::Application::instance().matrixSession();
    m_profile.userId = m_userId;
    m_profile.displayName = m_userId;
    if (session) {
        m_isOwnProfile = (session->userId() == m_userId);
    } else {
        m_isOwnProfile = false;
    }

    setupUi();
    populateFromProfile(m_profile);
}

void UserProfileDialog::setupUi()
{
    setWindowTitle(tr("User Profile"));
    setMinimumSize(380, 440);
    setMaximumWidth(480);
    setModal(true);

    setStyleSheet(QStringLiteral(
        "UserProfileDialog { background-color: #f5f5f5; }"
    ));

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);

    // ── Header section ──────────────────────────────────────────
    auto *headerLayout = new QVBoxLayout;
    headerLayout->setSpacing(6);

    // Avatar – centered
    auto *avatarRow = new QHBoxLayout;
    avatarRow->addStretch();
    m_avatar = new AvatarWidget(QString(), QString(), 80, this);
    avatarRow->addWidget(m_avatar);
    avatarRow->addStretch();
    headerLayout->addLayout(avatarRow);

    // Display name – label + edit
    auto *nameRow = new QHBoxLayout;
    nameRow->addStretch();

    m_displayNameLabel = new QLabel(this);
    m_displayNameLabel->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: bold; color: #212121;"));
    m_displayNameLabel->setAlignment(Qt::AlignCenter);
    nameRow->addWidget(m_displayNameLabel);

    m_displayNameEdit = new QLineEdit(this);
    m_displayNameEdit->setStyleSheet(QStringLiteral("font-size: 18px; font-weight: bold; padding: 2px 6px;"));
    m_displayNameEdit->setAlignment(Qt::AlignCenter);
    m_displayNameEdit->setVisible(false);
    m_displayNameEdit->setMaximumWidth(260);
    nameRow->addWidget(m_displayNameEdit);

    nameRow->addStretch();
    headerLayout->addLayout(nameRow);

    // User ID
    m_userIdLabel = new QLabel(this);
    m_userIdLabel->setStyleSheet(QStringLiteral("font-size: 12px; color: #757575;"));
    m_userIdLabel->setAlignment(Qt::AlignCenter);
    headerLayout->addWidget(m_userIdLabel);

    // Presence row
    auto *presenceRow = new QHBoxLayout;
    presenceRow->addStretch();

    m_presenceDot = new QLabel(this);
    m_presenceDot->setFixedSize(10, 10);
    presenceRow->addWidget(m_presenceDot);

    m_presenceLabel = new QLabel(this);
    m_presenceLabel->setStyleSheet(QStringLiteral("font-size: 12px; color: #616161;"));
    presenceRow->addWidget(m_presenceLabel);

    presenceRow->addStretch();
    headerLayout->addLayout(presenceRow);

    // Status message
    auto *statusRow = new QHBoxLayout;
    statusRow->addStretch();

    m_statusMessageLabel = new QLabel(this);
    m_statusMessageLabel->setWordWrap(true);
    m_statusMessageLabel->setAlignment(Qt::AlignCenter);
    m_statusMessageLabel->setStyleSheet(QStringLiteral(
        "font-size: 12px; font-style: italic; color: #9E9E9E; padding: 4px 16px;"
    ));
    statusRow->addWidget(m_statusMessageLabel);

    m_statusMessageEdit = new QLineEdit(this);
    m_statusMessageEdit->setPlaceholderText(tr("Set a status message…"));
    m_statusMessageEdit->setStyleSheet(QStringLiteral(
        "font-size: 12px; padding: 2px 6px;"
    ));
    m_statusMessageEdit->setAlignment(Qt::AlignCenter);
    m_statusMessageEdit->setMaximumWidth(280);
    m_statusMessageEdit->setVisible(false);
    statusRow->addWidget(m_statusMessageEdit);

    statusRow->addStretch();
    headerLayout->addLayout(statusRow);

    headerLayout->addSpacing(4);

    mainLayout->addLayout(headerLayout);

    // ── Separator ───────────────────────────────────────────────
    auto *separator1 = new QFrame(this);
    separator1->setFrameShape(QFrame::HLine);
    separator1->setStyleSheet(QStringLiteral("color: #E0E0E0;"));
    mainLayout->addWidget(separator1);

    // ── Action buttons ──────────────────────────────────────────
    auto *buttonsLayout = new QVBoxLayout;
    buttonsLayout->setSpacing(6);

    m_startChatButton = new QPushButton(tr("Start Chat"), this);
    m_startChatButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #1976D2; color: white; border: none;"
        " border-radius: 4px; padding: 8px 16px; font-size: 13px; font-weight: bold; }"
        "QPushButton:hover { background-color: #1565C0; }"
        "QPushButton:pressed { background-color: #0D47A1; }"
    ));
    m_startChatButton->setCursor(Qt::PointingHandCursor);
    buttonsLayout->addWidget(m_startChatButton);

    m_inviteButton = new QPushButton(tr("Invite to Room"), this);
    m_inviteButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #388E3C; color: white; border: none;"
        " border-radius: 4px; padding: 8px 16px; font-size: 13px; }"
        "QPushButton:hover { background-color: #2E7D32; }"
        "QPushButton:pressed { background-color: #1B5E20; }"
    ));
    m_inviteButton->setCursor(Qt::PointingHandCursor);
    buttonsLayout->addWidget(m_inviteButton);

    m_blockButton = new QPushButton(tr("Block User"), this);
    m_blockButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #D32F2F; color: white; border: none;"
        " border-radius: 4px; padding: 8px 16px; font-size: 13px; }"
        "QPushButton:hover { background-color: #C62828; }"
        "QPushButton:pressed { background-color: #B71C1C; }"
    ));
    m_blockButton->setCursor(Qt::PointingHandCursor);
    buttonsLayout->addWidget(m_blockButton);

    m_encryptionKeysButton = new QPushButton(tr("View Encryption Keys"), this);
    m_encryptionKeysButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #5C6BC0; color: white; border: none;"
        " border-radius: 4px; padding: 8px 16px; font-size: 13px; }"
        "QPushButton:hover { background-color: #3F51B5; }"
        "QPushButton:pressed { background-color: #303F9F; }"
    ));
    m_encryptionKeysButton->setCursor(Qt::PointingHandCursor);
    buttonsLayout->addWidget(m_encryptionKeysButton);

    mainLayout->addLayout(buttonsLayout);

    // ── Separator ───────────────────────────────────────────────
    auto *separator2 = new QFrame(this);
    separator2->setFrameShape(QFrame::HLine);
    separator2->setStyleSheet(QStringLiteral("color: #E0E0E0;"));
    mainLayout->addWidget(separator2);

    // ── Information section ────────────────────────────────────
    auto *infoLayout = new QFormLayout;
    infoLayout->setContentsMargins(8, 0, 8, 0);
    infoLayout->setSpacing(8);

    m_joinedLabel = new QLabel(this);
    m_joinedLabel->setStyleSheet(QStringLiteral("font-size: 12px; color: #424242;"));
    infoLayout->addRow(tr("Joined:"), m_joinedLabel);

    m_lastSeenLabel = new QLabel(this);
    m_lastSeenLabel->setStyleSheet(QStringLiteral("font-size: 12px; color: #424242;"));
    infoLayout->addRow(tr("Last seen:"), m_lastSeenLabel);

    m_deviceCountLabel = new QLabel(this);
    m_deviceCountLabel->setStyleSheet(QStringLiteral("font-size: 12px; color: #424242;"));
    infoLayout->addRow(tr("Sessions:"), m_deviceCountLabel);

    mainLayout->addLayout(infoLayout);

    mainLayout->addSpacing(4);

    // ── Edit / Save buttons (own profile) ──────────────────────
    auto *editButtonRow = new QHBoxLayout;
    editButtonRow->addStretch();

    m_editButton = new QPushButton(tr("Edit Profile"), this);
    m_editButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: transparent; color: #1976D2; border: 1px solid #1976D2;"
        " border-radius: 4px; padding: 6px 20px; font-size: 12px; }"
        "QPushButton:hover { background-color: #E3F2FD; }"
    ));
    m_editButton->setCursor(Qt::PointingHandCursor);
    m_editButton->setVisible(m_isOwnProfile);
    editButtonRow->addWidget(m_editButton);

    m_saveButton = new QPushButton(tr("Save"), this);
    m_saveButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: #43A047; color: white; border: none;"
        " border-radius: 4px; padding: 6px 20px; font-size: 12px; font-weight: bold; }"
        "QPushButton:hover { background-color: #388E3C; }"
    ));
    m_saveButton->setCursor(Qt::PointingHandCursor);
    m_saveButton->setVisible(false);
    editButtonRow->addWidget(m_saveButton);

    m_cancelEditButton = new QPushButton(tr("Cancel"), this);
    m_cancelEditButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: transparent; color: #757575; border: 1px solid #BDBDBD;"
        " border-radius: 4px; padding: 6px 20px; font-size: 12px; }"
        "QPushButton:hover { background-color: #EEEEEE; }"
    ));
    m_cancelEditButton->setCursor(Qt::PointingHandCursor);
    m_cancelEditButton->setVisible(false);
    editButtonRow->addWidget(m_cancelEditButton);

    editButtonRow->addStretch();
    mainLayout->addLayout(editButtonRow);

    // ── Close button (bottom) ───────────────────────────────────
    auto *closeRow = new QHBoxLayout;
    closeRow->addStretch();
    auto *closeButton = new QPushButton(tr("Close"), this);
    closeButton->setStyleSheet(QStringLiteral(
        "QPushButton { background-color: transparent; color: #616161;"
        " border: 1px solid #BDBDBD; border-radius: 4px; padding: 6px 24px; font-size: 12px; }"
        "QPushButton:hover { background-color: #EEEEEE; }"
    ));
    closeButton->setCursor(Qt::PointingHandCursor);
    closeRow->addWidget(closeButton);
    closeRow->addStretch();
    mainLayout->addLayout(closeRow);

    // ── Connections ─────────────────────────────────────────────
    connect(m_startChatButton, &QPushButton::clicked, this, &UserProfileDialog::onStartChat);
    connect(m_inviteButton, &QPushButton::clicked, this, &UserProfileDialog::onInviteToRoom);
    connect(m_blockButton, &QPushButton::clicked, this, &UserProfileDialog::onBlockUser);
    connect(m_encryptionKeysButton, &QPushButton::clicked,
            this, &UserProfileDialog::onViewEncryptionKeys);
    connect(m_editButton, &QPushButton::clicked, this, &UserProfileDialog::onEditProfile);
    connect(m_saveButton, &QPushButton::clicked, this, &UserProfileDialog::onSaveProfile);
    connect(m_cancelEditButton, &QPushButton::clicked, this, [this]() {
        setEditable(false);
    });
    connect(closeButton, &QPushButton::clicked, this, &QDialog::accept);

    // Hide action buttons that don't apply to own profile
    if (m_isOwnProfile) {
        m_startChatButton->setVisible(false);
        m_inviteButton->setVisible(false);
        m_blockButton->setVisible(false);
    }
}

void UserProfileDialog::populateFromProfile(const UserProfileData &profile)
{
    m_avatar->setName(profile.displayName);
    if (!profile.avatarUrl.isEmpty()) {
        m_avatar->setImageUrl(profile.avatarUrl);
    }

    m_displayNameLabel->setText(profile.displayName.isEmpty() ? profile.userId : profile.displayName);
    m_displayNameEdit->setText(profile.displayName);

    // Format user ID as @user:server
    if (profile.userId.contains(':')) {
        m_userIdLabel->setText(profile.userId);
    } else {
        m_userIdLabel->setText(QStringLiteral("@%1:matrix.org").arg(profile.userId));
    }

    // Presence indicator
    const QColor dotColor = presenceColor(profile.presence);
    m_presenceDot->setStyleSheet(QStringLiteral(
        "background-color: %1; border-radius: 5px;"
    ).arg(dotColor.name()));
    m_presenceLabel->setText(presenceText(profile.presence));

    // Status message
    if (profile.statusMessage.isEmpty()) {
        m_statusMessageLabel->setText(tr("No status message"));
    } else {
        m_statusMessageLabel->setText(profile.statusMessage);
    }
    m_statusMessageEdit->setText(profile.statusMessage);

    // Info section
    if (profile.joinedDate.isValid()) {
        m_joinedLabel->setText(profile.joinedDate.toString(QStringLiteral("dd MMM yyyy")));
    } else {
        m_joinedLabel->setText(tr("Unknown"));
    }

    if (profile.lastActiveAgo >= 0) {
        const qint64 minutes = profile.lastActiveAgo / 60000;
        if (minutes < 1) {
            m_lastSeenLabel->setText(tr("Just now"));
        } else if (minutes < 60) {
            m_lastSeenLabel->setText(tr("%1 min ago").arg(minutes));
        } else if (minutes < 1440) {
            m_lastSeenLabel->setText(tr("%1 hours ago").arg(minutes / 60));
        } else {
            m_lastSeenLabel->setText(tr("%1 days ago").arg(minutes / 1440));
        }
    } else {
        m_lastSeenLabel->setText(tr("Unknown"));
    }

    if (profile.deviceCount > 0) {
        m_deviceCountLabel->setText(QString::number(profile.deviceCount));
    } else {
        m_deviceCountLabel->setText(tr("Unknown"));
    }
}

void UserProfileDialog::setEditable(bool editable)
{
    m_isEditing = editable;

    m_displayNameLabel->setVisible(!editable);
    m_displayNameEdit->setVisible(editable);
    m_statusMessageLabel->setVisible(!editable);
    m_statusMessageEdit->setVisible(editable);

    m_editButton->setVisible(!editable);
    m_saveButton->setVisible(editable);
    m_cancelEditButton->setVisible(editable);

    if (editable) {
        m_displayNameEdit->setFocus();
        m_displayNameEdit->selectAll();
    }
}

QColor UserProfileDialog::presenceColor(const QString &presence)
{
    if (presence == QStringLiteral("online"))
        return QColor(QStringLiteral("#4CAF50"));
    if (presence == QStringLiteral("unavailable"))
        return QColor(QStringLiteral("#FFC107"));
    if (presence == QStringLiteral("busy"))
        return QColor(QStringLiteral("#F44336"));
    return QColor(QStringLiteral("#9E9E9E")); // offline / unknown
}

QString UserProfileDialog::presenceText(const QString &presence)
{
    if (presence == QStringLiteral("online"))
        return tr("Online");
    if (presence == QStringLiteral("unavailable"))
        return tr("Away");
    if (presence == QStringLiteral("busy"))
        return tr("Busy");
    return tr("Offline");
}

// ── Slots ──────────────────────────────────────────────────────────

void UserProfileDialog::onStartChat()
{
    emit chatStarted(m_userId);
    accept();
}

void UserProfileDialog::onInviteToRoom()
{
    bool ok = false;
    const QString roomId = QInputDialog::getText(
        this, tr("Invite to Room"),
        tr("Enter the room ID or alias to invite this user:"),
        QLineEdit::Normal, QString(), &ok);

    if (ok && !roomId.isEmpty()) {
        emit inviteRequested(m_userId);
        QMessageBox::information(this, tr("Invitation"),
                                 tr("User has been invited to the room."));
    }
}

void UserProfileDialog::onBlockUser()
{
    auto reply = QMessageBox::question(
        this,
        tr("Block User"),
        tr("Are you sure you want to block \"%1\"?\n\n"
           "Blocked users will not be able to send you messages.")
            .arg(m_profile.displayName.isEmpty() ? m_userId : m_profile.displayName),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit userBlocked(m_userId);
        QMessageBox::information(this, tr("Blocked"),
                                 tr("\"%1\" has been blocked.")
                                     .arg(m_profile.displayName.isEmpty()
                                              ? m_userId
                                              : m_profile.displayName));
        m_blockButton->setEnabled(false);
        m_blockButton->setText(tr("Blocked"));
    }
}

void UserProfileDialog::onViewEncryptionKeys()
{
    emit encryptionKeysRequested(m_userId);
}

void UserProfileDialog::onEditProfile()
{
    setEditable(true);
}

void UserProfileDialog::onSaveProfile()
{
    const QString newName = m_displayNameEdit->text().trimmed();
    const QString newStatus = m_statusMessageEdit->text().trimmed();

    if (newName.isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"),
                             tr("Display name cannot be empty."));
        m_displayNameEdit->setFocus();
        return;
    }

    m_displayNameLabel->setText(newName);
    m_statusMessageLabel->setText(newStatus.isEmpty() ? tr("No status message") : newStatus);
    m_avatar->setName(newName);

    m_profile.displayName = newName;
    m_profile.statusMessage = newStatus;

    setEditable(false);
    emit profileUpdated(newName, m_profile.avatarUrl, newStatus);
}
