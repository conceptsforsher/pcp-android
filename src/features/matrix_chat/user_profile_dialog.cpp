#include "user_profile_dialog.hpp"
#include "../../protocol/protocol_manager.hpp"
#include "../../protocol/protocol_session.hpp"

#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>

namespace pcp_chat {

UserProfileDialog::UserProfileDialog(ProtocolManager *manager, QWidget *parent)
    : QDialog(parent)
    , m_protocolManager(manager)
{
    setWindowTitle("User Profile");
    setMinimumSize(400, 300);
    setupUi();
    loadProfile();
}

void UserProfileDialog::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    // Avatar section
    auto *avatarLayout = new QHBoxLayout();
    m_avatarLabel = new QLabel();
    m_avatarLabel->setFixedSize(80, 80);
    m_avatarLabel->setStyleSheet(
        "background-color: #ddd; border-radius: 40px; text-align: center; font-size: 32px;");
    m_avatarLabel->setAlignment(Qt::AlignCenter);
    m_avatarLabel->setText("👤");
    avatarLayout->addWidget(m_avatarLabel);

    auto *avatarInfoLayout = new QVBoxLayout();
    m_userIdLabel = new QLabel("Not logged in");
    m_userIdLabel->setStyleSheet("font-size: 14px; font-weight: bold;");
    avatarInfoLayout->addWidget(m_userIdLabel);

    m_changeAvatarBtn = new QPushButton("Change Avatar...");
    connect(m_changeAvatarBtn, &QPushButton::clicked, this, &UserProfileDialog::onChangeAvatar);
    avatarInfoLayout->addWidget(m_changeAvatarBtn);

    avatarLayout->addLayout(avatarInfoLayout);
    avatarLayout->addStretch();
    layout->addLayout(avatarLayout);

    // Profile form
    auto *formLayout = new QFormLayout();

    m_displayNameEdit = new QLineEdit();
    m_displayNameEdit->setPlaceholderText("Display name");
    formLayout->addRow("Display Name:", m_displayNameEdit);

    m_statusEdit = new QTextEdit();
    m_statusEdit->setPlaceholderText("Status message");
    m_statusEdit->setMaximumHeight(80);
    formLayout->addRow("Status:", m_statusEdit);

    layout->addLayout(formLayout);

    // Buttons
    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    m_saveBtn = new QPushButton("Save");
    m_saveBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; "
        "border: none; border-radius: 4px; padding: 8px 24px; }"
        "QPushButton:hover { background-color: #45a049; }");
    connect(m_saveBtn, &QPushButton::clicked, this, &UserProfileDialog::saveProfile);
    btnLayout->addWidget(m_saveBtn);

    auto *closeBtn = new QPushButton("Close");
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::accept);
    btnLayout->addWidget(closeBtn);

    layout->addLayout(btnLayout);
}

void UserProfileDialog::loadProfile()
{
    auto sessions = m_protocolManager->allSessions();
    if (sessions.isEmpty()) return;

    auto *session = sessions.first();
    m_userIdLabel->setText(session->userId());
    m_displayNameEdit->setText(session->displayName());
}

void UserProfileDialog::saveProfile()
{
    QMessageBox::information(this, "Profile", "Profile saved.");
}

void UserProfileDialog::onChangeAvatar()
{
    QString filename = QFileDialog::getOpenFileName(this, "Select Avatar",
        "", "Images (*.png *.jpg *.jpeg *.gif)");
    if (!filename.isEmpty()) {
        // Upload avatar
    }
}

} // namespace pcp_chat
