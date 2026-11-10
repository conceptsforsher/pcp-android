#include "connection_status_widget.hpp"

#include <QPainter>
#include <QMouseEvent>
#include <QToolTip>
#include <QtMath>
#include <iterator>

#include "app/application.hpp"
#include "protocol/protocol_manager.hpp"
#include "protocol/protocol_session.hpp"

namespace pcp {

static constexpr int kIndicatorSpacing = 8;
static constexpr int kProtocolCircleRadius = 8;
static constexpr int kStatusDotRadius = 4;
static constexpr int kFullModeIndicatorWidth = 110;
static constexpr int kCompactModeIndicatorWidth = 20;
static constexpr int kWidgetHeight = 26;
static constexpr int kUpdateIntervalMs = 5000;
static constexpr int kPulseIntervalMs = 50;

ConnectionStatusWidget::ConnectionStatusWidget(QWidget *parent)
    : QWidget(parent)
    , m_updateTimer(new QTimer(this))
    , m_pulseTimer(new QTimer(this))
{
    constexpr ProtocolType allTypes[] = {
        ProtocolType::MATRIX,
        ProtocolType::IRC,
        ProtocolType::LEMMY,
        ProtocolType::TELEGRAM,
        ProtocolType::DELTACHAT
    };

    for (auto type : allTypes) {
        ProtocolIndicator ind;
        ind.type = type;
        ind.state = ConnectionState::DISCONNECTED;
        m_indicators.insert(type, ind);
    }

    setupTimer();

    auto &app = Application::instance();
    connect(&app, &Application::connectionStateChanged,
            this, [this](ProtocolType type, ConnectionState state) {
        if (m_indicators.contains(type)) {
            auto &ind = m_indicators[type];
            ind.state = state;
            if (state == ConnectionState::CONNECTED && !ind.connectedSince.isValid()) {
                ind.connectedSince = QDateTime::currentDateTime();
            } else if (state == ConnectionState::DISCONNECTED) {
                ind.connectedSince = QDateTime();
                ind.latencyMs = -1;
            }
            updateStatuses();
        }
    });

    setMouseTracking(true);
    setFixedHeight(kWidgetHeight);
    updateStatuses();
}

ConnectionStatusWidget::~ConnectionStatusWidget() = default;

void ConnectionStatusWidget::setDisplayMode(DisplayMode mode)
{
    if (m_mode != mode) {
        m_mode = mode;
        updateGeometry();
        update();
    }
}

ConnectionStatusWidget::DisplayMode ConnectionStatusWidget::displayMode() const
{
    return m_mode;
}

void ConnectionStatusWidget::setServerUrl(ProtocolType type, const QString &url)
{
    if (m_indicators.contains(type)) {
        m_indicators[type].serverUrl = url;
    }
}

QString ConnectionStatusWidget::serverUrl(ProtocolType type) const
{
    return m_indicators.value(type).serverUrl;
}

void ConnectionStatusWidget::setLatency(ProtocolType type, int latencyMs)
{
    if (m_indicators.contains(type)) {
        m_indicators[type].latencyMs = latencyMs;
    }
}

int ConnectionStatusWidget::latency(ProtocolType type) const
{
    return m_indicators.value(type).latencyMs;
}

QSize ConnectionStatusWidget::sizeHint() const
{
    const int count = static_cast<int>(m_indicators.size());
    const int w = m_mode == DisplayMode::Full
        ? count * kFullModeIndicatorWidth + (count - 1) * kIndicatorSpacing + 4
        : count * kCompactModeIndicatorWidth + (count - 1) * kIndicatorSpacing + 4;
    return QSize(w, kWidgetHeight);
}

QSize ConnectionStatusWidget::minimumSizeHint() const
{
    return sizeHint();
}

void ConnectionStatusWidget::paintEvent(QPaintEvent * /*event*/)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const QFont labelFont(QStringLiteral("sans-serif"), 9);
    const QFont circleFont(QStringLiteral("sans-serif"), 8, QFont::Bold);
    QFontMetrics labelFm(labelFont);

    for (const auto &ind : m_indicators) {
        const QRectF &b = ind.bounds;
        if (b.isEmpty()) {
            continue;
        }

        const QColor stateColor = colorForState(ind.state);
        const QString letter = letterForProtocol(ind.type);

        const qreal circleX = b.x() + kProtocolCircleRadius;
        const qreal circleY = b.center().y();
        const QRectF circleRect(circleX - kProtocolCircleRadius,
                                circleY - kProtocolCircleRadius,
                                kProtocolCircleRadius * 2,
                                kProtocolCircleRadius * 2);

        painter.setPen(Qt::NoPen);
        painter.setBrush(stateColor);
        painter.drawEllipse(circleRect);

        painter.setFont(circleFont);
        painter.setPen(Qt::white);
        painter.drawText(circleRect, Qt::AlignCenter, letter);

        if (m_mode == DisplayMode::Full) {
            const qreal textX = circleRect.right() + 4;
            const qreal textY = b.y();
            const qreal textWidth = b.right() - textX - kStatusDotRadius * 2 - 6;
            const QRectF textRect(textX, textY, textWidth, b.height());

            painter.setFont(labelFont);
            const QString stateText = textForState(ind.state);
            const QColor textColor = (ind.state == ConnectionState::CONNECTED)
                ? QColor(0, 150, 0) : palette().color(QPalette::WindowText);
            painter.setPen(textColor);
            painter.drawText(textRect, Qt::AlignVCenter | Qt::AlignLeft,
                             labelFm.elidedText(stateText, Qt::ElideRight,
                                                static_cast<int>(textWidth)));
        }

        const qreal dotX = m_mode == DisplayMode::Full
            ? b.right() - kStatusDotRadius - 2
            : circleRect.right() + 6;
        const qreal dotY = b.center().y();
        const QRectF dotRect(dotX - kStatusDotRadius,
                             dotY - kStatusDotRadius,
                             kStatusDotRadius * 2,
                             kStatusDotRadius * 2);

        QColor dotColor = stateColor;
        if (ind.state == ConnectionState::CONNECTING) {
            const int alpha = static_cast<int>(127 + 128 * qSin(m_pulsePhase));
            dotColor.setAlpha(alpha);
        }

        painter.setPen(Qt::NoPen);
        painter.setBrush(dotColor);
        painter.drawEllipse(dotRect);
    }
}

void ConnectionStatusWidget::mouseMoveEvent(QMouseEvent *event)
{
    const int idx = indicatorIndexAt(event->pos());
    if (idx < 0 || idx >= static_cast<int>(m_indicators.size())) {
        setCursor(Qt::ArrowCursor);
        return;
    }

    setCursor(Qt::PointingHandCursor);

    auto it = m_indicators.begin();
    std::advance(it, idx);
    const auto &ind = it.value();

    QString tooltip = QStringLiteral("<b>%1</b><br>").arg(letterForProtocol(ind.type));
    tooltip += QStringLiteral("Status: %1<br>").arg(textForState(ind.state));

    if (!ind.serverUrl.isEmpty()) {
        tooltip += QStringLiteral("Server: %1<br>").arg(ind.serverUrl.toHtmlEscaped());
    }

    if (ind.connectedSince.isValid()) {
        const qint64 secs = ind.connectedSince.secsTo(QDateTime::currentDateTime());
        const QString duration = secs < 60
            ? QStringLiteral("%1s").arg(secs)
            : secs < 3600
                ? QStringLiteral("%1m %2s").arg(secs / 60).arg(secs % 60)
                : QStringLiteral("%1h %2m").arg(secs / 3600).arg((secs % 3600) / 60);
        tooltip += QStringLiteral("Connected: %1<br>").arg(duration);
    }

    if (ind.latencyMs >= 0) {
        tooltip += QStringLiteral("Latency: %1ms").arg(ind.latencyMs);
    }

    QToolTip::showText(event->globalPosition().toPoint(), tooltip, this);
}

void ConnectionStatusWidget::mousePressEvent(QMouseEvent *event)
{
    const int idx = indicatorIndexAt(event->pos());
    if (idx < 0 || idx >= static_cast<int>(m_indicators.size())) {
        QWidget::mousePressEvent(event);
        return;
    }

    auto it = m_indicators.begin();
    std::advance(it, idx);
    const auto &ind = it.value();

    if (event->button() == Qt::LeftButton) {
        if (ind.state == ConnectionState::DISCONNECTED ||
            ind.state == ConnectionState::ERROR) {
            auto *mgr = &::ProtocolManager::instance();
            if (mgr) {
                auto *session = mgr->getSession(ind.type);
                if (session && session->connectionState() != ConnectionState::CONNECTED) {
                    session->open();
                }
            }
        }
    }
}

void ConnectionStatusWidget::setupTimer()
{
    m_updateTimer->setInterval(kUpdateIntervalMs);
    connect(m_updateTimer, &QTimer::timeout, this, &ConnectionStatusWidget::updateStatuses);
    m_updateTimer->start();

    m_pulseTimer->setInterval(kPulseIntervalMs);
    connect(m_pulseTimer, &QTimer::timeout, this, &ConnectionStatusWidget::advancePulse);
    m_pulseTimer->start();
}

void ConnectionStatusWidget::updateStatuses()
{
    auto &mgr = ::ProtocolManager::instance();

    bool changed = false;

    constexpr ProtocolType monitoredTypes[] = {
        ProtocolType::MATRIX,
        ProtocolType::IRC,
        ProtocolType::LEMMY,
        ProtocolType::TELEGRAM,
        ProtocolType::DELTACHAT
    };

    for (auto type : monitoredTypes) {
        if (!m_indicators.contains(type)) {
            continue;
        }

        auto &ind = m_indicators[type];
        auto *session = mgr.getSession(type);

        ConnectionState newState = session
            ? session->connectionState()
            : ConnectionState::DISCONNECTED;

        if (ind.state != newState) {
            ind.state = newState;
            if (newState == ConnectionState::CONNECTED && !ind.connectedSince.isValid()) {
                ind.connectedSince = QDateTime::currentDateTime();
            } else if (newState == ConnectionState::DISCONNECTED) {
                ind.connectedSince = QDateTime();
                ind.latencyMs = -1;
            }
            changed = true;
        }
    }

    layoutIndicators();

    if (changed || m_pulseTimer->isActive()) {
        update();
    }
}

void ConnectionStatusWidget::advancePulse()
{
    m_pulsePhase += 0.15;
    if (m_pulsePhase > 2.0 * M_PI) {
        m_pulsePhase -= 2.0 * M_PI;
    }

    bool hasConnecting = false;
    for (const auto &ind : m_indicators) {
        if (ind.state == ConnectionState::CONNECTING) {
            hasConnecting = true;
            break;
        }
    }

    if (hasConnecting) {
        update();
    }
}

void ConnectionStatusWidget::layoutIndicators()
{
    const int count = static_cast<int>(m_indicators.size());
    if (count == 0) {
        return;
    }

    const qreal indicatorWidth = m_mode == DisplayMode::Full
        ? static_cast<qreal>(kFullModeIndicatorWidth)
        : static_cast<qreal>(kCompactModeIndicatorWidth);

    const qreal totalWidth = count * indicatorWidth + (count - 1) * kIndicatorSpacing;
    qreal startX = (width() - totalWidth) / 2.0;

    int i = 0;
    for (auto it = m_indicators.begin(); it != m_indicators.end(); ++it, ++i) {
        const qreal x = startX + i * (indicatorWidth + kIndicatorSpacing);
        it->bounds = QRectF(x, 0, indicatorWidth, kWidgetHeight);
    }
}

int ConnectionStatusWidget::indicatorIndexAt(const QPoint &pos) const
{
    int idx = 0;
    for (auto it = m_indicators.begin(); it != m_indicators.end(); ++it, ++idx) {
        if (it->bounds.contains(pos)) {
            return idx;
        }
    }
    return -1;
}

QString ConnectionStatusWidget::letterForProtocol(ProtocolType type)
{
    switch (type) {
    case ProtocolType::MATRIX:    return QStringLiteral("M");
    case ProtocolType::IRC:       return QStringLiteral("#");
    case ProtocolType::LEMMY:     return QStringLiteral("L");
    case ProtocolType::TELEGRAM:  return QStringLiteral("T");
    case ProtocolType::DELTACHAT: return QStringLiteral("D");
    }
    return QStringLiteral("?");
}

QColor ConnectionStatusWidget::colorForState(ConnectionState state)
{
    switch (state) {
    case ConnectionState::CONNECTED:   return QColor(46, 160, 67);    // green
    case ConnectionState::CONNECTING:  return QColor(218, 165, 32);   // yellow/orange
    case ConnectionState::DISCONNECTED:return QColor(207, 34, 46);    // red
    case ConnectionState::ERROR:       return QColor(207, 34, 46);    // red
    case ConnectionState::REGISTERED:  return QColor(46, 160, 67);    // green
    }
    return QColor(128, 128, 128);
}

QString ConnectionStatusWidget::textForState(ConnectionState state)
{
    switch (state) {
    case ConnectionState::CONNECTED:   return QStringLiteral("Connected");
    case ConnectionState::CONNECTING:  return QStringLiteral("Connecting...");
    case ConnectionState::DISCONNECTED:return QStringLiteral("Disconnected");
    case ConnectionState::ERROR:       return QStringLiteral("Error");
    case ConnectionState::REGISTERED:  return QStringLiteral("Connected");
    }
    return QStringLiteral("Unknown");
}

} // namespace pcp
