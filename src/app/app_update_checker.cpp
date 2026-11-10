#include "app_update_checker.hpp"

#include <QDesktopServices>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QTimer>
#include <QUrl>

namespace pcp {

static constexpr int kPeriodicCheckIntervalMs = 24 * 60 * 60 * 1000;
static const QString kReleasesUrl =
    QStringLiteral("https://api.github.com/repos/pcp-chat/pcp-android-concept/releases/latest");

AppUpdateChecker &AppUpdateChecker::instance()
{
    static AppUpdateChecker checker;
    return checker;
}

AppUpdateChecker::AppUpdateChecker(QObject *parent)
    : QObject(parent)
    , m_nam(new QNetworkAccessManager(this))
    , m_periodicTimer(new QTimer(this))
{
}

AppUpdateChecker::~AppUpdateChecker() = default;

void AppUpdateChecker::checkForUpdates()
{
    QUrl url(kReleasesUrl);
    QNetworkRequest request{url};
    request.setHeader(QNetworkRequest::ContentTypeHeader,
                      QStringLiteral("application/json"));
    request.setRawHeader("Accept", "application/vnd.github+json");
    request.setRawHeader("User-Agent", "PcpAndroidConcept");

    QNetworkReply *reply = m_nam->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onReplyFinished(reply);
    });
}

void AppUpdateChecker::checkForUpdatesPeriodically()
{
    m_periodicTimer->setInterval(kPeriodicCheckIntervalMs);
    m_periodicTimer->setSingleShot(false);
    connect(m_periodicTimer, &QTimer::timeout, this, &AppUpdateChecker::checkForUpdates);

    if (!m_periodicTimer->isActive()) {
        m_periodicTimer->start();
        checkForUpdates();
    }
}

void AppUpdateChecker::setCurrentVersion(const QString &version)
{
    m_currentVersion = version;
}

QString AppUpdateChecker::currentVersion() const
{
    return m_currentVersion;
}

bool AppUpdateChecker::isUpdateAvailable() const
{
    return m_updateAvailable;
}

QString AppUpdateChecker::latestVersion() const
{
    return m_latestVersion;
}

QString AppUpdateChecker::downloadUrl() const
{
    return m_downloadUrl;
}

QString AppUpdateChecker::releaseNotes() const
{
    return m_releaseNotes;
}

void AppUpdateChecker::openDownloadPage()
{
    if (!m_downloadUrl.isEmpty()) {
        QDesktopServices::openUrl(QUrl(m_downloadUrl));
    }
}

void AppUpdateChecker::onReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        m_updateAvailable = false;
        emit checkFailed(reply->errorString());
        return;
    }

    const QByteArray data = reply->readAll();
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);

    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        m_updateAvailable = false;
        emit checkFailed(QStringLiteral("Failed to parse release info"));
        return;
    }

    const QJsonObject json = doc.object();
    QString tagName = json.value(QStringLiteral("tag_name")).toString();

    if (tagName.isEmpty()) {
        m_updateAvailable = false;
        emit checkFailed(QStringLiteral("No tag_name in release info"));
        return;
    }

    if (tagName.startsWith(QLatin1Char('v')) || tagName.startsWith(QLatin1Char('V'))) {
        tagName = tagName.mid(1);
    }

    const QVersionNumber latest = QVersionNumber::fromString(tagName);
    const QVersionNumber current = QVersionNumber::fromString(m_currentVersion);

    if (latest.isNull() || current.isNull()) {
        m_updateAvailable = false;
        emit checkFailed(QStringLiteral("Invalid version format"));
        return;
    }

    const int cmp = QVersionNumber::compare(latest, current);

    if (cmp > 0) {
        m_latestVersion = tagName;
        m_downloadUrl = json.value(QStringLiteral("html_url")).toString();
        m_releaseNotes = json.value(QStringLiteral("body")).toString();
        m_updateAvailable = true;
        emit updateAvailable(m_latestVersion, m_downloadUrl, m_releaseNotes);
    } else {
        m_updateAvailable = false;
        emit noUpdateAvailable();
    }
}

} // namespace pcp
