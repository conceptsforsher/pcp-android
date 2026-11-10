#pragma once

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonArray>
#include <QDateTime>
#include <QImage>
#include <QByteArray>
#include <QVector>
#include <QStringList>

class PcpNative : public QObject
{
    Q_OBJECT

public:
    static PcpNative* instance();

    // --- Jump to Date (MSC3030) ---
    Q_INVOKABLE QString validateAndBuildTimestamp(const QString& dateStr);
    Q_INVOKABLE QJsonObject parseResponse(const QString& response);
    Q_INVOKABLE QJsonObject parseJumpToDate(const QString& data);

    // --- Message Content ---
    Q_INVOKABLE QString exportMessage(const QJsonObject& msg);
    Q_INVOKABLE void cacheMessage(const QString& roomId, const QJsonObject& msg);
    Q_INVOKABLE void storeEvent(const QString& roomId, const QJsonObject& event);
    Q_INVOKABLE QJsonArray getEvents(const QString& roomId, int limit);
    Q_INVOKABLE QJsonArray getRooms();
    Q_INVOKABLE void transactionBegin();
    Q_INVOKABLE void transactionCommit();
    Q_INVOKABLE void transactionRollback();

    // --- Formatting / Translation ---
    Q_INVOKABLE QString markdownToHtml(const QString& md);
    Q_INVOKABLE QString llmRequest(const QString& prompt);
    Q_INVOKABLE QString llmResponse(const QString& response);
    Q_INVOKABLE QString translateApi(const QString& text, const QString& from, const QString& to);
    Q_INVOKABLE QString formatNotification(const QJsonObject& notif);
    Q_INVOKABLE QString formatTimestamp(qint64 ts);

    // --- Proxy / Yggdrasil / I2P ---
    Q_INVOKABLE bool setProxy(const QString& host, int port, const QString& type);
    Q_INVOKABLE void enableYggdrasil(bool enabled);
    Q_INVOKABLE void enableI2P(bool enabled);

    // --- Push / Notifications ---
    Q_INVOKABLE bool evaluatePush(const QJsonObject& push);
    Q_INVOKABLE int notificationPriority(const QJsonObject& notif);
    Q_INVOKABLE bool matchKeywords(const QString& text, const QStringList& keywords);
    Q_INVOKABLE bool isQuietHours(const QDateTime& now);

    // --- AI / ML ---
    Q_INVOKABLE QJsonObject llmIntegrate(const QString& prompt, const QJsonObject& context);
    Q_INVOKABLE QJsonObject executeAgentTool(const QString& tool, const QJsonObject& params);
    Q_INVOKABLE QString languageDetect(const QString& text);
    Q_INVOKABLE QString scheduleAlarm(const QString& cron, const QJsonObject& action);
    Q_INVOKABLE bool cancelAlarm(const QString& alarmId);

    // --- Rich Features ---
    Q_INVOKABLE QString mirrorRoom(const QString& roomId);
    Q_INVOKABLE QString maskUser(const QString& userId);
    Q_INVOKABLE QStringList rainbowText(const QString& text);
    Q_INVOKABLE void scheduleEdit(const QString& roomId, const QString& eventId,
                                  const QString& newText, const QDateTime& time);
    Q_INVOKABLE QImage drawSvg(const QString& svg);
    Q_INVOKABLE QJsonObject profileSwiper(const QJsonArray& profiles);
    Q_INVOKABLE QString uploadChunked(const QString& filePath);
    Q_INVOKABLE QVector<double> audioWaveform(const QByteArray& audio);
    Q_INVOKABLE QImage thumbnail(const QString& path, int w, int h);

    // --- Auth / Account ---
    Q_INVOKABLE QJsonObject buildLoginBody(const QString& user, const QString& pass,
                                           const QString& homeserver);
    Q_INVOKABLE bool validatePassword(const QString& pass);
    Q_INVOKABLE QString encryptData(const QString& data, const QString& key);
    Q_INVOKABLE QString decryptData(const QString& data, const QString& key);
    Q_INVOKABLE QJsonObject exportAccount(const QString& userId);
    Q_INVOKABLE bool importAccount(const QJsonObject& data);

    // --- Misc ---
    Q_INVOKABLE bool isBidiTextSafe(const QString& text);
    Q_INVOKABLE QStringList spellcheck(const QString& text);
    Q_INVOKABLE QJsonObject emojiAnalysis(const QString& text);
    Q_INVOKABLE void buildSearchIndex(const QJsonArray& events);
    Q_INVOKABLE QJsonArray searchEvents(const QString& query);
    Q_INVOKABLE bool desyncDetector(const QString& roomId);
    Q_INVOKABLE void trackLatency(const QString& server, qint64 ms);
    Q_INVOKABLE QString contentFilter(const QString& text);
    Q_INVOKABLE QJsonObject masquerade(const QString& userId);

signals:
    void yggdrasilEnabledChanged(bool enabled);
    void i2pEnabledChanged(bool enabled);
    void proxyConfigured(const QString& host, int port);

private:
    explicit PcpNative(QObject* parent = nullptr);
    ~PcpNative() override = default;
    PcpNative(const PcpNative&) = delete;
    PcpNative& operator=(const PcpNative&) = delete;

    bool m_yggdrasilEnabled = false;
    bool m_i2pEnabled = false;
};
