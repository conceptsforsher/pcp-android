#pragma once

#include <QDialog>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>

namespace pcp_chat {

class ProtocolManager;

class AccountSwitcher : public QDialog
{
    Q_OBJECT

public:
    explicit AccountSwitcher(ProtocolManager *manager, QWidget *parent = nullptr);

signals:
    void accountSelected(const QString &sessionId);
    void addAccountRequested();
    void accountRemoved(const QString &sessionId);

private:
    void refreshAccounts();
    void setupUi();
    void onAccountClicked(int row);
    void onAddAccount();
    void onRemoveAccount();

    ProtocolManager *m_protocolManager;
    QListWidget *m_accountList;
    QPushButton *m_addBtn;
    QPushButton *m_removeBtn;
    QPushButton *m_switchBtn;
    QLabel *m_infoLabel;
};

} // namespace pcp_chat
