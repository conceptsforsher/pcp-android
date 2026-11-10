#include "features/home/message_context_menu.hpp"

#include <QApplication>
#include <QClipboard>
#include <QDialog>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QMenu>
#include <QMessageBox>
#include <QRegularExpression>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QWidget>

#include "app/application.hpp"
#include "irc/irc_session.hpp"
#include "irc/irc_session_holder.hpp"
#include "lemmy/lemmy_session.hpp"
#include "lemmy/lemmy_session_holder.hpp"
#include "matrix/matrix_session.hpp"

// ─── Static factory ──────────────────────────────────────────────────────────

QMenu *MessageContextMenu::create(const ProtocolMessage &msg, QWidget *parent)
{
    auto *ctx = new MessageContextMenu(parent);
    ctx->m_message = msg;

    auto *menu = new QMenu(parent);
    menu->setStyleSheet(
        QStringLiteral("QMenu { background: #ffffff; border: 1px solid #d1d5db; "
                       "border-radius: 8px; padding: 4px; }"
                       "QMenu::item { padding: 8px 32px 8px 12px; "
                       "border-radius: 4px; }"
                       "QMenu::item:selected { background: #f3f4f6; }"
                       "QMenu::separator { height: 1px; background: #e5e7eb; "
                       "margin: 4px 8px; }"));

    QObject::connect(menu, &QMenu::destroyed, ctx, &QObject::deleteLater);

    ctx->populateMenu(menu, msg);

    return menu;
}

// ─── Constructor ─────────────────────────────────────────────────────────────

MessageContextMenu::MessageContextMenu(QObject *parent)
    : QObject(parent)
{
}

// ─── Menu population ─────────────────────────────────────────────────────────

void MessageContextMenu::populateMenu(QMenu *menu, const ProtocolMessage &msg)
{
    const QString msgId = messageIdString(msg);

    // ── Copy Text ────────────────────────────────────────────────────────────
    if (!msg.text.isEmpty()) {
        auto *copyText = menu->addAction(tr("Copy Text"));
        connect(copyText, &QAction::triggered, this, [this, msg]() {
            copyToClipboard(msg.text);
        });
    }

    // ── Copy Link ────────────────────────────────────────────────────────────
    if (messageContainsUrl(msg.text)) {
        auto *copyLink = menu->addAction(tr("Copy Link"));
        const QString link = extractUrl(msg.text);
        connect(copyLink, &QAction::triggered, this, [link]() {
            copyToClipboard(link);
        });
    }

    menu->addSeparator();

    // ── Reply ────────────────────────────────────────────────────────────────
    auto *replyAction = menu->addAction(tr("Reply"));
    connect(replyAction, &QAction::triggered, this,
            [this, msgId]() { emit replyRequested(msgId); });

    // ── Forward ──────────────────────────────────────────────────────────────
    auto *forwardAction = menu->addAction(tr("Forward"));
    connect(forwardAction, &QAction::triggered, this,
            [this, msgId]() { emit forwardRequested(msgId); });

    // ── React ────────────────────────────────────────────────────────────────
    if (protocolSupportsReact(msg.protocolType)) {
        auto *reactAction = menu->addAction(tr("React"));
        connect(reactAction, &QAction::triggered, this,
                [this, msgId]() { emit reactRequested(msgId); });
    }

    menu->addSeparator();

    // ── Edit ─────────────────────────────────────────────────────────────────
    if (isOwnMessage(msg) && protocolSupportsEdit(msg.protocolType)) {
        auto *editAction = menu->addAction(tr("Edit"));
        connect(editAction, &QAction::triggered, this,
                [this, msgId]() { emit editRequested(msgId); });
    }

    // ── Delete / Redact ──────────────────────────────────────────────────────
    if (isOwnMessage(msg) && protocolSupportsDelete(msg.protocolType)) {
        auto *deleteAction = menu->addAction(tr("Delete / Redact"));
        connect(deleteAction, &QAction::triggered, this, [this, msgId, menu]() {
            auto *parentWidget = qobject_cast<QWidget *>(menu->parent());
            const int ret = QMessageBox::question(
                parentWidget,
                tr("Confirm Deletion"),
                tr("Are you sure you want to delete this message?\n\n"
                   "This action cannot be undone."),
                QMessageBox::Yes | QMessageBox::No,
                QMessageBox::No);

            if (ret == QMessageBox::Yes)
                emit deleteRequested(msgId);
        });
    }

    menu->addSeparator();

    // ── Pin / Unpin ──────────────────────────────────────────────────────────
    const bool isPinned = msg.metadata.value(QStringLiteral("pinned")).toBool(false);
    if (isPinned) {
        auto *unpinAction = menu->addAction(tr("Unpin"));
        connect(unpinAction, &QAction::triggered, this,
                [this, msgId]() { emit unpinRequested(msgId); });
    } else {
        auto *pinAction = menu->addAction(tr("Pin"));
        connect(pinAction, &QAction::triggered, this,
                [this, msgId]() { emit pinRequested(msgId); });
    }

    // ── Mark as Read until here ──────────────────────────────────────────────
    if (!msg.roomId.isEmpty()) {
        auto *markReadAction = menu->addAction(tr("Mark as Read until here"));
        connect(markReadAction, &QAction::triggered, this,
                [this, msg]() {
                    // Ask the protocol session to mark room as read up to this point
                    pcp::Application &app = pcp::Application::instance();
                    switch (msg.protocolType) {
                    case ProtocolType::MATRIX:
                        if (auto *mx = app.matrixSession())
                            mx->markAsRead(msg.roomId);
                        break;
                    case ProtocolType::IRC:
                        if (auto *ircHolder = app.ircSessionHolder()) {
                            for (auto *irc : ircHolder->getAllSessions())
                                irc->markAsRead(msg.roomId);
                        }
                        break;
                    case ProtocolType::LEMMY:
                        if (auto *lemmyHolder = app.lemmySessionHolder()) {
                            for (auto *lemmy : lemmyHolder->getAllSessions())
                                lemmy->markAsRead(msg.roomId);
                        }
                        break;
                    default:
                        break;
                    }
                });
    }

    menu->addSeparator();

    // ── Permalink ────────────────────────────────────────────────────────────
    if (protocolSupportsPermalink(msg.protocolType)) {
        auto *permalinkAction = menu->addAction(tr("Copy Permalink"));
        connect(permalinkAction, &QAction::triggered, this, [msg]() {
            QString permalink;
            switch (msg.protocolType) {
            case ProtocolType::MATRIX: {
                auto *mx = pcp::Application::instance().matrixSession();
                if (mx) {
                    permalink = QStringLiteral("%1/_matrix/client/r0/rooms/%2/%3")
                        .arg(mx->homeserverUrl(), msg.roomId,
                             QString::number(msg.id));
                }
                break;
            }
            case ProtocolType::IRC: {
                permalink = QStringLiteral("irc://%1/%2").arg(msg.roomId, msg.senderId);
                break;
            }
            case ProtocolType::LEMMY: {
                auto *lemmyHolder = pcp::Application::instance().lemmySessionHolder();
                if (lemmyHolder) {
                    const auto sessions = lemmyHolder->getAllSessions();
                    if (!sessions.isEmpty()) {
                        permalink = QStringLiteral("%1/post/%2")
                            .arg(sessions.at(0)->userId(), QString::number(msg.id));
                    }
                }
                break;
            }
            default:
                break;
            }
            copyToClipboard(permalink);
        });
    }

    // ── View Source ──────────────────────────────────────────────────────────
    auto *viewSourceAction = menu->addAction(tr("View Source"));
    connect(viewSourceAction, &QAction::triggered, this, [msg, menu]() {
        auto *parentWidget = qobject_cast<QWidget *>(menu->parent()) ?
            qobject_cast<QWidget *>(menu->parent()) : menu;

        QJsonObject source;
        source[QStringLiteral("id")] = msg.id;
        source[QStringLiteral("sender_id")] = msg.senderId;
        source[QStringLiteral("sender_name")] = msg.senderName;
        source[QStringLiteral("text")] = msg.text;
        source[QStringLiteral("room_id")] = msg.roomId;
        source[QStringLiteral("timestamp")] = msg.timestamp;
        source[QStringLiteral("content_type")] = static_cast<int>(msg.contentType);
        source[QStringLiteral("is_edited")] = msg.isEdited;
        source[QStringLiteral("is_encrypted")] = msg.isEncrypted;
        source[QStringLiteral("protocol")] = Protocol::protocolTypeToString(msg.protocolType);
        source[QStringLiteral("metadata")] = msg.metadata;

        QDialog *dialog = new QDialog(parentWidget);
        dialog->setWindowTitle(tr("Message Source"));
        dialog->resize(500, 400);
        dialog->setModal(true);

        auto *layout = new QVBoxLayout(dialog);

        auto *label = new QLabel(tr("Raw message data:"), dialog);
        label->setStyleSheet(QStringLiteral("font-weight: bold;"));
        layout->addWidget(label);

        auto *textEdit = new QTextEdit(dialog);
        textEdit->setReadOnly(true);
        textEdit->setFont(QFont(QStringLiteral("monospace"), 10));
        textEdit->setPlainText(
            QString::fromUtf8(QJsonDocument(source).toJson(QJsonDocument::Indented)));
        layout->addWidget(textEdit);

        auto *buttonBox = new QDialogButtonBox(QDialogButtonBox::Close, dialog);
        connect(buttonBox, &QDialogButtonBox::rejected, dialog, &QDialog::reject);
        layout->addWidget(buttonBox);

        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->open();
    });

    // ── Report ───────────────────────────────────────────────────────────────
    auto *reportAction = menu->addAction(tr("Report"));
    connect(reportAction, &QAction::triggered, this, [this, msgId, menu]() {
        auto *parentWidget = qobject_cast<QWidget *>(menu->parent());

        QStringList reasons = {
            tr("Spam"),
            tr("Harassment"),
            tr("Violence"),
            tr("Hate Speech"),
            tr("NSFW Content"),
            tr("Impersonation"),
            tr("Other"),
        };

        bool ok = false;
        const QString reason = QInputDialog::getItem(
            parentWidget, tr("Report Message"),
            tr("Select reason for reporting:"),
            reasons, 0, false, &ok);

        if (ok && !reason.isEmpty()) {
            emit reportRequested(msgId);
            // Store report reason in application settings or trigger report API
            Q_UNUSED(reason);
        }
    });
}

// ─── Static helpers ──────────────────────────────────────────────────────────

bool MessageContextMenu::messageContainsUrl(const QString &text)
{
    static const QRegularExpression urlRe(
        QStringLiteral(R"(https?://[^\s<>"{}|\\^`\[\]]+)"),
        QRegularExpression::CaseInsensitiveOption);
    return urlRe.match(text).hasMatch();
}

QString MessageContextMenu::extractUrl(const QString &text)
{
    static const QRegularExpression urlRe(
        QStringLiteral(R"(https?://[^\s<>"{}|\\^`\[\]]+)"),
        QRegularExpression::CaseInsensitiveOption);
    auto match = urlRe.match(text);
    return match.hasMatch() ? match.captured(0) : QString();
}

bool MessageContextMenu::isOwnMessage(const ProtocolMessage &msg)
{
    const pcp::Application &app = pcp::Application::instance();

    switch (msg.protocolType) {
    case ProtocolType::MATRIX:
        if (auto *mx = app.matrixSession())
            return msg.senderId == mx->userId();
        break;
    case ProtocolType::IRC:
        if (auto *ircHolder = app.ircSessionHolder()) {
            const auto sessions = ircHolder->getAllSessions();
            for (auto *irc : sessions) {
                if (msg.senderId.contains(irc->userId().split(QLatin1Char('!')).value(0)))
                    return true;
            }
        }
        break;
    case ProtocolType::LEMMY:
        if (auto *lemmyHolder = app.lemmySessionHolder()) {
            const auto sessions = lemmyHolder->getAllSessions();
            for (auto *lemmy : sessions) {
                if (msg.senderId == lemmy->userId())
                    return true;
            }
        }
        break;
    default:
        break;
    }
    return false;
}

bool MessageContextMenu::protocolSupportsEdit(ProtocolType type)
{
    return type == ProtocolType::MATRIX || type == ProtocolType::LEMMY;
}

bool MessageContextMenu::protocolSupportsDelete(ProtocolType type)
{
    // Matrix redaction, Lemmy delete, and IRC (no delete of sent messages in IRC)
    return type == ProtocolType::MATRIX || type == ProtocolType::LEMMY;
}

bool MessageContextMenu::protocolSupportsReact(ProtocolType type)
{
    return type == ProtocolType::MATRIX;  // Matrix has native reactions API
}

bool MessageContextMenu::protocolSupportsPermalink(ProtocolType type)
{
    return type == ProtocolType::MATRIX || type == ProtocolType::IRC || type == ProtocolType::LEMMY;
}

QString MessageContextMenu::messageIdString(const ProtocolMessage &msg)
{
    return QString::number(msg.id);
}

void MessageContextMenu::copyToClipboard(const QString &text)
{
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(text);
}

#include "message_context_menu.moc"
