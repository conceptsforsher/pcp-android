#include "welcome_widget.hpp"
#include <QFont>
#include <QSpacerItem>

namespace pcp_chat {

WelcomeWidget::WelcomeWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
}

void WelcomeWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Spacer
    layout->addStretch(2);

    // Title
    m_titleLabel = new QLabel("Welcome to PCP");
    QFont titleFont;
    titleFont.setPixelSize(32);
    titleFont.setBold(true);
    m_titleLabel->setFont(titleFont);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(m_titleLabel);

    layout->addSpacing(10);

    // Subtitle
    m_subtitleLabel = new QLabel(
        "100% C++ desktop Matrix client.
"
        "Fast. Secure. PCP.");
    m_subtitleLabel->setAlignment(Qt::AlignCenter);
    m_subtitleLabel->setStyleSheet("color: #888; font-size: 14px;");
    layout->addWidget(m_subtitleLabel);

    layout->addSpacing(30);

    // Login button
    m_loginBtn = new QPushButton("Log In");
    m_loginBtn->setFixedSize(200, 45);
    m_loginBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "border-radius: 8px; font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45a049; }");
    layout->addWidget(m_loginBtn, 0, Qt::AlignCenter);

    layout->addSpacing(12);

    // Create account button
    m_createAccountBtn = new QPushButton("Create Account");
    m_createAccountBtn->setFixedSize(200, 45);
    m_createAccountBtn->setStyleSheet(
        "QPushButton { background-color: transparent; color: #4CAF50; "
        "border: 2px solid #4CAF50; border-radius: 8px; font-size: 16px; }"
        "QPushButton:hover { background-color: rgba(76, 175, 80, 0.1); }");
    layout->addWidget(m_createAccountBtn, 0, Qt::AlignCenter);

    // Spacer
    layout->addStretch(3);

    // Version label
    auto *versionLabel = new QLabel("v0.3.0 — pcp.chat");
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("color: #666; font-size: 11px;");
    layout->addWidget(versionLabel);

    connect(m_loginBtn, &QPushButton::clicked, this, &WelcomeWidget::loginRequested);
    connect(m_createAccountBtn, &QPushButton::clicked, this, &WelcomeWidget::createAccountRequested);
}

} // namespace pcp_chat
