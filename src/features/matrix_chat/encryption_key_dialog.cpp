#include "features/matrix_chat/encryption_key_dialog.hpp"

#include <QApplication>
#include <QCheckBox>
#include <QClipboard>
#include <QDateTime>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QFrame>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QInputDialog>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QScrollArea>
#include <QSplitter>
#include <QTabWidget>
#include <QTextEdit>
#include <QTimer>
#include <QToolTip>
#include <QVBoxLayout>

namespace {

QFrame *createSeparator(QWidget *parent)
{
    auto *line = new QFrame(parent);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    return line;
}

QString elapsedString(const QDateTime &dt)
{
    if (!dt.isValid())
        return EncryptionKeyDialog::tr("Never");

    const auto now = QDateTime::currentDateTime();
    const qint64 secs = dt.secsTo(now);
    if (secs < 60)
        return EncryptionKeyDialog::tr("Just now");
    if (secs < 3600)
        return EncryptionKeyDialog::tr("%1 minutes ago").arg(secs / 60);
    if (secs < 86400)
        return EncryptionKeyDialog::tr("%1 hours ago").arg(secs / 3600);
    if (secs < 604800)
        return EncryptionKeyDialog::tr("%1 days ago").arg(secs / 86400);
    return dt.toString(QStringLiteral("yyyy-MM-dd hh:mm"));
}

QString formatFingerprint(const QString &key)
{
    if (key.length() < 12)
        return key;
    QString result;
    for (int i = 0; i < key.length(); ++i) {
        if (i > 0 && i % 4 == 0)
            result += QLatin1Char(' ');
        result += key.at(i);
    }
    return result.toUpper();
}

} // anonymous namespace

EncryptionKeyDialog::EncryptionKeyDialog(QWidget *parent)
    : QDialog(parent)
    , m_tabWidget(nullptr)
    , m_devicesSplitter(nullptr)
    , m_deviceList(nullptr)
    , m_deviceDetailPanel(nullptr)
    , m_deviceDetailIdLabel(nullptr)
    , m_deviceDetailKeyLabel(nullptr)
    , m_deviceDetailAlgoLabel(nullptr)
    , m_deviceDetailSeenLabel(nullptr)
    , m_deviceDetailStatusLabel(nullptr)
    , m_verifyDeviceButton(nullptr)
    , m_signOutDeviceButton(nullptr)
    , m_copyDeviceIdButton(nullptr)
    , m_usersSplitter(nullptr)
    , m_userList(nullptr)
    , m_userDetailPanel(nullptr)
    , m_userDetailNameLabel(nullptr)
    , m_userDetailIdLabel(nullptr)
    , m_userDetailKeyLabel(nullptr)
    , m_userDetailStatusLabel(nullptr)
    , m_userDetailDeviceCountLabel(nullptr)
    , m_userDeviceList(nullptr)
    , m_verifyUserButton(nullptr)
    , m_resetVerificationButton(nullptr)
    , m_backupStatusLabel(nullptr)
    , m_backupLastTimeLabel(nullptr)
    , m_backupLocationLabel(nullptr)
    , m_backupProgressBar(nullptr)
    , m_backupNowButton(nullptr)
    , m_restoreBackupButton(nullptr)
    , m_recoveryKeyEdit(nullptr)
    , m_recoveryGroup(nullptr)
    , m_exportButton(nullptr)
    , m_importButton(nullptr)
    , m_buttonBox(nullptr)
    , m_backupEnabled(false)
{
    setupUi();
    populateDevices();
    populateUsers();
    loadBackupStatus();
}

EncryptionKeyDialog::~EncryptionKeyDialog() = default;

// ---------------------------------------------------------------------------
// UI Setup
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::setupUi()
{
    setWindowTitle(tr("Encryption Keys"));
    setMinimumSize(680, 560);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createMyDevicesTab(), tr("My Devices"));
    m_tabWidget->addTab(createUserKeysTab(), tr("User Keys"));
    m_tabWidget->addTab(createKeyBackupTab(), tr("Key Backup"));
    mainLayout->addWidget(m_tabWidget, 1);

    // ---- Bottom bar with export/import + close ----
    auto *bottomBar = new QHBoxLayout;
    bottomBar->setContentsMargins(0, 4, 0, 0);
    bottomBar->setSpacing(8);

    m_exportButton = new QPushButton(tr("Export Keys..."), this);
    bottomBar->addWidget(m_exportButton);

    m_importButton = new QPushButton(tr("Import Keys..."), this);
    bottomBar->addWidget(m_importButton);

    bottomBar->addStretch();

    m_buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, this);
    bottomBar->addWidget(m_buttonBox);

    mainLayout->addLayout(bottomBar);

    // ---- Connections ----
    QObject::connect(m_exportButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onExportKeysClicked);
    QObject::connect(m_importButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onImportKeysClicked);
    QObject::connect(m_buttonBox, &QDialogButtonBox::rejected,
                     this, &QDialog::accept);
}

// ---------------------------------------------------------------------------
// My Devices Tab
// ---------------------------------------------------------------------------

QWidget *EncryptionKeyDialog::createMyDevicesTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_devicesSplitter = new QSplitter(Qt::Horizontal, tab);

    // ---- Left side: device list ----
    auto *listContainer = new QWidget(m_devicesSplitter);
    auto *listLayout = new QVBoxLayout(listContainer);
    listLayout->setContentsMargins(8, 8, 4, 8);
    listLayout->setSpacing(8);

    auto *deviceListHeader = new QLabel(tr("<b>Your Devices</b>"), listContainer);
    listLayout->addWidget(deviceListHeader);

    m_deviceList = new QListWidget(listContainer);
    m_deviceList->setAlternatingRowColors(true);
    m_deviceList->setSelectionMode(QAbstractItemView::SingleSelection);
    listLayout->addWidget(m_deviceList, 1);

    // ---- Right side: detail panel ----
    m_deviceDetailPanel = createDeviceDetailPanel();
    m_deviceDetailPanel->setVisible(false);

    m_devicesSplitter->addWidget(listContainer);
    m_devicesSplitter->addWidget(m_deviceDetailPanel);
    m_devicesSplitter->setStretchFactor(0, 1);
    m_devicesSplitter->setStretchFactor(1, 2);
    m_devicesSplitter->setSizes({240, 400});

    layout->addWidget(m_devicesSplitter, 1);

    QObject::connect(m_deviceList, &QListWidget::currentItemChanged,
                     this, [this](QListWidgetItem *, QListWidgetItem *) {
                         onDeviceSelectionChanged();
                     });

    return tab;
}

QWidget *EncryptionKeyDialog::createDeviceDetailPanel()
{
    auto *panel = new QWidget();
    auto *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    auto *header = new QLabel(tr("<b>Device Details</b>"), panel);
    QFont hdrFont = header->font();
    hdrFont.setPointSize(13);
    hdrFont.setBold(true);
    header->setFont(hdrFont);
    layout->addWidget(header);

    m_deviceDetailStatusLabel = new QLabel(panel);
    m_deviceDetailStatusLabel->setWordWrap(true);
    layout->addWidget(m_deviceDetailStatusLabel);

    layout->addWidget(createSeparator(panel));

    // Device ID row (copyable)
    auto *idHeader = new QLabel(tr("<b>Device ID</b>"), panel);
    layout->addWidget(idHeader);

    auto *idRow = new QHBoxLayout;
    idRow->setSpacing(6);
    m_deviceDetailIdLabel = new QLabel(panel);
    m_deviceDetailIdLabel->setWordWrap(true);
    m_deviceDetailIdLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_deviceDetailIdLabel->setStyleSheet(QStringLiteral("font-family: monospace; color: #555;"));
    idRow->addWidget(m_deviceDetailIdLabel, 1);

    m_copyDeviceIdButton = new QPushButton(tr("Copy"), panel);
    m_copyDeviceIdButton->setFixedWidth(60);
    QObject::connect(m_copyDeviceIdButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onDeviceDetailsCopied);
    idRow->addWidget(m_copyDeviceIdButton);
    layout->addLayout(idRow);

    layout->addWidget(createSeparator(panel));

    // Device key (fingerprint)
    auto *keyHeader = new QLabel(tr("<b>Device Key (Fingerprint)</b>"), panel);
    layout->addWidget(keyHeader);
    m_deviceDetailKeyLabel = new QLabel(panel);
    m_deviceDetailKeyLabel->setWordWrap(true);
    m_deviceDetailKeyLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_deviceDetailKeyLabel->setStyleSheet(QStringLiteral("font-family: monospace; color: #333; font-size: 13px;"));
    layout->addWidget(m_deviceDetailKeyLabel);

    layout->addWidget(createSeparator(panel));

    // Algorithms
    auto *algoHeader = new QLabel(tr("<b>Algorithms</b>"), panel);
    layout->addWidget(algoHeader);
    m_deviceDetailAlgoLabel = new QLabel(panel);
    m_deviceDetailAlgoLabel->setWordWrap(true);
    m_deviceDetailAlgoLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(m_deviceDetailAlgoLabel);

    layout->addWidget(createSeparator(panel));

    // Last seen
    auto *seenHeader = new QLabel(tr("<b>Last Seen</b>"), panel);
    layout->addWidget(seenHeader);
    m_deviceDetailSeenLabel = new QLabel(panel);
    m_deviceDetailSeenLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    layout->addWidget(m_deviceDetailSeenLabel);

    layout->addSpacing(8);

    // Action buttons
    auto *btnRow = new QHBoxLayout;
    btnRow->setSpacing(8);

    m_verifyDeviceButton = new QPushButton(tr("Verify"), panel);
    m_verifyDeviceButton->setStyleSheet(
        QStringLiteral("QPushButton { background-color: #27ae60; color: white; border: none; "
                       "padding: 8px 16px; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #219a52; } "
                       "QPushButton:pressed { background-color: #1e8449; }"));
    QObject::connect(m_verifyDeviceButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onVerifyDeviceClicked);
    btnRow->addWidget(m_verifyDeviceButton);

    m_signOutDeviceButton = new QPushButton(tr("Sign Out"), panel);
    m_signOutDeviceButton->setStyleSheet(
        QStringLiteral("QPushButton { background-color: #e74c3c; color: white; border: none; "
                       "padding: 8px 16px; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #c0392b; } "
                       "QPushButton:pressed { background-color: #a93226; }"));
    QObject::connect(m_signOutDeviceButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onSignOutDeviceClicked);
    btnRow->addWidget(m_signOutDeviceButton);

    btnRow->addStretch();
    layout->addLayout(btnRow);

    layout->addStretch();
    return panel;
}

// ---------------------------------------------------------------------------
// User Keys Tab
// ---------------------------------------------------------------------------

QWidget *EncryptionKeyDialog::createUserKeysTab()
{
    auto *tab = new QWidget(this);
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_usersSplitter = new QSplitter(Qt::Horizontal, tab);

    // ---- Left side: user list ----
    auto *listContainer = new QWidget(m_usersSplitter);
    auto *listLayout = new QVBoxLayout(listContainer);
    listLayout->setContentsMargins(8, 8, 4, 8);
    listLayout->setSpacing(8);

    auto *userListHeader = new QLabel(tr("<b>Users in Encrypted Rooms</b>"), listContainer);
    listLayout->addWidget(userListHeader);

    m_userList = new QListWidget(listContainer);
    m_userList->setAlternatingRowColors(true);
    m_userList->setSelectionMode(QAbstractItemView::SingleSelection);
    listLayout->addWidget(m_userList, 1);

    // ---- Right side: user detail panel ----
    m_userDetailPanel = createUserDetailPanel();
    m_userDetailPanel->setVisible(false);

    m_usersSplitter->addWidget(listContainer);
    m_usersSplitter->addWidget(m_userDetailPanel);
    m_usersSplitter->setStretchFactor(0, 1);
    m_usersSplitter->setStretchFactor(1, 2);
    m_usersSplitter->setSizes({240, 400});

    layout->addWidget(m_usersSplitter, 1);

    QObject::connect(m_userList, &QListWidget::currentItemChanged,
                     this, [this](QListWidgetItem *, QListWidgetItem *) {
                         onUserSelectionChanged();
                     });

    return tab;
}

QWidget *EncryptionKeyDialog::createUserDetailPanel()
{
    auto *panel = new QWidget();
    auto *layout = new QVBoxLayout(panel);
    layout->setContentsMargins(12, 12, 12, 12);
    layout->setSpacing(10);

    auto *header = new QLabel(tr("<b>User Keys</b>"), panel);
    QFont hdrFont = header->font();
    hdrFont.setPointSize(13);
    hdrFont.setBold(true);
    header->setFont(hdrFont);
    layout->addWidget(header);

    m_userDetailNameLabel = new QLabel(panel);
    m_userDetailNameLabel->setWordWrap(true);
    layout->addWidget(m_userDetailNameLabel);

    m_userDetailIdLabel = new QLabel(panel);
    m_userDetailIdLabel->setStyleSheet(QStringLiteral("font-family: monospace; color: #888; font-size: 11px;"));
    m_userDetailIdLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_userDetailIdLabel->setWordWrap(true);
    layout->addWidget(m_userDetailIdLabel);

    m_userDetailStatusLabel = new QLabel(panel);
    m_userDetailStatusLabel->setWordWrap(true);
    layout->addWidget(m_userDetailStatusLabel);

    layout->addWidget(createSeparator(panel));

    m_userDetailDeviceCountLabel = new QLabel(panel);
    layout->addWidget(m_userDetailDeviceCountLabel);

    m_userDeviceList = new QListWidget(panel);
    m_userDeviceList->setMaximumHeight(160);
    m_userDeviceList->setAlternatingRowColors(true);
    layout->addWidget(m_userDeviceList);

    layout->addSpacing(8);

    auto *btnRow = new QHBoxLayout;
    btnRow->setSpacing(8);

    m_verifyUserButton = new QPushButton(tr("Verify User"), panel);
    m_verifyUserButton->setStyleSheet(
        QStringLiteral("QPushButton { background-color: #27ae60; color: white; border: none; "
                       "padding: 8px 16px; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #219a52; } "
                       "QPushButton:pressed { background-color: #1e8449; }"));
    QObject::connect(m_verifyUserButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onVerifyUserClicked);
    btnRow->addWidget(m_verifyUserButton);

    m_resetVerificationButton = new QPushButton(tr("Reset Verification"), panel);
    m_resetVerificationButton->setStyleSheet(
        QStringLiteral("QPushButton { background-color: #e67e22; color: white; border: none; "
                       "padding: 8px 16px; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #d35400; } "
                       "QPushButton:pressed { background-color: #ba4a00; }"));
    QObject::connect(m_resetVerificationButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onResetVerificationClicked);
    btnRow->addWidget(m_resetVerificationButton);

    btnRow->addStretch();
    layout->addLayout(btnRow);

    layout->addStretch();
    return panel;
}

// ---------------------------------------------------------------------------
// Key Backup Tab
// ---------------------------------------------------------------------------

QWidget *EncryptionKeyDialog::createKeyBackupTab()
{
    auto *scrollArea = new QScrollArea(this);
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    auto *page = new QWidget(scrollArea);
    auto *layout = new QVBoxLayout(page);
    layout->setContentsMargins(24, 20, 24, 20);
    layout->setSpacing(14);

    // ---- Backup status section ----
    auto *statusGroup = new QGroupBox(tr("Backup Status"), page);
    auto *statusLayout = new QVBoxLayout(statusGroup);
    statusLayout->setSpacing(8);

    m_backupStatusLabel = new QLabel(statusGroup);
    m_backupStatusLabel->setWordWrap(true);
    statusLayout->addWidget(m_backupStatusLabel);

    m_backupLastTimeLabel = new QLabel(statusGroup);
    statusLayout->addWidget(m_backupLastTimeLabel);

    m_backupLocationLabel = new QLabel(statusGroup);
    m_backupLocationLabel->setWordWrap(true);
    m_backupLocationLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    statusLayout->addWidget(m_backupLocationLabel);

    layout->addWidget(statusGroup);

    // ---- Progress bar ----
    m_backupProgressBar = new QProgressBar(page);
    m_backupProgressBar->setRange(0, 100);
    m_backupProgressBar->setValue(0);
    m_backupProgressBar->setVisible(false);
    m_backupProgressBar->setTextVisible(true);
    layout->addWidget(m_backupProgressBar);

    // ---- Action buttons ----
    auto *actionGroup = new QGroupBox(tr("Actions"), page);
    auto *actionLayout = new QVBoxLayout(actionGroup);
    actionLayout->setSpacing(8);

    m_backupNowButton = new QPushButton(tr("Backup Now"), actionGroup);
    m_backupNowButton->setStyleSheet(
        QStringLiteral("QPushButton { padding: 10px 16px; font-weight: bold; }"));
    QObject::connect(m_backupNowButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onBackupNowClicked);
    actionLayout->addWidget(m_backupNowButton);

    layout->addWidget(actionGroup);

    // ---- Restore section ----
    m_recoveryGroup = new QGroupBox(tr("Restore from Backup"), page);
    auto *restoreLayout = new QVBoxLayout(m_recoveryGroup);
    restoreLayout->setSpacing(8);

    auto *recoveryLabel = new QLabel(
        tr("Enter your recovery key or passphrase to restore your encryption keys:"),
        m_recoveryGroup);
    recoveryLabel->setWordWrap(true);
    restoreLayout->addWidget(recoveryLabel);

    m_recoveryKeyEdit = new QLineEdit(m_recoveryGroup);
    m_recoveryKeyEdit->setPlaceholderText(tr("Recovery key / passphrase"));
    m_recoveryKeyEdit->setEchoMode(QLineEdit::Password);
    m_recoveryKeyEdit->setClearButtonEnabled(true);
    QObject::connect(m_recoveryKeyEdit, &QLineEdit::textChanged,
                     this, &EncryptionKeyDialog::onRecoveryKeyTextChanged);
    restoreLayout->addWidget(m_recoveryKeyEdit);

    m_restoreBackupButton = new QPushButton(tr("Restore from Backup"), m_recoveryGroup);
    m_restoreBackupButton->setEnabled(false);
    m_restoreBackupButton->setStyleSheet(
        QStringLiteral("QPushButton { padding: 10px 16px; font-weight: bold; }"));
    QObject::connect(m_restoreBackupButton, &QPushButton::clicked,
                     this, &EncryptionKeyDialog::onRestoreFromBackupClicked);
    restoreLayout->addWidget(m_restoreBackupButton);

    layout->addWidget(m_recoveryGroup);

    layout->addStretch();

    scrollArea->setWidget(page);
    return scrollArea;
}

// ---------------------------------------------------------------------------
// Data population
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::populateDevices()
{
    m_devices.clear();

    {
        DeviceInfo d;
        d.deviceId = QStringLiteral("ABCDEFGHIJKL");
        d.displayName = QStringLiteral("Pixel 8 Pro");
        d.deviceKey = QStringLiteral("3a7b 9f2c 1d4e 6f80 a2b3 c4d5 e6f7 8091");
        d.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2"),
                         QStringLiteral("m.megolm.v1.aes-sha2") };
        d.lastSeen = QDateTime::currentDateTime().addSecs(-120);
        d.verificationStatus = DeviceInfo::Verified;
        m_devices.append(d);
    }
    {
        DeviceInfo d;
        d.deviceId = QStringLiteral("MNOPQRSTUVWX");
        d.displayName = QStringLiteral("Desktop Linux");
        d.deviceKey = QStringLiteral("b4c5 d6e7 f809 1a2b 3c4d 5e6f 7081 92a3");
        d.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2"),
                         QStringLiteral("m.megolm.v1.aes-sha2") };
        d.lastSeen = QDateTime::currentDateTime().addSecs(-3600);
        d.verificationStatus = DeviceInfo::Verified;
        m_devices.append(d);
    }
    {
        DeviceInfo d;
        d.deviceId = QStringLiteral("YZABCDEFGHIJKLMN");
        d.displayName = QStringLiteral("Laptop (old session)");
        d.deviceKey = QStringLiteral("d5e6 f708 192a 3b4c 5d6e 7f80 91a2 b3c4");
        d.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2"),
                         QStringLiteral("m.megolm.v1.aes-sha2") };
        d.lastSeen = QDateTime::currentDateTime().addSecs(-86400 * 3);
        d.verificationStatus = DeviceInfo::NotVerified;
        m_devices.append(d);
    }
    {
        DeviceInfo d;
        d.deviceId = QStringLiteral("OPQRSTUVWXYZAB");
        d.displayName = QStringLiteral("Unknown Device");
        d.deviceKey = QStringLiteral("e6f7 8091 a2b3 c4d5 e6f7 8091 a2b3 c4d5");
        d.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2") };
        d.lastSeen = QDateTime::currentDateTime().addSecs(-86400 * 14);
        d.verificationStatus = DeviceInfo::Unverified;
        m_devices.append(d);
    }

    updateDeviceList();
}

void EncryptionKeyDialog::updateDeviceList()
{
    m_deviceList->clear();
    for (const auto &dev : m_devices) {
        const QString label = QStringLiteral("%1  %2%3%4  %5")
            .arg(dev.displayName,
                 verificationBadgeHtml(dev.verificationStatus),
                 QStringLiteral("  "),
                 truncatedDeviceId(dev.deviceId),
                 elapsedString(dev.lastSeen));

        auto *item = new QListWidgetItem(label, m_deviceList);
        item->setData(Qt::UserRole, dev.deviceId);
        item->setSizeHint(QSize(0, 36));
    }
}

void EncryptionKeyDialog::populateUsers()
{
    m_users.clear();

    {
        UserKeyInfo u;
        u.userId = QStringLiteral("@alice:matrix.org");
        u.displayName = QStringLiteral("Alice");
        u.deviceId = QStringLiteral("ALICE01");
        u.deviceKey = QStringLiteral("a1b2 c3d4 e5f6 7081 92a3 b4c5 d6e7 f809");
        u.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2"),
                         QStringLiteral("m.megolm.v1.aes-sha2") };
        u.keyStatus = UserKeyInfo::KeyVerified;
        m_users.append(u);
    }
    {
        UserKeyInfo u;
        u.userId = QStringLiteral("@bob:matrix.org");
        u.displayName = QStringLiteral("Bob");
        u.deviceId = QStringLiteral("BOB02");
        u.deviceKey = QStringLiteral("c3d4 e5f6 7081 92a3 b4c5 d6e7 f809 1a2b");
        u.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2"),
                         QStringLiteral("m.megolm.v1.aes-sha2") };
        u.keyStatus = UserKeyInfo::KeyNotVerified;
        m_users.append(u);
    }
    {
        UserKeyInfo u;
        u.userId = QStringLiteral("@carol:matrix.org");
        u.displayName = QStringLiteral("Carol");
        u.deviceId = QStringLiteral("CAROL03");
        u.deviceKey = QStringLiteral("d5e6 f708 192a 3b4c 5d6e 7f80 91a2 b3c4");
        u.algorithms = { QStringLiteral("olm.v1.curve25519-aes-sha2"),
                         QStringLiteral("m.megolm.v1.aes-sha2") };
        u.keyStatus = UserKeyInfo::KeyChanged;
        m_users.append(u);
    }

    updateUserList();
}

void EncryptionKeyDialog::updateUserList()
{
    m_userList->clear();
    for (const auto &user : m_users) {
        const QString label = QStringLiteral("%1  %2%3%4")
            .arg(user.displayName,
                 keyStatusBadgeHtml(user.keyStatus),
                 QStringLiteral("  "),
                 user.userId);

        auto *item = new QListWidgetItem(label, m_userList);
        item->setData(Qt::UserRole, user.userId);
        item->setSizeHint(QSize(0, 36));
    }
}

// ---------------------------------------------------------------------------
// Backup status
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::loadBackupStatus()
{
    m_backupEnabled = true;
    m_lastBackupTime = QDateTime::currentDateTime().addSecs(-3600 * 3);
    m_backupLocation = QStringLiteral("~/.local/share/pcp-chat/backups/e2e_keys_2026-05-23.bak");

    updateBackupStatus();
}

void EncryptionKeyDialog::updateBackupStatus()
{
    if (m_backupEnabled) {
        m_backupStatusLabel->setText(
            QStringLiteral("<span style='color: #27ae60; font-weight: bold;'>\xE2\x97\x8F %1</span>")
                .arg(tr("Backup enabled")));
    } else {
        m_backupStatusLabel->setText(
            QStringLiteral("<span style='color: #e74c3c; font-weight: bold;'>\xE2\x97\x8F %1</span>")
                .arg(tr("Backup disabled")));
    }

    m_backupLastTimeLabel->setText(
        tr("Last backup: %1").arg(elapsedString(m_lastBackupTime)));

    m_backupLocationLabel->setText(
        tr("Location: %1").arg(m_backupLocation));
}

// ---------------------------------------------------------------------------
// Slots — My Devices
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::onDeviceSelectionChanged()
{
    auto *item = m_deviceList->currentItem();
    if (!item) {
        m_deviceDetailPanel->setVisible(false);
        return;
    }

    const QString deviceId = item->data(Qt::UserRole).toString();
    const DeviceInfo *dev = nullptr;
    for (const auto &d : m_devices) {
        if (d.deviceId == deviceId) {
            dev = &d;
            break;
        }
    }
    if (!dev) {
        m_deviceDetailPanel->setVisible(false);
        return;
    }

    m_deviceDetailIdLabel->setText(dev->deviceId);
    m_deviceDetailKeyLabel->setText(formatFingerprint(dev->deviceKey));
    m_deviceDetailAlgoLabel->setText(dev->algorithms.join(QStringLiteral(", ")));
    m_deviceDetailSeenLabel->setText(dev->lastSeen.toString(QStringLiteral("yyyy-MM-dd hh:mm:ss")));

    switch (dev->verificationStatus) {
    case DeviceInfo::Verified:
        m_deviceDetailStatusLabel->setText(
            QStringLiteral("<span style='color: #27ae60; font-weight: bold;'>\xE2\x9C\x93 %1</span>")
                .arg(tr("Verified")));
        m_verifyDeviceButton->setEnabled(false);
        break;
    case DeviceInfo::NotVerified:
        m_deviceDetailStatusLabel->setText(
            QStringLiteral("<span style='color: #e74c3c; font-weight: bold;'>\xE2\x9C\x97 %1</span>")
                .arg(tr("Not Verified")));
        m_verifyDeviceButton->setEnabled(true);
        break;
    case DeviceInfo::Unverified:
        m_deviceDetailStatusLabel->setText(
            QStringLiteral("<span style='color: #f39c12; font-weight: bold;'>? %1</span>")
                .arg(tr("Unverified")));
        m_verifyDeviceButton->setEnabled(true);
        break;
    }

    m_deviceDetailPanel->setVisible(true);
}

void EncryptionKeyDialog::onVerifyDeviceClicked()
{
    auto *item = m_deviceList->currentItem();
    if (!item)
        return;

    const QString deviceId = item->data(Qt::UserRole).toString();

    auto *flowDialog = new QDialog(this);
    flowDialog->setWindowTitle(tr("Verify Device"));
    flowDialog->setMinimumSize(420, 380);
    flowDialog->setModal(true);

    auto *dlgLayout = new QVBoxLayout(flowDialog);
    dlgLayout->setContentsMargins(16, 16, 16, 16);
    dlgLayout->setSpacing(12);

    auto *header = new QLabel(tr("SAS Verification"), flowDialog);
    QFont hdrFont = header->font();
    hdrFont.setPointSize(14);
    hdrFont.setBold(true);
    header->setFont(hdrFont);
    header->setAlignment(Qt::AlignCenter);
    dlgLayout->addWidget(header);

    auto *descLabel = new QLabel(
        tr("Compare the emoji below with the other device. If they match, "
           "confirm to complete verification."), flowDialog);
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignCenter);
    dlgLayout->addWidget(descLabel);

    auto *emojiFrame = new QFrame(flowDialog);
    emojiFrame->setFrameShape(QFrame::StyledPanel);
    emojiFrame->setStyleSheet(
        QStringLiteral("QFrame { background-color: #f5f5f5; border-radius: 8px; padding: 16px; }"));
    auto *emojiLayout = new QHBoxLayout(emojiFrame);
    emojiLayout->setAlignment(Qt::AlignCenter);
    emojiLayout->setSpacing(16);

    const QStringList emojis = {
        QStringLiteral("\xF0\x9F\x90\xB1"), // cat
        QStringLiteral("\xF0\x9F\x9A\x80"), // rocket
        QStringLiteral("\xF0\x9F\x8D\x95"), // pizza
        QStringLiteral("\xF0\x9F\x8C\x9F"), // star
        QStringLiteral("\xF0\x9F\x8E\xB8"), // guitar
        QStringLiteral("\xF0\x9F\x8C\xB8"), // cherry blossom
        QStringLiteral("\xF0\x9F\x90\xB6"), // dog
    };
    for (const auto &emoji : emojis) {
        auto *emojiLabel = new QLabel(emoji, emojiFrame);
        emojiLabel->setStyleSheet(QStringLiteral("font-size: 32px; background: transparent;"));
        emojiLabel->setAlignment(Qt::AlignCenter);
        emojiLayout->addWidget(emojiLabel);
    }
    dlgLayout->addWidget(emojiFrame);

    auto *deviceIdLabel = new QLabel(
        tr("Device ID: %1").arg(deviceId), flowDialog);
    deviceIdLabel->setStyleSheet(QStringLiteral("font-family: monospace; color: #888; font-size: 11px;"));
    deviceIdLabel->setAlignment(Qt::AlignCenter);
    deviceIdLabel->setWordWrap(true);
    dlgLayout->addWidget(deviceIdLabel);

    auto *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(12);
    btnLayout->addStretch();

    auto *cancelBtn = new QPushButton(tr("Cancel"), flowDialog);
    btnLayout->addWidget(cancelBtn);

    auto *matchBtn = new QPushButton(tr("They Match"), flowDialog);
    matchBtn->setStyleSheet(
        QStringLiteral("QPushButton { background-color: #27ae60; color: white; border: none; "
                       "padding: 8px 20px; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #219a52; }"));
    btnLayout->addWidget(matchBtn);

    btnLayout->addStretch();
    dlgLayout->addLayout(btnLayout);

    QObject::connect(cancelBtn, &QPushButton::clicked, flowDialog, &QDialog::reject);
    QObject::connect(matchBtn, &QPushButton::clicked, flowDialog, [flowDialog, deviceId, this]() {
        // Mark device as verified
        for (auto &dev : m_devices) {
            if (dev.deviceId == deviceId) {
                dev.verificationStatus = DeviceInfo::Verified;
                break;
            }
        }
        updateDeviceList();
        onDeviceSelectionChanged();
        emit deviceVerificationRequested(deviceId);
        flowDialog->accept();
    });

    flowDialog->show();
}

void EncryptionKeyDialog::onSignOutDeviceClicked()
{
    auto *item = m_deviceList->currentItem();
    if (!item)
        return;

    const QString deviceId = item->data(Qt::UserRole).toString();

    const auto reply = QMessageBox::question(
        this,
        tr("Sign Out Device"),
        tr("Are you sure you want to sign out device \"%1\"?\n\n"
           "This will remove the device from your account and invalidate "
           "its encryption keys.")
            .arg(deviceId),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_devices.removeIf([&deviceId](const DeviceInfo &d) {
            return d.deviceId == deviceId;
        });
        updateDeviceList();
        m_deviceDetailPanel->setVisible(false);
        emit signOutDeviceRequested(deviceId);
    }
}

void EncryptionKeyDialog::onDeviceDetailsCopied()
{
    QClipboard *clipboard = QApplication::clipboard();
    if (clipboard && m_deviceDetailIdLabel) {
        clipboard->setText(m_deviceDetailIdLabel->text());
        QToolTip::showText(m_copyDeviceIdButton->mapToGlobal(QPoint(0, 0)),
                           tr("Copied to clipboard!"),
                           m_copyDeviceIdButton,
                           QRect(),
                           2000);
    }
}

// ---------------------------------------------------------------------------
// Slots — User Keys
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::onUserSelectionChanged()
{
    auto *item = m_userList->currentItem();
    if (!item) {
        m_userDetailPanel->setVisible(false);
        return;
    }

    const QString userId = item->data(Qt::UserRole).toString();
    const UserKeyInfo *user = nullptr;
    for (const auto &u : m_users) {
        if (u.userId == userId) {
            user = &u;
            break;
        }
    }
    if (!user) {
        m_userDetailPanel->setVisible(false);
        return;
    }

    m_userDetailNameLabel->setText(
        QStringLiteral("<b>%1</b>").arg(user->displayName.toHtmlEscaped()));
    m_userDetailIdLabel->setText(user->userId);

    switch (user->keyStatus) {
    case UserKeyInfo::KeyVerified:
        m_userDetailStatusLabel->setText(
            QStringLiteral("<span style='color: #27ae60; font-weight: bold;'>\xE2\x9C\x93 %1</span>")
                .arg(tr("Key Verified")));
        m_verifyUserButton->setEnabled(false);
        m_resetVerificationButton->setEnabled(true);
        break;
    case UserKeyInfo::KeyNotVerified:
        m_userDetailStatusLabel->setText(
            QStringLiteral("<span style='color: #333; font-weight: bold;'>\xE2\x97\x8F %1</span>")
                .arg(tr("Not Verified")));
        m_verifyUserButton->setEnabled(true);
        m_resetVerificationButton->setEnabled(false);
        break;
    case UserKeyInfo::KeyChanged:
        m_userDetailStatusLabel->setText(
            QStringLiteral("<span style='color: #e74c3c; font-weight: bold;'>\xE2\x9A\xA0 %1</span>")
                .arg(tr("Key Changed — Requires Re-verification")));
        m_verifyUserButton->setEnabled(true);
        m_resetVerificationButton->setEnabled(true);
        break;
    }

    m_userDetailDeviceCountLabel->setText(
        tr("<b>Devices:</b> %1").arg(1));

    m_userDeviceList->clear();
    const QString deviceEntry = QStringLiteral("%1  |  %2")
        .arg(user->deviceId, formatFingerprint(user->deviceKey));
    m_userDeviceList->addItem(deviceEntry);

    m_userDetailPanel->setVisible(true);
}

void EncryptionKeyDialog::onVerifyUserClicked()
{
    auto *item = m_userList->currentItem();
    if (!item)
        return;

    const QString userId = item->data(Qt::UserRole).toString();

    // Show SAS verification dialog (same emoji flow as device verification)
    auto *flowDialog = new QDialog(this);
    flowDialog->setWindowTitle(tr("Verify User"));
    flowDialog->setMinimumSize(420, 380);
    flowDialog->setModal(true);

    auto *dlgLayout = new QVBoxLayout(flowDialog);
    dlgLayout->setContentsMargins(16, 16, 16, 16);
    dlgLayout->setSpacing(12);

    auto *header = new QLabel(tr("SAS Verification"), flowDialog);
    QFont hdrFont = header->font();
    hdrFont.setPointSize(14);
    hdrFont.setBold(true);
    header->setFont(hdrFont);
    header->setAlignment(Qt::AlignCenter);
    dlgLayout->addWidget(header);

    auto *descLabel = new QLabel(
        tr("Compare the emoji below with %1. If they match, confirm to complete verification.")
            .arg(userId), flowDialog);
    descLabel->setWordWrap(true);
    descLabel->setAlignment(Qt::AlignCenter);
    dlgLayout->addWidget(descLabel);

    auto *emojiFrame = new QFrame(flowDialog);
    emojiFrame->setFrameShape(QFrame::StyledPanel);
    emojiFrame->setStyleSheet(
        QStringLiteral("QFrame { background-color: #f5f5f5; border-radius: 8px; padding: 16px; }"));
    auto *emojiLayout = new QHBoxLayout(emojiFrame);
    emojiLayout->setAlignment(Qt::AlignCenter);
    emojiLayout->setSpacing(16);

    const QStringList emojis = {
        QStringLiteral("\xF0\x9F\x8C\x88"), // rainbow
        QStringLiteral("\xF0\x9F\x94\x91"), // lock
        QStringLiteral("\xF0\x9F\x8C\xB5"), // cactus
        QStringLiteral("\xF0\x9F\x90\xA2"), // dolphin
        QStringLiteral("\xF0\x9F\x8E\xA8"), // artist palette
        QStringLiteral("\xF0\x9F\x8D\x80"), // four leaf clover
        QStringLiteral("\xF0\x9F\x90\xA0"), // mountain goat
    };
    for (const auto &emoji : emojis) {
        auto *emojiLabel = new QLabel(emoji, emojiFrame);
        emojiLabel->setStyleSheet(QStringLiteral("font-size: 32px; background: transparent;"));
        emojiLabel->setAlignment(Qt::AlignCenter);
        emojiLayout->addWidget(emojiLabel);
    }
    dlgLayout->addWidget(emojiFrame);

    auto *userIdLabel = new QLabel(tr("User: %1").arg(userId), flowDialog);
    userIdLabel->setStyleSheet(QStringLiteral("font-family: monospace; color: #888; font-size: 11px;"));
    userIdLabel->setAlignment(Qt::AlignCenter);
    userIdLabel->setWordWrap(true);
    dlgLayout->addWidget(userIdLabel);

    auto *btnLayout = new QHBoxLayout;
    btnLayout->setSpacing(12);
    btnLayout->addStretch();

    auto *cancelBtn = new QPushButton(tr("Cancel"), flowDialog);
    btnLayout->addWidget(cancelBtn);

    auto *matchBtn = new QPushButton(tr("They Match"), flowDialog);
    matchBtn->setStyleSheet(
        QStringLiteral("QPushButton { background-color: #27ae60; color: white; border: none; "
                       "padding: 8px 20px; border-radius: 4px; font-weight: bold; } "
                       "QPushButton:hover { background-color: #219a52; }"));
    btnLayout->addWidget(matchBtn);

    btnLayout->addStretch();
    dlgLayout->addLayout(btnLayout);

    QObject::connect(cancelBtn, &QPushButton::clicked, flowDialog, &QDialog::reject);
    QObject::connect(matchBtn, &QPushButton::clicked, flowDialog, [flowDialog, userId, this]() {
        for (auto &user : m_users) {
            if (user.userId == userId) {
                user.keyStatus = UserKeyInfo::KeyVerified;
                break;
            }
        }
        updateUserList();
        onUserSelectionChanged();
        emit userVerificationRequested(userId);
        flowDialog->accept();
    });

    flowDialog->show();
}

void EncryptionKeyDialog::onResetVerificationClicked()
{
    auto *item = m_userList->currentItem();
    if (!item)
        return;

    const QString userId = item->data(Qt::UserRole).toString();

    const auto reply = QMessageBox::question(
        this,
        tr("Reset Verification"),
        tr("Are you sure you want to reset verification for \"%1\"?\n\n"
           "This will mark their keys as unverified and you will need "
           "to re-verify before future encrypted communication.")
            .arg(userId),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        for (auto &user : m_users) {
            if (user.userId == userId) {
                user.keyStatus = UserKeyInfo::KeyNotVerified;
                break;
            }
        }
        updateUserList();
        onUserSelectionChanged();
        emit userVerificationResetRequested(userId);
    }
}

// ---------------------------------------------------------------------------
// Slots — Key Backup
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::onBackupNowClicked()
{
    m_backupProgressBar->setVisible(true);
    m_backupProgressBar->setValue(10);

    // Simulate backup progress
    m_backupProgressBar->setValue(50);
    m_backupProgressBar->setValue(100);

    m_lastBackupTime = QDateTime::currentDateTime();
    updateBackupStatus();

    emit backupNowRequested();

    QTimer::singleShot(1500, this, [this]() {
        m_backupProgressBar->setVisible(false);
        m_backupProgressBar->setValue(0);
    });
}

void EncryptionKeyDialog::onRestoreFromBackupClicked()
{
    const QString recoveryKey = m_recoveryKeyEdit->text().trimmed();
    if (recoveryKey.isEmpty()) {
        QMessageBox::warning(
            this,
            tr("Recovery Key Required"),
            tr("Please enter your recovery key or passphrase to restore from backup."));
        return;
    }

    const auto reply = QMessageBox::question(
        this,
        tr("Restore Keys"),
        tr("This will replace your current encryption keys with the keys "
           "from the backup. Continue?"),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        emit restoreFromBackupRequested(recoveryKey);
        m_recoveryKeyEdit->clear();
    }
}

void EncryptionKeyDialog::onRecoveryKeyTextChanged(const QString &text)
{
    m_restoreBackupButton->setEnabled(!text.trimmed().isEmpty());
}

// ---------------------------------------------------------------------------
// Slots — Export / Import
// ---------------------------------------------------------------------------

void EncryptionKeyDialog::onExportKeysClicked()
{
    const QString filePath = QFileDialog::getSaveFileName(
        this,
        tr("Export Encryption Keys"),
        QStringLiteral("e2e_keys_%1.txt").arg(
            QDateTime::currentDateTime().toString(QStringLiteral("yyyy-MM-dd"))),
        tr("Key Files (*.txt *.bak);;All Files (*)"));

    if (!filePath.isEmpty()) {
        emit exportKeysRequested(filePath);
    }
}

void EncryptionKeyDialog::onImportKeysClicked()
{
    const QString filePath = QFileDialog::getOpenFileName(
        this,
        tr("Import Encryption Keys"),
        QString(),
        tr("Key Files (*.txt *.bak);;All Files (*)"));

    if (filePath.isEmpty())
        return;

    bool ok = false;
    const QString passphrase = QInputDialog::getText(
        this,
        tr("Import Keys"),
        tr("Enter passphrase for the key file (leave empty if none):"),
        QLineEdit::Password,
        QString(),
        &ok);

    if (ok) {
        emit importKeysRequested(filePath, passphrase);
    }
}

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

QString EncryptionKeyDialog::verificationBadgeHtml(DeviceInfo::VerificationStatus status) const
{
    switch (status) {
    case DeviceInfo::Verified:
        return QStringLiteral(
            "<span style='color: #27ae60; font-weight: bold; font-size: 14px;'"
            ">\xE2\x9C\x93</span>");
    case DeviceInfo::NotVerified:
        return QStringLiteral(
            "<span style='color: #e74c3c; font-weight: bold; font-size: 14px;'"
            ">\xE2\x9C\x97</span>");
    case DeviceInfo::Unverified:
        return QStringLiteral(
            "<span style='color: #f39c12; font-weight: bold; font-size: 14px;'"
            ">?</span>");
    }
    return QString();
}

QString EncryptionKeyDialog::keyStatusBadgeHtml(UserKeyInfo::KeyStatus status) const
{
    switch (status) {
    case UserKeyInfo::KeyVerified:
        return QStringLiteral(
            "<span style='color: #27ae60; font-weight: bold; font-size: 14px;'"
            ">\xE2\x9C\x93</span>");
    case UserKeyInfo::KeyNotVerified:
        return QStringLiteral(
            "<span style='color: #333; font-weight: bold; font-size: 14px;'"
            ">\xE2\x97\x8F</span>");
    case UserKeyInfo::KeyChanged:
        return QStringLiteral(
            "<span style='color: #e74c3c; font-weight: bold; font-size: 14px;'"
            ">\xE2\x9A\xA0</span>");
    }
    return QString();
}

QString EncryptionKeyDialog::truncatedDeviceId(const QString &deviceId) const
{
    if (deviceId.length() <= 12)
        return deviceId;
    return deviceId.left(6) + QStringLiteral("...") + deviceId.right(6);
}
