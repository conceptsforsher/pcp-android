#include "matrix_auth_dialog.hpp"
#include "../../protocol/protocol_manager.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>
#include <QDesktopServices>
#include <QMessageBox>
#include <QTimer>

namespace pcp_chat {

MatrixAuthDialog::MatrixAuthDialog(ProtocolManager *manager, QWidget *parent)
    : QDialog(parent)
    , m_protocolManager(manager)
{
    setWindowTitle("PCP — Sign In");
    setMinimumSize(450, 400);

    auto *mainLayout = new QVBoxLayout(this);

    // Tab widget
    m_tabWidget = new QTabWidget();
    setupLoginTab();
    setupRegisterTab();
    setupSsoTab();
    mainLayout->addWidget(m_tabWidget);

    // Cancel button
    auto *cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    mainLayout->addWidget(cancelBtn, 0, Qt::AlignRight);

    // Auto-discover server when URL changes
    connect(m_loginServer, &QLineEdit::editingFinished, this, &MatrixAuthDialog::onServerDiscovery);
}

void MatrixAuthDialog::setupLoginTab()
{
    auto *tab = new QWidget();
    auto *layout = new QFormLayout(tab);

    m_loginServer = new QLineEdit("https://matrix.org");
    m_loginServer->setPlaceholderText("https://matrix.org");
    layout->addRow("Homeserver URL:", m_loginServer);

    m_loginUser = new QLineEdit();
    m_loginUser->setPlaceholderText("@user:matrix.org");
    layout->addRow("Username:", m_loginUser);

    m_loginPassword = new QLineEdit();
    m_loginPassword->setEchoMode(QLineEdit::Password);
    m_loginPassword->setPlaceholderText("Password");
    layout->addRow("Password:", m_loginPassword);

    m_loginRemember = new QCheckBox("Remember me");
    m_loginRemember->setChecked(true);
    layout->addRow("", m_loginRemember);

    m_loginProgress = new QProgressBar();
    m_loginProgress->setRange(0, 0);
    m_loginProgress->setVisible(false);
    layout->addRow("", m_loginProgress);

    m_loginError = new QLabel();
    m_loginError->setStyleSheet("color: red;");
    m_loginError->setWordWrap(true);
    layout->addRow("", m_loginError);

    m_loginBtn = new QPushButton("Sign In");
    m_loginBtn->setFixedHeight(40);
    m_loginBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "border-radius: 6px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45a049; }");
    connect(m_loginBtn, &QPushButton::clicked, this, &MatrixAuthDialog::onLoginClicked);
    layout->addRow("", m_loginBtn);

    m_tabWidget->addTab(tab, "Password");
}

void MatrixAuthDialog::setupRegisterTab()
{
    auto *tab = new QWidget();
    auto *layout = new QFormLayout(tab);

    m_regServer = new QLineEdit("https://matrix.org");
    layout->addRow("Homeserver URL:", m_regServer);

    m_regUser = new QLineEdit();
    m_regUser->setPlaceholderText("username");
    layout->addRow("Username:", m_regUser);

    m_regPassword = new QLineEdit();
    m_regPassword->setEchoMode(QLineEdit::Password);
    layout->addRow("Password:", m_regPassword);

    m_regConfirm = new QLineEdit();
    m_regConfirm->setEchoMode(QLineEdit::Password);
    layout->addRow("Confirm Password:", m_regConfirm);

    m_regEmail = new QLineEdit();
    m_regEmail->setPlaceholderText("user@example.com");
    layout->addRow("Email:", m_regEmail);

    m_regError = new QLabel();
    m_regError->setStyleSheet("color: red;");
    m_regError->setWordWrap(true);
    layout->addRow("", m_regError);

    m_registerBtn = new QPushButton("Create Account");
    m_registerBtn->setFixedHeight(40);
    connect(m_registerBtn, &QPushButton::clicked, this, &MatrixAuthDialog::onRegisterClicked);
    layout->addRow("", m_registerBtn);

    m_tabWidget->addTab(tab, "Register");
}

void MatrixAuthDialog::setupSsoTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *infoLabel = new QLabel(
        "Single Sign-On allows you to log in using your homeserver's "
        "identity provider (Google, GitHub, GitLab, etc.)");
    infoLabel->setWordWrap(true);
    layout->addWidget(infoLabel);

    auto *formLayout = new QFormLayout();
    m_ssoServer = new QLineEdit("https://matrix.org");
    formLayout->addRow("Homeserver URL:", m_ssoServer);
    layout->addLayout(formLayout);

    m_ssoStartBtn = new QPushButton("Continue with SSO");
    m_ssoStartBtn->setFixedHeight(40);
    connect(m_ssoStartBtn, &QPushButton::clicked, this, &MatrixAuthDialog::onSsoLoginClicked);
    layout->addWidget(m_ssoStartBtn);

    m_ssoStatus = new QLabel();
    m_ssoStatus->setStyleSheet("color: #888;");
    layout->addWidget(m_ssoStatus);

    layout->addStretch();

    m_tabWidget->addTab(tab, "Single Sign-On");
}

void MatrixAuthDialog::onLoginClicked()
{
    QString server = m_loginServer->text().trimmed();
    QString user = m_loginUser->text().trimmed();
    QString password = m_loginPassword->text();

    if (server.isEmpty() || user.isEmpty() || password.isEmpty()) {
        showError("All fields are required");
        return;
    }

    showProgress(true);
    setFormEnabled(false);
    m_loginError->clear();

    // Matrix login API call
    auto *manager = new QNetworkAccessManager(this);
    QUrl url(server + "/_matrix/client/v3/login");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body;
    body["type"] = "m.login.password";
    body["identifier"] = QJsonObject{{"type", "m.id.user"}, {"user", user}};
    body["password"] = password;
    body["initial_device_display_name"] = "PCP Desktop";

    auto *reply = manager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        showProgress(false);
        setFormEnabled(true);

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            QString userId = obj["user_id"].toString();
            QString token = obj["access_token"].toString();
            QString deviceId = obj["device_id"].toString();

            if (m_loginRemember->isChecked()) {
                // Save session
                m_protocolManager->restoreSession("matrix", userId, server, token);
            }

            onLoginSuccess(userId, server);
            accept();
        } else {
            int statusCode = reply->attribute(
                QNetworkRequest::HttpStatusCodeAttribute).toInt();
            if (statusCode == 403) {
                showError("Invalid username or password");
            } else if (statusCode == 429) {
                showError("Too many attempts. Please wait and try again.");
            } else {
                showError("Login failed: " + reply->errorString());
            }
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MatrixAuthDialog::onRegisterClicked()
{
    QString server = m_regServer->text().trimmed();
    QString user = m_regUser->text().trimmed();
    QString password = m_regPassword->text();
    QString confirm = m_regConfirm->text();

    if (server.isEmpty() || user.isEmpty() || password.isEmpty()) {
        m_regError->setText("All fields are required");
        return;
    }

    if (password != confirm) {
        m_regError->setText("Passwords do not match");
        return;
    }

    if (password.length() < 8) {
        m_regError->setText("Password must be at least 8 characters");
        return;
    }

    setFormEnabled(false);

    // Registration flow: check server capabilities first
    auto *manager = new QNetworkAccessManager(this);
    QUrl url(server + "/_matrix/client/v3/register");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject body;
    body["username"] = user;
    body["password"] = password;
    body["initial_device_display_name"] = "PCP Desktop";

    auto *reply = manager->post(request, QJsonDocument(body).toJson());
    connect(reply, &QNetworkReply::finished, this, [=]() {
        setFormEnabled(true);

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            QString userId = obj["user_id"].toString();
            QString token = obj["access_token"].toString();

            QMessageBox::information(this, "Account Created",
                "Account created successfully!\n" + userId);

            m_protocolManager->restoreSession("matrix", userId, server, token);
            accept();
        } else {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QString error = doc.object()["error"].toString();
            m_regError->setText(error.isEmpty() ? "Registration failed" : error);
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MatrixAuthDialog::onSsoLoginClicked()
{
    QString server = m_ssoServer->text().trimmed();
    if (server.isEmpty()) {
        m_ssoStatus->setText("Please enter a homeserver URL");
        return;
    }

    m_ssoStatus->setText("Opening browser for SSO authentication...");

    // Build SSO URL
    QString redirectUrl = "pcp-chat://sso/callback";
    QUrl ssoUrl(server + "/_matrix/client/v3/login/sso/redirect");
    ssoUrl.setQuery("redirectUrl=" + QUrl::toPercentEncoding(redirectUrl));

    QDesktopServices::openUrl(ssoUrl);
}

void MatrixAuthDialog::onServerDiscovery()
{
    // Try to discover well-known info for better UX
}

void MatrixAuthDialog::onLoginSuccess(const QString &userId, const QString &server)
{
    m_loginError->setStyleSheet("color: green;");
    m_loginError->setText("Logged in as " + userId);
}

void MatrixAuthDialog::onLoginError(const QString &error)
{
    showError(error);
}

void MatrixAuthDialog::onRegistrationSuccess()
{
    accept();
}

void MatrixAuthDialog::showError(const QString &message)
{
    m_loginError->setStyleSheet("color: red;");
    m_loginError->setText(message);
}

void MatrixAuthDialog::showProgress(bool show)
{
    m_loginProgress->setVisible(show);
}

void MatrixAuthDialog::setFormEnabled(bool enabled)
{
    m_loginServer->setEnabled(enabled);
    m_loginUser->setEnabled(enabled);
    m_loginPassword->setEnabled(enabled);
    m_loginBtn->setEnabled(enabled);
}

} // namespace pcp_chat
