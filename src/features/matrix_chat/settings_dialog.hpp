#pragma once

#include <QDialog>
#include <QTabWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>
#include <QSpinBox>
#include <QListWidget>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

namespace pcp_chat {

class ProtocolManager;

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(ProtocolManager *manager, QWidget *parent = nullptr);

private:
    void setupGeneralTab();
    void setupNotificationsTab();
    void setupNetworkTab();
    void setupAppearanceTab();
    void setupEncryptionTab();
    void setupAdvancedTab();
    void loadSettings();
    void saveSettings();
    void applySettings();

    ProtocolManager *m_protocolManager;
    QTabWidget *m_tabWidget;

    // General settings
    QCheckBox *m_autoStart;
    QCheckBox *m_minimizeToTray;
    QCheckBox *m_showReadReceipts;
    QComboBox *m_languageSelector;

    // Notification settings
    QCheckBox *m_enableNotifications;
    QCheckBox *m_enableSounds;
    QCheckBox *m_showMessagePreviews;
    QComboBox *m_notificationStyle;
    QSpinBox *m_notificationDuration;

    // Network settings
    QLineEdit *m_proxyHost;
    QSpinBox *m_proxyPort;
    QComboBox *m_proxyType;
    QCheckBox *m_proxyEnabled;
    QSpinBox *m_connectionTimeout;
    QCheckBox *m_useTor;

    // Appearance settings
    QComboBox *m_themeSelector;
    QComboBox *m_fontFamily;
    QSpinBox *m_fontSize;
    QComboBox *m_messageStyle;
    QCheckBox *m_compactMode;
    QCheckBox *m_showAvatars;
    QCheckBox *m_animateEmoji;

    // Encryption settings
    QCheckBox *m_encryptionEnabled;
    QCheckBox *m_autoAcceptKeyRequests;
    QCheckBox *m_autoBackupKeys;
    QLineEdit *m_backupPassphrase;

    // Advanced
    QCheckBox *m_developerMode;
    QCheckBox *m_enableDebugLogging;
    QCheckBox *m_sendCrashReports;
    QSpinBox *m_maxCacheSize;
};

} // namespace pcp_chat
