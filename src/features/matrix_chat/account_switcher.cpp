#include "account_switcher.hpp"
#include "../../protocol/protocol_manager.hpp"
#include "../../protocol/protocol_session.hpp"

#include <QMessageBox>
#include <QLabel>

namespace pcp_chat {

AccountSwitcher::AccountSwitcher(ProtocolManager *manager, QWidget *parent)
    : QDialog(parent)
    , m_protocolManager(manager)
{
    setWindowTitle("Switch Account");
    setMinimumSize(400, 300);
    setupUi();
    refreshAccounts();
}

void AccountSwitcher::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    m_infoLabel = new QLabel("Select an account to switch to:");
    layout->addWidget(m_infoLabel);

    m_accountList = new QListWidget();
    m_accountList->setIconSize(QSize(40, 40));
    connect(m_accountList, &QListWidget::currentRowChanged,
            this, &AccountSwitcher::onAccountClicked);
    layout->addWidget(m_accountList);

    auto *btnLayout = new QHBoxLayout();
    m_addBtn = new QPushButton("Add Account");
    connect(m_addBtn, &QPushButton::clicked, this, &AccountSwitcher::onAddAccount);

    m_removeBtn = new QPushButton("Remove");
    connect(m_removeBtn, &QPushButton::clicked, this, &AccountSwitcher::onRemoveAccount);

    m_switchBtn = new QPushButton("Switch");
    m_switchBtn->setEnabled(false);
    connect(m_switchBtn, &QPushButton::clicked, this, [this]() {
        if (m_accountList->currentRow() >= 0) {
            accept();
        }
    });

    btnLayout->addWidget(m_addBtn);
    btnLayout->addWidget(m_removeBtn);
    btnLayout->addStretch();
    btnLayout->addWidget(m_switchBtn);
    layout->addLayout(btnLayout);

    auto *cancelBtn = new QPushButton("Cancel");
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    layout->addWidget(cancelBtn);
}

void AccountSwitcher::refreshAccounts()
{
    m_accountList->clear();

    auto sessions = m_protocolManager->allSessions();
    for (auto *session : sessions) {
        QString label = QString("[%1] %2 @ %3
(%4 rooms)")
            .arg(protocolTypeToString(session->protocolType()),
                 session->displayName(),
                 session->sessionId(),
                 QString::number(session->rooms().size()));

        auto *item = new QListWidgetItem(label);
        item->setData(Qt::UserRole, session->sessionId());
        m_accountList->addItem(item);
    }
}

void AccountSwitcher::onAccountClicked(int row)
{
    m_switchBtn->setEnabled(row >= 0);
    m_removeBtn->setEnabled(row >= 0);
}

void AccountSwitcher::onAddAccount()
{
    emit addAccountRequested();
}

void AccountSwitcher::onRemoveAccount()
{
    if (m_accountList->currentRow() < 0) return;

    auto *item = m_accountList->currentItem();
    QString sessionId = item->data(Qt::UserRole).toString();

    auto reply = QMessageBox::question(this, "Remove Account",
        "Are you sure you want to remove this account?",
        QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_protocolManager->removeSession(sessionId);
        refreshAccounts();
    }
}

} // namespace pcp_chat
