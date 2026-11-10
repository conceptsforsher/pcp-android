#pragma once

#include <QWidget>
#include <QHash>
#include <QDateTime>
#include <QString>
#include <QTimer>

#include "protocol/protocol_type.hpp"

namespace pcp {

class ConnectionStatusWidget : public QWidget
{
    Q_OBJECT

public:
    enum class DisplayMode {
        Full,
        Compact
    };

    explicit ConnectionStatusWidget(QWidget *parent = nullptr);
    ~ConnectionStatusWidget() override;

    void setDisplayMode(DisplayMode mode);
    DisplayMode displayMode() const;

    void setServerUrl(ProtocolType type, const QString &url);
    QString serverUrl(ProtocolType type) const;

    void setLatency(ProtocolType type, int latencyMs);
    int latency(ProtocolType type) const;

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

private:
    struct ProtocolIndicator {
        ProtocolType type;
        ConnectionState state = ConnectionState::DISCONNECTED;
        QString serverUrl;
        QDateTime connectedSince;
        int latencyMs = -1;
        QRectF bounds;
    };

    void setupTimer();
    void updateStatuses();
    void advancePulse();
    void layoutIndicators();
    int indicatorIndexAt(const QPoint &pos) const;

    static QString letterForProtocol(ProtocolType type);
    static QColor colorForState(ConnectionState state);
    static QString textForState(ConnectionState state);

    QTimer *m_updateTimer;
    QTimer *m_pulseTimer;
    DisplayMode m_mode = DisplayMode::Full;
    QHash<ProtocolType, ProtocolIndicator> m_indicators;
    qreal m_pulsePhase = 0.0;
};

} // namespace pcp
