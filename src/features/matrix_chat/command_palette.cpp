#include "command_palette.hpp"
#include "../../protocol/protocol_manager.hpp"

#include <QApplication>
#include <QKeyEvent>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

namespace pcp_chat {

CommandPalette::CommandPalette(ProtocolManager *manager, QWidget *parent)
    : QDialog(parent)
    , m_protocolManager(manager)
{
    setWindowTitle("Command Palette");
    setMinimumSize(500, 350);
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Dialog);

    setupUi();
    registerBuiltinCommands();
}

void CommandPalette::setupUi()
{
    auto *layout = new QVBoxLayout(this);

    m_searchInput = new QLineEdit();
    m_searchInput->setPlaceholderText("Type a command...");
    m_searchInput->setFont(QFont("", 14));
    m_searchInput->setStyleSheet("padding: 8px; border: 1px solid #ccc; border-radius: 4px;");
    connect(m_searchInput, &QLineEdit::textChanged, this, &CommandPalette::filterCommands);
    layout->addWidget(m_searchInput);

    m_commandList = new QListWidget();
    m_commandList->setFont(QFont("", 11));
    m_commandList->setStyleSheet("QListWidget { border: none; }");
    connect(m_commandList, &QListWidget::itemActivated, this, [this]() {
        executeSelected();
    });
    layout->addWidget(m_commandList);

    m_hintLabel = new QLabel("Press Enter to execute, Esc to close");
    m_hintLabel->setStyleSheet("color: #888; font-size: 11px; padding: 4px;");
    layout->addWidget(m_hintLabel);

    // Override key handling
    m_searchInput->installEventFilter(this);

    // Initial population
    filterCommands("");
    m_searchInput->setFocus();
}

void CommandPalette::registerBuiltinCommands()
{
    m_commands.append({"New Chat", "Start a new direct message", "Ctrl+N", []() {}});
    m_commands.append({"Join Room", "Join a room by ID or alias", "Ctrl+J", []() {}});
    m_commands.append({"Create Room", "Create a new Matrix room", "Ctrl+Shift+N", []() {}});
    m_commands.append({"Settings", "Open application settings", "Ctrl+,", []() {}});
    m_commands.append({"Toggle Dark Mode", "Switch between light and dark themes", "", []() {}});
    m_commands.append({"Toggle Compact Mode", "Switch layout density", "Ctrl+Shift+C", []() {}});
    m_commands.append({"Log Viewer", "Open log viewer", "", []() {}});
    m_commands.append({"Room Directory", "Browse public rooms", "", []() {}});
    m_commands.append({"User Directory", "Search for users", "Ctrl+Shift+U", []() {}});
    m_commands.append({"Spaces", "Explore spaces", "Ctrl+Shift+S", []() {}});
    m_commands.append({"Keyboard Shortcuts", "Show keyboard shortcuts reference", "Ctrl+/", []() {}});
    m_commands.append({"About", "About PCP", "", []() {}});
}

void CommandPalette::filterCommands(const QString &filter)
{
    m_commandList->clear();

    for (const auto &cmd : m_commands) {
        if (filter.isEmpty() ||
            cmd.name.contains(filter, Qt::CaseInsensitive) ||
            cmd.description.contains(filter, Qt::CaseInsensitive)) {
            QString label = cmd.name;
            if (!cmd.shortcut.isEmpty())
                label += "  [" + cmd.shortcut + "]";
            auto *item = new QListWidgetItem(label);
            item->setData(Qt::UserRole, cmd.name);
            m_commandList->addItem(item);
        }
    }

    if (m_commandList->count() > 0)
        m_commandList->setCurrentRow(0);
}

void CommandPalette::executeSelected()
{
    if (auto *item = m_commandList->currentItem()) {
        QString name = item->data(Qt::UserRole).toString();
        for (const auto &cmd : m_commands) {
            if (cmd.name == name && cmd.action) {
                cmd.action();
                break;
            }
        }
        accept();
    }
}

} // namespace pcp_chat
