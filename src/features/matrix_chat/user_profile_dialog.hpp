#pragma once

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QTextEdit>

namespace pcp_chat {

class ProtocolManager;

class UserProfileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserProfileDialog(ProtocolManager *manager, QWidget *parent = nullptr);

private:
    void setupUi();
    void loadProfile();
    void saveProfile();
    void onChangeAvatar();

    ProtocolManager *m_protocolManager;
    QLabel *m_avatarLabel;
    QLabel *m_userIdLabel;
    QLineEdit *m_displayNameEdit;
    QTextEdit *m_statusEdit;
    QPushButton *m_saveBtn;
    QPushButton *m_changeAvatarBtn;
};

} // namespace pcp_chat
