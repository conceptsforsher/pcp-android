#include "settings_dialog.hpp"
#include "../../protocol/protocol_manager.hpp"

#include <QSettings>
#include <QFontDatabase>
#include <QMessageBox>
#include <QApplication>
#include <QSslConfiguration>

namespace pcp_chat {

SettingsDialog::SettingsDialog(ProtocolManager *manager, QWidget *parent)
    : QDialog(parent)
    , m_protocolManager(manager)
{
    setWindowTitle("Settings");
    setMinimumSize(600, 450);

    auto *mainLayout = new QVBoxLayout(this);

    m_tabWidget = new QTabWidget();
    setupGeneralTab();
    setupNotificationsTab();
    setupNetworkTab();
    setupAppearanceTab();
    setupEncryptionTab();
    setupAdvancedTab();
    mainLayout->addWidget(m_tabWidget);

    // Action buttons
    auto *btnLayout = new QHBoxLayout();
    btnLayout->addStretch();

    auto *applyBtn = new QPushButton("Apply");
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::applySettings);

    auto *saveBtn = new QPushButton("Save");
    connect(saveBtn, &QPushButton::clicked, this, &SettingsDialog::saveSettings);

    auto *cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);

    btnLayout->addWidget(applyBtn);
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    mainLayout->addLayout(btnLayout);

    loadSettings();
}

void SettingsDialog::setupGeneralTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *groupGeneral = new QGroupBox("General");
    auto *formGeneral = new QFormLayout(groupGeneral);

    m_autoStart = new QCheckBox("Start on system login");
    formGeneral->addRow("", m_autoStart);

    m_minimizeToTray = new QCheckBox("Minimize to system tray");
    formGeneral->addRow("", m_minimizeToTray);

    m_showReadReceipts = new QCheckBox("Show read receipts");
    m_showReadReceipts->setChecked(true);
    formGeneral->addRow("", m_showReadReceipts);

    m_languageSelector = new QComboBox();
    m_languageSelector->addItems({"System Default", "English", "Русский", "Deutsch",
                                   "Français", "Español", "日本語", "中文"});
    formGeneral->addRow("Language:", m_languageSelector);

    layout->addWidget(groupGeneral);
    layout->addStretch();
    m_tabWidget->addTab(tab, "General");
}

void SettingsDialog::setupNotificationsTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *groupNotif = new QGroupBox("Notifications");
    auto *formNotif = new QFormLayout(groupNotif);

    m_enableNotifications = new QCheckBox("Enable desktop notifications");
    m_enableNotifications->setChecked(true);
    formNotif->addRow("", m_enableNotifications);

    m_enableSounds = new QCheckBox("Play notification sounds");
    m_enableSounds->setChecked(true);
    formNotif->addRow("", m_enableSounds);

    m_showMessagePreviews = new QCheckBox("Show message preview in notifications");
    m_showMessagePreviews->setChecked(true);
    formNotif->addRow("", m_showMessagePreviews);

    m_notificationStyle = new QComboBox();
    m_notificationStyle->addItems({"System Default", "PCP Style", "Minimal"});
    formNotif->addRow("Style:", m_notificationStyle);

    m_notificationDuration = new QSpinBox();
    m_notificationDuration->setRange(1000, 30000);
    m_notificationDuration->setValue(5000);
    m_notificationDuration->setSuffix(" ms");
    m_notificationDuration->setSingleStep(1000);
    formNotif->addRow("Duration:", m_notificationDuration);

    layout->addWidget(groupNotif);
    layout->addStretch();
    m_tabWidget->addTab(tab, "Notifications");
}

void SettingsDialog::setupNetworkTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *groupProxy = new QGroupBox("Proxy");
    auto *formProxy = new QFormLayout(groupProxy);

    m_proxyEnabled = new QCheckBox("Enable proxy");
    formProxy->addRow("", m_proxyEnabled);

    m_proxyType = new QComboBox();
    m_proxyType->addItems({"HTTP", "SOCKS5"});
    formProxy->addRow("Type:", m_proxyType);

    m_proxyHost = new QLineEdit();
    m_proxyHost->setPlaceholderText("proxy.example.com");
    formProxy->addRow("Host:", m_proxyHost);

    m_proxyPort = new QSpinBox();
    m_proxyPort->setRange(1, 65535);
    m_proxyPort->setValue(1080);
    formProxy->addRow("Port:", m_proxyPort);

    layout->addWidget(groupProxy);

    auto *groupConn = new QGroupBox("Connection");
    auto *formConn = new QFormLayout(groupConn);

    m_connectionTimeout = new QSpinBox();
    m_connectionTimeout->setRange(5000, 120000);
    m_connectionTimeout->setValue(30000);
    m_connectionTimeout->setSuffix(" ms");
    m_connectionTimeout->setSingleStep(5000);
    formConn->addRow("Timeout:", m_connectionTimeout);

    m_useTor = new QCheckBox("Route traffic through Tor");
    formConn->addRow("", m_useTor);

    layout->addWidget(groupConn);
    layout->addStretch();
    m_tabWidget->addTab(tab, "Network");
}

void SettingsDialog::setupAppearanceTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *groupAppearance = new QGroupBox("Appearance");
    auto *formAppearance = new QFormLayout(groupAppearance);

    m_themeSelector = new QComboBox();
    m_themeSelector->addItems({"System", "Light", "Dark", "High Contrast", "Retro"});
    formAppearance->addRow("Theme:", m_themeSelector);

    QFontDatabase fontDb;
    m_fontFamily = new QComboBox();
    m_fontFamily->addItems(fontDb.families());
    formAppearance->addRow("Font:", m_fontFamily);

    m_fontSize = new QSpinBox();
    m_fontSize->setRange(8, 24);
    m_fontSize->setValue(13);
    formAppearance->addRow("Font size:", m_fontSize);

    m_messageStyle = new QComboBox();
    m_messageStyle->addItems({"Bubbles", "Compact", "Modern"});
    formAppearance->addRow("Message style:", m_messageStyle);

    m_compactMode = new QCheckBox("Compact mode");
    formAppearance->addRow("", m_compactMode);

    m_showAvatars = new QCheckBox("Show avatars");
    m_showAvatars->setChecked(true);
    formAppearance->addRow("", m_showAvatars);

    m_animateEmoji = new QCheckBox("Animate emoji");
    m_animateEmoji->setChecked(true);
    formAppearance->addRow("", m_animateEmoji);

    layout->addWidget(groupAppearance);
    layout->addStretch();
    m_tabWidget->addTab(tab, "Appearance");
}

void SettingsDialog::setupEncryptionTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *groupEncryption = new QGroupBox("End-to-End Encryption");
    auto *formEncryption = new QFormLayout(groupEncryption);

    m_encryptionEnabled = new QCheckBox("Enable end-to-end encryption");
    m_encryptionEnabled->setChecked(true);
    formEncryption->addRow("", m_encryptionEnabled);

    m_autoAcceptKeyRequests = new QCheckBox("Auto-accept key requests from verified devices");
    formEncryption->addRow("", m_autoAcceptKeyRequests);

    m_autoBackupKeys = new QCheckBox("Auto-backup encryption keys");
    m_autoBackupKeys->setChecked(true);
    formEncryption->addRow("", m_autoBackupKeys);

    m_backupPassphrase = new QLineEdit();
    m_backupPassphrase->setEchoMode(QLineEdit::Password);
    m_backupPassphrase->setPlaceholderText("Enter security passphrase");
    formEncryption->addRow("Backup passphrase:", m_backupPassphrase);

    layout->addWidget(groupEncryption);
    layout->addStretch();
    m_tabWidget->addTab(tab, "Encryption");
}

void SettingsDialog::setupAdvancedTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);

    auto *groupAdvanced = new QGroupBox("Advanced");
    auto *formAdvanced = new QFormLayout(groupAdvanced);

    m_developerMode = new QCheckBox("Developer mode");
    formAdvanced->addRow("", m_developerMode);

    m_enableDebugLogging = new QCheckBox("Enable debug logging");
    formAdvanced->addRow("", m_enableDebugLogging);

    m_sendCrashReports = new QCheckBox("Send anonymous crash reports");
    m_sendCrashReports->setChecked(true);
    formAdvanced->addRow("", m_sendCrashReports);

    m_maxCacheSize = new QSpinBox();
    m_maxCacheSize->setRange(50, 5000);
    m_maxCacheSize->setValue(500);
    m_maxCacheSize->setSuffix(" MB");
    m_maxCacheSize->setSingleStep(50);
    formAdvanced->addRow("Max cache size:", m_maxCacheSize);

    layout->addWidget(groupAdvanced);
    layout->addStretch();
    m_tabWidget->addTab(tab, "Advanced");
}

void SettingsDialog::loadSettings()
{
    QSettings settings("PCP", "pcp_chat");

    m_autoStart->setChecked(settings.value("general/autostart", false).toBool());
    m_minimizeToTray->setChecked(settings.value("general/minimize_to_tray", true).toBool());
    m_showReadReceipts->setChecked(settings.value("general/show_read_receipts", true).toBool());
    m_enableNotifications->setChecked(settings.value("notifications/enabled", true).toBool());
    m_enableSounds->setChecked(settings.value("notifications/sounds", true).toBool());
    m_showMessagePreviews->setChecked(settings.value("notifications/previews", true).toBool());
    m_proxyEnabled->setChecked(settings.value("network/proxy_enabled", false).toBool());
    m_proxyHost->setText(settings.value("network/proxy_host").toString());
    m_proxyPort->setValue(settings.value("network/proxy_port", 1080).toInt());
    m_themeSelector->setCurrentText(settings.value("appearance/theme", "System").toString());
    m_fontSize->setValue(settings.value("appearance/font_size", 13).toInt());
    m_compactMode->setChecked(settings.value("appearance/compact", false).toBool());
    m_showAvatars->setChecked(settings.value("appearance/show_avatars", true).toBool());
    m_encryptionEnabled->setChecked(settings.value("encryption/enabled", true).toBool());
    m_developerMode->setChecked(settings.value("advanced/developer_mode", false).toBool());
    m_enableDebugLogging->setChecked(settings.value("advanced/debug_logging", false).toBool());
    m_maxCacheSize->setValue(settings.value("advanced/max_cache_mb", 500).toInt());
}

void SettingsDialog::saveSettings()
{
    QSettings settings("PCP", "pcp_chat");

    settings.setValue("general/autostart", m_autoStart->isChecked());
    settings.setValue("general/minimize_to_tray", m_minimizeToTray->isChecked());
    settings.setValue("general/show_read_receipts", m_showReadReceipts->isChecked());
    settings.setValue("notifications/enabled", m_enableNotifications->isChecked());
    settings.setValue("notifications/sounds", m_enableSounds->isChecked());
    settings.setValue("notifications/previews", m_showMessagePreviews->isChecked());
    settings.setValue("network/proxy_enabled", m_proxyEnabled->isChecked());
    settings.setValue("network/proxy_host", m_proxyHost->text());
    settings.setValue("network/proxy_port", m_proxyPort->value());
    settings.setValue("appearance/theme", m_themeSelector->currentText());
    settings.setValue("appearance/font_size", m_fontSize->value());
    settings.setValue("appearance/compact", m_compactMode->isChecked());
    settings.setValue("appearance/show_avatars", m_showAvatars->isChecked());
    settings.setValue("encryption/enabled", m_encryptionEnabled->isChecked());
    settings.setValue("advanced/developer_mode", m_developerMode->isChecked());
    settings.setValue("advanced/debug_logging", m_enableDebugLogging->isChecked());
    settings.setValue("advanced/max_cache_mb", m_maxCacheSize->value());

    QMessageBox::information(this, "Settings", "Settings saved successfully.");
}

void SettingsDialog::applySettings()
{
    saveSettings();
}

} // namespace pcp_chat
