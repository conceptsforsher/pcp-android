#pragma once

#include <QWidget>
#include <QVector>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include <QMenu>
#include <QAction>
#include "protocol/protocol_type.hpp"

namespace pcp {

struct AccountEntry {
    ProtocolType protocolType = ProtocolType::MATRIX;
    QString accountId;
    QString displayName;
    QString serverName;
    QString avatarUrl;
    ConnectionState connectionState = ConnectionState::DISCONNECTED;
};

class AccountSwitcher : public QWidget
{
    Q_OBJECT

public:
    explicit AccountSwitcher(QWidget *parent = nullptr);
    ~AccountSwitcher() override;

    QVector<AccountEntry> accounts() const;
    AccountEntry currentAccount() const;
    void setCurrentAccount(const QString &accountId, ProtocolType type);

public slots:
    void refreshAccounts();
    void onConnectionStateChanged(ProtocolType type, ConnectionState state);

signals:
    void accountSwitched(ProtocolType type, QString accountId);
    void addAccountRequested(ProtocolType type);
    void logoutRequested(ProtocolType type, QString accountId);
    void profileEditRequested();

private slots:
    void onToggleClicked();
    void onAccountActionTriggered();
    void onAddAccountTriggered();
    void onLogoutRequested();
    void onSetStatusTriggered();
    void onEditProfileTriggered();
    void onViewProfileTriggered();

private:
    void setupUi();
    void rebuildMenu();
    QMenu *createProfileMenu();
    QAction *createAccountAction(const AccountEntry &entry);
    void updateToggleAppearance();

    static QColor statusDotColor(ConnectionState state);
    static QString protocolDisplayName(ProtocolType type);
    static QString protocolIconChar(ProtocolType type);
    static QColor protocolColor(ProtocolType type);
    static QString initialsFromName(const QString &name);
    static QPixmap makeAvatarPixmap(const QString &name, int size, const QColor &bg);

    QPushButton *m_toggleButton = nullptr;
    QMenu *m_menu = nullptr;
    QMenu *m_profileMenu = nullptr;

    QVector<AccountEntry> m_accounts;
    QString m_currentAccountId;
    ProtocolType m_currentProtocolType = ProtocolType::MATRIX;
};

} // namespace pcp
