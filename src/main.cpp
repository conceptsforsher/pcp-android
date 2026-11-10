// PCP — 100% C++ Qt6 desktop Matrix client
// Entry point: parses CLI args, initializes application, runs event loop

#include "app/application.hpp"
#include <QCommandLineParser>
#include <QDir>
#include <QStandardPaths>
#include <iostream>

int main(int argc, char *argv[])
{
    // High-DPI support
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
#endif

    pcp_chat::Application app(argc, argv);

    QCoreApplication::setOrganizationName("PCP");
    QCoreApplication::setOrganizationDomain("pcp.chat");
    QCoreApplication::setApplicationName("PCP");
    QCoreApplication::setApplicationVersion("0.3.0");

    // Setup configuration directory
    const QString configDir = QStandardPaths::writableLocation(
        QStandardPaths::AppConfigLocation);
    QDir().mkpath(configDir);
    app.setConfigDirectory(configDir);

    // Parse command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription(
        "PCP — 100% C++ desktop Matrix/IRC/Lemmy client");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption debugOption(
        QStringList() << "d" << "debug",
        "Enable debug logging");
    parser.addOption(debugOption);

    QCommandLineOption logFileOption(
        QStringList() << "l" << "log-file",
        "Write logs to <file>",
        "file");
    parser.addOption(logFileOption);

    QCommandLineOption configOption(
        QStringList() << "c" << "config",
        "Use <directory> as config directory",
        "directory");
    parser.addOption(configOption);

    QCommandLineOption noCrashReportOption(
        "no-crash-report",
        "Disable automatic crash reporting");
    parser.addOption(noCrashReportOption);

    QCommandLineOption proxyOption(
        "proxy",
        "Use <url> as proxy (socks5:// or http://)",
        "url");
    parser.addOption(proxyOption);

    parser.process(app);

    // Apply parsed options
    app.setDebugLogging(parser.isSet(debugOption));
    if (parser.isSet(logFileOption))
        app.setLogFile(parser.value(logFileOption));
    if (parser.isSet(configOption))
        app.setConfigDirectory(parser.value(configOption));
    if (parser.isSet(noCrashReportOption))
        app.setCrashReportingEnabled(false);
    if (parser.isSet(proxyOption))
        app.setProxyUrl(parser.value(proxyOption));

    // Initialize application subsystems
    if (!app.initialize()) {
        std::cerr << "Failed to initialize application" << std::endl;
        return 1;
    }

    // Run the application
    return app.run();
}
