#include "matrix_login_widget.hpp"
#include "../../protocol/protocol_manager.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QUrl>

namespace pcp_chat {

MatrixLoginWidget::MatrixLoginWidget(ProtocolManager *manager, QWidget *parent)
    : QWidget(parent)
    , m_protocolManager(manager)
{
    setupUi();
}

void MatrixLoginWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    auto *titleLabel = new QLabel("Sign in to your Matrix account");
    QFont titleFont;
    titleFont.setPixelSize(18);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    auto *formLayout = new QFormLayout();

    m_serverEdit = new QLineEdit("https://matrix.org");
    m_serverEdit->setPlaceholderText("https://matrix.org");
    formLayout->addRow("Homeserver:", m_serverEdit);

    m_userEdit = new QLineEdit();
    m_userEdit->setPlaceholderText("@user:matrix.org");
    formLayout->addRow("Username:", m_userEdit);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setEchoMode(QLineEdit::Password);
    m_passwordEdit->setPlaceholderText("Password");
    formLayout->addRow("Password:", m_passwordEdit);

    m_rememberCheck = new QCheckBox("Remember me on this device");
    m_rememberCheck->setChecked(true);
    formLayout->addRow("", m_rememberCheck);

    layout->addLayout(formLayout);

    m_progressBar = new QProgressBar();
    m_progressBar->setRange(0, 0);
    m_progressBar->setVisible(false);
    layout->addWidget(m_progressBar);

    m_errorLabel = new QLabel();
    m_errorLabel->setStyleSheet("color: red;");
    m_errorLabel->setWordWrap(true);
    layout->addWidget(m_errorLabel);

    m_loginBtn = new QPushButton("Sign In");
    m_loginBtn->setFixedHeight(40);
    m_loginBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "border-radius: 6px; font-size: 14px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45a049; }");
    connect(m_loginBtn, &QPushButton::clicked, this, &MatrixLoginWidget::onLoginClicked);
    layout->addWidget(m_loginBtn);

    layout->addStretch();
}

void MatrixLoginWidget::onLoginClicked()
{
    QString server = m_serverEdit->text().trimmed();
    QString user = m_userEdit->text().trimmed();
    QString password = m_passwordEdit->text();

    if (server.isEmpty() || user.isEmpty() || password.isEmpty()) {
        m_errorLabel->setText("All fields are required");
        return;
    }

    setFormEnabled(false);
    m_progressBar->setVisible(true);
    m_errorLabel->clear();

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
        setFormEnabled(true);
        m_progressBar->setVisible(false);

        if (reply->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
            QJsonObject obj = doc.object();
            QString userId = obj["user_id"].toString();
            QString token = obj["access_token"].toString();

            if (m_rememberCheck->isChecked()) {
                m_protocolManager->restoreSession("matrix", userId, server, token);
            }

            emit loginSuccess(userId, server);
        } else {
            emit loginError("Login failed: " + reply->errorString());
            m_errorLabel->setText("Invalid credentials. Please try again.");
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void MatrixLoginWidget::setFormEnabled(bool enabled)
{
    m_serverEdit->setEnabled(enabled);
    m_userEdit->setEnabled(enabled);
    m_passwordEdit->setEnabled(enabled);
    m_loginBtn->setEnabled(enabled);
}

} // namespace pcp_chat
