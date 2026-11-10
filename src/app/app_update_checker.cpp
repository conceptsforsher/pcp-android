#include "app_update_checker.hpp"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QApplication>
#include <QDebug>

namespace pcp_chat {

static const QString UPDATE_URL = "https://pcp.chat/api/updates/latest.json";

AppUpdateChecker &AppUpdateChecker::instance()
{
    static AppUpdateChecker inst;
    return inst;
}

AppUpdateChecker::AppUpdateChecker(QObject *parent) : QObject(parent) {}
void AppUpdateChecker::checkForUpdates() { instance().performCheck(); }
AppUpdateChecker::UpdateInfo AppUpdateChecker::latestUpdateInfo() { return instance().m_latestInfo; }
bool AppUpdateChecker::isUpdateAvailable() { return instance().m_latestInfo.available; }

void AppUpdateChecker::performCheck()
{
    auto *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(UPDATE_URL));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = manager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply, manager]() {
        if (reply->error() == QNetworkReply::NoError) {
            parseResponse(reply->readAll());
        }
        reply->deleteLater();
        manager->deleteLater();
    });
}

void AppUpdateChecker::parseResponse(const QByteArray &data)
{
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) return;

    QJsonObject obj = doc.object();
    m_latestInfo.version = obj["version"].toString();
    m_latestInfo.url = obj["url"].toString();
    m_latestInfo.changelog = obj["changelog"].toString();
    m_latestInfo.mandatory = obj["mandatory"].toBool();
    m_latestInfo.sizeBytes = obj["size"].toInteger();

    QVersionNumber latest = QVersionNumber::fromString(m_latestInfo.version);
    QVersionNumber current = QVersionNumber::fromString(QApplication::applicationVersion());

    if (latest > current) {
        m_latestInfo.available = true;
        emit updateAvailable(m_latestInfo);
    }
    m_checked = true;
}

void AppUpdateChecker::downloadUpdate()
{
    // Initiate download of the update package
}

} // namespace pcp_chat
