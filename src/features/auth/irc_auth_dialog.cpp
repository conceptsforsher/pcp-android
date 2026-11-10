#include "features/auth/irc_auth_dialog.hpp"

#include <QCheckBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "irc/irc_session_holder.hpp"

IrcAuthDialog::IrcAuthDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void IrcAuthDialog::setupUi()
{
    setWindowTitle(tr("Add IRC Server"));
    setMinimumWidth(400);

    auto *mainLayout = new QVBoxLayout(this);

    auto *formLayout = new QFormLayout;

    m_host = new QLineEdit(this);
    m_host->setPlaceholderText(QStringLiteral("irc.libera.chat"));

    m_port = new QSpinBox(this);
    m_port->setRange(1, 65535);
    m_port->setValue(6667);

    m_ssl = new QCheckBox(tr("Use SSL/TLS"), this);

    m_nickname = new QLineEdit(this);
    m_username = new QLineEdit(this);
    m_realName = new QLineEdit(this);
    m_password = new QLineEdit(this);
    m_password->setEchoMode(QLineEdit::Password);
    m_password->setPlaceholderText(tr("Optional"));

    formLayout->addRow(tr("Server Host:"), m_host);
    formLayout->addRow(tr("Port:"), m_port);
    formLayout->addRow(QString(), m_ssl);
    formLayout->addRow(tr("Nickname:"), m_nickname);
    formLayout->addRow(tr("Username:"), m_username);
    formLayout->addRow(tr("Real Name:"), m_realName);
    formLayout->addRow(tr("Password:"), m_password);

    mainLayout->addLayout(formLayout);
    mainLayout->addSpacing(12);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    m_loginButton = new QPushButton(tr("Login"), this);
    m_loginButton->setDefault(true);
    auto *cancelButton = new QPushButton(tr("Cancel"), this);

    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(m_loginButton, &QPushButton::clicked, this, &IrcAuthDialog::onLoginClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

IrcServerInfo IrcAuthDialog::serverInfo() const
{
    IrcServerInfo info;
    info.host = m_host->text().trimmed();
    info.port = static_cast<quint16>(m_port->value());
    info.useTls = m_ssl->isChecked();
    info.nick = m_nickname->text().trimmed();
    info.user = m_username->text().trimmed();
    info.realName = m_realName->text().trimmed();
    info.password = m_password->text();
    return info;
}

void IrcAuthDialog::onLoginClicked()
{
    if (m_host->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Server host must not be empty."));
        m_host->setFocus();
        return;
    }

    if (m_nickname->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Nickname must not be empty."));
        m_nickname->setFocus();
        return;
    }

    if (m_username->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Username must not be empty."));
        m_username->setFocus();
        return;
    }

    m_loginButton->setEnabled(false);

    pcp::Application::instance().addIrcSession();

    accept();
}
