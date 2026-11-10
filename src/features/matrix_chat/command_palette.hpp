#pragma once

#include <QDialog>
#include <QLineEdit>
#include <QListWidget>
#include <QLabel>
#include <QVBoxLayout>

namespace pcp_chat {

class ProtocolManager;

class CommandPalette : public QDialog
{
    Q_OBJECT

public:
    explicit CommandPalette(ProtocolManager *manager, QWidget *parent = nullptr);

    struct Command {
        QString name;
        QString description;
        QString shortcut;
        std::function<void()> action;
    };

private:
    void setupUi();
    void registerBuiltinCommands();
    void filterCommands(const QString &filter);
    void executeSelected();

    ProtocolManager *m_protocolManager;
    QLineEdit *m_searchInput;
    QListWidget *m_commandList;
    QLabel *m_hintLabel;
    QList<Command> m_commands;
};

} // namespace pcp_chat
