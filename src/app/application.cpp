#include "app/application.hpp"

#include <QSettings>

#include "irc/irc_session_holder.hpp"
#include "lemmy/lemmy_session_holder.hpp"
#include "matrix/matrix_session.hpp"
#include "protocol/protocol_manager.hpp"

namespace pcp {

Application& Application::instance()
{
    static Application app;
    return app;
}

Application::Application(QObject* parent)
    : QObject(parent)
{
}

Application::~Application()
{
    if (m_initialized) {
        shutdown();
    }
}

void Application::loadSettings()
{
    QSettings settings;
    loadMatrixSettings();
    loadIrcSettings();
    loadLemmySettings();
    loadPreferences();
}

void Application::loadMatrixSettings()
{
    Q_UNUSED(this);
}

void Application::loadIrcSettings()
{
    Q_UNUSED(this);
}

void Application::loadLemmySettings()
{
    Q_UNUSED(this);
}

void Application::loadPreferences()
{
    Q_UNUSED(this);
}

void Application::startProtocolManager()
{
    if (m_initialized) {
        return;
    }

    auto &pm = ProtocolManager::instance();
    connect(&pm, &ProtocolManager::overallStateChanged,
            this, [this](ConnectionState state) {
                emit connectionStateChanged(ProtocolType::MATRIX, state);
            });

    pm.openAll();
    m_initialized = true;
    emit initialized();
}

void Application::shutdown()
{
    if (!m_initialized) {
        return;
    }

    ProtocolManager::instance().closeAll();
    IrcSessionHolder::instance()->disconnectAll();
    LemmySessionHolder::instance()->disconnectAll();

    m_initialized = false;
    emit shutdownCompleted();
}

ProtocolManager* Application::protocolManager() const
{
    return &ProtocolManager::instance();
}

MatrixSession* Application::matrixSession() const
{
    auto *session = ProtocolManager::instance().getSession(ProtocolType::MATRIX);
    return qobject_cast<MatrixSession*>(session);
}

IrcSessionHolder* Application::ircSessionHolder() const
{
    return IrcSessionHolder::instance();
}

LemmySessionHolder* Application::lemmySessionHolder() const
{
    return LemmySessionHolder::instance();
}

void Application::addIrcSession()
{
    // Placeholder - IRC session creation would go here
}

void Application::addLemmySession()
{
    // Placeholder - Lemmy session creation would go here
}

} // namespace pcp
