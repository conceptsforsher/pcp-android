#include "pcp_native.hpp"

#include <QDebug>
#include <QJsonDocument>
#include <QUuid>
#include <QFileInfo>
#include <QDateTime>

// ============================================================================
// Singleton & Constructor
// ============================================================================

PcpNative* PcpNative::instance()
{
    static PcpNative inst;
    return &inst;
}

PcpNative::PcpNative(QObject* parent)
    : QObject(parent)
    , m_yggdrasilEnabled(false)
    , m_i2pEnabled(false)
{
}

// ============================================================================
// Jump to Date (MSC3030)
// ============================================================================

#ifdef USE_PCP_NATIVE
QString PcpNative::validateAndBuildTimestamp(const QString& dateStr)
{
    return QString(); // real impl
}
#else
QString PcpNative::validateAndBuildTimestamp(const QString& dateStr)
{
    qDebug() << "[PcpNative] validateAndBuildTimestamp:" << dateStr;
    Q_UNUSED(dateStr);
    return QString();
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::parseResponse(const QString& response)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::parseResponse(const QString& response)
{
    qDebug() << "[PcpNative] parseResponse len:" << response.size();
    Q_UNUSED(response);
    return QJsonObject();
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::parseJumpToDate(const QString& data)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::parseJumpToDate(const QString& data)
{
    qDebug() << "[PcpNative] parseJumpToDate:" << data;
    Q_UNUSED(data);
    return QJsonObject();
}
#endif

// ============================================================================
// Message Content
// ============================================================================

#ifdef USE_PCP_NATIVE
QString PcpNative::exportMessage(const QJsonObject& msg)
{
    return QString(); // real impl
}
#else
QString PcpNative::exportMessage(const QJsonObject& msg)
{
    qDebug() << "[PcpNative] exportMessage";
    QJsonDocument doc(msg);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::cacheMessage(const QString& roomId, const QJsonObject& msg)
{
    // real impl
}
#else
void PcpNative::cacheMessage(const QString& roomId, const QJsonObject& msg)
{
    qDebug() << "[PcpNative] cacheMessage room:" << roomId;
    Q_UNUSED(msg);
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::storeEvent(const QString& roomId, const QJsonObject& event)
{
    // real impl
}
#else
void PcpNative::storeEvent(const QString& roomId, const QJsonObject& event)
{
    qDebug() << "[PcpNative] storeEvent room:" << roomId;
    Q_UNUSED(event);
}
#endif

#ifdef USE_PCP_NATIVE
QJsonArray PcpNative::getEvents(const QString& roomId, int limit)
{
    return QJsonArray(); // real impl
}
#else
QJsonArray PcpNative::getEvents(const QString& roomId, int limit)
{
    qDebug() << "[PcpNative] getEvents room:" << roomId << "limit:" << limit;
    Q_UNUSED(roomId);
    Q_UNUSED(limit);
    return QJsonArray();
}
#endif

#ifdef USE_PCP_NATIVE
QJsonArray PcpNative::getRooms()
{
    return QJsonArray(); // real impl
}
#else
QJsonArray PcpNative::getRooms()
{
    qDebug() << "[PcpNative] getRooms";
    return QJsonArray();
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::transactionBegin()
{
    // real impl
}
#else
void PcpNative::transactionBegin()
{
    qDebug() << "[PcpNative] transactionBegin";
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::transactionCommit()
{
    // real impl
}
#else
void PcpNative::transactionCommit()
{
    qDebug() << "[PcpNative] transactionCommit";
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::transactionRollback()
{
    // real impl
}
#else
void PcpNative::transactionRollback()
{
    qDebug() << "[PcpNative] transactionRollback";
}
#endif

// ============================================================================
// Formatting / Translation
// ============================================================================

#ifdef USE_PCP_NATIVE
QString PcpNative::markdownToHtml(const QString& md)
{
    return QString(); // real impl
}
#else
QString PcpNative::markdownToHtml(const QString& md)
{
    qDebug() << "[PcpNative] markdownToHtml len:" << md.size();
    Q_UNUSED(md);
    return md;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::llmRequest(const QString& prompt)
{
    return QString(); // real impl
}
#else
QString PcpNative::llmRequest(const QString& prompt)
{
    qDebug() << "[PcpNative] llmRequest:" << prompt.left(80);
    Q_UNUSED(prompt);
    return QStringLiteral("{\"response\":\"stub llm request echo\"}");
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::llmResponse(const QString& response)
{
    return QString(); // real impl
}
#else
QString PcpNative::llmResponse(const QString& response)
{
    qDebug() << "[PcpNative] llmResponse len:" << response.size();
    Q_UNUSED(response);
    return QStringLiteral("{\"handled\":true}");
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::translateApi(const QString& text, const QString& from, const QString& to)
{
    return QString(); // real impl
}
#else
QString PcpNative::translateApi(const QString& text, const QString& from, const QString& to)
{
    qDebug() << "[PcpNative] translateApi from:" << from << "to:" << to;
    Q_UNUSED(from);
    Q_UNUSED(to);
    return text;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::formatNotification(const QJsonObject& notif)
{
    return QString(); // real impl
}
#else
QString PcpNative::formatNotification(const QJsonObject& notif)
{
    qDebug() << "[PcpNative] formatNotification";
    QJsonDocument doc(notif);
    return QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::formatTimestamp(qint64 ts)
{
    return QString(); // real impl
}
#else
QString PcpNative::formatTimestamp(qint64 ts)
{
    qDebug() << "[PcpNative] formatTimestamp:" << ts;
    return QDateTime::fromMSecsSinceEpoch(ts).toString(Qt::ISODate);
}
#endif

// ============================================================================
// Proxy / Yggdrasil / I2P
// ============================================================================

#ifdef USE_PCP_NATIVE
bool PcpNative::setProxy(const QString& host, int port, const QString& type)
{
    return false; // real impl
}
#else
bool PcpNative::setProxy(const QString& host, int port, const QString& type)
{
    qDebug() << "[PcpNative] setProxy" << host << port << type;
    emit proxyConfigured(host, port);
    return true;
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::enableYggdrasil(bool enabled)
{
    // real impl
}
#else
void PcpNative::enableYggdrasil(bool enabled)
{
    qDebug() << "[PcpNative] enableYggdrasil:" << enabled;
    m_yggdrasilEnabled = enabled;
    emit yggdrasilEnabledChanged(enabled);
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::enableI2P(bool enabled)
{
    // real impl
}
#else
void PcpNative::enableI2P(bool enabled)
{
    qDebug() << "[PcpNative] enableI2P:" << enabled;
    m_i2pEnabled = enabled;
    emit i2pEnabledChanged(enabled);
}
#endif

// ============================================================================
// Push / Notifications
// ============================================================================

#ifdef USE_PCP_NATIVE
bool PcpNative::evaluatePush(const QJsonObject& push)
{
    return false; // real impl
}
#else
bool PcpNative::evaluatePush(const QJsonObject& push)
{
    qDebug() << "[PcpNative] evaluatePush";
    Q_UNUSED(push);
    return true;
}
#endif

#ifdef USE_PCP_NATIVE
int PcpNative::notificationPriority(const QJsonObject& notif)
{
    return 0; // real impl
}
#else
int PcpNative::notificationPriority(const QJsonObject& notif)
{
    qDebug() << "[PcpNative] notificationPriority";
    Q_UNUSED(notif);
    return 0;
}
#endif

#ifdef USE_PCP_NATIVE
bool PcpNative::matchKeywords(const QString& text, const QStringList& keywords)
{
    return false; // real impl
}
#else
bool PcpNative::matchKeywords(const QString& text, const QStringList& keywords)
{
    qDebug() << "[PcpNative] matchKeywords keys:" << keywords.size();
    for (const QString& kw : keywords) {
        if (text.contains(kw, Qt::CaseInsensitive))
            return true;
    }
    return false;
}
#endif

#ifdef USE_PCP_NATIVE
bool PcpNative::isQuietHours(const QDateTime& now)
{
    return false; // real impl
}
#else
bool PcpNative::isQuietHours(const QDateTime& now)
{
    qDebug() << "[PcpNative] isQuietHours:" << now.toString(Qt::ISODate);
    Q_UNUSED(now);
    return false;
}
#endif

// ============================================================================
// AI / ML
// ============================================================================

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::llmIntegrate(const QString& prompt, const QJsonObject& context)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::llmIntegrate(const QString& prompt, const QJsonObject& context)
{
    qDebug() << "[PcpNative] llmIntegrate prompt:" << prompt.left(80);
    QJsonObject result = context;
    result[QStringLiteral("_echo")] = prompt;
    result[QStringLiteral("_stub")] = true;
    return result;
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::executeAgentTool(const QString& tool, const QJsonObject& params)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::executeAgentTool(const QString& tool, const QJsonObject& params)
{
    qDebug() << "[PcpNative] executeAgentTool:" << tool;
    Q_UNUSED(params);
    QJsonObject result;
    result[QStringLiteral("tool")] = tool;
    result[QStringLiteral("stub")] = true;
    return result;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::languageDetect(const QString& text)
{
    return QString(); // real impl
}
#else
QString PcpNative::languageDetect(const QString& text)
{
    qDebug() << "[PcpNative] languageDetect len:" << text.size();
    Q_UNUSED(text);
    return QStringLiteral("en");
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::scheduleAlarm(const QString& cron, const QJsonObject& action)
{
    return QString(); // real impl
}
#else
QString PcpNative::scheduleAlarm(const QString& cron, const QJsonObject& action)
{
    qDebug() << "[PcpNative] scheduleAlarm cron:" << cron;
    Q_UNUSED(action);
    return QUuid::createUuid().toString(QUuid::WithoutBraces);
}
#endif

#ifdef USE_PCP_NATIVE
bool PcpNative::cancelAlarm(const QString& alarmId)
{
    return false; // real impl
}
#else
bool PcpNative::cancelAlarm(const QString& alarmId)
{
    qDebug() << "[PcpNative] cancelAlarm:" << alarmId;
    Q_UNUSED(alarmId);
    return true;
}
#endif

// ============================================================================
// Rich Features
// ============================================================================

#ifdef USE_PCP_NATIVE
QString PcpNative::mirrorRoom(const QString& roomId)
{
    return QString(); // real impl
}
#else
QString PcpNative::mirrorRoom(const QString& roomId)
{
    qDebug() << "[PcpNative] mirrorRoom:" << roomId;
    return roomId;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::maskUser(const QString& userId)
{
    return QString(); // real impl
}
#else
QString PcpNative::maskUser(const QString& userId)
{
    qDebug() << "[PcpNative] maskUser:" << userId;
    return userId;
}
#endif

#ifdef USE_PCP_NATIVE
QStringList PcpNative::rainbowText(const QString& text)
{
    return QStringList(); // real impl
}
#else
QStringList PcpNative::rainbowText(const QString& text)
{
    qDebug() << "[PcpNative] rainbowText:" << text.left(40);
    return QStringList{text};
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::scheduleEdit(const QString& roomId, const QString& eventId,
                                     const QString& newText, const QDateTime& time)
{
    // real impl
}
#else
void PcpNative::scheduleEdit(const QString& roomId, const QString& eventId,
                                     const QString& newText, const QDateTime& time)
{
    qDebug() << "[PcpNative] scheduleEdit room:" << roomId
             << "event:" << eventId << "at:" << time.toString(Qt::ISODate);
    Q_UNUSED(newText);
}
#endif

#ifdef USE_PCP_NATIVE
QImage PcpNative::drawSvg(const QString& svg)
{
    return QImage(); // real impl
}
#else
QImage PcpNative::drawSvg(const QString& svg)
{
    qDebug() << "[PcpNative] drawSvg len:" << svg.size();
    Q_UNUSED(svg);
    return QImage();
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::profileSwiper(const QJsonArray& profiles)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::profileSwiper(const QJsonArray& profiles)
{
    qDebug() << "[PcpNative] profileSwiper count:" << profiles.size();
    Q_UNUSED(profiles);
    QJsonObject result;
    result[QStringLiteral("direction")] = QStringLiteral("right");
    result[QStringLiteral("stub")] = true;
    return result;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::uploadChunked(const QString& filePath)
{
    return QString(); // real impl
}
#else
QString PcpNative::uploadChunked(const QString& filePath)
{
    qDebug() << "[PcpNative] uploadChunked:" << filePath;
    QFileInfo fi(filePath);
    return QStringLiteral("mxc://stub/%1").arg(fi.fileName());
}
#endif

#ifdef USE_PCP_NATIVE
QVector<double> PcpNative::audioWaveform(const QByteArray& audio)
{
    return QVector<double>(); // real impl
}
#else
QVector<double> PcpNative::audioWaveform(const QByteArray& audio)
{
    qDebug() << "[PcpNative] audioWaveform size:" << audio.size();
    Q_UNUSED(audio);
    return QVector<double>();
}
#endif

#ifdef USE_PCP_NATIVE
QImage PcpNative::thumbnail(const QString& path, int w, int h)
{
    return QImage(); // real impl
}
#else
QImage PcpNative::thumbnail(const QString& path, int w, int h)
{
    qDebug() << "[PcpNative] thumbnail:" << path << w << "x" << h;
    Q_UNUSED(path);
    Q_UNUSED(w);
    Q_UNUSED(h);
    return QImage();
}
#endif

// ============================================================================
// Auth / Account
// ============================================================================

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::buildLoginBody(const QString& user, const QString& pass,
                                              const QString& homeserver)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::buildLoginBody(const QString& user, const QString& pass,
                                              const QString& homeserver)
{
    qDebug() << "[PcpNative] buildLoginBody user:" << user << "hs:" << homeserver;
    Q_UNUSED(pass);
    QJsonObject body;
    body[QStringLiteral("type")] = QStringLiteral("m.login.password");
    body[QStringLiteral("user")] = user;
    body[QStringLiteral("password")] = QStringLiteral("********");
    body[QStringLiteral("initial_device_display_name")] = QStringLiteral("PCP Android");
    return body;
}
#endif

#ifdef USE_PCP_NATIVE
bool PcpNative::validatePassword(const QString& pass)
{
    return false; // real impl
}
#else
bool PcpNative::validatePassword(const QString& pass)
{
    qDebug() << "[PcpNative] validatePassword len:" << pass.size();
    Q_UNUSED(pass);
    return true;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::encryptData(const QString& data, const QString& key)
{
    return QString(); // real impl
}
#else
QString PcpNative::encryptData(const QString& data, const QString& key)
{
    qDebug() << "[PcpNative] encryptData data-len:" << data.size() << "key-len:" << key.size();
    Q_UNUSED(key);
    return data;
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::decryptData(const QString& data, const QString& key)
{
    return QString(); // real impl
}
#else
QString PcpNative::decryptData(const QString& data, const QString& key)
{
    qDebug() << "[PcpNative] decryptData data-len:" << data.size() << "key-len:" << key.size();
    Q_UNUSED(key);
    return data;
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::exportAccount(const QString& userId)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::exportAccount(const QString& userId)
{
    qDebug() << "[PcpNative] exportAccount:" << userId;
    QJsonObject account;
    account[QStringLiteral("user_id")] = userId;
    account[QStringLiteral("exported_at")] = QDateTime::currentDateTimeUtc().toString(Qt::ISODate);
    return account;
}
#endif

#ifdef USE_PCP_NATIVE
bool PcpNative::importAccount(const QJsonObject& data)
{
    return false; // real impl
}
#else
bool PcpNative::importAccount(const QJsonObject& data)
{
    qDebug() << "[PcpNative] importAccount";
    Q_UNUSED(data);
    return true;
}
#endif

// ============================================================================
// Misc
// ============================================================================

#ifdef USE_PCP_NATIVE
bool PcpNative::isBidiTextSafe(const QString& text)
{
    return false; // real impl
}
#else
bool PcpNative::isBidiTextSafe(const QString& text)
{
    qDebug() << "[PcpNative] isBidiTextSafe len:" << text.size();
    Q_UNUSED(text);
    return true;
}
#endif

#ifdef USE_PCP_NATIVE
QStringList PcpNative::spellcheck(const QString& text)
{
    return QStringList(); // real impl
}
#else
QStringList PcpNative::spellcheck(const QString& text)
{
    qDebug() << "[PcpNative] spellcheck len:" << text.size();
    Q_UNUSED(text);
    return QStringList();
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::emojiAnalysis(const QString& text)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::emojiAnalysis(const QString& text)
{
    qDebug() << "[PcpNative] emojiAnalysis len:" << text.size();
    Q_UNUSED(text);
    QJsonObject result;
    result[QStringLiteral("count")] = 0;
    result[QStringLiteral("list")] = QJsonArray();
    return result;
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::buildSearchIndex(const QJsonArray& events)
{
    // real impl
}
#else
void PcpNative::buildSearchIndex(const QJsonArray& events)
{
    qDebug() << "[PcpNative] buildSearchIndex count:" << events.size();
    Q_UNUSED(events);
}
#endif

#ifdef USE_PCP_NATIVE
QJsonArray PcpNative::searchEvents(const QString& query)
{
    return QJsonArray(); // real impl
}
#else
QJsonArray PcpNative::searchEvents(const QString& query)
{
    qDebug() << "[PcpNative] searchEvents:" << query;
    Q_UNUSED(query);
    return QJsonArray();
}
#endif

#ifdef USE_PCP_NATIVE
bool PcpNative::desyncDetector(const QString& roomId)
{
    return false; // real impl
}
#else
bool PcpNative::desyncDetector(const QString& roomId)
{
    qDebug() << "[PcpNative] desyncDetector:" << roomId;
    Q_UNUSED(roomId);
    return false;
}
#endif

#ifdef USE_PCP_NATIVE
void PcpNative::trackLatency(const QString& server, qint64 ms)
{
    // real impl
}
#else
void PcpNative::trackLatency(const QString& server, qint64 ms)
{
    qDebug() << "[PcpNative] trackLatency" << server << ms << "ms";
}
#endif

#ifdef USE_PCP_NATIVE
QString PcpNative::contentFilter(const QString& text)
{
    return QString(); // real impl
}
#else
QString PcpNative::contentFilter(const QString& text)
{
    qDebug() << "[PcpNative] contentFilter len:" << text.size();
    return text;
}
#endif

#ifdef USE_PCP_NATIVE
QJsonObject PcpNative::masquerade(const QString& userId)
{
    return QJsonObject(); // real impl
}
#else
QJsonObject PcpNative::masquerade(const QString& userId)
{
    qDebug() << "[PcpNative] masquerade:" << userId;
    QJsonObject result;
    result[QStringLiteral("user_id")] = userId;
    result[QStringLiteral("display_name")] = userId;
    result[QStringLiteral("avatar_url")] = QString();
    return result;
}
#endif
