#ifndef PROGRESSIVE_IRC_CLIENT_HPP
#define PROGRESSIVE_IRC_CLIENT_HPP

#include "irc_types.hpp"
#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <memory>
#include <map>

namespace progressive {
namespace irc {

using MessageCallback = std::function<void(const IrcMessage&)>;
using StateCallback = std::function<void(IrcConnectionState)>;
using ErrorCallback = std::function<void(int code, const std::string& message)>;

class IrcClient {
public:
    IrcClient();
    ~IrcClient();

    IrcClient(const IrcClient&) = delete;
    IrcClient& operator=(const IrcClient&) = delete;

    void connect(const IrcServerConfig& config);
    void disconnect();
    void reconnect();

    void sendRaw(const std::string& line);
    void join(const std::string& channel);
    void part(const std::string& channel, const std::string& reason = "");
    void privmsg(const std::string& target, const std::string& text);
    void notice(const std::string& target, const std::string& text);
    void nick(const std::string& newNick);
    void whois(const std::string& nick);
    void topic(const std::string& channel, const std::string& newTopic = "");
    void names(const std::string& channel);
    void kick(const std::string& channel, const std::string& nick, const std::string& reason = "");
    void mode(const std::string& channel, const std::string& modes, const std::string& param = "");
    void quit(const std::string& reason = "Leaving");
    void pingServer();
    void ctcpRequest(const std::string& target, const std::string& request);
    void ctcpReply(const std::string& target, const std::string& reply);

    void setMessageCallback(MessageCallback cb);
    void setStateCallback(StateCallback cb);
    void setErrorCallback(ErrorCallback cb);

    IrcConnectionState getState() const { return state.load(); }
    bool isConnected() const { return state == IrcConnectionState::REGISTERED; }
    std::string getNick() const { return config.nick; }
    std::string getServerHost() const { return config.host; }

private:
    void receiveLoop();
    void processLine(const std::string& line);
    void handleMessage(const IrcMessage& msg);
    void handleNickConflict();
    void startPingTimer();
    void stopPingTimer();

    IrcServerConfig config;
    std::atomic<IrcConnectionState> state{IrcConnectionState::DISCONNECTED};

    int sockFd = -1;
    std::unique_ptr<std::thread> recvThread;
    std::unique_ptr<std::thread> pingThread;
    std::atomic<bool> running{false};

    std::mutex sendMutex;
    std::mutex callbackMutex;

    MessageCallback messageCallback;
    StateCallback stateCallback;
    ErrorCallback errorCallback;

    std::string readBuffer;
    int nickConflictCounter = 0;

    std::vector<std::string> joinedChannels;
};

std::string formatIrcMessage(const std::string& command, const std::vector<std::string>& params, const std::string& trailing = "");

} // namespace irc
} // namespace progressive

#endif // PROGRESSIVE_IRC_CLIENT_HPP
