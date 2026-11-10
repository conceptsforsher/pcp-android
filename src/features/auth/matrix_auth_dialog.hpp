#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QTabWidget>
#include <QGroupBox>
#include <QProgressBar>
#include <QStackedWidget>

namespace pcp_chat {

class ProtocolManager;

class MatrixAuthDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MatrixAuthDialog(ProtocolManager *manager, QWidget *parent = nullptr);

private slots:
    void onLoginClicked();
    void onRegisterClicked();
    void onSsoLoginClicked();
    void onServerDiscovery();
    void onLoginSuccess(const QString &userId, const QString &server);
    void onLoginError(const QString &error);
    void onRegistrationSuccess();

private:
    void setupLoginTab();
    void setupRegisterTab();
    void setupSsoTab();
    void showError(const QString &message);
    void showProgress(bool show);
    void setFormEnabled(bool enabled);

    ProtocolManager *m_protocolManager;

    QTabWidget *m_tabWidget;
    QStackedWidget *m_stack;

    // Login fields
    QLineEdit *m_loginServer;
    QLineEdit *m_loginUser;
    QLineEdit *m_loginPassword;
    QCheckBox *m_loginRemember;
    QPushButton *m_loginBtn;
    QLabel *m_loginError;
    QProgressBar *m_loginProgress;

    // Register fields
    QLineEdit *m_regServer;
    QLineEdit *m_regUser;
    QLineEdit *m_regPassword;
    QLineEdit *m_regConfirm;
    QLineEdit *m_regEmail;
    QPushButton *m_registerBtn;
    QLabel *m_regError;

    // SSO fields
    QLineEdit *m_ssoServer;
    QPushButton *m_ssoStartBtn;
    QLabel *m_ssoStatus;
};

} // namespace pcp_chat
