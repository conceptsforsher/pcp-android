#include "features/home/pinned_messages_widget.hpp"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QPainter>
#include <QPushButton>
#include <QScrollArea>
#include <QTimer>
#include <QUrl>
#include <QFont>

#include "app/application.hpp"
#include "matrix/matrix_session.hpp"
#include "util/json_util.hpp"

namespace {

constexpr int kAvatarSize = 28;

const char *kDarkStyle = R"(
    PinnedMessagesWidget {
        background-color: #1e1e2e;
    }
    PinnedMessagesWidget QListWidget {
        background-color: #1e1e2e;
        color: #cdd6f4;
        border: none;
        outline: none;
    }
    PinnedMessagesWidget QListWidget::item {
        background-color: #1e1e2e;
        border-bottom: 1px solid #313244;
        padding: 4px 8px;
    }
    PinnedMessagesWidget QListWidget::item:hover {
        background-color: #2a2a3c;
    }
    PinnedMessagesWidget QListWidget::item:selected {
        background-color: #45475a;
    }
    PinnedMessagesWidget QLabel {
        background: transparent;
    }
    PinnedMessagesWidget QPushButton {
        background-color: #313244;
        color: #cdd6f4;
        border: 1px solid #45475a;
        border-radius: 6px;
        padding: 6px 12px;
        font-size: 12px;
    }
    PinnedMessagesWidget QPushButton:hover {
        background-color: #45475a;
        border-color: #89b4fa;
    }
)";

} // anonymous namespace

QString PinnedMessagesWidget::initialsFromName(const QString &name)
{
    if (name.isEmpty())
        return QStringLiteral("?");

    const QStringList parts = name.split(QChar::Space, Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return QStringLiteral("?");

    QString result;
    result += parts.first().at(0).toUpper();
    if (parts.size() >= 2)
        result += parts.last().at(0).toUpper();

    return result;
}

QColor PinnedMessagesWidget::colorFromName(const QString &name)
{
    if (name.isEmpty())
        return QColor(0x6B, 0x72, 0x80);

    const uint hash = qHash(name);
    const int hue = static_cast<int>(hash % 360);
    return QColor::fromHsl(hue, 180, 160);
}

QPixmap PinnedMessagesWidget::makeAvatarPixmap(const QString &name, int size, const QColor &bgColor)
{
    QPixmap pix(size, size);
    pix.fill(Qt::transparent);

    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    QRect circleRect(0, 0, size, size);
    painter.setPen(Qt::NoPen);
    painter.setBrush(bgColor);
    painter.drawEllipse(circleRect);

    QString letters = initialsFromName(name);

    QFont font;
    font.setPixelSize(size / 2);
    font.setBold(true);
    painter.setFont(font);
    painter.setPen(Qt::white);
    painter.drawText(circleRect, Qt::AlignCenter, letters);

    painter.end();
    return pix;
}

namespace {

QString formatPinDate(const QDateTime &dt)
{
    if (!dt.isValid())
        return {};

    const QDateTime now = QDateTime::currentDateTime();
    qint64 secsAgo = dt.secsTo(now);

    if (secsAgo < 60)
        return QObject::tr("just now");
    if (secsAgo < 3600)
        return QObject::tr("%1m ago").arg(static_cast<int>(secsAgo / 60));
    if (secsAgo < 86400)
        return QObject::tr("%1h ago").arg(static_cast<int>(secsAgo / 3600));
    if (secsAgo < 604800)
        return QObject::tr("%1d ago").arg(static_cast<int>(secsAgo / 86400));

    return dt.toString(QStringLiteral("MMM d, yyyy"));
}

} // anonymous namespace

// ─── PinnedMessagesWidget ────────────────────────────────────────────────────

PinnedMessagesWidget::PinnedMessagesWidget(const QString &roomId, QWidget *parent)
    : QWidget(parent)
    , m_roomId(roomId)
    , m_nam(new QNetworkAccessManager(this))
{
    setStyleSheet(QString::fromLatin1(kDarkStyle));
    setupUi();
    fetchPinnedMessages();
}

void PinnedMessagesWidget::setupUi()
{
    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(12, 10, 12, 10);
    mainLayout->setSpacing(8);

    // ── Header row ──
    auto *headerRow = new QHBoxLayout();
    headerRow->setSpacing(8);

    m_headerLabel = new QLabel(tr("Pinned Messages"), this);
    m_headerLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-size: 15px; font-weight: bold;"));
    headerRow->addWidget(m_headerLabel, 1);

    m_pinNewButton = new QPushButton(tr("Pin New"), this);
    headerRow->addWidget(m_pinNewButton);

    m_closeButton = new QPushButton(QStringLiteral("\u00d7"), this);
    m_closeButton->setFixedSize(28, 28);
    m_closeButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: #313244; color: #cdd6f4; border: none; "
        "border-radius: 14px; font-size: 16px; font-weight: bold; }"
        "QPushButton:hover { background: #f38ba8; color: #1e1e2e; }"));
    m_closeButton->setCursor(Qt::PointingHandCursor);
    headerRow->addWidget(m_closeButton);

    mainLayout->addLayout(headerRow);

    // ── Empty state label ──
    m_emptyLabel = new QLabel(tr("No pinned messages"), this);
    m_emptyLabel->setAlignment(Qt::AlignCenter);
    m_emptyLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 13px; padding: 24px;"));
    m_emptyLabel->hide();
    mainLayout->addWidget(m_emptyLabel);

    // ── Message list ──
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->setFrameShape(QFrame::NoFrame);
    m_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_scrollArea->setStyleSheet(QStringLiteral(
        "QScrollArea { background: transparent; border: none; }"
        "QScrollBar:vertical { background: #1e1e2e; width: 6px; }"
        "QScrollBar::handle:vertical { background: #45475a; border-radius: 3px; }"
        "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height: 0; }"));

    m_messageList = new QListWidget(m_scrollArea);
    m_messageList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_messageList->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    m_messageList->setSelectionMode(QAbstractItemView::SingleSelection);
    m_messageList->setMaximumHeight(300);
    m_scrollArea->setWidget(m_messageList);
    mainLayout->addWidget(m_scrollArea, 1);

    // ── Connections ──
    connect(m_pinNewButton, &QPushButton::clicked,
            this, &PinnedMessagesWidget::onPinNewClicked);
    connect(m_closeButton, &QPushButton::clicked,
            this, &PinnedMessagesWidget::onCloseClicked);
    connect(m_messageList, &QListWidget::itemClicked,
            this, &PinnedMessagesWidget::onItemClicked);
}

// ─── Animation helpers ───────────────────────────────────────────────────────

void PinnedMessagesWidget::setSlideOffset(qreal offset)
{
    m_slideOffset = offset;
    update();
}

void PinnedMessagesWidget::showAnimated()
{
    if (m_animation)
        m_animation->stop();

    if (!m_animation) {
        m_animation = new QPropertyAnimation(this, "slideOffset", this);
        m_animation->setDuration(250);
        m_animation->setEasingCurve(QEasingCurve::OutCubic);
    }

    m_animation->setStartValue(1.0);
    m_animation->setEndValue(0.0);
    m_animation->start();

    show();
    raise();
}

void PinnedMessagesWidget::hideAnimated()
{
    if (m_animation)
        m_animation->stop();

    if (!m_animation) {
        m_animation = new QPropertyAnimation(this, "slideOffset", this);
        m_animation->setDuration(200);
        m_animation->setEasingCurve(QEasingCurve::InCubic);
    }

    m_animation->setStartValue(0.0);
    m_animation->setEndValue(1.0);

    connect(m_animation, &QPropertyAnimation::finished, this, [this]() {
        hide();
        emit dismissed();
    });

    m_animation->start();
}

void PinnedMessagesWidget::onCloseClicked()
{
    hideAnimated();
}

// ─── Fetch pinned messages ───────────────────────────────────────────────────

void PinnedMessagesWidget::fetchPinnedMessages()
{
    m_loading = true;
    m_pinnedMessages.clear();
    m_messageList->clear();

    auto *session = pcp::Application::instance().matrixSession();
    if (!session) {
        m_emptyLabel->setText(tr("No Matrix session available."));
        m_emptyLabel->show();
        m_messageList->hide();
        m_loading = false;
        return;
    }

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.pinned_events")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError
            && reply->error() != QNetworkReply::ContentNotFoundError) {
            m_emptyLabel->setText(tr("Failed to load pinned messages."));
            m_emptyLabel->show();
            m_messageList->hide();
            m_loading = false;
            return;
        }

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        const QJsonArray pinned = JsonUtil::optArray(json, QStringLiteral("pinned"));

        if (pinned.isEmpty()) {
            m_emptyLabel->setText(tr("No pinned messages"));
            m_emptyLabel->show();
            m_messageList->hide();
            m_loading = false;
            return;
        }

        QStringList eventIds;
        eventIds.reserve(pinned.size());
        for (const auto &val : pinned) {
            const QString eventId = val.toString();
            if (!eventId.isEmpty())
                eventIds.append(eventId);
        }

        if (eventIds.isEmpty()) {
            m_emptyLabel->setText(tr("No pinned messages"));
            m_emptyLabel->show();
            m_messageList->hide();
            m_loading = false;
            return;
        }

        // Fetch each pinned event's details
        for (const QString &eventId : std::as_const(eventIds)) {
            fetchPinnedEventDetail(eventId);
        }
    });
}

void PinnedMessagesWidget::fetchPinnedEventDetail(const QString &eventId)
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/event/%2")
                             .arg(m_roomId, eventId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->get(req);

    connect(reply, &QNetworkReply::finished, this, [this, reply, eventId]() {
        reply->deleteLater();

        if (reply->error() != QNetworkReply::NoError)
            return;

        const QJsonObject json = JsonUtil::parseObject(QString::fromUtf8(reply->readAll()));
        const QString type = JsonUtil::optString(json, QStringLiteral("type"));
        if (type != QStringLiteral("m.room.message"))
            return;

        const QJsonObject content = JsonUtil::optObject(json, QStringLiteral("content"));
        const QString body = JsonUtil::optString(content, QStringLiteral("body"));
        if (body.isEmpty())
            return;

        const QString senderId = JsonUtil::optString(json, QStringLiteral("sender"));
        qint64 originTs = JsonUtil::optLong(json, QStringLiteral("origin_server_ts"));

        PinnedMessageInfo info;
        info.eventId = eventId;
        info.senderId = senderId;
        // senderName will be fetched or displayed as senderId
        info.senderName = senderId;
        info.bodyPreview = truncateBody(body);
        info.pinnedAt = originTs > 0 ? QDateTime::fromMSecsSinceEpoch(originTs) : QDateTime();

        m_pinnedMessages.append(info);

        // Display as they come in, most recent first
        std::sort(m_pinnedMessages.begin(), m_pinnedMessages.end(),
                  [](const PinnedMessageInfo &a, const PinnedMessageInfo &b) {
                      return a.pinnedAt > b.pinnedAt;
                  });

        m_messageList->clear();
        m_emptyLabel->hide();
        m_messageList->show();

        for (const auto &pin : std::as_const(m_pinnedMessages)) {
            auto *item = new QListWidgetItem();
            item->setData(Qt::UserRole, pin.eventId);
            item->setSizeHint(QSize(0, 64));

            QWidget *row = createMessageItemWidget(pin);
            m_messageList->addItem(item);
            m_messageList->setItemWidget(item, row);
        }

        m_loading = false;
    });
}

// ─── Item widget ─────────────────────────────────────────────────────────────

QWidget *PinnedMessagesWidget::createMessageItemWidget(const PinnedMessageInfo &info)
{
    auto *rowWidget = new QWidget(m_messageList);
    rowWidget->setStyleSheet(QStringLiteral("background: transparent;"));

    auto *row = new QHBoxLayout(rowWidget);
    row->setContentsMargins(4, 6, 4, 6);
    row->setSpacing(8);

    // Avatar
    QColor avatarColor = colorFromName(info.senderName);
    QPixmap avatarPix = makeAvatarPixmap(info.senderName, kAvatarSize, avatarColor);

    auto *avatarLabel = new QLabel(rowWidget);
    avatarLabel->setFixedSize(kAvatarSize, kAvatarSize);
    avatarLabel->setPixmap(avatarPix);
    avatarLabel->setAlignment(Qt::AlignCenter);
    row->addWidget(avatarLabel);

    // Center: sender + preview + date
    auto *centerLayout = new QVBoxLayout();
    centerLayout->setSpacing(2);

    auto *senderLabel = new QLabel(info.senderName, rowWidget);
    senderLabel->setStyleSheet(QStringLiteral(
        "color: #cdd6f4; font-weight: bold; font-size: 12px;"));
    centerLayout->addWidget(senderLabel);

    auto *previewLabel = new QLabel(info.bodyPreview, rowWidget);
    previewLabel->setStyleSheet(QStringLiteral(
        "color: #a6adc8; font-size: 11px;"));
    previewLabel->setWordWrap(true);
    centerLayout->addWidget(previewLabel);

    auto *dateLabel = new QLabel(formatPinDate(info.pinnedAt), rowWidget);
    dateLabel->setStyleSheet(QStringLiteral(
        "color: #6c7086; font-size: 10px;"));
    centerLayout->addWidget(dateLabel);

    row->addLayout(centerLayout, 1);

    // Unpin button
    auto *unpinButton = new QPushButton(tr("Unpin"), rowWidget);
    unpinButton->setStyleSheet(QStringLiteral(
        "QPushButton { background: #f38ba8; color: #1e1e2e; border: none; "
        "border-radius: 4px; padding: 4px 10px; font-size: 11px; font-weight: bold; }"
        "QPushButton:hover { background: #eba0ac; }"));
    unpinButton->setCursor(Qt::PointingHandCursor);

    const QString eventId = info.eventId;
    connect(unpinButton, &QPushButton::clicked, this, [this, eventId]() {
        onUnpinClicked(eventId);
    });

    row->addWidget(unpinButton);

    rowWidget->setLayout(row);
    return rowWidget;
}

// ─── Truncation helper ───────────────────────────────────────────────────────

QString PinnedMessagesWidget::truncateBody(const QString &body, int maxLen)
{
    if (body.length() <= maxLen)
        return body;

    return body.left(maxLen - 3) + QStringLiteral("...");
}

// ─── Actions ─────────────────────────────────────────────────────────────────

void PinnedMessagesWidget::onItemClicked(QListWidgetItem *item)
{
    if (!item)
        return;

    const QString eventId = item->data(Qt::UserRole).toString();
    if (!eventId.isEmpty())
        emit messageSelected(eventId);
}

void PinnedMessagesWidget::onPinNewClicked()
{
    // Placeholder: would open a dialog to search for and pin a message.
    // The actual implementation would POST /_matrix/client/r0/rooms/{roomId}/state/m.room.pinned_events
    QMessageBox::information(this, tr("Pin New Message"),
                             tr("Pin New Message dialog would be shown here. "
                                "Select a message from recent history to pin it."));
}

void PinnedMessagesWidget::onUnpinClicked(const QString &eventId)
{
    auto *session = pcp::Application::instance().matrixSession();
    if (!session)
        return;

    // Remove from the local list
    auto it = std::remove_if(m_pinnedMessages.begin(), m_pinnedMessages.end(),
                              [&eventId](const PinnedMessageInfo &info) {
                                  return info.eventId == eventId;
                              });
    m_pinnedMessages.erase(it, m_pinnedMessages.end());

    // Rebuild the pinned array without this eventId
    QJsonArray updatedPinned;
    for (const auto &pin : std::as_const(m_pinnedMessages))
        updatedPinned.append(pin.eventId);

    QJsonObject content;
    content[QStringLiteral("pinned")] = updatedPinned;

    const QString path = QStringLiteral("/_matrix/client/r0/rooms/%1/state/m.room.pinned_events")
                             .arg(m_roomId);
    QUrl url(session->homeserverUrl() + path);
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    req.setRawHeader("Authorization", ("Bearer " + session->accessToken()).toUtf8());

    QNetworkReply *reply = m_nam->put(req, QJsonDocument(content).toJson(QJsonDocument::Compact));

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();

        // Refresh the list
        m_messageList->clear();

        if (m_pinnedMessages.isEmpty()) {
            m_emptyLabel->setText(tr("No pinned messages"));
            m_emptyLabel->show();
            m_messageList->hide();
            return;
        }

        m_emptyLabel->hide();
        m_messageList->show();
        for (const auto &pin : std::as_const(m_pinnedMessages)) {
            auto *item = new QListWidgetItem();
            item->setData(Qt::UserRole, pin.eventId);
            item->setSizeHint(QSize(0, 64));

            QWidget *row = createMessageItemWidget(pin);
            m_messageList->addItem(item);
            m_messageList->setItemWidget(item, row);
        }
    });
}

#include "pinned_messages_widget.moc"
