#pragma once

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QCheckBox>
#include <QProgressBar>

namespace pcp_chat {

class ProtocolManager;

class MatrixLoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MatrixLoginWidget(ProtocolManager *manager, QWidget *parent = nullptr);

signals:
    void loginSuccess(const QString &userId, const QString &homeserver);
    void loginError(const QString &error);

private:
    void setupUi();
    void onLoginClicked();
    void setFormEnabled(bool enabled);

    ProtocolManager *m_protocolManager;
    QLineEdit *m_serverEdit;
    QLineEdit *m_userEdit;
    QLineEdit *m_passwordEdit;
    QCheckBox *m_rememberCheck;
    QPushButton *m_loginBtn;
    QLabel *m_errorLabel;
    QProgressBar *m_progressBar;
};

} // namespace pcp_chat
