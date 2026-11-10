#include "features/matrix_chat/create_room_dialog.hpp"

#include <QCheckBox>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QPushButton>
#include <QTextEdit>
#include <QUrl>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

CreateRoomDialog::CreateRoomDialog(QWidget *parent)
    : QDialog(parent)
    , m_nam(new QNetworkAccessManager(this))
{
    setupUi();
}

void CreateRoomDialog::setupUi()
{
    setWindowTitle(tr("Create Matrix Room"));
    setMinimumWidth(450);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);

    auto *formLayout = new QFormLayout;

    m_roomName = new QLineEdit(this);
    m_roomName->setPlaceholderText(tr("e.g. General Discussion"));
    formLayout->addRow(tr("Room Name:"), m_roomName);

    m_topic = new QTextEdit(this);
    m_topic->setMaximumHeight(80);
    m_topic->setPlaceholderText(tr("Optional room topic..."));
    formLayout->addRow(tr("Topic:"), m_topic);

    m_roomAlias = new QLineEdit(this);
    m_roomAlias->setPlaceholderText(tr("e.g. my-room (optional)"));
    formLayout->addRow(tr("Room Alias:"), m_roomAlias);

    m_visibility = new QComboBox(this);
    m_visibility->addItem(tr("Public"), QStringLiteral("public"));
    m_visibility->addItem(tr("Private"), QStringLiteral("private"));
    formLayout->addRow(tr("Visibility:"), m_visibility);

    m_encryptionToggle = new QCheckBox(tr("Enable End-to-End Encryption"), this);
    formLayout->addRow(m_encryptionToggle);

    mainLayout->addLayout(formLayout);

    mainLayout->addSpacing(12);

    auto *progressBar = new QProgressBar(this);
    progressBar->setRange(0, 0);
    progressBar->setVisible(false);
    progressBar->setObjectName(QStringLiteral("progressBar"));
    mainLayout->addWidget(progressBar);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setVisible(false);
    mainLayout->addWidget(m_statusLabel);

    mainLayout->addSpacing(12);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    m_createButton = new QPushButton(tr("Create Room"), this);
    m_createButton->setDefault(true);
    auto *cancelButton = new QPushButton(tr("Cancel"), this);

    buttonLayout->addWidget(m_createButton);
    buttonLayout->addWidget(cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_createButton, &QPushButton::clicked, this, &CreateRoomDialog::onCreateClicked);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void CreateRoomDialog::onCreateClicked()
{
    const QString name = m_roomName->text().trimmed();
    if (name.isEmpty()) {
        QMessageBox::warning(this, tr("Validation Error"), tr("Room name must not be empty."));
        m_roomName->setFocus();
        return;
    }

    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        return;
    }

    m_createButton->setEnabled(false);
    m_statusLabel->setVisible(true);
    m_statusLabel->setText(tr("Creating room..."));

    auto *progressBar = findChild<QProgressBar *>(QStringLiteral("progressBar"));
    if (progressBar) {
        progressBar->setVisible(true);
    }

    QJsonObject body;
    body[QStringLiteral("name")] = name;

    const QString topic = m_topic->toPlainText().trimmed();
    if (!topic.isEmpty())
        body[QStringLiteral("topic")] = topic;

    const QString alias = m_roomAlias->text().trimmed();
    if (!alias.isEmpty())
        body[QStringLiteral("room_alias_name")] = alias;

    body[QStringLiteral("visibility")] = m_visibility->currentData().toString();

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

    QUrl url(session->homeserverUrl() + QStringLiteral("/_matrix/client/r0/createRoom"));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply, name]() {
        reply->deleteLater();
        m_createButton->setEnabled(true);

        auto *progressBar = findChild<QProgressBar *>(QStringLiteral("progressBar"));
        if (progressBar) {
            progressBar->setVisible(false);
        }

        if (reply->error() != QNetworkReply::NoError) {
            m_statusLabel->setText(tr("Error: %1").arg(reply->errorString()));
            QMessageBox::critical(this, tr("Create Room Failed"),
                                  tr("Could not create room: %1").arg(reply->errorString()));
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        const QString roomId = JsonUtil::optString(json, QStringLiteral("room_id"));

        if (roomId.isEmpty()) {
            m_statusLabel->setText(tr("Error: No room ID returned."));
            return;
        }

        m_statusLabel->setText(tr("Room created successfully!\nRoom ID: %1").arg(roomId));
        QMessageBox::information(this, tr("Room Created"),
                                 tr("Room \"%1\" created successfully.\nRoom ID: %2").arg(name, roomId));
        accept();
    });
}
