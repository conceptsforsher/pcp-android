#pragma once

#include <QObject>
#include <QString>
#include <QTimer>
#include <QHash>
#include <QJsonObject>

class QNetworkAccessManager;

namespace pcp_chat {

class MatrixSession;

class MatrixSyncManager : public QObject
{
    Q_OBJECT

public:
    explicit MatrixSyncManager(MatrixSession *session, QObject *parent = nullptr);
    ~MatrixSyncManager() override;

    void start();
    void stop();
    void forceSync();

    QString nextBatch() const { return m_nextBatch; }
    bool isSyncing() const { return m_syncing; }

    enum class SyncState { Idle, Syncing, Error, Retrying };
    SyncState state() const { return m_state; }

signals:
    void syncStateChanged(SyncState state);
    void syncCompleted(const QString &nextBatch);
    void syncError(const QString &message, int retryAfterMs);
    void roomsUpdated(const QJsonObject &roomsData);
    void presenceUpdated(const QJsonObject &presenceData);
    void accountDataUpdated(const QJsonObject &accountData);

private:
    void processSync();
    void handleSyncResponse(const QByteArray &response);
    void handleSyncError(int statusCode, const QString &error);
    void scheduleNextSync();
    void scheduleRetry(int retryAfterMs);

    MatrixSession *m_session;
    QNetworkAccessManager *m_network;
    QString m_nextBatch;
    QTimer m_syncTimer;
    QTimer m_retryTimer;
    bool m_syncing = false;
    bool m_initialSync = true;
    SyncState m_state = SyncState::Idle;
    int m_consecutiveErrors = 0;
    int m_syncTimeoutMs = 30000;
};

} // namespace pcp_chat
