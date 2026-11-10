#include "status_message_editor.hpp"
#include <QIcon>

namespace pcp_chat {

StatusMessageEditor::StatusMessageEditor(QWidget *parent) : QWidget(parent)
{
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    m_statusInput = new QLineEdit();
    m_statusInput->setPlaceholderText("Set status message...");
    m_statusInput->setFixedWidth(200);
    m_statusInput->setStyleSheet("border: none; color: #aaa; font-size: 11px;");
    layout->addWidget(m_statusInput);

    m_setBtn = new QPushButton("✓");
    m_setBtn->setFixedSize(24, 24);
    m_setBtn->setStyleSheet("border: none; color: #4CAF50;");
    layout->addWidget(m_setBtn);

    connect(m_setBtn, &QPushButton::clicked, this, [this]() {
        emit statusChanged(m_statusInput->text());
    });

    connect(m_statusInput, &QLineEdit::returnPressed, this, [this]() {
        emit statusChanged(m_statusInput->text());
    });
}

void StatusMessageEditor::setStatus(const QString &status)
{
    m_statusInput->setText(status);
}

} // namespace pcp_chat
