#include "matrix_sync_manager.hpp"
#include "matrix_session.hpp"

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QCryptographicHash>
#include <QDebug>

namespace pcp_chat {

MatrixSyncManager::MatrixSyncManager(MatrixSession *session, QObject *parent)
    : QObject(parent)
    , m_session(session)
    , m_network(session->findChild<QNetworkAccessManager *>())
{
}

MatrixSyncManager::~MatrixSyncManager()
{
    stop();
}

void MatrixSyncManager::start()
{
    if (m_syncing) return;

    m_initialSync = true;
    m_consecutiveErrors = 0;

    QObject::connect(&m_syncTimer, &QTimer::timeout, this, &MatrixSyncManager::processSync);
    processSync();
}

void MatrixSyncManager::stop()
{
    m_syncTimer.stop();
    m_retryTimer.stop();
    m_syncing = false;
    m_state = SyncState::Idle;
    emit syncStateChanged(m_state);
}

void MatrixSyncManager::forceSync()
{
    if (m_syncTimer.isActive())
        m_syncTimer.stop();
    processSync();
}

void MatrixSyncManager::processSync()
{
    if (!m_session || !m_session->isConnected())
        return;

    m_syncing = true;
    m_state = SyncState::Syncing;
    emit syncStateChanged(m_state);

    if (!m_network) {
        m_network = m_session->findChild<QNetworkAccessManager *>();
        if (!m_network) {
            handleSyncError(500, "No network manager");
            return;
        }
    }

    QString homeserver = m_session->homeserver();
    QUrl url(homeserver + "/_matrix/client/v3/sync");
    QUrlQuery query;

    if (!m_nextBatch.isEmpty()) {
        query.addQueryItem("since", m_nextBatch);
    }

    // Set sync filter
    if (m_initialSync) {
        query.addQueryItem("full_state", "true");
    }

    query.addQueryItem("timeout", QString::number(m_syncTimeoutMs));
    url.setQuery(query);

    QNetworkRequest request(url);
    request.setRawHeader("Authorization",
                         ("Bearer " + m_session->accessToken()).toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setTransferTimeout(m_syncTimeoutMs + 15000);

    auto *reply = m_network->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        if (reply->error() == QNetworkReply::NoError) {
            handleSyncResponse(reply->readAll());
        } else {
            int statusCode = reply->attribute(
                QNetworkRequest::HttpStatusCodeAttribute).toInt();
            handleSyncError(statusCode, reply->errorString());
        }
        reply->deleteLater();
    });
}

void MatrixSyncManager::handleSyncResponse(const QByteArray &response)
{
    m_syncing = false;
    m_consecutiveErrors = 0;

    QJsonDocument doc = QJsonDocument::fromJson(response);
    if (!doc.isObject()) {
        handleSyncError(500, "Invalid sync response");
        return;
    }

    QJsonObject obj = doc.object();
    QString prevBatch = m_nextBatch;
    m_nextBatch = obj["next_batch"].toString();

    // Process room data
    if (obj.contains("rooms")) {
        QJsonObject rooms = obj["rooms"].toObject();
        emit roomsUpdated(rooms);
    }

    // Process presence
    if (obj.contains("presence")) {
        emit presenceUpdated(obj["presence"].toObject());
    }

    // Process account data
    if (obj.contains("account_data")) {
        emit accountDataUpdated(obj["account_data"].toObject());
    }

    m_initialSync = false;
    m_state = SyncState::Idle;
    emit syncStateChanged(m_state);
    emit syncCompleted(m_nextBatch);

    // Schedule next sync
    scheduleNextSync();
}

void MatrixSyncManager::handleSyncError(int statusCode, const QString &error)
{
    m_syncing = false;
    m_consecutiveErrors++;

    QString errorMsg = QString("Sync error (HTTP %1): %2").arg(statusCode).arg(error);
    qWarning() << errorMsg;

    m_state = SyncState::Error;
    emit syncStateChanged(m_state);

    // Determine retry delay
    int retryAfter = 1000; // Default 1 second

    if (statusCode == 401 || statusCode == 403) {
        // Auth failure - stop syncing
        m_state = SyncState::Error;
        emit syncError("Authentication failed", 0);
        stop();
        return;
    }

    if (statusCode == 429) {
        retryAfter = 5000; // Rate limited
    } else if (m_consecutiveErrors > 5) {
        retryAfter = 30000; // Exponential backoff
    } else if (m_consecutiveErrors > 3) {
        retryAfter = 10000;
    } else {
        retryAfter = 5000;
    }

    m_state = SyncState::Retrying;
    emit syncStateChanged(m_state);
    emit syncError(errorMsg, retryAfter);

    scheduleRetry(retryAfter);
}

void MatrixSyncManager::scheduleNextSync()
{
    if (!m_syncTimer.isActive()) {
        m_syncTimer.start(30000);
    }
}

void MatrixSyncManager::scheduleRetry(int retryAfterMs)
{
    m_syncTimer.stop();
    QTimer::singleShot(retryAfterMs, this, &MatrixSyncManager::processSync);
}

} // namespace pcp_chat
