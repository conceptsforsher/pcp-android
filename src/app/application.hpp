#pragma once

#include <QObject>

#include "protocol/protocol_type.hpp"

class ProtocolManager;
class MatrixSession;
class IrcSessionHolder;
class LemmySessionHolder;

namespace pcp {

class Application : public QObject
{
    Q_OBJECT

public:
    static Application& instance();

    void loadSettings();
    void startProtocolManager();
    void shutdown();

    ProtocolManager* protocolManager() const;
    MatrixSession* matrixSession() const;
    IrcSessionHolder* ircSessionHolder() const;
    LemmySessionHolder* lemmySessionHolder() const;

    void addIrcSession();
    void addLemmySession();

signals:
    void connectionStateChanged(ProtocolType type, ConnectionState state);
    void initialized();
    void shutdownCompleted();

private:
    Application(QObject* parent = nullptr);
    ~Application() override;
    Q_DISABLE_COPY(Application)

    void loadMatrixSettings();
    void loadIrcSettings();
    void loadLemmySettings();
    void loadPreferences();

    bool m_initialized = false;
};

} // namespace pcp
