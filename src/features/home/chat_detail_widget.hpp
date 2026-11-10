#pragma once

#include <QWidget>
#include <QTextBrowser>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QSplitter>

namespace pcp_chat {

class ProtocolManager;

class ChatDetailWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ChatDetailWidget(QWidget *parent = nullptr);
    void setProtocolManager(ProtocolManager *manager);

    bool hasActiveSearch() const;
    void clearFocus();

public slots:
    void loadRoom(const QString &roomId);
    void addMessage(const QString &sender, const QString &body,
                    const QString &timestamp, bool isOwn = false);
    void showSystemMessage(const QString &text);
    void showTypingIndicator(const QString &userName);
    void hideTypingIndicator();
    void setRoomTopic(const QString &topic);
    void setRoomName(const QString &name);

signals:
    void messageSent(const QString &roomId, const QString &body);
    void fileAttachmentRequested();
    void emojiPickerRequested();
    void roomInfoRequested();

private:
    void setupUi();
    void onSendClicked();
    void onInputReturnPressed();
    void onAttachmentClicked();
    void onEmojiClicked();
    void onRoomInfoClicked();

    QLabel *m_roomNameLabel;
    QLabel *m_roomTopicLabel;
    QLabel *m_typingLabel;
    QTextBrowser *m_messageView;
    QLineEdit *m_messageInput;
    QPushButton *m_sendBtn;
    QPushButton *m_attachBtn;
    QPushButton *m_emojiBtn;
    QPushButton *m_infoBtn;
    QString m_currentRoomId;
    ProtocolManager *m_protocolManager = nullptr;
};

} // namespace pcp_chat
