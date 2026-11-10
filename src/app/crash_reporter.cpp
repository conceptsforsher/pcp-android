#include "crash_reporter.hpp"

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStandardPaths>
#include <QSysInfo>
#include <QTextStream>

#include <csignal>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>

#ifdef __linux__
#include <execinfo.h>
#endif

// ---------------------------------------------------------------------------
// Static members
// ---------------------------------------------------------------------------

CrashReporter *CrashReporter::s_instance = nullptr;
QByteArray CrashReporter::s_crashDirBytes;
QByteArray CrashReporter::s_appVersionBytes;
QByteArray CrashReporter::s_osInfoBytes;

// ---------------------------------------------------------------------------
// CrashInfo
// ---------------------------------------------------------------------------

QString CrashInfo::toString() const
{
    QString result;
    QTextStream ts(&result);
    ts << "=== Crash Report ===\n";
    ts << "Timestamp: " << timestamp.toString(Qt::ISODate) << '\n';
    ts << "Signal: " << signalName << " (" << signalNumber << ")\n";
    ts << "App Version: " << appVersion << '\n';
    ts << "OS: " << osInfo << "\n\nStack Trace:\n";
    for (int i = 0; i < stackTrace.size(); ++i)
        ts << "  #" << i << ' ' << stackTrace[i] << '\n';
    return result;
}

QString CrashInfo::signalNameFromNumber(int sig)
{
    switch (sig) {
    case SIGSEGV: return QStringLiteral("SIGSEGV");
    case SIGABRT: return QStringLiteral("SIGABRT");
    case SIGFPE:  return QStringLiteral("SIGFPE");
    case SIGILL:  return QStringLiteral("SIGILL");
    case SIGBUS:  return QStringLiteral("SIGBUS");
    default:      return QStringLiteral("UNKNOWN(%1)").arg(sig);
    }
}

// ---------------------------------------------------------------------------
// CrashReporter
// ---------------------------------------------------------------------------

CrashReporter::CrashReporter(QObject *parent)
    : QObject(parent)
{
    m_crashDir =
        QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation)
        + QStringLiteral("/pcp-chat/crashes");
    if (m_crashDir.isEmpty()
        || !m_crashDir.startsWith(QLatin1Char('/'))) {
        m_crashDir = QStringLiteral("/tmp/pcp-chat/crashes");
    }

#ifdef __linux__
    m_osInfo = QStringLiteral("Linux %1 (%2)")
                   .arg(QSysInfo::kernelVersion(),
                        QSysInfo::currentCpuArchitecture());
#else
    m_osInfo = QSysInfo::prettyProductName();
#endif
}

CrashReporter::~CrashReporter() = default;

CrashReporter *CrashReporter::instance()
{
    if (!s_instance)
        s_instance = new CrashReporter(qApp);
    return s_instance;
}

void CrashReporter::init(const QString &appVersion)
{
    if (m_initialized)
        return;
    m_initialized = true;

    m_appVersion = appVersion;

    // Pre-serialize strings for async-signal-safe access in the handler.
    s_crashDirBytes = m_crashDir.toUtf8();
    s_appVersionBytes = m_appVersion.toUtf8();
    s_osInfoBytes = m_osInfo.toUtf8();

    QDir().mkpath(m_crashDir);

    installSignalHandlers();
}

void CrashReporter::installSignalHandlers()
{
    auto install = [](int sig) {
        struct sigaction sa {};
        sa.sa_handler = &CrashReporter::signalHandler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESETHAND; // restore default after first catch
        sigaction(sig, &sa, nullptr);
    };

    install(SIGSEGV);
    install(SIGABRT);
    install(SIGFPE);
}

QString CrashReporter::osInformation()
{
#ifdef __linux__
    return QStringLiteral("Linux %1 (%2)")
        .arg(QSysInfo::kernelVersion(), QSysInfo::currentCpuArchitecture());
#else
    return QSysInfo::prettyProductName();
#endif
}

// ---------------------------------------------------------------------------
// Async-signal-safe crash handler
//
// Only POSIX async-signal-safe functions are deliberately used here.
// snprintf() is included as a widely-accepted practical extension; glibc's
// implementation is thread-safe and commonly employed in crash handlers.
// ---------------------------------------------------------------------------

void CrashReporter::signalHandler(int sig)
{
    // Build a time-based filename inside s_crashDirBytes.
    // We write directly under s_crashDirBytes with /crash_<epoch>.log

    char filePath[1024];
    time_t now = time(nullptr);

    {
        // Manual path construction to avoid non-safe functions.
        const char *dir = s_crashDirBytes.constData();
        int dirLen = s_crashDirBytes.size();
        // Cap directory length
        if (dirLen > 900) dirLen = 900;

        memcpy(filePath, dir, dirLen);
        int pos = dirLen;
        // Write "/crash_"
        memcpy(filePath + pos, "/crash_", 7);
        pos += 7;
        // Write epoch seconds as decimal
        pos += snprintf(filePath + pos, sizeof(filePath) - pos, "%lld",
                        static_cast<long long>(now));
        memcpy(filePath + pos, ".log", 4);
        pos += 4;
        filePath[pos] = '\0';
    }

    int fd = open(filePath, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd < 0) {
        // Fallback: write to stderr
        fd = STDERR_FILENO;
    }

    // Signal name
    const char *sigName = "UNKNOWN";
    switch (sig) {
    case SIGSEGV: sigName = "SIGSEGV"; break;
    case SIGABRT: sigName = "SIGABRT"; break;
    case SIGFPE:  sigName = "SIGFPE";  break;
    case SIGILL:  sigName = "SIGILL";  break;
    case SIGBUS:  sigName = "SIGBUS";  break;
    }

    // Build header
    char buf[2048];
    int len = snprintf(buf, sizeof(buf),
                       "=== Crash Report ===\n"
                       "Timestamp: %lld\n"
                       "Signal: %s (%d)\n"
                       "App Version: %s\n"
                       "OS: %s\n\nStack Trace:\n",
                       static_cast<long long>(now), sigName, sig,
                       s_appVersionBytes.constData(),
                       s_osInfoBytes.constData());
    write(fd, buf, static_cast<size_t>(len));

#ifdef __linux__
    void *frames[128];
    int frameCount = backtrace(frames, 128);

    // Write raw addresses (safe)
    for (int i = 0; i < frameCount; ++i) {
        len = snprintf(buf, sizeof(buf), "  #%d %p\n", i, frames[i]);
        write(fd, buf, static_cast<size_t>(len));
    }

    // Write human-readable symbols (async-signal-safe on Linux/glibc)
    if (fd != STDERR_FILENO) {
        backtrace_symbols_fd(frames, frameCount, fd);
    }
#endif

    write(fd, "\n--- End of Crash Report ---\n", 28);

    if (fd != STDERR_FILENO)
        close(fd);

    // Re-raise with default handler (generates core dump if enabled)
    signal(sig, SIG_DFL);
    raise(sig);
}

// ---------------------------------------------------------------------------
// Previous crash detection
// ---------------------------------------------------------------------------

void CrashReporter::checkForPreviousCrash()
{
    QDir dir(m_crashDir);
    QStringList filters{QStringLiteral("crash_*.log")};
    QFileInfoList files =
        dir.entryInfoList(filters, QDir::Files, QDir::Time);

    if (files.isEmpty())
        return;

    const QFileInfo &latest = files.first();
    QFile file(latest.absoluteFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QString crashLog = QString::fromUtf8(file.readAll());
    file.close();

    // Parse timestamp from content or fall back to file modification time
    QDateTime ts = latest.lastModified();

    const QString tsPrefix = QStringLiteral("Timestamp: ");
    int tsIdx = crashLog.indexOf(tsPrefix);
    if (tsIdx >= 0) {
        QString tsStr =
            crashLog.mid(tsIdx + tsPrefix.length(),
                         crashLog.indexOf(QLatin1Char('\n'), tsIdx)
                             - tsIdx - tsPrefix.length())
                .trimmed();
        QDateTime parsed = QDateTime::fromString(tsStr, Qt::ISODate);
        if (!parsed.isValid())
            parsed.setSecsSinceEpoch(tsStr.toLongLong());
        if (parsed.isValid())
            ts = parsed;
    }

    m_lastCrash.timestamp = ts;
    m_lastCrash.signalNumber = 0;
    m_lastCrash.appVersion = m_appVersion;
    m_lastCrash.osInfo = m_osInfo;
    m_lastCrash.stackTrace.clear();

    // Move the file to a "reported" subdirectory so we don't re-report it
    QString reportedDir = m_crashDir + QStringLiteral("/reported");
    QDir().mkpath(reportedDir);
    QFile::rename(latest.absoluteFilePath(),
                  reportedDir + QLatin1Char('/') + latest.fileName());

    emit previousCrashDetected(crashLog, ts);
}

// ---------------------------------------------------------------------------
// Report sending (stub)
// ---------------------------------------------------------------------------

void CrashReporter::sendCrashReport(const QString &crashLog)
{
    if (m_reportEndpoint.isEmpty()) {
        // Stub: save locally
        QString reportedDir = m_crashDir + QStringLiteral("/reported");
        QDir().mkpath(reportedDir);

        QFile file(reportedDir
                   + QStringLiteral("/crash_report_%1.txt")
                         .arg(QDateTime::currentDateTime()
                                  .toString(QStringLiteral("yyyyMMdd_HHmmss"))));
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream stream(&file);
            stream << crashLog;
        }
        return;
    }

    // When an endpoint is configured, HTTP POST would go here.
    // For now, we treat the presence of an endpoint the same way
    // (local save) as the stub path above.
    QString reportedDir = m_crashDir + QStringLiteral("/reported");
    QDir().mkpath(reportedDir);

    QFile file(reportedDir
               + QStringLiteral("/crash_report_%1.txt")
                     .arg(QDateTime::currentDateTime()
                              .toString(QStringLiteral("yyyyMMdd_HHmmss"))));
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "Endpoint: " << m_reportEndpoint << '\n';
        stream << crashLog;
    }
}

void CrashReporter::setReportEndpoint(const QString &url)
{
    m_reportEndpoint = url;
}

CrashInfo CrashReporter::lastCrash() const
{
    return m_lastCrash;
}

QString CrashReporter::crashDirectory() const
{
    return m_crashDir;
}
