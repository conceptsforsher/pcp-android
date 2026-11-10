#include "pcp/irc_client.hpp"
#include "pcp/irc_parser.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>
#include <cstring>
#include <chrono>
#include <sstream>

namespace pcp {
namespace irc {

IrcClient::IrcClient() {}

IrcClient::~IrcClient() {
    disconnect();
}

static std::string resolveHost(const std::string& host) {
    struct addrinfo hints{}, *res;
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    int ret = getaddrinfo(host.c_str(), nullptr, &hints, &res);
    if (ret != 0 || !res) return "";

    char ip[INET_ADDRSTRLEN];
    auto* addr = reinterpret_cast<struct sockaddr_in*>(res->ai_addr);
    inet_ntop(AF_INET, &addr->sin_addr, ip, sizeof(ip));
    std::string result(ip);
    freeaddrinfo(res);
    return result;
}

void IrcClient::connect(const IrcServerConfig& cfg) {
    if (running) disconnect();
    config = cfg;
    nickConflictCounter = 0;

    state = IrcConnectionState::CONNECTING;
    if (stateCallback) stateCallback(state);

    sockFd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockFd < 0) {
        state = IrcConnectionState::ERROR;
        if (errorCallback) errorCallback(errno, strerror(errno));
        if (stateCallback) stateCallback(state);
        return;
    }

    struct sockaddr_in server{};
    server.sin_family = AF_INET;
    server.sin_port = htons(config.port);

    std::string ip = resolveHost(config.host);
    if (ip.empty() || inet_pton(AF_INET, ip.c_str(), &server.sin_addr) <= 0) {
        state = IrcConnectionState::ERROR;
        if (errorCallback) errorCallback(errno, "Failed to resolve host");
        if (stateCallback) stateCallback(state);
        close(sockFd);
        sockFd = -1;
        return;
    }

    struct timeval tv{5, 0};
    setsockopt(sockFd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    setsockopt(sockFd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv));

    if (::connect(sockFd, reinterpret_cast<struct sockaddr*>(&server), sizeof(server)) < 0) {
        state = IrcConnectionState::ERROR;
        if (errorCallback) errorCallback(errno, strerror(errno));
        if (stateCallback) stateCallback(state);
        close(sockFd);
        sockFd = -1;
        return;
    }

    state = IrcConnectionState::CONNECTED;
    if (stateCallback) stateCallback(state);

    if (!config.password.empty()) {
        sendRaw("PASS " + config.password);
    }

    sendRaw("NICK " + config.nick);
    sendRaw("USER " + config.user + " 0 * :" + config.realName);

    running = true;
    recvThread = std::make_unique<std::thread>(&IrcClient::receiveLoop, this);
    startPingTimer();
}

void IrcClient::disconnect() {
    stopPingTimer();

    if (state > IrcConnectionState::DISCONNECTED) {
        sendRaw("QUIT :" + std::string(config.realName));
    }

    running = false;

    if (sockFd >= 0) {
        shutdown(sockFd, SHUT_RDWR);
        close(sockFd);
        sockFd = -1;
    }

    if (recvThread && recvThread->joinable()) {
        recvThread->join();
    }
    recvThread.reset();

    state = IrcConnectionState::DISCONNECTED;
    if (stateCallback) stateCallback(state);
}

void IrcClient::reconnect() {
    disconnect();
    connect(config);
}

void IrcClient::sendRaw(const std::string& line) {
    std::lock_guard<std::mutex> lock(sendMutex);
    if (sockFd < 0) return;

    std::string msg = line + "\r\n";
    ssize_t sent = send(sockFd, msg.c_str(), msg.size(), MSG_NOSIGNAL);
    (void)sent;
}

void IrcClient::join(const std::string& channel) {
    std::string chName = channel;
    if (!chName.empty() && chName[0] != '#') chName = "#" + chName;
    sendRaw("JOIN " + chName);
    joinedChannels.push_back(chName);
}

void IrcClient::part(const std::string& channel, const std::string& reason) {
    sendRaw("PART " + channel + (reason.empty() ? "" : " :" + reason));
    auto it = std::find(joinedChannels.begin(), joinedChannels.end(), channel);
    if (it != joinedChannels.end()) joinedChannels.erase(it);
}

void IrcClient::privmsg(const std::string& target, const std::string& text) {
    sendRaw("PRIVMSG " + target + " :" + text);
}

void IrcClient::notice(const std::string& target, const std::string& text) {
    sendRaw("NOTICE " + target + " :" + text);
}

void IrcClient::nick(const std::string& newNick) {
    sendRaw("NICK " + newNick);
}

void IrcClient::whois(const std::string& nick) {
    sendRaw("WHOIS " + nick);
}

void IrcClient::topic(const std::string& channel, const std::string& newTopic) {
    if (newTopic.empty()) {
        sendRaw("TOPIC " + channel);
    } else {
        sendRaw("TOPIC " + channel + " :" + newTopic);
    }
}

void IrcClient::names(const std::string& channel) {
    sendRaw("NAMES " + channel);
}

void IrcClient::kick(const std::string& channel, const std::string& nick, const std::string& reason) {
    sendRaw("KICK " + channel + " " + nick + (reason.empty() ? "" : " :" + reason));
}

void IrcClient::mode(const std::string& channel, const std::string& modes, const std::string& param) {
    sendRaw("MODE " + channel + " " + modes + (param.empty() ? "" : " " + param));
}

void IrcClient::quit(const std::string& reason) {
    sendRaw("QUIT :" + reason);
}

void IrcClient::pingServer() {
    std::ostringstream ss;
    ss << "PING :" << std::chrono::system_clock::now().time_since_epoch().count();
    sendRaw(ss.str());
}

void IrcClient::ctcpRequest(const std::string& target, const std::string& request) {
    privmsg(target, "\x01" + request + "\x01");
}

void IrcClient::ctcpReply(const std::string& target, const std::string& reply) {
    notice(target, "\x01" + reply + "\x01");
}

void IrcClient::setMessageCallback(MessageCallback cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    messageCallback = std::move(cb);
}

void IrcClient::setStateCallback(StateCallback cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    stateCallback = std::move(cb);
}

void IrcClient::setErrorCallback(ErrorCallback cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    errorCallback = std::move(cb);
}

void IrcClient::receiveLoop() {
    char buf[4096];
    while (running && sockFd >= 0) {
        ssize_t n = recv(sockFd, buf, sizeof(buf) - 1, 0);
        if (n <= 0) {
            if (n == 0 || (errno != EAGAIN && errno != EWOULDBLOCK && errno != EINTR)) {
                if (running) {
                    state = IrcConnectionState::ERROR;
                    if (errorCallback) errorCallback(n == 0 ? 0 : errno, n == 0 ? "Connection lost" : strerror(errno));
                    if (stateCallback) stateCallback(state);
                }
                break;
            }
            continue;
        }

        buf[n] = '\0';
        readBuffer += buf;

        size_t nl;
        while ((nl = readBuffer.find('\n')) != std::string::npos) {
            std::string line = readBuffer.substr(0, nl);
            readBuffer.erase(0, nl + 1);
            if (!line.empty()) {
                processLine(line);
            }
        }
    }
}

void IrcClient::processLine(const std::string& line) {
    auto msg = IrcParser::parse(line);
    handleMessage(msg);
}

void IrcClient::handleMessage(const IrcMessage& msg) {
    switch (msg.command) {
        case IrcCommand::PING:
            sendRaw("PONG :" + msg.trailing);
            break;
        case IrcCommand::RPL_WELCOME:
            state = IrcConnectionState::REGISTERED;
            if (stateCallback) stateCallback(state);
            break;
        case IrcCommand::ERR_NICKNAMEINUSE:
            handleNickConflict();
            break;
        default:
            break;
    }

    if (messageCallback) {
        messageCallback(msg);
    }
}

void IrcClient::handleNickConflict() {
    nickConflictCounter++;
    std::string newNick = config.nick + std::to_string(nickConflictCounter);
    if (newNick.length() > 9) {
        newNick = config.nick.substr(0, 9 - std::to_string(nickConflictCounter).length()) + std::to_string(nickConflictCounter);
    }
    sendRaw("NICK " + newNick);
}

void IrcClient::startPingTimer() {
    pingThread = std::make_unique<std::thread>([this]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::seconds(120));
            if (running) pingServer();
        }
    });
}

void IrcClient::stopPingTimer() {
    if (pingThread && pingThread->joinable()) {
        pingThread->detach();
    }
    pingThread.reset();
}

std::string formatIrcMessage(const std::string& command, const std::vector<std::string>& params, const std::string& trailing) {
    std::ostringstream ss;
    ss << command;
    for (const auto& p : params) {
        ss << " " << p;
    }
    if (!trailing.empty()) {
        ss << " :" << trailing;
    }
    return ss.str();
}

} // namespace irc
} // namespace pcp
