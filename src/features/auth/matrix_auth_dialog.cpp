#include "features/auth/matrix_auth_dialog.hpp"

#include <QFormLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>

#include "app/application.hpp"

MatrixAuthDialog::MatrixAuthDialog(QWidget *parent)
    : QDialog(parent)
{
    setupUi();
}

void MatrixAuthDialog::setupUi()
{
    setWindowTitle(tr("Add Matrix Account"));
    setMinimumWidth(400);

    auto *mainLayout = new QVBoxLayout(this);

    auto *formLayout = new QFormLayout;

    m_homeserverUrl = new QLineEdit(QStringLiteral("https://matrix.org"), this);
    m_username = new QLineEdit(this);
    m_password = new QLineEdit(this);
    m_password->setEchoMode(QLineEdit::Password);

    formLayout->addRow(tr("Homeserver URL:"), m_homeserverUrl);
    formLayout->addRow(tr("Username:"), m_username);
    formLayout->addRow(tr("Password:"), m_password);

    mainLayout->addLayout(formLayout);

    mainLayout->addSpacing(12);

    auto *progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0);
    progressBar->setVisible(false);
    progressBar->setObjectName(QStringLiteral("progressBar"));
    mainLayout->addWidget(progressBar);

    mainLayout->addSpacing(12);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    m_registerButton = new QPushButton(tr("Register..."), this);
    m_loginButton = new QPushButton(tr("Login"), this);
    m_loginButton->setDefault(true);

    auto *cancelButton = new QPushButton(tr("Cancel"), this);

    buttonLayout->addWidget(m_registerButton);
    buttonLayout->addWidget(m_loginButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(m_loginButton, &QPushButton::clicked, this, &MatrixAuthDialog::onLoginClicked);
    connect(m_registerButton, &QPushButton::clicked, this, &MatrixAuthDialog::onRegisterClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void MatrixAuthDialog::onLoginClicked()
{
    const QString url = m_homeserverUrl->text().trimmed();
    const QString user = m_username->text().trimmed();
    const QString pass = m_password->text();

    if (url.isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Homeserver URL must not be empty."));
        m_homeserverUrl->setFocus();
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

    auto *progressBar = findChild<QProgressBar *>(QStringLiteral("progressBar"));
    if (progressBar) {
        progressBar->setVisible(true);
    }
    m_loginButton->setEnabled(false);
    m_registerButton->setEnabled(false);

    pcp::Application::instance().startProtocolManager();

    if (progressBar) {
        progressBar->setVisible(false);
    }
    m_loginButton->setEnabled(true);
    m_registerButton->setEnabled(true);

    accept();
}

void MatrixAuthDialog::onRegisterClicked()
{
    QMessageBox::information(this, tr("Register"),
                             tr("Please visit your homeserver's registration page to create an account."));
}
