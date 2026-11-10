#include "features/matrix_chat/room_upgrade_dialog.hpp"

#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProgressBar>
#include <QPushButton>
#include <QUrl>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

static const char *kUpgradeDialogStyleSheet = R"(
    RoomUpgradeDialog {
        background-color: #1e1e2e;
    }
    RoomUpgradeDialog QLabel {
        color: #cdd6f4;
        font-size: 12px;
        background: transparent;
    }
    RoomUpgradeDialog QLabel#titleLabel {
        font-size: 15px;
        font-weight: bold;
        color: #cdd6f4;
    }
    RoomUpgradeDialog QLabel#warningLabel {
        color: #f38ba8;
        font-size: 12px;
        padding: 10px;
        border: 1px solid #f38ba8;
        border-radius: 6px;
        background-color: rgba(243, 139, 168, 0.08);
    }
    RoomUpgradeDialog QLabel#changesLabel {
        color: #a6adc8;
        font-size: 12px;
        padding: 8px 10px;
        border: 1px solid #45475a;
        border-radius: 6px;
        background-color: #181825;
    }
    RoomUpgradeDialog QLabel#statusLabel {
        color: #a6e3a1;
        font-size: 12px;
        padding: 4px 0px;
    }
    RoomUpgradeDialog QLabel#statusError {
        color: #f38ba8;
        font-size: 12px;
    }
    RoomUpgradeDialog QLabel#newRoomIdLabel {
        color: #89b4fa;
        font-size: 12px;
        font-weight: bold;
        padding: 4px 0px;
    }
    RoomUpgradeDialog QComboBox {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 6px 10px;
        font-size: 12px;
        min-height: 20px;
    }
    RoomUpgradeDialog QComboBox:hover {
        border-color: #89b4fa;
    }
    RoomUpgradeDialog QComboBox QAbstractItemView {
        background-color: #313244;
        color: #cdd6f4;
        selection-background-color: #45475a;
        border: 1px solid #45475a;
        outline: none;
    }
    RoomUpgradeDialog QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 6px 18px;
        font-size: 12px;
        min-height: 28px;
    }
    RoomUpgradeDialog QPushButton:hover {
        background-color: #45475a;
        border-color: #89b4fa;
    }
    RoomUpgradeDialog QPushButton:disabled {
        color: #585b70;
        border-color: #45475a;
    }
    RoomUpgradeDialog QPushButton#upgradeButton {
        background-color: #f38ba8;
        color: #1e1e2e;
        font-weight: bold;
        border: none;
    }
    RoomUpgradeDialog QPushButton#upgradeButton:hover {
        background-color: #f5a0b0;
    }
    RoomUpgradeDialog QPushButton#upgradeButton:disabled {
        background-color: #585b70;
        color: #313244;
    }
    RoomUpgradeDialog QPushButton#goToNewRoomButton {
        background-color: #a6e3a1;
        color: #1e1e2e;
        font-weight: bold;
        border: none;
    }
    RoomUpgradeDialog QPushButton#goToNewRoomButton:hover {
        background-color: #94e2d5;
    }
    RoomUpgradeDialog QProgressBar {
        background-color: #313244;
        border: none;
        border-radius: 4px;
        height: 4px;
    }
    RoomUpgradeDialog QProgressBar::chunk {
        background-color: #f38ba8;
        border-radius: 4px;
    }
)";

static const QList<QPair<QString, QString>> kAvailableVersions = {
    {QStringLiteral("8"),  QStringLiteral("v8 — Knock, restricted join rules")},
    {QStringLiteral("9"),  QStringLiteral("v9 — Improved redaction algorithm")},
    {QStringLiteral("10"), QStringLiteral("v10 — Extended power level ranges")},
};

static QString versionFeatures(const QString &version)
{
    if (version == QStringLiteral("8")) {
        return QStringLiteral(
            "\xE2\x80\xA2 Knock join rule (members can request to join)\n"
            "\xE2\x80\xA2 Restricted join rule (allow based on other rooms)\n"
            "\xE2\x80\xA2 Updated event ID format");
    }
    if (version == QStringLiteral("9")) {
        return QStringLiteral(
            "\xE2\x80\xA2 Improved redaction algorithm (redactions keep edit history)\n"
            "\xE2\x80\xA2 Better protection against editing historical events\n"
            "\xE2\x80\xA2 All v8 features included");
    }
    if (version == QStringLiteral("10")) {
        return QStringLiteral(
            "\xE2\x80\xA2 Extended power level integer range (up to 2^53)\n"
            "\xE2\x80\xA2 More granular permission assignments\n"
            "\xE2\x80\xA2 All v9 features included");
    }
    return QStringLiteral("\xE2\x80\xA2 No specific version features listed");
}

RoomUpgradeDialog::RoomUpgradeDialog(const QString &roomId, QWidget *parent)
    : QDialog(parent)
    , m_roomId(roomId)
    , m_nam(new QNetworkAccessManager(this))
{
    setStyleSheet(QString::fromLatin1(kUpgradeDialogStyleSheet));
    setupUi();
    fetchCurrentVersion();
}

void RoomUpgradeDialog::setupUi()
{
    setWindowTitle(tr("Upgrade Room"));
    setMinimumWidth(480);
    setMinimumHeight(420);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    auto *titleLabel = new QLabel(tr("Danger Zone: Upgrade Room"), this);
    titleLabel->setObjectName(QStringLiteral("titleLabel"));
    mainLayout->addWidget(titleLabel);

    auto *formLayout = new QFormLayout;
    formLayout->setSpacing(8);

    m_currentVersionLabel = new QLabel(QStringLiteral("..."), this);
    m_currentVersionLabel->setStyleSheet(QStringLiteral(
        "font-weight: bold; color: #89b4fa;"));
    formLayout->addRow(tr("Current version:"), m_currentVersionLabel);

    m_targetVersionCombo = new QComboBox(this);
    for (const auto &entry : kAvailableVersions) {
        m_targetVersionCombo->addItem(entry.second, entry.first);
    }
    m_targetVersionCombo->setCurrentIndex(-1);
    formLayout->addRow(tr("Target version:"), m_targetVersionCombo);

    mainLayout->addLayout(formLayout);

    m_warningLabel = new QLabel(this);
    m_warningLabel->setObjectName(QStringLiteral("warningLabel"));
    m_warningLabel->setWordWrap(true);
    m_warningLabel->setText(tr(
        "\xE2\x9A\xA0\xEF\xB8\x8F  WARNING: Upgrading creates a new room. "
        "The old room will be replaced and all members will be "
        "automatically migrated. Message history stays in the old room."));
    mainLayout->addWidget(m_warningLabel);

    m_changesLabel = new QLabel(this);
    m_changesLabel->setObjectName(QStringLiteral("changesLabel"));
    m_changesLabel->setWordWrap(true);
    m_changesLabel->setText(tr("Select a target version above to see what changes."));
    mainLayout->addWidget(m_changesLabel);

    auto *progressBar = new QProgressBar(this);
    progressBar->setObjectName(QStringLiteral("progressBar"));
    progressBar->setRange(0, 0);
    progressBar->setVisible(false);
    progressBar->setFixedHeight(4);
    mainLayout->addWidget(progressBar);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setObjectName(QStringLiteral("statusLabel"));
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setVisible(false);
    mainLayout->addWidget(m_statusLabel);

    m_newRoomIdLabel = new QLabel(this);
    m_newRoomIdLabel->setObjectName(QStringLiteral("newRoomIdLabel"));
    m_newRoomIdLabel->setWordWrap(true);
    m_newRoomIdLabel->setVisible(false);
    m_newRoomIdLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    mainLayout->addWidget(m_newRoomIdLabel);

    mainLayout->addStretch();

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->setSpacing(10);
    buttonLayout->addStretch();

    m_upgradeButton = new QPushButton(tr("Upgrade Room"), this);
    m_upgradeButton->setObjectName(QStringLiteral("upgradeButton"));
    m_upgradeButton->setEnabled(false);
    m_upgradeButton->setCursor(Qt::PointingHandCursor);
    buttonLayout->addWidget(m_upgradeButton);

    m_goToNewRoomButton = new QPushButton(tr("Go to New Room"), this);
    m_goToNewRoomButton->setObjectName(QStringLiteral("goToNewRoomButton"));
    m_goToNewRoomButton->setVisible(false);
    m_goToNewRoomButton->setCursor(Qt::PointingHandCursor);
    buttonLayout->addWidget(m_goToNewRoomButton);

    m_cancelButton = new QPushButton(tr("Cancel"), this);
    buttonLayout->addWidget(m_cancelButton);

    mainLayout->addLayout(buttonLayout);

    connect(m_targetVersionCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &RoomUpgradeDialog::onVersionChanged);
    connect(m_upgradeButton, &QPushButton::clicked,
            this, &RoomUpgradeDialog::onUpgradeClicked);
    connect(m_goToNewRoomButton, &QPushButton::clicked,
            this, &RoomUpgradeDialog::onGoToNewRoomClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void RoomUpgradeDialog::onVersionChanged(int index)
{
    m_upgradeButton->setEnabled(index >= 0);

    if (index < 0) {
        m_changesLabel->setText(tr("Select a target version above to see what changes."));
        return;
    }

    const QString version = m_targetVersionCombo->currentData().toString();
    m_changesLabel->setText(
        tr("New features in %1:\n%2")
            .arg(m_targetVersionCombo->currentText().section(QStringLiteral(" \xE2\x80\x94"), 0, 0),
                 versionFeatures(version)));
}

void RoomUpgradeDialog::fetchCurrentVersion()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        m_currentVersionLabel->setText(tr("Unknown (no session)"));
        return;
    }

    QUrl url(session->homeserverUrl() + QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.create")
                 .arg(m_roomId));
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError) {
            m_currentVersionLabel->setText(tr("Unknown"));
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        m_currentVersion = JsonUtil::optString(json, QStringLiteral("room_version"), QStringLiteral("1"));
        m_currentVersionLabel->setText(m_currentVersion);
    });
}

void RoomUpgradeDialog::onUpgradeClicked()
{
    if (!m_targetVersionCombo->currentData().isValid()) {
        return;
    }

    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        return;
    }

    m_upgradeButton->setEnabled(false);
    m_cancelButton->setEnabled(false);

    auto *progressBar = findChild<QProgressBar *>(QStringLiteral("progressBar"));
    if (progressBar) {
        progressBar->setVisible(true);
    }

    m_statusLabel->setObjectName(QStringLiteral("statusLabel"));
    m_statusLabel->setVisible(true);
    m_statusLabel->setText(tr("Upgrading room... Please wait."));

    const QString newVersion = m_targetVersionCombo->currentData().toString();

    QJsonObject body;
    body[QStringLiteral("new_version")] = newVersion;

    QUrl url(session->homeserverUrl() + QStringLiteral("/_matrix/client/r0/rooms/%1/upgrade")
                 .arg(m_roomId));
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        auto *progressBar = findChild<QProgressBar *>(QStringLiteral("progressBar"));
        if (progressBar) {
            progressBar->setVisible(false);
        }

        m_cancelButton->setEnabled(true);
        m_upgradeButton->setEnabled(true);

        if (reply->error() != QNetworkReply::NoError) {
            m_statusLabel->setObjectName(QStringLiteral("statusError"));
            m_statusLabel->setText(tr("Upgrade failed: %1").arg(reply->errorString()));
            QMessageBox::critical(this, tr("Upgrade Failed"),
                                  tr("Could not upgrade room: %1").arg(reply->errorString()));
            m_statusLabel->setObjectName(QStringLiteral("statusLabel"));
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        m_newRoomId = JsonUtil::optString(json, QStringLiteral("replacement_room"));

        if (m_newRoomId.isEmpty()) {
            m_statusLabel->setObjectName(QStringLiteral("statusError"));
            m_statusLabel->setText(tr("Upgrade completed but no replacement room ID received."));
            m_statusLabel->setObjectName(QStringLiteral("statusLabel"));
            return;
        }

        m_statusLabel->setText(tr("Room upgraded successfully!"));
        m_newRoomIdLabel->setText(tr("New room ID: %1").arg(m_newRoomId));
        m_newRoomIdLabel->setVisible(true);

        m_upgradeButton->setEnabled(false);
        m_upgradeButton->setVisible(false);
        m_goToNewRoomButton->setVisible(true);
        m_cancelButton->setText(tr("Close"));
        m_targetVersionCombo->setEnabled(false);
    });
}

void RoomUpgradeDialog::onGoToNewRoomClicked()
{
    emit goToNewRoom(m_newRoomId);
    accept();
}
