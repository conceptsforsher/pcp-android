#include "irc_session.hpp"
#include "../protocol/protocol_room.hpp"
#include "../protocol/protocol_message.hpp"

#include <QTcpSocket>
#include <QSslSocket>
#include <QNetworkAccessManager>
#include <QDateTime>
#include <QDebug>
#include <QUuid>

namespace pcp_chat {

IrcSession::IrcSession(QNetworkAccessManager *network, QObject *parent)
    : ProtocolSession(parent)
    , m_networkManager(network)
{
    m_protocolType = ProtocolType::IRC;
    m_nickname = "pcp_" + QUuid::createUuid().toString().mid(1, 8);
}

IrcSession::~IrcSession()
{
    m_pingTimer.stop();
    if (m_socket) {
        m_socket->disconnectFromHost();
        if (m_socket->state() != QAbstractSocket::UnconnectedState)
            m_socket->waitForDisconnected(3000);
        delete m_socket;
    }
}

void IrcSession::connect()
{
    if (m_server.isEmpty()) {
        emit connectionError("No server configured");
        return;
    }

    if (m_useSsl) {
        auto *sslSocket = new QSslSocket(this);
        sslSocket->connectToHostEncrypted(m_server, m_port);
        m_socket = sslSocket;
    } else {
        m_socket = new QTcpSocket(this);
        m_socket->connectToHost(m_server, m_port);
    }

    QObject::connect(m_socket, &QTcpSocket::connected, this, [this]() {
        if (!m_password.isEmpty())
            sendRaw("PASS " + m_password);
        sendRaw("NICK " + m_nickname);
        sendRaw("USER " + m_nickname + " 0 * :" + m_realName);
    });

    QObject::connect(m_socket, &QTcpSocket::readyRead, this, [this]() {
        m_readBuffer += QString::fromUtf8(m_socket->readAll());
        while (m_readBuffer.contains("\r\n")) {
            int pos = m_readBuffer.indexOf("\r\n");
            QString line = m_readBuffer.left(pos);
            m_readBuffer = m_readBuffer.mid(pos + 2);
            processLine(line);
        }
    });

    QObject::connect(m_socket, &QTcpSocket::disconnected, this, [this]() {
        setConnected(false);
        m_registered = false;
        emit disconnected("Connection closed");
    });

    QObject::connect(m_socket, &QAbstractSocket::errorOccurred, this, [this](QAbstractSocket::SocketError err) {
        emit connectionError(m_socket->errorString());
    });
}

void IrcSession::disconnect()
{
    m_pingTimer.stop();
    if (m_socket) {
        sendRaw("QUIT :PCP signing off");
        m_socket->disconnectFromHost();
    }
}

void IrcSession::reconnect()
{
    disconnect();
    QTimer::singleShot(5000, this, &IrcSession::connect);
}

void IrcSession::sendMessage(const QString &roomId, const QString &body, const QString &replyTo)
{
    Q_UNUSED(replyTo);
    sendPrivmsg(roomId, body);
}

void IrcSession::sendTyping(const QString &roomId, bool typing)
{
    Q_UNUSED(roomId); Q_UNUSED(typing);
    // IRC doesn't support typing indicators
}

void IrcSession::markRead(const QString &roomId) { Q_UNUSED(roomId); }

void IrcSession::joinRoom(const QString &roomId)
{
    sendJoin(roomId);
}

void IrcSession::leaveRoom(const QString &roomId)
{
    sendPart(roomId);
}

void IrcSession::sendRaw(const QString &line)
{
    if (m_socket && m_socket->isOpen())
        m_socket->write((line + "\r\n").toUtf8());
}

void IrcSession::sendPrivmsg(const QString &target, const QString &message)
{
    sendRaw("PRIVMSG " + target + " :" + message);
}

void IrcSession::sendNotice(const QString &target, const QString &message)
{
    sendRaw("NOTICE " + target + " :" + message);
}

void IrcSession::sendJoin(const QString &channel)
{
    sendRaw("JOIN " + channel);
}

void IrcSession::sendPart(const QString &channel)
{
    sendRaw("PART " + channel + " :Leaving");
}

void IrcSession::sendNick(const QString &newNick)
{
    sendRaw("NICK " + newNick);
}

void IrcSession::sendWhois(const QString &nick)
{
    sendRaw("WHOIS " + nick);
}

void IrcSession::processLine(const QString &line)
{
    emit rawLineReceived(line);

    if (line.startsWith("PING ")) {
        sendRaw("PONG " + line.mid(5));
        return;
    }

    // Parse IRC message format: ":prefix COMMAND params :trailing"
    QString rest = line;
    QString prefix;

    if (rest.startsWith(":")) {
        int space = rest.indexOf(" ");
        prefix = rest.mid(1, space - 1);
        rest = rest.mid(space + 1);
    }

    int space = rest.indexOf(" ");
    QString command = space > 0 ? rest.left(space).toUpper() : rest.toUpper();
    QString paramsStr = space > 0 ? rest.mid(space + 1) : "";

    // Split params, respecting trailing parameter
    QStringList params;
    if (paramsStr.contains(" :")) {
        int colon = paramsStr.indexOf(" :");
        params = paramsStr.left(colon).split(" ", Qt::SkipEmptyParts);
        params.append(paramsStr.mid(colon + 2));
    } else {
        params = paramsStr.split(" ", Qt::SkipEmptyParts);
    }

    // Handle commands
    if (command == "PRIVMSG") {
        QString target = params.value(0);
        QString text = params.value(1);
        handlePrivmsg(prefix, target, text);
    } else if (command == "NOTICE") {
        QString target = params.value(0);
        QString text = params.value(1);
        // Notices are informational, less important
    } else if (command == "JOIN") {
        handleJoin(prefix, params.value(0));
    } else if (command == "PART") {
        handlePart(prefix, params.value(0), params.value(1));
    } else if (command == "NICK") {
        handleNick(prefix, params.value(0));
    } else if (command == "TOPIC") {
        handleTopic(prefix, params.value(0), params.value(1));
    } else if (command == "QUIT") {
        handleQuit(prefix, params.value(0));
    } else if (command == "MODE") {
        handleMode(prefix, params.value(0), params.value(1), params.value(2));
    } else if (command == "KICK") {
        handleKick(prefix, params.value(0), params.value(1), params.value(2));
    }

    // Numeric replies (001-599)
    bool isNumeric = false;
    int numericCode = command.toInt(&isNumeric);
    if (isNumeric) {
        handleNumeric(numericCode, params);
    }
}

void IrcSession::handlePing(const QString &server) { sendRaw("PONG " + server); }

void IrcSession::handlePrivmsg(const QString &prefix, const QString &target, const QString &text)
{
    auto [nick, userhost] = parsePrefix(prefix);

    ProtocolMessage msg;
    msg.protocol = ProtocolType::IRC;
    msg.roomId = target;
    msg.senderId = nick;
    msg.senderName = nick;
    msg.body = text;
    msg.timestamp = QDateTime::currentDateTime();

    if (text.startsWith("\001ACTION ") && text.endsWith("\001")) {
        msg.type = MessageType::Emote;
        msg.body = text.mid(8, text.length() - 9);
    }

    emitMessage(msg);
}

void IrcSession::handleJoin(const QString &prefix, const QString &channel)
{
    auto [nick, userhost] = parsePrefix(prefix);

    if (nick == m_nickname) {
        ProtocolRoom room;
        room.id = channel;
        room.protocol = ProtocolType::IRC;
        room.type = RoomType::Channel;
        room.name = channel;
        addRoom(room);
    }
}

void IrcSession::handlePart(const QString &prefix, const QString &channel, const QString &reason)
{
    auto [nick, userhost] = parsePrefix(prefix);
    Q_UNUSED(nick); Q_UNUSED(reason);
}

void IrcSession::handleNick(const QString &prefix, const QString &newNick)
{
    auto [oldNick, userhost] = parsePrefix(prefix);
    emit nickChanged(oldNick, newNick);
}

void IrcSession::handleTopic(const QString &prefix, const QString &channel, const QString &topic)
{
    Q_UNUSED(prefix);
    if (hasRoom(channel)) {
        ProtocolRoom room = this->room(channel);
        room.topic = topic;
        updateRoom(room);
    }
}

void IrcSession::handleQuit(const QString &prefix, const QString &reason)
{
    Q_UNUSED(prefix); Q_UNUSED(reason);
}

void IrcSession::handleMode(const QString &prefix, const QString &channel,
                              const QString &modes, const QString &target)
{
    Q_UNUSED(prefix); Q_UNUSED(channel); Q_UNUSED(modes); Q_UNUSED(target);
}

void IrcSession::handleKick(const QString &prefix, const QString &channel,
                              const QString &target, const QString &reason)
{
    Q_UNUSED(prefix); Q_UNUSED(channel); Q_UNUSED(target); Q_UNUSED(reason);
}

void IrcSession::handleNumeric(int code, const QStringList &params)
{
    switch (code) {
        case 1: // RPL_WELCOME
            m_registered = true;
            setConnected(true);
            startPingTimer();
            break;
        case 375: case 372: case 376: // MOTD
            if (params.size() >= 2) {
                QString motdLine = params.size() > 2 ? params[2] : params[1];
                if (motdLine.startsWith(":-"))
                    motdLine = motdLine.mid(1).trimmed();
                if (!motdLine.isEmpty()) {
                    QString motd = params.value(2);
                    emit motdReceived(motd);
                }
            }
            break;
        case 353: // RPL_NAMREPLY
            if (params.size() >= 4) {
                QString channel = params[2];
                QStringList users = params[3].split(" ", Qt::SkipEmptyParts);
                emit usersListReceived(channel, users);
            }
            break;
        default:
            break;
    }
}

void IrcSession::startPingTimer()
{
    QObject::connect(&m_pingTimer, &QTimer::timeout, this, [this]() {
        sendRaw("PING :" + m_server);
    });
    m_pingTimer.start(60000);
}

QPair<QString, QString> IrcSession::parsePrefix(const QString &prefix)
{
    int excl = prefix.indexOf("!");
    if (excl > 0) {
        QString nick = prefix.left(excl);
        QString userhost = prefix.mid(excl + 1);
        return {nick, userhost};
    }
    return {prefix, ""};
}

} // namespace pcp_chat
