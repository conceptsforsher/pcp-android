#include "features/auth/lemmy_auth_dialog.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

#include "app/application.hpp"

LemmyAuthDialog::LemmyAuthDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void LemmyAuthDialog::setupUi()
{
    setWindowTitle(tr("Add Lemmy Instance"));
    setMinimumWidth(400);

    auto *mainLayout = new QVBoxLayout(this);

    auto *formLayout = new QFormLayout;

    m_instanceUrl = new QLineEdit(QStringLiteral("https://lemmy.ml"), this);
    m_username = new QLineEdit(this);
    m_password = new QLineEdit(this);
    m_password->setEchoMode(QLineEdit::Password);

    formLayout->addRow(tr("Instance URL:"), m_instanceUrl);
    formLayout->addRow(tr("Username:"), m_username);
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

    connect(m_loginButton, &QPushButton::clicked, this, &LemmyAuthDialog::onLoginClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void LemmyAuthDialog::onLoginClicked()
{
    const QString url = m_instanceUrl->text().trimmed();
    const QString user = m_username->text().trimmed();
    const QString pass = m_password->text();

    if (url.isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Instance URL must not be empty."));
        m_instanceUrl->setFocus();
        return;
    }

    if (user.isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Username must not be empty."));
        m_username->setFocus();
        return;
    }

    if (pass.isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Password must not be empty."));
        m_password->setFocus();
        return;
    }

    m_loginButton->setEnabled(false);

    pcp::Application::instance().addLemmySession();

    accept();
}
