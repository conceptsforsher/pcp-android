#include "features/matrix_chat/settings_dialog.hpp"

#include <QTabWidget>
#include <QComboBox>
#include <QCheckBox>
#include <QSlider>
#include <QSpinBox>
#include <QGroupBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QTimeEdit>
#include <QDialogButtonBox>
#include <QLabel>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QSettings>
#include <QFont>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
    , m_tabWidget(nullptr)
    , m_buttonBox(nullptr)
    , m_languageCombo(nullptr)
    , m_startMinimizedCheck(nullptr)
    , m_closeToTrayCheck(nullptr)
    , m_autoConnectCheck(nullptr)
    , m_themeCombo(nullptr)
    , m_fontSizeSlider(nullptr)
    , m_fontSizeLabel(nullptr)
    , m_bubbleStyleCombo(nullptr)
    , m_showAvatarsCheck(nullptr)
    , m_showTimestampsCheck(nullptr)
    , m_proxyTypeCombo(nullptr)
    , m_proxyHostEdit(nullptr)
    , m_proxyPortSpin(nullptr)
    , m_proxyUserEdit(nullptr)
    , m_proxyPasswordEdit(nullptr)
    , m_proxyGroup(nullptr)
    , m_timeoutSpin(nullptr)
    , m_yggdrasilCheck(nullptr)
    , m_i2pCheck(nullptr)
    , m_enableNotificationsCheck(nullptr)
    , m_showPreviewCheck(nullptr)
    , m_soundCheck(nullptr)
    , m_matrixNotifyCheck(nullptr)
    , m_ircNotifyCheck(nullptr)
    , m_lemmyNotifyCheck(nullptr)
    , m_keywordsEdit(nullptr)
    , m_quietFromEdit(nullptr)
    , m_quietToEdit(nullptr)
{
    setupUi();
    loadSettings();
}

SettingsDialog::~SettingsDialog() = default;

void SettingsDialog::setupUi()
{
    setWindowTitle(tr("Settings"));
    setMinimumSize(520, 440);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 12, 12, 12);
    mainLayout->setSpacing(12);

    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(createGeneralTab(), tr("General"));
    m_tabWidget->addTab(createAppearanceTab(), tr("Appearance"));
    m_tabWidget->addTab(createNetworkTab(), tr("Network"));
    m_tabWidget->addTab(createNotificationsTab(), tr("Notifications"));
    m_tabWidget->addTab(createAboutTab(), tr("About"));

    m_buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply, this);

    mainLayout->addWidget(m_tabWidget);
    mainLayout->addWidget(m_buttonBox);

    QObject::connect(m_buttonBox, &QDialogButtonBox::accepted,
                     this, &SettingsDialog::onAccepted);
    QObject::connect(m_buttonBox, &QDialogButtonBox::rejected,
                     this, &QDialog::reject);
    QObject::connect(m_buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked,
                     this, &SettingsDialog::onApplied);

    QObject::connect(m_proxyTypeCombo, &QComboBox::currentIndexChanged,
                     this, [this](int index) {
                         const bool enabled = (index > 0);
                         m_proxyHostEdit->setEnabled(enabled);
                         m_proxyPortSpin->setEnabled(enabled);
                         m_proxyUserEdit->setEnabled(enabled);
                         m_proxyPasswordEdit->setEnabled(enabled);
                     });

    QObject::connect(m_enableNotificationsCheck, &QCheckBox::toggled,
                     this, [this](bool checked) {
                         m_showPreviewCheck->setEnabled(checked);
                         m_soundCheck->setEnabled(checked);
                         m_matrixNotifyCheck->setEnabled(checked);
                         m_ircNotifyCheck->setEnabled(checked);
                         m_lemmyNotifyCheck->setEnabled(checked);
                         m_keywordsEdit->setEnabled(checked);
                         m_quietFromEdit->setEnabled(checked);
                         m_quietToEdit->setEnabled(checked);
                     });
}

QWidget *SettingsDialog::createGeneralTab()
{
    auto *tab = new QWidget();
    auto *layout = new QFormLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_languageCombo = new QComboBox(tab);
    m_languageCombo->addItems({
        tr("System"),
        QStringLiteral("English"),
        QStringLiteral("Deutsch"),
        QStringLiteral("Русский"),
        QStringLiteral("Français"),
        QStringLiteral("Español"),
        QStringLiteral("日本語"),
        QStringLiteral("中文"),
    });
    layout->addRow(tr("Language:"), m_languageCombo);

    m_startMinimizedCheck = new QCheckBox(tr("Start minimized to tray"), tab);
    layout->addRow(m_startMinimizedCheck);

    m_closeToTrayCheck = new QCheckBox(tr("Close to tray instead of quitting"), tab);
    layout->addRow(m_closeToTrayCheck);

    m_autoConnectCheck = new QCheckBox(tr("Auto-connect on startup"), tab);
    layout->addRow(m_autoConnectCheck);

    return tab;
}

QWidget *SettingsDialog::createAppearanceTab()
{
    auto *tab = new QWidget();
    auto *layout = new QFormLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_themeCombo = new QComboBox(tab);
    m_themeCombo->addItems({
        tr("Light"),
        tr("Dark"),
        tr("System"),
    });
    layout->addRow(tr("Theme:"), m_themeCombo);

    auto *fontRow = new QHBoxLayout();
    m_fontSizeSlider = new QSlider(Qt::Horizontal, tab);
    m_fontSizeSlider->setRange(10, 24);
    m_fontSizeSlider->setTickPosition(QSlider::TicksBelow);
    m_fontSizeSlider->setTickInterval(2);
    m_fontSizeLabel = new QLabel(QStringLiteral("14 pt"), tab);
    m_fontSizeLabel->setMinimumWidth(48);
    fontRow->addWidget(m_fontSizeSlider, 1);
    fontRow->addWidget(m_fontSizeLabel);
    layout->addRow(tr("Font size:"), fontRow);

    QObject::connect(m_fontSizeSlider, &QSlider::valueChanged,
                     this, [this](int value) {
                         m_fontSizeLabel->setText(tr("%1 pt").arg(value));
                     });

    m_bubbleStyleCombo = new QComboBox(tab);
    m_bubbleStyleCombo->addItems({
        tr("Rounded"),
        tr("Square"),
    });
    layout->addRow(tr("Message bubble style:"), m_bubbleStyleCombo);

    m_showAvatarsCheck = new QCheckBox(tr("Show avatars"), tab);
    layout->addRow(m_showAvatarsCheck);

    m_showTimestampsCheck = new QCheckBox(tr("Show timestamps"), tab);
    layout->addRow(m_showTimestampsCheck);

    return tab;
}

QWidget *SettingsDialog::createNetworkTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(12);

    m_proxyGroup = new QGroupBox(tr("Proxy"), tab);
    auto *proxyLayout = new QGridLayout(m_proxyGroup);
    proxyLayout->setSpacing(8);

    m_proxyTypeCombo = new QComboBox(m_proxyGroup);
    m_proxyTypeCombo->addItems({
        tr("None"),
        QStringLiteral("SOCKS5"),
        QStringLiteral("HTTP"),
    });
    proxyLayout->addWidget(new QLabel(tr("Type:"), m_proxyGroup), 0, 0);
    proxyLayout->addWidget(m_proxyTypeCombo, 0, 1);

    proxyLayout->addWidget(new QLabel(tr("Host:"), m_proxyGroup), 1, 0);
    m_proxyHostEdit = new QLineEdit(m_proxyGroup);
    m_proxyHostEdit->setPlaceholderText(QStringLiteral("127.0.0.1"));
    proxyLayout->addWidget(m_proxyHostEdit, 1, 1);

    proxyLayout->addWidget(new QLabel(tr("Port:"), m_proxyGroup), 2, 0);
    m_proxyPortSpin = new QSpinBox(m_proxyGroup);
    m_proxyPortSpin->setRange(1, 65535);
    m_proxyPortSpin->setValue(1080);
    proxyLayout->addWidget(m_proxyPortSpin, 2, 1);

    proxyLayout->addWidget(new QLabel(tr("Username:"), m_proxyGroup), 3, 0);
    m_proxyUserEdit = new QLineEdit(m_proxyGroup);
    proxyLayout->addWidget(m_proxyUserEdit, 3, 1);

    proxyLayout->addWidget(new QLabel(tr("Password:"), m_proxyGroup), 4, 0);
    m_proxyPasswordEdit = new QLineEdit(m_proxyGroup);
    m_proxyPasswordEdit->setEchoMode(QLineEdit::Password);
    proxyLayout->addWidget(m_proxyPasswordEdit, 4, 1);

    layout->addWidget(m_proxyGroup);

    auto *timeoutRow = new QHBoxLayout();
    auto *timeoutLabel = new QLabel(tr("Connection timeout:"), tab);
    m_timeoutSpin = new QSpinBox(tab);
    m_timeoutSpin->setRange(5, 120);
    m_timeoutSpin->setSuffix(tr(" s"));
    m_timeoutSpin->setValue(30);
    timeoutRow->addWidget(timeoutLabel);
    timeoutRow->addWidget(m_timeoutSpin);
    timeoutRow->addStretch();
    layout->addLayout(timeoutRow);

    m_yggdrasilCheck = new QCheckBox(tr("Enable Yggdrasil network"), tab);
    layout->addWidget(m_yggdrasilCheck);

    m_i2pCheck = new QCheckBox(tr("Enable I2P network"), tab);
    layout->addWidget(m_i2pCheck);

    layout->addStretch();
    return tab;
}

QWidget *SettingsDialog::createNotificationsTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(16, 16, 16, 16);
    layout->setSpacing(10);

    m_enableNotificationsCheck = new QCheckBox(tr("Enable notifications"), tab);
    layout->addWidget(m_enableNotificationsCheck);

    m_showPreviewCheck = new QCheckBox(tr("Show message preview in notification"), tab);
    layout->addWidget(m_showPreviewCheck);

    m_soundCheck = new QCheckBox(tr("Play sound on new message"), tab);
    layout->addWidget(m_soundCheck);

    auto *protocolGroup = new QGroupBox(tr("Per-protocol notifications"), tab);
    auto *protocolLayout = new QVBoxLayout(protocolGroup);
    protocolLayout->setSpacing(6);

    m_matrixNotifyCheck = new QCheckBox(QStringLiteral("Matrix"), protocolGroup);
    m_ircNotifyCheck = new QCheckBox(QStringLiteral("IRC"), protocolGroup);
    m_lemmyNotifyCheck = new QCheckBox(QStringLiteral("Lemmy"), protocolGroup);

    protocolLayout->addWidget(m_matrixNotifyCheck);
    protocolLayout->addWidget(m_ircNotifyCheck);
    protocolLayout->addWidget(m_lemmyNotifyCheck);
    layout->addWidget(protocolGroup);

    layout->addWidget(new QLabel(tr("Keyword highlight (one per line):"), tab));
    m_keywordsEdit = new QTextEdit(tab);
    m_keywordsEdit->setMaximumHeight(80);
    m_keywordsEdit->setPlaceholderText(tr("urgent\n@me\nimportant"));
    layout->addWidget(m_keywordsEdit);

    auto *quietGroup = new QGroupBox(tr("Quiet hours"), tab);
    auto *quietLayout = new QHBoxLayout(quietGroup);
    quietLayout->setSpacing(8);

    quietLayout->addWidget(new QLabel(tr("From:"), quietGroup));
    m_quietFromEdit = new QTimeEdit(quietGroup);
    m_quietFromEdit->setDisplayFormat(QStringLiteral("HH:mm"));
    m_quietFromEdit->setTime(QTime(22, 0));
    quietLayout->addWidget(m_quietFromEdit);

    quietLayout->addWidget(new QLabel(tr("To:"), quietGroup));
    m_quietToEdit = new QTimeEdit(quietGroup);
    m_quietToEdit->setDisplayFormat(QStringLiteral("HH:mm"));
    m_quietToEdit->setTime(QTime(8, 0));
    quietLayout->addWidget(m_quietToEdit);

    quietLayout->addStretch();
    layout->addWidget(quietGroup);

    layout->addStretch();
    return tab;
}

QWidget *SettingsDialog::createAboutTab()
{
    auto *tab = new QWidget();
    auto *layout = new QVBoxLayout(tab);
    layout->setContentsMargins(24, 24, 24, 24);
    layout->setSpacing(8);

    auto *titleLabel = new QLabel(tr("PCP"), tab);
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(20);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    layout->addWidget(titleLabel);

    auto *versionLabel = new QLabel(tr("Version %1").arg(QStringLiteral("1.0.0")), tab);
    versionLabel->setStyleSheet(QStringLiteral("color: #888; font-size: 12px;"));
    layout->addWidget(versionLabel);

    layout->addSpacing(8);

    auto *descLabel = new QLabel(
        tr("PCP Concept - 100%% C++ Matrix/IRC/Lemmy client"), tab);
    descLabel->setWordWrap(true);
    descLabel->setStyleSheet(QStringLiteral("font-size: 13px;"));
    layout->addWidget(descLabel);

    layout->addSpacing(4);

    auto *detailLabel = new QLabel(
        tr("A unified chat client connecting to multiple decentralized "
           "networks from a single application. Built entirely in C++ with Qt6."), tab);
    detailLabel->setWordWrap(true);
    detailLabel->setStyleSheet(QStringLiteral("color: #666; font-size: 12px;"));
    layout->addWidget(detailLabel);

    layout->addSpacing(16);

    auto *linkLabel = new QLabel(tab);
    linkLabel->setText(QStringLiteral(
        "<a href='https://github.com/pcp-chat/concept' "
        "style='color: #2b579a;'>github.com/pcp-chat/concept</a>"));
    linkLabel->setTextFormat(Qt::RichText);
    linkLabel->setOpenExternalLinks(true);
    linkLabel->setTextInteractionFlags(Qt::LinksAccessibleByMouse | Qt::TextSelectableByMouse);
    layout->addWidget(linkLabel);

    layout->addStretch();
    return tab;
}

void SettingsDialog::loadSettings()
{
    QSettings settings(QStringLiteral("PCP"), QStringLiteral("PCP"));

    settings.beginGroup(QStringLiteral("General"));
    m_languageCombo->setCurrentIndex(
        settings.value(QStringLiteral("language"), 0).toInt());
    m_startMinimizedCheck->setChecked(
        settings.value(QStringLiteral("startMinimized"), false).toBool());
    m_closeToTrayCheck->setChecked(
        settings.value(QStringLiteral("closeToTray"), true).toBool());
    m_autoConnectCheck->setChecked(
        settings.value(QStringLiteral("autoConnect"), false).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("Appearance"));
    m_themeCombo->setCurrentIndex(
        settings.value(QStringLiteral("theme"), 2).toInt());
    int fontSize = settings.value(QStringLiteral("fontSize"), 14).toInt();
    m_fontSizeSlider->setValue(fontSize);
    m_bubbleStyleCombo->setCurrentIndex(
        settings.value(QStringLiteral("bubbleStyle"), 0).toInt());
    m_showAvatarsCheck->setChecked(
        settings.value(QStringLiteral("showAvatars"), true).toBool());
    m_showTimestampsCheck->setChecked(
        settings.value(QStringLiteral("showTimestamps"), true).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("Network"));
    int proxyType = settings.value(QStringLiteral("proxyType"), 0).toInt();
    m_proxyTypeCombo->setCurrentIndex(proxyType);
    m_proxyHostEdit->setText(
        settings.value(QStringLiteral("proxyHost")).toString());
    m_proxyPortSpin->setValue(
        settings.value(QStringLiteral("proxyPort"), 1080).toInt());
    m_proxyUserEdit->setText(
        settings.value(QStringLiteral("proxyUser")).toString());
    m_proxyPasswordEdit->setText(
        settings.value(QStringLiteral("proxyPassword")).toString());
    m_timeoutSpin->setValue(
        settings.value(QStringLiteral("timeout"), 30).toInt());
    m_yggdrasilCheck->setChecked(
        settings.value(QStringLiteral("yggdrasil"), false).toBool());
    m_i2pCheck->setChecked(
        settings.value(QStringLiteral("i2p"), false).toBool());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("Notifications"));
    bool notifyEnabled = settings.value(QStringLiteral("enabled"), true).toBool();
    m_enableNotificationsCheck->setChecked(notifyEnabled);
    m_showPreviewCheck->setChecked(
        settings.value(QStringLiteral("showPreview"), true).toBool());
    m_soundCheck->setChecked(
        settings.value(QStringLiteral("sound"), true).toBool());
    m_matrixNotifyCheck->setChecked(
        settings.value(QStringLiteral("matrix"), true).toBool());
    m_ircNotifyCheck->setChecked(
        settings.value(QStringLiteral("irc"), true).toBool());
    m_lemmyNotifyCheck->setChecked(
        settings.value(QStringLiteral("lemmy"), true).toBool());
    m_keywordsEdit->setPlainText(
        settings.value(QStringLiteral("keywords")).toString());
    m_quietFromEdit->setTime(
        settings.value(QStringLiteral("quietFrom"), QTime(22, 0)).toTime());
    m_quietToEdit->setTime(
        settings.value(QStringLiteral("quietTo"), QTime(8, 0)).toTime());
    settings.endGroup();

    m_showPreviewCheck->setEnabled(notifyEnabled);
    m_soundCheck->setEnabled(notifyEnabled);
    m_matrixNotifyCheck->setEnabled(notifyEnabled);
    m_ircNotifyCheck->setEnabled(notifyEnabled);
    m_lemmyNotifyCheck->setEnabled(notifyEnabled);
    m_keywordsEdit->setEnabled(notifyEnabled);
    m_quietFromEdit->setEnabled(notifyEnabled);
    m_quietToEdit->setEnabled(notifyEnabled);

    const bool proxyEnabled = (proxyType > 0);
    m_proxyHostEdit->setEnabled(proxyEnabled);
    m_proxyPortSpin->setEnabled(proxyEnabled);
    m_proxyUserEdit->setEnabled(proxyEnabled);
    m_proxyPasswordEdit->setEnabled(proxyEnabled);
}

void SettingsDialog::saveSettings()
{
    QSettings settings(QStringLiteral("PCP"), QStringLiteral("PCP"));

    settings.beginGroup(QStringLiteral("General"));
    settings.setValue(QStringLiteral("language"), m_languageCombo->currentIndex());
    settings.setValue(QStringLiteral("startMinimized"), m_startMinimizedCheck->isChecked());
    settings.setValue(QStringLiteral("closeToTray"), m_closeToTrayCheck->isChecked());
    settings.setValue(QStringLiteral("autoConnect"), m_autoConnectCheck->isChecked());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("Appearance"));
    settings.setValue(QStringLiteral("theme"), m_themeCombo->currentIndex());
    settings.setValue(QStringLiteral("fontSize"), m_fontSizeSlider->value());
    settings.setValue(QStringLiteral("bubbleStyle"), m_bubbleStyleCombo->currentIndex());
    settings.setValue(QStringLiteral("showAvatars"), m_showAvatarsCheck->isChecked());
    settings.setValue(QStringLiteral("showTimestamps"), m_showTimestampsCheck->isChecked());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("Network"));
    settings.setValue(QStringLiteral("proxyType"), m_proxyTypeCombo->currentIndex());
    settings.setValue(QStringLiteral("proxyHost"), m_proxyHostEdit->text());
    settings.setValue(QStringLiteral("proxyPort"), m_proxyPortSpin->value());
    settings.setValue(QStringLiteral("proxyUser"), m_proxyUserEdit->text());
    settings.setValue(QStringLiteral("proxyPassword"), m_proxyPasswordEdit->text());
    settings.setValue(QStringLiteral("timeout"), m_timeoutSpin->value());
    settings.setValue(QStringLiteral("yggdrasil"), m_yggdrasilCheck->isChecked());
    settings.setValue(QStringLiteral("i2p"), m_i2pCheck->isChecked());
    settings.endGroup();

    settings.beginGroup(QStringLiteral("Notifications"));
    settings.setValue(QStringLiteral("enabled"), m_enableNotificationsCheck->isChecked());
    settings.setValue(QStringLiteral("showPreview"), m_showPreviewCheck->isChecked());
    settings.setValue(QStringLiteral("sound"), m_soundCheck->isChecked());
    settings.setValue(QStringLiteral("matrix"), m_matrixNotifyCheck->isChecked());
    settings.setValue(QStringLiteral("irc"), m_ircNotifyCheck->isChecked());
    settings.setValue(QStringLiteral("lemmy"), m_lemmyNotifyCheck->isChecked());
    settings.setValue(QStringLiteral("keywords"), m_keywordsEdit->toPlainText());
    settings.setValue(QStringLiteral("quietFrom"), m_quietFromEdit->time());
    settings.setValue(QStringLiteral("quietTo"), m_quietToEdit->time());
    settings.endGroup();
}

void SettingsDialog::onAccepted()
{
    saveSettings();
    emit settingsChanged();
    accept();
}

void SettingsDialog::onApplied()
{
    saveSettings();
    emit settingsChanged();
}
