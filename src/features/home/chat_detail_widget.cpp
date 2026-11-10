#include "chat_detail_widget.hpp"
#include "../../protocol/protocol_manager.hpp"

#include <QDateTime>
#include <QScrollBar>
#include <QFileDialog>
#include <QFont>
#include <QStyle>

namespace pcp_chat {

ChatDetailWidget::ChatDetailWidget(QWidget *parent) : QWidget(parent)
{
    setupUi();
}

void ChatDetailWidget::setupUi()
{
    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    // Room header
    auto *headerLayout = new QHBoxLayout();

    m_roomNameLabel = new QLabel("Select a room");
    QFont nameFont;
    nameFont.setPixelSize(16);
    nameFont.setBold(true);
    m_roomNameLabel->setFont(nameFont);
    headerLayout->addWidget(m_roomNameLabel);

    headerLayout->addStretch();

    m_infoBtn = new QPushButton("ℹ");
    m_infoBtn->setFixedSize(32, 32);
    m_infoBtn->setToolTip("Room info");
    connect(m_infoBtn, &QPushButton::clicked, this, &ChatDetailWidget::onRoomInfoClicked);
    headerLayout->addWidget(m_infoBtn);

    layout->addLayout(headerLayout);

    // Topic
    m_roomTopicLabel = new QLabel();
    m_roomTopicLabel->setStyleSheet("color: #888; font-size: 12px; padding-left: 4px;");
    m_roomTopicLabel->setWordWrap(true);
    layout->addWidget(m_roomTopicLabel);

    // Message view
    m_messageView = new QTextBrowser();
    m_messageView->setOpenExternalLinks(true);
    m_messageView->setReadOnly(true);
    layout->addWidget(m_messageView);

    // Typing indicator
    m_typingLabel = new QLabel();
    m_typingLabel->setStyleSheet("color: #4CAF50; font-style: italic; font-size: 12px; padding-left: 4px;");
    layout->addWidget(m_typingLabel);

    // Input bar
    auto *inputLayout = new QHBoxLayout();

    m_attachBtn = new QPushButton("📎");
    m_attachBtn->setFixedSize(36, 36);
    m_attachBtn->setToolTip("Attach file");
    connect(m_attachBtn, &QPushButton::clicked, this, &ChatDetailWidget::onAttachmentClicked);
    inputLayout->addWidget(m_attachBtn);

    m_messageInput = new QLineEdit();
    m_messageInput->setPlaceholderText("Type a message...");
    m_messageInput->setMinimumHeight(36);
    connect(m_messageInput, &QLineEdit::returnPressed, this, &ChatDetailWidget::onInputReturnPressed);
    inputLayout->addWidget(m_messageInput);

    m_emojiBtn = new QPushButton("😊");
    m_emojiBtn->setFixedSize(36, 36);
    m_emojiBtn->setToolTip("Emoji picker");
    connect(m_emojiBtn, &QPushButton::clicked, this, &ChatDetailWidget::onEmojiClicked);
    inputLayout->addWidget(m_emojiBtn);

    m_sendBtn = new QPushButton("Send");
    m_sendBtn->setFixedHeight(36);
    m_sendBtn->setStyleSheet(
        "QPushButton { background-color: #4CAF50; color: white; border: none; "
        "border-radius: 6px; padding: 4px 16px; font-weight: bold; }"
        "QPushButton:hover { background-color: #45a049; }");
    connect(m_sendBtn, &QPushButton::clicked, this, &ChatDetailWidget::onSendClicked);
    inputLayout->addWidget(m_sendBtn);

    layout->addLayout(inputLayout);
}

void ChatDetailWidget::setProtocolManager(ProtocolManager *manager)
{
    m_protocolManager = manager;
}

bool ChatDetailWidget::hasActiveSearch() const { return false; }
void ChatDetailWidget::clearFocus() { m_messageInput->clearFocus(); }

void ChatDetailWidget::loadRoom(const QString &roomId)
{
    m_currentRoomId = roomId;
    m_messageView->clear();
    m_messageInput->setEnabled(true);
    m_messageInput->setFocus();
}

void ChatDetailWidget::addMessage(const QString &sender, const QString &body,
                                    const QString &timestamp, bool isOwn)
{
    QString html = QString(
        "<div style='margin: 4px 0;'>"
        "<b style='color: %3;'>%1</b> "
        "<span style='color: #999; font-size: 10px;'>%4</span><br>"
        "<span style='color: %5;'>%2</span>"
        "</div>")
        .arg(sender.toHtmlEscaped(),
             body.toHtmlEscaped(),
             isOwn ? "#4CAF50" : "#2196F3",
             timestamp,
             isOwn ? "#e8e8e8" : "#ffffff");

    m_messageView->append(html);

    // Auto-scroll to bottom
    QScrollBar *bar = m_messageView->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDetailWidget::showSystemMessage(const QString &text)
{
    m_messageView->append(
        QString("<div style='color: #888; font-style: italic; text-align: center; "
                "margin: 8px 0;'>%1</div>").arg(text.toHtmlEscaped()));
}

void ChatDetailWidget::showTypingIndicator(const QString &userName)
{
    m_typingLabel->setText(userName + " is typing...");
}

void ChatDetailWidget::hideTypingIndicator()
{
    m_typingLabel->clear();
}

void ChatDetailWidget::setRoomTopic(const QString &topic)
{
    m_roomTopicLabel->setText(topic);
}

void ChatDetailWidget::setRoomName(const QString &name)
{
    m_roomNameLabel->setText(name);
}

void ChatDetailWidget::onSendClicked()
{
    QString text = m_messageInput->text().trimmed();
    if (text.isEmpty() || m_currentRoomId.isEmpty()) return;

    emit messageSent(m_currentRoomId, text);
    addMessage("You", text,
               QDateTime::currentDateTime().toString("hh:mm"), true);
    m_messageInput->clear();
}

void ChatDetailWidget::onInputReturnPressed()
{
    onSendClicked();
}

void ChatDetailWidget::onAttachmentClicked()
{
    emit fileAttachmentRequested();
}

void ChatDetailWidget::onEmojiClicked()
{
    emit emojiPickerRequested();
}

void ChatDetailWidget::onRoomInfoClicked()
{
    emit roomInfoRequested();
}

} // namespace pcp_chat
