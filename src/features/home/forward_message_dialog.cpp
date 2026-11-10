#include "features/home/forward_message_dialog.hpp"

#include <QCheckBox>
#include <QDateTime>
#include <QEventLoop>
#include <QFrame>
#include <QHBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QPushButton>
#include <QRegularExpression>
#include <QTextEdit>
#include <QTimer>
#include <QUrl>
#include <QVBoxLayout>

#include "app/application.hpp"
#include "irc/irc_session.hpp"
#include "irc/irc_session_holder.hpp"
#include "lemmy/lemmy_client.hpp"
#include "lemmy/lemmy_session.hpp"
#include "lemmy/lemmy_session_holder.hpp"
#include "matrix/matrix_session.hpp"
#include "protocol/protocol_manager.hpp"

namespace {

class RoomAvatarWidget : public QWidget
{
public:
    RoomAvatarWidget(const QString &name, const QColor &color, int size, QWidget *parent = nullptr)
        : QWidget(parent)
        , m_name(name)
        , m_color(color)
        , m_size(size)
    {
        setFixedSize(size, size);
    }

protected:
    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing);

        p.setBrush(m_color);
        p.setPen(Qt::NoPen);
        p.drawEllipse(rect().adjusted(1, 1, -1, -1));

        QFont font;
        font.setPixelSize(m_size / 3);
        font.setBold(true);
        p.setFont(font);
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, ForwardMessageDialog::initialsFromName(m_name));
    }

private:
    QString m_name;
    QColor m_color;
    int m_size;
};

class ProtocolBadgeWidget : public QWidget
{
public:
    ProtocolBadgeWidget(const QString &text, const QColor &color, QWidget *parent = nullptr)
        : QWidget(parent)
    {
        auto *layout = new QHBoxLayout(this);
        layout->setContentsMargins(0, 0, 0, 0);

        auto *label = new QLabel(text, this);
        label->setStyleSheet(
            QStringLiteral(
                "color: %1; font-size: 9px; font-weight: bold; "
                "background: %2; border-radius: 3px; padding: 1px 4px;")
                .arg(color.lighter(180).name(), color.darker(150).name()));
        layout->addWidget(label);
    }
};

} // anonymous namespace

ForwardMessageDialog::ForwardMessageDialog(const ProtocolMessage &message,
                                           QWidget *parent)
    : QDialog(parent)
    , m_message(message)
    , m_nam(new QNetworkAccessManager(this))
    , m_searchTimer(new QTimer(this))
{
    m_searchTimer->setSingleShot(true);
    m_searchTimer->setInterval(kSearchDebounceMs);
    connect(m_searchTimer, &QTimer::timeout, this, &ForwardMessageDialog::onSearchDebounceTimeout);

    // Determine the original source room
    ProtocolManager *pm = pcp::Application::instance().protocolManager();
    if (pm) {
        const auto allRooms = pm->getAllRooms();
        for (const auto &room : allRooms) {
            if (room.id == m_message.roomId) {
                m_originalSource = {room, room.name, room.protocolType};
                break;
            }
        }
    }

    setupUi();
    populateRooms();
}

void ForwardMessageDialog::setupUi()
{
    setWindowTitle(tr("Forward Message"));
    setMinimumSize(480, 560);
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(10);

    // ── Message preview ──────────────────────────────────────────────────────
    auto *previewHeader = new QLabel(tr("Message to forward:"), this);
    previewHeader->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 12px;"));
    mainLayout->addWidget(previewHeader);

    auto *previewFrame = new QFrame(this);
    previewFrame->setFrameShape(QFrame::StyledPanel);
    previewFrame->setStyleSheet(
        QStringLiteral("QFrame { background: #f3f4f6; border: 1px solid #d1d5db; "
                       "border-radius: 8px; padding: 8px; }"));
    auto *previewLayout = new QVBoxLayout(previewFrame);
    previewLayout->setContentsMargins(10, 8, 10, 8);
    previewLayout->setSpacing(4);

    QString senderText = m_message.senderName.isEmpty()
                             ? m_message.senderId
                             : m_message.senderName;
    auto *senderLabel = new QLabel(
        QStringLiteral("<b>%1</b>  "
                       "<span style='color:#6b7280; font-size:11px;'>%2</span>")
            .arg(senderText.toHtmlEscaped(),
                 formatTimestamp(m_message.timestamp)),
        previewFrame);
    senderLabel->setTextFormat(Qt::RichText);
    previewLayout->addWidget(senderLabel);

    QString content = m_message.text;
    if (content.length() > 200)
        content = content.left(200) + QStringLiteral("\u2026");

    m_previewLabel = new QLabel(content, previewFrame);
    m_previewLabel->setWordWrap(true);
    m_previewLabel->setMaximumHeight(36);
    m_previewLabel->setStyleSheet(QStringLiteral("color: #374151; font-size: 12px;"));
    previewLayout->addWidget(m_previewLabel);

    mainLayout->addWidget(previewFrame);

    // ── Search bar ───────────────────────────────────────────────────────────
    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search for destination room or user..."));
    m_searchEdit->setClearButtonEnabled(true);
    mainLayout->addWidget(m_searchEdit);

    connect(m_searchEdit, &QLineEdit::textChanged,
            this, &ForwardMessageDialog::onSearchTextChanged);

    // ── Destination list ─────────────────────────────────────────────────────
    auto *destHeader = new QLabel(tr("Select destinations:"), this);
    destHeader->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 12px;"));
    mainLayout->addWidget(destHeader);

    m_roomList = new QListWidget(this);
    m_roomList->setSelectionMode(QAbstractItemView::NoSelection);
    m_roomList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_roomList->setStyleSheet(
        QStringLiteral("QListWidget { border: 1px solid #d1d5db; border-radius: 6px; }"
                       "QListWidget::item { border-bottom: 1px solid #e5e7eb; "
                       "padding: 4px; }"));
    mainLayout->addWidget(m_roomList, 1);

    connect(m_roomList, &QListWidget::itemChanged, this, [this]() {
        int count = 0;
        for (int i = 0; i < m_roomList->count(); ++i) {
            auto *item = m_roomList->item(i);
            if (item && item->checkState() == Qt::Checked)
                ++count;
        }
        m_forwardButton->setEnabled(count > 0);
        m_forwardButton->setText(count > 0 ? tr("Forward (%1)").arg(count)
                                           : tr("Forward"));
    });

    // ── Add note ─────────────────────────────────────────────────────────────
    auto *noteHeader = new QLabel(tr("Add note (optional):"), this);
    noteHeader->setStyleSheet(QStringLiteral("font-weight: bold; font-size: 12px;"));
    mainLayout->addWidget(noteHeader);

    m_noteEdit = new QTextEdit(this);
    m_noteEdit->setPlaceholderText(
        tr("Add an optional message to include with the forwarded message..."));
    m_noteEdit->setMaximumHeight(70);
    m_noteEdit->setStyleSheet(
        QStringLiteral("QTextEdit { border: 1px solid #d1d5db; border-radius: 6px; "
                       "padding: 6px; }"));
    mainLayout->addWidget(m_noteEdit);

    // ── Status label ─────────────────────────────────────────────────────────
    m_statusLabel = new QLabel(this);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setVisible(false);
    m_statusLabel->setStyleSheet(
        QStringLiteral("color: #6b7280; font-size: 11px;"));
    mainLayout->addWidget(m_statusLabel);

    // ── Buttons ──────────────────────────────────────────────────────────────
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();

    m_cancelButton = new QPushButton(tr("Cancel"), this);
    m_forwardButton = new QPushButton(tr("Forward"), this);
    m_forwardButton->setDefault(true);
    m_forwardButton->setEnabled(false);

    m_forwardButton->setStyleSheet(
        QStringLiteral("QPushButton { background: #2563eb; color: white; "
                       "border: none; border-radius: 6px; padding: 6px 16px; "
                       "font-weight: bold; }"
                       "QPushButton:hover { background: #1d4ed8; }"
                       "QPushButton:disabled { background: #93c5fd; }"));

    buttonLayout->addWidget(m_forwardButton);
    buttonLayout->addWidget(m_cancelButton);
    mainLayout->addLayout(buttonLayout);

    connect(m_forwardButton, &QPushButton::clicked,
            this, &ForwardMessageDialog::onForwardClicked);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
}

void ForwardMessageDialog::populateRooms()
{
    ProtocolManager *pm = pcp::Application::instance().protocolManager();
    if (!pm)
        return;

    const auto allRooms = pm->getAllRooms();
    m_destinations.clear();
    m_recentRoomIds.clear();

    // Collect rooms except the source
    QVector<ProtocolRoom> candidateRooms;
    for (const auto &room : allRooms) {
        if (room.id != m_message.roomId)
            candidateRooms.append(room);
    }

    // Sort by lastMessageTimestamp descending
    std::sort(candidateRooms.begin(), candidateRooms.end(),
              [](const ProtocolRoom &a, const ProtocolRoom &b) {
                  return a.lastMessageTimestamp > b.lastMessageTimestamp;
              });

    const qsizetype count =
        std::min(candidateRooms.size(), static_cast<qsizetype>(kMaxRecentRooms));
    for (qsizetype i = 0; i < count; ++i) {
        const auto &room = candidateRooms.at(i);
        DestinationItem dest;
        dest.room = room;
        dest.name = room.name.isEmpty() ? room.id : room.name;
        dest.protocolType = room.protocolType;
        m_destinations.append(dest);
        m_recentRoomIds.append(room.id);
        addDestinationItem(dest);
    }

    if (m_destinations.isEmpty()) {
        m_statusLabel->setText(tr("No available rooms to forward to."));
        m_statusLabel->setVisible(true);
    }
}

void ForwardMessageDialog::filterRooms(const QString &query)
{
    m_roomList->blockSignals(true);
    m_roomList->clear();
    m_roomList->blockSignals(false);

    if (query.isEmpty()) {
        m_statusLabel->setVisible(false);
        for (const auto &dest : std::as_const(m_destinations))
            addDestinationItem(dest);
        return;
    }

    const QString lowerQuery = query.toLower();
    bool anyMatch = false;

    for (const auto &dest : std::as_const(m_destinations)) {
        if (dest.name.toLower().contains(lowerQuery) ||
            dest.room.id.toLower().contains(lowerQuery)) {
            addDestinationItem(dest);
            anyMatch = true;
        }
    }

    if (!anyMatch) {
        m_statusLabel->setText(tr("No rooms match \"%1\".").arg(query));
        m_statusLabel->setVisible(true);
    } else {
        m_statusLabel->setVisible(false);
    }
}

void ForwardMessageDialog::addDestinationItem(const DestinationItem &dest)
{
    auto *item = new QListWidgetItem();
    item->setData(Qt::UserRole, dest.room.id);
    item->setData(Qt::UserRole + 1, static_cast<int>(dest.protocolType));
    item->setData(Qt::UserRole + 2, dest.name);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Unchecked);
    item->setSizeHint(QSize(0, kAvatarSize + 12));

    auto *rowWidget = new QWidget(m_roomList);
    auto *rowLayout = new QHBoxLayout(rowWidget);
    rowLayout->setContentsMargins(8, 4, 8, 4);
    rowLayout->setSpacing(10);

    const QColor avatarColor = avatarColorFromName(dest.name);
    auto *avatar =
        new RoomAvatarWidget(dest.name, avatarColor, kAvatarSize, rowWidget);
    rowLayout->addWidget(avatar);

    auto *nameLabel = new QLabel(dest.name, rowWidget);
    nameLabel->setStyleSheet(
        QStringLiteral("font-weight: bold; font-size: 13px;"));
    rowLayout->addWidget(nameLabel, 1);

    auto *badge = new ProtocolBadgeWidget(protocolBadgeText(dest.protocolType),
                                          protocolBadgeColor(dest.protocolType),
                                          rowWidget);
    rowLayout->addWidget(badge);

    m_roomList->addItem(item);
    m_roomList->setItemWidget(item, rowWidget);
}

void ForwardMessageDialog::onSearchTextChanged(const QString & /*text*/)
{
    m_searchTimer->start();
}

void ForwardMessageDialog::onSearchDebounceTimeout()
{
    const QString query = m_searchEdit->text().trimmed();
    filterRooms(query);
}

void ForwardMessageDialog::onForwardClicked()
{
    // Collect selected destinations
    struct Selection {
        QString roomId;
        ProtocolType protocolType;
    };
    QVector<Selection> selected;

    for (int i = 0; i < m_roomList->count(); ++i) {
        auto *item = m_roomList->item(i);
        if (item && item->checkState() == Qt::Checked) {
            Selection sel;
            sel.roomId = item->data(Qt::UserRole).toString();
            sel.protocolType =
                static_cast<ProtocolType>(item->data(Qt::UserRole + 1).toInt());
            selected.append(sel);
        }
    }

    if (selected.isEmpty()) {
        QMessageBox::information(
            this, tr("No Selection"),
            tr("Please select at least one destination to forward to."));
        return;
    }

    QString note = m_noteEdit->toPlainText().trimmed();
    QString senderName = m_message.senderName.isEmpty()
                             ? m_message.senderId
                             : m_message.senderName;
    QString forwardText =
        QStringLiteral("[Forwarded from %1]: %2").arg(senderName, m_message.text);

    if (!note.isEmpty())
        forwardText = note + QStringLiteral("\n\n") + forwardText;

    m_forwardButton->setEnabled(false);
    m_statusLabel->setText(tr("Forwarding..."));
    m_statusLabel->setVisible(true);

    int successCount = 0;
    int failCount = 0;

    for (const auto &sel : selected) {
        bool ok = false;

        switch (sel.protocolType) {
        case ProtocolType::MATRIX: {
            auto *session =
                pcp::Application::instance().matrixSession();
            if (!session)
                break;

            QJsonObject content;
            content[QStringLiteral("msgtype")] = QStringLiteral("m.text");
            content[QStringLiteral("body")] = forwardText;

            QJsonObject forwardMeta;
            forwardMeta[QStringLiteral("original_sender")] =
                m_message.senderId;
            forwardMeta[QStringLiteral("original_sender_name")] =
                m_message.senderName;
            forwardMeta[QStringLiteral("original_room")] =
                m_message.roomId;
            forwardMeta[QStringLiteral("original_timestamp")] =
                m_message.timestamp;
            content[QStringLiteral("m.forwarded")] = forwardMeta;

            QJsonObject relatesTo;
            relatesTo[QStringLiteral("rel_type")] =
                QStringLiteral("m.forwarded");
            content[QStringLiteral("m.relates_to")] = relatesTo;

            const QString txnId =
                QString::number(QDateTime::currentMSecsSinceEpoch());
            const QString path =
                QStringLiteral(
                    "/_matrix/client/r0/rooms/%1/send/m.room.message/%2")
                    .arg(sel.roomId, txnId);

            QUrl url(session->homeserverUrl() + path);
            QNetworkRequest req(url);
            req.setHeader(QNetworkRequest::ContentTypeHeader,
                          QStringLiteral("application/json"));
            req.setRawHeader(
                "Authorization",
                ("Bearer " + session->accessToken()).toUtf8());

            QNetworkReply *reply = m_nam->put(
                req, QJsonDocument(content).toJson(QJsonDocument::Compact));

            // Block until this request completes
            QEventLoop loop;
            connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
            loop.exec();

            reply->deleteLater();
            ok = (reply->error() == QNetworkReply::NoError);
            break;
        }
        case ProtocolType::IRC: {
            auto *holder =
                pcp::Application::instance().ircSessionHolder();
            if (!holder)
                break;

            auto sessions = holder->getAllSessions();
            for (auto *irc : sessions) {
                if (irc->connectionState() == ConnectionState::REGISTERED) {
                    irc->sendMessage(sel.roomId, forwardText,
                                     ProtocolContentType::TEXT);
                    ok = true;
                    break;
                }
            }
            break;
        }
        case ProtocolType::LEMMY: {
            auto *holder =
                pcp::Application::instance().lemmySessionHolder();
            if (!holder)
                break;

            auto sessions = holder->getAllSessions();
            for (auto *lemmy : sessions) {
                if (lemmy->connectionState() == ConnectionState::REGISTERED) {
                    lemmy->sendMessage(sel.roomId, forwardText,
                                       ProtocolContentType::TEXT);
                    ok = true;
                    break;
                }
            }
            break;
        }
        default:
            break;
        }

        if (ok)
            ++successCount;
        else
            ++failCount;
    }

    m_forwardButton->setEnabled(true);
    m_statusLabel->setVisible(false);

    if (failCount == 0) {
        QMessageBox::information(
            this, tr("Message Forwarded"),
            tr("Message forwarded successfully to %n destination(s).", "",
               selected.size()));
        accept();
    } else if (successCount > 0) {
        QMessageBox::warning(
            this, tr("Partial Success"),
            tr("Forwarded to %1 room(s), %2 failed.").arg(successCount).arg(failCount));
        accept();
    } else {
        QMessageBox::critical(
            this, tr("Forward Failed"),
            tr("Could not forward to any of the selected destinations."));
    }
}

// ─── Static utility methods ──────────────────────────────────────────────────

QString ForwardMessageDialog::formatTimestamp(qint64 timestampMs)
{
    if (timestampMs <= 0)
        return {};

    qint64 ms = timestampMs;
    if (timestampMs < 1'000'000'000'000LL)
        ms = timestampMs * 1000;

    const QDateTime dt = QDateTime::fromMSecsSinceEpoch(ms);
    const QDate today = QDate::currentDate();
    if (dt.date() == today)
        return dt.toString(QStringLiteral("HH:mm"));

    if (dt.date().year() == today.year())
        return dt.toString(QStringLiteral("MMM d, HH:mm"));

    return dt.toString(QStringLiteral("MMM d yyyy, HH:mm"));
}

QString ForwardMessageDialog::initialsFromName(const QString &name)
{
    if (name.isEmpty())
        return QStringLiteral("?");

    const QStringList parts =
        name.split(QRegularExpression(QStringLiteral(R"(\s+)")),
                   Qt::SkipEmptyParts);
    if (parts.size() >= 2)
        return QString(parts.at(0).at(0).toUpper()) +
               parts.at(1).at(0).toUpper();

    const QString word = parts.at(0);
    if (word.length() >= 2)
        return QString(word.at(0).toUpper()) + word.at(1).toUpper();

    return QString(word.at(0).toUpper());
}

QColor ForwardMessageDialog::avatarColorFromName(const QString &name)
{
    if (name.isEmpty())
        return QColor(0x6B, 0x72, 0x80);

    const uint hash = qHash(name);
    const int hue = static_cast<int>(hash % 360);
    return QColor::fromHsl(hue, 180, 160);
}

QString ForwardMessageDialog::protocolBadgeText(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:
        return QStringLiteral("Matrix");
    case ProtocolType::IRC:
        return QStringLiteral("IRC");
    case ProtocolType::LEMMY:
        return QStringLiteral("Lemmy");
    case ProtocolType::TELEGRAM:
        return QStringLiteral("Telegram");
    case ProtocolType::DELTACHAT:
        return QStringLiteral("DeltaChat");
    }
    return QStringLiteral("Unknown");
}

QColor ForwardMessageDialog::protocolBadgeColor(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:
        return QColor(0x10, 0xb9, 0x81);
    case ProtocolType::IRC:
        return QColor(0x63, 0x66, 0xf1);
    case ProtocolType::LEMMY:
        return QColor(0xf5, 0x9e, 0x0b);
    case ProtocolType::TELEGRAM:
        return QColor(0x0e, 0xa5, 0xe9);
    case ProtocolType::DELTACHAT:
        return QColor(0x8b, 0x5c, 0xf6);
    }
    return QColor(0x6b, 0x72, 0x80);
}

#include "forward_message_dialog.moc"
