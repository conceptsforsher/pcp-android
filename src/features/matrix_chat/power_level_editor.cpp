#include "features/matrix_chat/power_level_editor.hpp"

#include <QCheckBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPushButton>
#include <QSpinBox>
#include <QTableWidget>
#include <QUrl>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

namespace {

constexpr int kDefaultUsersDefault = 0;
constexpr int kDefaultEventsDefault = 50;
constexpr int kDefaultStateDefault = 50;
constexpr int kDefaultBan = 50;
constexpr int kDefaultKick = 50;
constexpr int kDefaultRedact = 50;
constexpr int kDefaultInvite = 50;

} // anonymous namespace

PowerLevelEditor::PowerLevelEditor(const QString &roomId, QWidget *parent)
    : QDialog(parent)
    , m_roomId(roomId)
    , m_nam(new QNetworkAccessManager(this))
    , m_usersDefault(nullptr)
    , m_eventsDefault(nullptr)
    , m_stateDefault(nullptr)
    , m_redact(nullptr)
    , m_ban(nullptr)
    , m_kick(nullptr)
    , m_invite(nullptr)
    , m_usersTable(nullptr)
    , m_eventsTable(nullptr)
    , m_addUserBtn(nullptr)
    , m_removeUserBtn(nullptr)
    , m_addEventBtn(nullptr)
    , m_removeEventBtn(nullptr)
    , m_notifyCheck(nullptr)
    , m_saveBtn(nullptr)
    , m_resetBtn(nullptr)
    , m_statusLabel(nullptr)
    , m_warningLabel(nullptr)
{
    setupUi();
    fetchCurrentPowerLevels();
}

void PowerLevelEditor::setupUi()
{
    setWindowTitle(tr("Power Level Editor"));
    setMinimumSize(580, 620);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(12);

    m_warningLabel = new QLabel(
        tr("\u26a0\ufe0f  Changing power levels can affect room functionality. "
           "Proceed with caution."),
        this);
    m_warningLabel->setWordWrap(true);
    m_warningLabel->setStyleSheet(QStringLiteral(
        "color: #92400e; background: #fef3c7; border: 1px solid #fcd34d; "
        "border-radius: 6px; padding: 10px; font-size: 12px;"));
    mainLayout->addWidget(m_warningLabel);

    // ─── Default Levels ──────────────────────────────────────────

    auto *defaultGroup = new QGroupBox(tr("Default Power Levels"), this);
    auto *defaultLayout = new QFormLayout(defaultGroup);
    defaultLayout->setSpacing(8);

    auto makeSpin = [this, defaultGroup](int value, const QString &tooltip) {
        auto *spin = new QSpinBox(defaultGroup);
        spin->setRange(-1, 100);
        spin->setValue(value);
        spin->setToolTip(tooltip);
        return spin;
    };

    m_usersDefault = makeSpin(kDefaultUsersDefault,
        tr("Default power level for all users in the room (0 = normal user)."));
    defaultLayout->addRow(tr("Users Default:"), m_usersDefault);

    m_eventsDefault = makeSpin(kDefaultEventsDefault,
        tr("Default level required to send message events (0 = anyone can send)."));
    defaultLayout->addRow(tr("Events Default:"), m_eventsDefault);

    m_stateDefault = makeSpin(kDefaultStateDefault,
        tr("Default level required to send state events (50 = moderator)."));
    defaultLayout->addRow(tr("State Default:"), m_stateDefault);

    m_ban = makeSpin(kDefaultBan,
        tr("Level required to ban users (50 = moderator)."));
    defaultLayout->addRow(tr("Ban:"), m_ban);

    m_kick = makeSpin(kDefaultKick,
        tr("Level required to kick users (50 = moderator)."));
    defaultLayout->addRow(tr("Kick:"), m_kick);

    m_redact = makeSpin(kDefaultRedact,
        tr("Level required to redact (delete) messages (50 = moderator)."));
    defaultLayout->addRow(tr("Redact:"), m_redact);

    m_invite = makeSpin(kDefaultInvite,
        tr("Level required to invite users (0 = anyone can invite, 50 = moderator)."));
    defaultLayout->addRow(tr("Invite:"), m_invite);

    mainLayout->addWidget(defaultGroup);

    // ─── User-specific Levels ────────────────────────────────────

    auto *usersGroup = new QGroupBox(tr("User-specific Power Levels"), this);
    auto *usersLayout = new QVBoxLayout(usersGroup);
    usersLayout->setSpacing(8);

    auto *usersHint = new QLabel(
        tr("Override default power levels for specific users. "
           "Higher values grant more privileges."),
        usersGroup);
    usersHint->setWordWrap(true);
    usersHint->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px;"));
    usersLayout->addWidget(usersHint);

    m_usersTable = new QTableWidget(0, 3, usersGroup);
    m_usersTable->setHorizontalHeaderLabels({tr("User ID"), tr("Display Name"), tr("Power Level")});
    m_usersTable->horizontalHeader()->setStretchLastSection(true);
    m_usersTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_usersTable->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_usersTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_usersTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_usersTable->setAlternatingRowColors(true);
    m_usersTable->verticalHeader()->setVisible(false);
    usersLayout->addWidget(m_usersTable);

    auto *usersBtnLayout = new QHBoxLayout();
    m_addUserBtn = new QPushButton(tr("Add User"), usersGroup);
    m_removeUserBtn = new QPushButton(tr("Remove Selected"), usersGroup);
    m_removeUserBtn->setEnabled(false);
    usersBtnLayout->addWidget(m_addUserBtn);
    usersBtnLayout->addWidget(m_removeUserBtn);
    usersBtnLayout->addStretch();
    usersLayout->addLayout(usersBtnLayout);

    mainLayout->addWidget(usersGroup);

    // ─── Event-specific Levels ───────────────────────────────────

    auto *eventsGroup = new QGroupBox(tr("Event-specific Power Levels"), this);
    auto *eventsLayout = new QVBoxLayout(eventsGroup);
    eventsLayout->setSpacing(8);

    auto *eventsHint = new QLabel(
        tr("Require a specific power level for certain event types. "
           "Leave empty to use the Events Default value."),
        eventsGroup);
    eventsHint->setWordWrap(true);
    eventsHint->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 11px;"));
    eventsLayout->addWidget(eventsHint);

    m_eventsTable = new QTableWidget(0, 2, eventsGroup);
    m_eventsTable->setHorizontalHeaderLabels({tr("Event Type"), tr("Required Level")});
    m_eventsTable->horizontalHeader()->setStretchLastSection(true);
    m_eventsTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    m_eventsTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_eventsTable->setSelectionMode(QAbstractItemView::SingleSelection);
    m_eventsTable->setAlternatingRowColors(true);
    m_eventsTable->verticalHeader()->setVisible(false);
    eventsLayout->addWidget(m_eventsTable);

    auto *eventsBtnLayout = new QHBoxLayout();
    m_addEventBtn = new QPushButton(tr("Add Event Type"), eventsGroup);
    m_removeEventBtn = new QPushButton(tr("Remove Selected"), eventsGroup);
    m_removeEventBtn->setEnabled(false);
    eventsBtnLayout->addWidget(m_addEventBtn);
    eventsBtnLayout->addWidget(m_removeEventBtn);
    eventsBtnLayout->addStretch();
    eventsLayout->addLayout(eventsBtnLayout);

    mainLayout->addWidget(eventsGroup);

    // ─── Bottom bar ──────────────────────────────────────────────

    m_notifyCheck = new QCheckBox(tr("Notify room of changes"), this);
    mainLayout->addWidget(m_notifyCheck);

    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setVisible(false);
    m_statusLabel->setStyleSheet(QStringLiteral("color: #6b7280; font-size: 12px;"));
    mainLayout->addWidget(m_statusLabel);

    auto *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();

    m_resetBtn = new QPushButton(tr("Reset to Defaults"), this);
    m_saveBtn = new QPushButton(tr("Save Changes"), this);
    m_saveBtn->setDefault(true);

    auto *cancelBtn = new QPushButton(tr("Cancel"), this);

    buttonLayout->addWidget(m_resetBtn);
    buttonLayout->addWidget(cancelBtn);
    buttonLayout->addWidget(m_saveBtn);
    mainLayout->addLayout(buttonLayout);

    connect(m_saveBtn, &QPushButton::clicked, this, &PowerLevelEditor::onSaveClicked);
    connect(m_resetBtn, &QPushButton::clicked, this, &PowerLevelEditor::onResetClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    connect(m_addUserBtn, &QPushButton::clicked, this, &PowerLevelEditor::onAddUserClicked);
    connect(m_removeUserBtn, &QPushButton::clicked, this, &PowerLevelEditor::onRemoveUserClicked);
    connect(m_addEventBtn, &QPushButton::clicked, this, &PowerLevelEditor::onAddEventClicked);
    connect(m_removeEventBtn, &QPushButton::clicked, this, &PowerLevelEditor::onRemoveEventClicked);

    connect(m_usersTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        m_removeUserBtn->setEnabled(!m_usersTable->selectedItems().isEmpty());
    });

    connect(m_eventsTable, &QTableWidget::itemSelectionChanged, this, [this]() {
        m_removeEventBtn->setEnabled(!m_eventsTable->selectedItems().isEmpty());
    });
}

void PowerLevelEditor::fetchCurrentPowerLevels()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    m_statusLabel->setText(tr("Loading power levels..."));
    m_statusLabel->setVisible(true);

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.power_levels")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        m_statusLabel->setVisible(false);

        if (reply->error() != QNetworkReply::NoError) {
            m_statusLabel->setText(tr("Using default values (could not fetch: %1)")
                                       .arg(reply->errorString()));
            m_statusLabel->setVisible(true);
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(
            QString::fromUtf8(reply->readAll()));
        const QJsonObject content = JsonUtil::optObject(json, QStringLiteral("content"));
        if (!content.isEmpty())
            populateFromJson(content);
        else
            m_statusLabel->setText(tr("No existing power levels found. Using defaults."));
    });
}

void PowerLevelEditor::populateFromJson(const QJsonObject &content)
{
    m_usersDefault->setValue(JsonUtil::optInt(content, QStringLiteral("users_default"), kDefaultUsersDefault));
    m_eventsDefault->setValue(JsonUtil::optInt(content, QStringLiteral("events_default"), kDefaultEventsDefault));
    m_stateDefault->setValue(JsonUtil::optInt(content, QStringLiteral("state_default"), kDefaultStateDefault));
    m_ban->setValue(JsonUtil::optInt(content, QStringLiteral("ban"), kDefaultBan));
    m_kick->setValue(JsonUtil::optInt(content, QStringLiteral("kick"), kDefaultKick));
    m_redact->setValue(JsonUtil::optInt(content, QStringLiteral("redact"), kDefaultRedact));
    m_invite->setValue(JsonUtil::optInt(content, QStringLiteral("invite"), kDefaultInvite));

    const QJsonObject users = JsonUtil::optObject(content, QStringLiteral("users"));
    for (auto it = users.begin(); it != users.end(); ++it) {
        addUserRow(it.key(), QString(), it.value().toInt());
    }

    const QJsonObject events = JsonUtil::optObject(content, QStringLiteral("events"));
    for (auto it = events.begin(); it != events.end(); ++it) {
        addEventRow(it.key(), it.value().toInt());
    }
}

void PowerLevelEditor::addUserRow(const QString &userId, const QString &displayName, int powerLevel)
{
    const int row = m_usersTable->rowCount();
    m_usersTable->insertRow(row);

    auto *userIdItem = new QTableWidgetItem(userId);
    userIdItem->setFlags(userIdItem->flags() & ~Qt::ItemIsEditable);
    m_usersTable->setItem(row, 0, userIdItem);

    auto *nameItem = new QTableWidgetItem(displayName);
    nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
    m_usersTable->setItem(row, 1, nameItem);

    auto *spin = new QSpinBox(m_usersTable);
    spin->setRange(-1, 100);
    spin->setValue(powerLevel);
    m_usersTable->setCellWidget(row, 2, spin);
}

void PowerLevelEditor::addEventRow(const QString &eventType, int requiredLevel)
{
    const int row = m_eventsTable->rowCount();
    m_eventsTable->insertRow(row);

    auto *typeItem = new QTableWidgetItem(eventType);
    m_eventsTable->setItem(row, 0, typeItem);

    auto *spin = new QSpinBox(m_eventsTable);
    spin->setRange(0, 100);
    spin->setValue(requiredLevel);
    m_eventsTable->setCellWidget(row, 1, spin);
}

void PowerLevelEditor::onAddUserClicked()
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Add User Power Level"));
    dialog->setMinimumWidth(380);
    dialog->setModal(true);

    auto *layout = new QFormLayout(dialog);
    layout->setSpacing(10);

    auto *userIdEdit = new QLineEdit(dialog);
    userIdEdit->setPlaceholderText(tr("@user:server.org"));
    layout->addRow(tr("User ID:"), userIdEdit);

    auto *displayEdit = new QLineEdit(dialog);
    displayEdit->setPlaceholderText(tr("Optional display name"));
    layout->addRow(tr("Display Name:"), displayEdit);

    auto *levelSpin = new QSpinBox(dialog);
    levelSpin->setRange(-1, 100);
    levelSpin->setValue(50);
    layout->addRow(tr("Power Level:"), levelSpin);

    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto *okBtn = new QPushButton(tr("Add"), dialog);
    auto *cancelDialogBtn = new QPushButton(tr("Cancel"), dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelDialogBtn);
    layout->addRow(btnLayout);

    connect(okBtn, &QPushButton::clicked, dialog, [dialog, userIdEdit, displayEdit, levelSpin, this]() {
        const QString uid = userIdEdit->text().trimmed();
        if (uid.isEmpty())
            return;
        addUserRow(uid, displayEdit->text().trimmed(), levelSpin->value());
        dialog->accept();
    });
    connect(cancelDialogBtn, &QPushButton::clicked, dialog, &QDialog::reject);

    dialog->exec();
    dialog->deleteLater();
}

void PowerLevelEditor::onRemoveUserClicked()
{
    const int row = m_usersTable->currentRow();
    if (row >= 0)
        m_usersTable->removeRow(row);
}

void PowerLevelEditor::onAddEventClicked()
{
    auto *dialog = new QDialog(this);
    dialog->setWindowTitle(tr("Add Event Power Level"));
    dialog->setMinimumWidth(400);
    dialog->setModal(true);

    auto *layout = new QFormLayout(dialog);
    layout->setSpacing(10);

    auto *typeEdit = new QLineEdit(dialog);
    typeEdit->setPlaceholderText(tr("e.g. m.room.name, m.room.topic"));
    layout->addRow(tr("Event Type:"), typeEdit);

    auto *levelSpin = new QSpinBox(dialog);
    levelSpin->setRange(0, 100);
    levelSpin->setValue(50);
    layout->addRow(tr("Required Level:"), levelSpin);

    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();
    auto *okBtn = new QPushButton(tr("Add"), dialog);
    auto *cancelDialogBtn = new QPushButton(tr("Cancel"), dialog);
    btnLayout->addWidget(okBtn);
    btnLayout->addWidget(cancelDialogBtn);
    layout->addRow(btnLayout);

    connect(okBtn, &QPushButton::clicked, dialog, [dialog, typeEdit, levelSpin, this]() {
        const QString type = typeEdit->text().trimmed();
        if (type.isEmpty())
            return;
        addEventRow(type, levelSpin->value());
        dialog->accept();
    });
    connect(cancelDialogBtn, &QPushButton::clicked, dialog, &QDialog::reject);

    dialog->exec();
    dialog->deleteLater();
}

void PowerLevelEditor::onRemoveEventClicked()
{
    const int row = m_eventsTable->currentRow();
    if (row >= 0)
        m_eventsTable->removeRow(row);
}

void PowerLevelEditor::onSaveClicked()
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        QMessageBox::warning(this, tr("Error"), tr("No Matrix session available."));
        return;
    }

    QJsonObject content;

    content[QStringLiteral("users_default")] = m_usersDefault->value();
    content[QStringLiteral("events_default")] = m_eventsDefault->value();
    content[QStringLiteral("state_default")] = m_stateDefault->value();
    content[QStringLiteral("ban")] = m_ban->value();
    content[QStringLiteral("kick")] = m_kick->value();
    content[QStringLiteral("redact")] = m_redact->value();
    content[QStringLiteral("invite")] = m_invite->value();

    QJsonObject users;
    for (int row = 0; row < m_usersTable->rowCount(); ++row) {
        const QString userId = m_usersTable->item(row, 0)
                                   ? m_usersTable->item(row, 0)->text()
                                   : QString();
        auto *spin = qobject_cast<QSpinBox *>(m_usersTable->cellWidget(row, 2));
        if (!userId.isEmpty() && spin)
            users[userId] = spin->value();
    }
    content[QStringLiteral("users")] = users;

    QJsonObject events;
    for (int row = 0; row < m_eventsTable->rowCount(); ++row) {
        const QString evType = m_eventsTable->item(row, 0)
                                   ? m_eventsTable->item(row, 0)->text()
                                   : QString();
        auto *spin = qobject_cast<QSpinBox *>(m_eventsTable->cellWidget(row, 1));
        if (!evType.isEmpty() && spin)
            events[evType] = spin->value();
    }
    content[QStringLiteral("events")] = events;

    m_saveBtn->setEnabled(false);
    m_statusLabel->setText(tr("Saving power levels..."));
    m_statusLabel->setVisible(true);

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.power_levels")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->put(req, QJsonDocument(content).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        m_saveBtn->setEnabled(true);

        if (reply->error() != QNetworkReply::NoError) {
            const QJsonObject json = JsonUtil::parseObject(
                QString::fromUtf8(reply->readAll()));
            const QString errorMsg = JsonUtil::optString(json, QStringLiteral("error"),
                                                         reply->errorString());
            m_statusLabel->setText(tr("Error: %1").arg(errorMsg));
            QMessageBox::critical(this, tr("Save Failed"),
                                  tr("Could not save power levels: %1").arg(errorMsg));
            return;
        }

        if (m_notifyCheck->isChecked()) {
            emit accepted();
        }

        m_statusLabel->setText(tr("Power levels saved successfully."));
        QMessageBox::information(this, tr("Saved"),
                                 tr("Power levels updated successfully."));
        m_statusLabel->setVisible(false);
        accept();
    });
}

void PowerLevelEditor::onResetClicked()
{
    if (QMessageBox::question(this, tr("Reset to Defaults"),
                              tr("Reset all power levels to their default values?"))
        == QMessageBox::Yes) {
        m_usersDefault->setValue(kDefaultUsersDefault);
        m_eventsDefault->setValue(kDefaultEventsDefault);
        m_stateDefault->setValue(kDefaultStateDefault);
        m_ban->setValue(kDefaultBan);
        m_kick->setValue(kDefaultKick);
        m_redact->setValue(kDefaultRedact);
        m_invite->setValue(kDefaultInvite);

        m_usersTable->setRowCount(0);
        m_eventsTable->setRowCount(0);

        m_statusLabel->setText(tr("Reset to default values. Click Save to apply."));
        m_statusLabel->setVisible(true);
    }
}

#include "power_level_editor.moc"
