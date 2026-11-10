#include "irc_session.hpp"

#include <QTcpSocket>
#include <QSslSocket>
#include <QSslError>
#include <QTimer>
#include <QDateTime>
#include <QRegularExpression>

IrcSession::IrcSession(const IrcServerInfo &info, QObject *parent)
    : IProtocolSession()
    , serverInfo_(info)
    , currentNick_(info.nick)
{
}

IrcSession::~IrcSession()
{
    close();
}

// ─── IProtocolSession interface ───────────────────────────────────────────────

void IrcSession::open()
{
    if (state_ == ConnectionState::CONNECTING || state_ == ConnectionState::CONNECTED) {
        return;
    }

    setConnectionState(ConnectionState::CONNECTING);
    registered_ = false;

    if (serverInfo_.useTls) {
        auto *ssl = new QSslSocket(this);
        socket_ = ssl;
        connect(ssl, &QSslSocket::encrypted, this, &IrcSession::onConnected);
        connect(ssl, QOverload<const QList<QSslError>&>::of(&QSslSocket::sslErrors),
                this, &IrcSession::onSslErrors);
        ssl->connectToHostEncrypted(serverInfo_.host, serverInfo_.port);
        if (!serverInfo_.host.isEmpty()) {
            ssl->ignoreSslErrors();
        }
    } else {
        auto *tcp = new QTcpSocket(this);
        socket_ = tcp;
        connect(tcp, &QTcpSocket::connected, this, &IrcSession::onConnected);
        tcp->connectToHost(serverInfo_.host, serverInfo_.port);
    }

    connect(socket_, &QTcpSocket::disconnected, this, &IrcSession::onDisconnected);
    connect(socket_, &QTcpSocket::readyRead, this, &IrcSession::onReadyRead);
    connect(socket_, &QTcpSocket::errorOccurred, this, &IrcSession::onError);
}

void IrcSession::close()
{
    if (state_ == ConnectionState::CONNECTED || state_ == ConnectionState::REGISTERED) {
        sendRaw(QStringLiteral("QUIT :%1").arg(serverInfo_.realName.isEmpty()
                                                ? QStringLiteral("PCP Android")
                                                : serverInfo_.realName));
        socket_->flush();
        socket_->disconnectFromHost();
    } else if (socket_ && socket_->state() != QAbstractSocket::UnconnectedState) {
        socket_->disconnectFromHost();
    }

    delete pingTimer_;
    pingTimer_ = nullptr;

    setConnectionState(ConnectionState::DISCONNECTED);
}

void IrcSession::sendMessage(const QString &roomId, const QString &text, ProtocolContentType contentType)
{
    if (state_ != ConnectionState::REGISTERED) {
        emit errorOccurred(QStringLiteral("Not connected to IRC server"));
        return;
    }

    if (contentType == ProtocolContentType::ACTION) {
        sendRaw(QStringLiteral("PRIVMSG %1 :\001ACTION %2\001").arg(roomId, text));
    } else {
        sendRaw(QStringLiteral("PRIVMSG %1 :%2").arg(roomId, text));
    }

    ProtocolMessage msg;
    msg.senderId = currentNick_;
    msg.senderName = currentNick_;
    msg.text = text;
    msg.roomId = roomId;
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = contentType;
    msg.protocolType = ProtocolType::IRC;

    addMessageToRoom(roomId, msg);
    emit messageReceived(msg);
}

QVector<ProtocolRoom> IrcSession::getRooms()
{
    QVector<ProtocolRoom> result;
    result.reserve(rooms_.size());
    for (const auto &room : rooms_) {
        result.append(room);
    }
    return result;
}

QVector<ProtocolMessage> IrcSession::getMessages(const QString &roomId, int limit)
{
    const auto it = messages_.find(roomId.toLower());
    if (it == messages_.end()) {
        return {};
    }

    const auto &list = it.value();
    if (limit <= 0 || limit >= list.size()) {
        return list;
    }

    return list.mid(list.size() - limit);
}

void IrcSession::markAsRead(const QString &roomId)
{
    auto *room = findRoom(roomId);
    if (room) {
        room->unreadCount = 0;
        emit roomUpdated(*room);
    }
}

void IrcSession::joinRoom(const QString &channel)
{
    if (state_ != ConnectionState::REGISTERED) {
        emit errorOccurred(QStringLiteral("Not connected to IRC server"));
        return;
    }

    QString ch = channel;
    if (!ch.startsWith(QLatin1Char('#')) && !ch.startsWith(QLatin1Char('&'))) {
        ch.prepend(QLatin1Char('#'));
    }

    sendRaw(QStringLiteral("JOIN %1").arg(ch));

    ProtocolRoom room;
    room.id = ch.toLower();
    room.name = ch;
    room.protocolType = ProtocolType::IRC;
    rooms_[ch.toLower()] = room;
}

void IrcSession::leaveRoom(const QString &roomId)
{
    if (state_ != ConnectionState::REGISTERED) {
        emit errorOccurred(QStringLiteral("Not connected to IRC server"));
        return;
    }

    sendRaw(QStringLiteral("PART %1 :Leaving").arg(roomId));
    rooms_.remove(roomId.toLower());
    messages_.remove(roomId.toLower());
}

QVector<ProtocolRoom> IrcSession::searchRooms(const QString &query)
{
    QVector<ProtocolRoom> result;
    for (const auto &room : rooms_) {
        if (room.name.contains(query, Qt::CaseInsensitive) ||
            room.topic.contains(query, Qt::CaseInsensitive)) {
            result.append(room);
        }
    }
    return result;
}

ProtocolType IrcSession::protocolType() const
{
    return ProtocolType::IRC;
}

ConnectionState IrcSession::connectionState() const
{
    return state_;
}

QString IrcSession::userId() const
{
    return currentNick_;
}

QString IrcSession::displayName() const
{
    return serverInfo_.realName.isEmpty() ? currentNick_ : serverInfo_.realName;
}

// ─── Slots ────────────────────────────────────────────────────────────────────

void IrcSession::onConnected()
{
    setConnectionState(ConnectionState::CONNECTED);

    if (!serverInfo_.password.isEmpty()) {
        sendRaw(QStringLiteral("PASS %1").arg(serverInfo_.password));
    }

    QString user = serverInfo_.user;
    if (user.isEmpty()) {
        user = serverInfo_.nick;
    }

    QString realName = serverInfo_.realName;
    if (realName.isEmpty()) {
        realName = serverInfo_.nick;
    }

    sendRaw(QStringLiteral("NICK %1").arg(serverInfo_.nick));
    sendRaw(QStringLiteral("USER %1 0 * :%2").arg(user, realName));
}

void IrcSession::onDisconnected()
{
    setConnectionState(ConnectionState::DISCONNECTED);
    registered_ = false;
    delete pingTimer_;
    pingTimer_ = nullptr;
}

void IrcSession::onReadyRead()
{
    while (socket_->canReadLine()) {
        QByteArray data = socket_->readLine();
        QString line = QString::fromUtf8(data).trimmed();

        if (line.isEmpty() && !readBuffer_.isEmpty()) {
            line = readBuffer_.trimmed();
            readBuffer_.clear();
        } else if (!readBuffer_.isEmpty()) {
            line = readBuffer_ + line;
            readBuffer_.clear();
        }

        if (line.isEmpty()) {
            continue;
        }

        parseLine(line);
    }

    if (socket_->bytesAvailable() > 0) {
        readBuffer_ += QString::fromUtf8(socket_->readAll());
    }
}

void IrcSession::onError()
{
    emit errorOccurred(socket_->errorString());
    if (state_ == ConnectionState::CONNECTING) {
        setConnectionState(ConnectionState::ERROR);
    }
}

void IrcSession::onSslErrors(const QList<QSslError> &errors)
{
    for (const auto &err : errors) {
        emit errorOccurred(QStringLiteral("SSL: %1").arg(err.errorString()));
    }
}

// ─── IRC line parsing ─────────────────────────────────────────────────────────

void IrcSession::parseLine(const QString &line)
{
    QString msg = line;

    QString prefix;
    if (msg.startsWith(QLatin1Char(':'))) {
        int space = msg.indexOf(QLatin1Char(' '));
        prefix = msg.mid(1, space - 1);
        msg = msg.mid(space + 1);
    }

    QString command;
    int cmdEnd = msg.indexOf(QLatin1Char(' '));
    if (cmdEnd == -1) {
        command = msg;
        msg.clear();
    } else {
        command = msg.left(cmdEnd);
        msg = msg.mid(cmdEnd + 1);
    }

    QStringList params;
    while (!msg.isEmpty()) {
        if (msg.startsWith(QLatin1Char(':'))) {
            params.append(msg.mid(1));
            break;
        }
        int space = msg.indexOf(QLatin1Char(' '));
        if (space == -1) {
            params.append(msg);
            break;
        }
        params.append(msg.left(space));
        msg = msg.mid(space + 1);
    }

    if (command == QLatin1String("PING")) {
        handlePing(params);
    } else if (command == QLatin1String("PRIVMSG")) {
        handlePrivmsg(prefix, params);
    } else if (command == QLatin1String("JOIN")) {
        handleJoin(prefix, params);
    } else if (command == QLatin1String("PART")) {
        handlePart(prefix, params);
    } else if (command == QLatin1String("QUIT")) {
        handleQuit(prefix, params);
    } else if (command == QLatin1String("NOTICE")) {
        handleNotice(prefix, params);
    } else if (command == QLatin1String("MODE")) {
        handleMode(prefix, params);
    } else if (command == QLatin1String("KICK")) {
        handleKick(prefix, params);
    } else if (command == QLatin1String("TOPIC")) {
        handleTopic(prefix, params);
    } else if (command == QLatin1String("NICK")) {
        handleNick(prefix, params);
    } else if (command == QLatin1String("ERROR")) {
        QString errMsg = params.isEmpty() ? QStringLiteral("Unknown IRC error") : params.join(QStringLiteral(" "));
        emit errorOccurred(errMsg);
        close();
    } else {
        bool isNumeric = false;
        int numeric = command.toInt(&isNumeric);
        if (isNumeric) {
            handleNumeric(numeric, prefix, params);
        }
    }
}

// ─── Message handlers ─────────────────────────────────────────────────────────

void IrcSession::handlePrivmsg(const QString &prefix, const QStringList &params)
{
    if (params.size() < 2) return;

    QString target = params[0];
    QString text = params[1];

    QString senderNick = extractNick(prefix);
    QString senderUser = extractUser(prefix);
    QString senderHost = extractHost(prefix);

    ProtocolContentType contentType = ProtocolContentType::TEXT;

    if (text.startsWith(QStringLiteral("\001")) && text.endsWith(QStringLiteral("\001"))) {
        QString ctcp = text.mid(1, text.length() - 2);
        if (ctcp.startsWith(QStringLiteral("ACTION "))) {
            contentType = ProtocolContentType::ACTION;
            text = ctcp.mid(7);
        } else {
            contentType = ProtocolContentType::SYSTEM;
        }
    }

    QString roomId;
    if (target.startsWith(QLatin1Char('#')) || target.startsWith(QLatin1Char('&'))) {
        roomId = target.toLower();
    } else {
        roomId = senderNick.toLower();
    }

    ProtocolMessage msg;
    msg.senderId = senderNick;
    msg.senderName = senderNick;
    msg.text = text;
    msg.roomId = roomId;
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = contentType;
    msg.protocolType = ProtocolType::IRC;

    addMessageToRoom(roomId, msg);
    emit messageReceived(msg);
}

void IrcSession::handleJoin(const QString &prefix, const QStringList &params)
{
    QString channel = params.isEmpty() ? QString() : params[0];
    if (channel.isEmpty()) return;

    QString nick = extractNick(prefix);

    ProtocolMessage msg;
    msg.senderId = nick;
    msg.senderName = nick;
    msg.text = QStringLiteral("%1 has joined").arg(nick);
    msg.roomId = channel.toLower();
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::JOIN;
    msg.protocolType = ProtocolType::IRC;

    if (nick.toLower() == currentNick_.toLower()) {
        ProtocolRoom room;
        room.id = channel.toLower();
        room.name = channel;
        room.protocolType = ProtocolType::IRC;
        rooms_[channel.toLower()] = room;
        sendRaw(QStringLiteral("MODE %1").arg(channel));
    } else {
        auto *room = findRoom(channel);
        if (room) {
            room->memberCount++;
            emit roomUpdated(*room);
        }
    }

    addMessageToRoom(channel.toLower(), msg);
    emit messageReceived(msg);
}

void IrcSession::handlePart(const QString &prefix, const QStringList &params)
{
    if (params.isEmpty()) return;

    QString channel = params[0];
    QString reason = params.size() > 1 ? params[1] : QString();
    QString nick = extractNick(prefix);

    ProtocolMessage msg;
    msg.senderId = nick;
    msg.senderName = nick;
    msg.text = reason.isEmpty()
        ? QStringLiteral("%1 has left").arg(nick)
        : QStringLiteral("%1 has left (%2)").arg(nick, reason);
    msg.roomId = channel.toLower();
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::PART;
    msg.protocolType = ProtocolType::IRC;

    if (nick.toLower() == currentNick_.toLower()) {
        rooms_.remove(channel.toLower());
        messages_.remove(channel.toLower());
    } else {
        auto *room = findRoom(channel);
        if (room && room->memberCount > 0) {
            room->memberCount--;
            emit roomUpdated(*room);
        }
    }

    addMessageToRoom(channel.toLower(), msg);
    emit messageReceived(msg);
}

void IrcSession::handleQuit(const QString &prefix, const QStringList &params)
{
    QString reason = params.isEmpty() ? QString() : params[0];
    QString nick = extractNick(prefix);

    ProtocolMessage msg;
    msg.senderId = nick;
    msg.senderName = nick;
    msg.text = reason.isEmpty()
        ? QStringLiteral("%1 has quit").arg(nick)
        : QStringLiteral("%1 has quit (%2)").arg(nick, reason);
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::PART;
    msg.protocolType = ProtocolType::IRC;

    for (auto it = rooms_.begin(); it != rooms_.end(); ++it) {
        msg.roomId = it.key();
        addMessageToRoom(it.key(), msg);
        if (it->memberCount > 0) {
            it->memberCount--;
            emit roomUpdated(it.value());
        }
        emit messageReceived(msg);
    }
}

void IrcSession::handleNotice(const QString &prefix, const QStringList &params)
{
    if (params.size() < 2) return;

    QString target = params[0];
    QString text = params[1];

    QString senderNick = extractNick(prefix);

    ProtocolMessage msg;
    msg.senderId = senderNick.isEmpty() ? currentNick_ : senderNick;
    msg.senderName = msg.senderId;
    msg.text = text;
    msg.roomId = target.startsWith(QLatin1Char('#')) ? target.toLower() : senderNick.toLower();
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::NOTICE;
    msg.protocolType = ProtocolType::IRC;

    addMessageToRoom(msg.roomId, msg);
    emit messageReceived(msg);
}

void IrcSession::handleMode(const QString &prefix, const QStringList &params)
{
    if (params.isEmpty()) return;

    QString channel = params[0];
    QString modeStr = params.size() > 1 ? params[1] : QString();
    QString nickParam = params.size() > 2 ? params[2] : QString();

    auto *room = findRoom(channel);
    if (!room) return;

    QString senderNick = extractNick(prefix);
    QString desc = modeStr;

    if (!nickParam.isEmpty()) {
        desc = QStringLiteral("%1 sets mode %2 %3").arg(
            senderNick.isEmpty() ? QStringLiteral("Server") : senderNick,
            modeStr,
            nickParam);
    } else if (!senderNick.isEmpty()) {
        desc = QStringLiteral("%1 sets mode %2").arg(senderNick, modeStr);
    }

    ProtocolMessage msg;
    msg.senderId = senderNick.isEmpty() ? QStringLiteral("server") : senderNick;
    msg.senderName = msg.senderId;
    msg.text = desc;
    msg.roomId = channel.toLower();
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::SYSTEM;
    msg.protocolType = ProtocolType::IRC;

    addMessageToRoom(channel.toLower(), msg);
    emit messageReceived(msg);
}

void IrcSession::handleKick(const QString &prefix, const QStringList &params)
{
    if (params.size() < 2) return;

    QString channel = params[0];
    QString targetNick = params[1];
    QString reason = params.size() > 2 ? params[2] : QString();

    QString kickerNick = extractNick(prefix);
    bool selfKicked = (targetNick.toLower() == currentNick_.toLower());

    ProtocolMessage msg;
    msg.senderId = kickerNick;
    msg.senderName = kickerNick;
    msg.text = reason.isEmpty()
        ? QStringLiteral("%1 was kicked by %2").arg(targetNick, kickerNick)
        : QStringLiteral("%1 was kicked by %2 (%3)").arg(targetNick, kickerNick, reason);
    msg.roomId = channel.toLower();
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::KICK;
    msg.protocolType = ProtocolType::IRC;

    if (selfKicked) {
        rooms_.remove(channel.toLower());
        messages_.remove(channel.toLower());
    } else {
        auto *room = findRoom(channel);
        if (room && room->memberCount > 0) {
            room->memberCount--;
            emit roomUpdated(*room);
        }
    }

    addMessageToRoom(channel.toLower(), msg);
    emit messageReceived(msg);
}

void IrcSession::handleTopic(const QString &prefix, const QStringList &params)
{
    if (params.size() < 2) return;

    QString channel = params[0];
    QString topic = params[1];

    QString senderNick = extractNick(prefix);

    auto *room = findRoom(channel);
    if (room) {
        room->topic = topic;
        emit roomUpdated(*room);
    }

    ProtocolMessage msg;
    msg.senderId = senderNick.isEmpty() ? QStringLiteral("server") : senderNick;
    msg.senderName = msg.senderId;
    msg.text = QStringLiteral("Topic changed by %1: %2").arg(
        senderNick.isEmpty() ? QStringLiteral("server") : senderNick, topic);
    msg.roomId = channel.toLower();
    msg.timestamp = QDateTime::currentSecsSinceEpoch();
    msg.contentType = ProtocolContentType::TOPIC;
    msg.protocolType = ProtocolType::IRC;

    addMessageToRoom(channel.toLower(), msg);
    emit messageReceived(msg);
}

void IrcSession::handleNick(const QString &prefix, const QStringList &params)
{
    if (params.isEmpty()) return;

    QString oldNick = extractNick(prefix);
    QString newNick = params[0];

    if (oldNick.toLower() == currentNick_.toLower()) {
        currentNick_ = newNick;
    }

    for (auto it = rooms_.begin(); it != rooms_.end(); ++it) {
        ProtocolMessage msg;
        msg.senderId = oldNick;
        msg.senderName = oldNick;
        msg.text = QStringLiteral("%1 is now known as %2").arg(oldNick, newNick);
        msg.roomId = it.key();
        msg.timestamp = QDateTime::currentSecsSinceEpoch();
        msg.contentType = ProtocolContentType::SYSTEM;
        msg.protocolType = ProtocolType::IRC;

        addMessageToRoom(it.key(), msg);
        emit messageReceived(msg);
    }
}

// ─── Numeric handlers ─────────────────────────────────────────────────────────

void IrcSession::handleNumeric(int numeric, const QString &prefix, const QStringList &params)
{
    switch (numeric) {
    case 1: // RPL_WELCOME
        registered_ = true;
        setConnectionState(ConnectionState::REGISTERED);

        if (!pingTimer_) {
            pingTimer_ = new QTimer(this);
            connect(pingTimer_, &QTimer::timeout, this, [this]() {
                if (state_ == ConnectionState::REGISTERED) {
                    sendRaw(QStringLiteral("PING %1").arg(serverInfo_.host));
                }
            });
            pingTimer_->start(120000);
        }
        break;

    case 2: // RPL_YOURHOST
    case 3: // RPL_CREATED
    case 4: // RPL_MYINFO
    case 5: { // RPL_BOUNCE / ISUPPORT
        ProtocolMessage msg;
        msg.senderId = QStringLiteral("server");
        msg.senderName = QStringLiteral("Server");
        msg.text = params.mid(1).join(QStringLiteral(" "));
        msg.roomId = QStringLiteral("*status");
        msg.timestamp = QDateTime::currentSecsSinceEpoch();
        msg.contentType = ProtocolContentType::SYSTEM;
        msg.protocolType = ProtocolType::IRC;
        addMessageToRoom(msg.roomId, msg);
        emit messageReceived(msg);
        break;
    }

    case 353: { // RPL_NAMREPLY
        if (params.size() < 4) return;
        QString channel = params[2];
        QStringList names = params[3].split(QLatin1Char(' '), Qt::SkipEmptyParts);

        auto *room = findRoom(channel);
        if (room) {
            room->memberCount += names.size();
            emit roomUpdated(*room);
        }

        ProtocolMessage msg;
        msg.senderId = QStringLiteral("server");
        msg.senderName = QStringLiteral("Names");
        msg.text = QStringLiteral("Users in %1: %2").arg(channel, names.join(QStringLiteral(", ")));
        msg.roomId = channel.toLower();
        msg.timestamp = QDateTime::currentSecsSinceEpoch();
        msg.contentType = ProtocolContentType::SYSTEM;
        msg.protocolType = ProtocolType::IRC;
        addMessageToRoom(msg.roomId, msg);
        emit messageReceived(msg);
        break;
    }

    case 366: // RPL_ENDOFNAMES
        // End of /NAMES list; nothing fancier needed
        break;

    case 332: { // RPL_TOPIC
        if (params.size() < 3) return;
        QString channel = params[1];
        QString topic = params[2];
        auto *room = findRoom(channel);
        if (room) {
            room->topic = topic;
            emit roomUpdated(*room);
        }
        break;
    }

    case 375: // RPL_MOTDSTART
    case 372: // RPL_MOTD
    case 376: // RPL_ENDOFMOTD
        break;

    case 421: // ERR_UNKNOWNCOMMAND
    case 422: // ERR_NOMOTD
        break;

    default: {
        if (numeric >= 400 && numeric < 600) {
            QString errMsg = params.isEmpty()
                ? QStringLiteral("IRC error %1").arg(numeric)
                : params.mid(1).join(QStringLiteral(" "));
            emit errorOccurred(QStringLiteral("[%1] %2").arg(numeric).arg(errMsg));
        }
        break;
    }
    }
}

void IrcSession::handlePing(const QStringList &params)
{
    QString token = params.isEmpty() ? QStringLiteral("") : params[0];
    sendRaw(QStringLiteral("PONG :%1").arg(token));
}

// ─── Internal helpers ─────────────────────────────────────────────────────────

void IrcSession::sendRaw(const QString &line)
{
    if (!socket_ || socket_->state() != QAbstractSocket::ConnectedState) {
        return;
    }
    socket_->write(line.toUtf8() + "\r\n");
}

void IrcSession::setConnectionState(ConnectionState state)
{
    if (state_ != state) {
        state_ = state;
        emit connectionStateChanged(ProtocolType::IRC, state);
    }
}

QString IrcSession::extractNick(const QString &prefix) const
{
    int excl = prefix.indexOf(QLatin1Char('!'));
    if (excl != -1) {
        return prefix.left(excl);
    }
    int at = prefix.indexOf(QLatin1Char('@'));
    if (at != -1) {
        return prefix;
    }
    return prefix;
}

QString IrcSession::extractUser(const QString &prefix) const
{
    int excl = prefix.indexOf(QLatin1Char('!'));
    if (excl == -1) return {};
    int at = prefix.indexOf(QLatin1Char('@'), excl);
    if (at == -1) return prefix.mid(excl + 1);
    return prefix.mid(excl + 1, at - excl - 1);
}

QString IrcSession::extractHost(const QString &prefix) const
{
    int at = prefix.indexOf(QLatin1Char('@'));
    if (at == -1) return {};
    return prefix.mid(at + 1);
}

ProtocolRoom *IrcSession::findRoom(const QString &channel)
{
    auto it = rooms_.find(channel.toLower());
    if (it == rooms_.end()) return nullptr;
    return &it.value();
}

void IrcSession::addMessageToRoom(const QString &roomId, const ProtocolMessage &msg)
{
    messages_[roomId.toLower()].append(msg);

    auto *room = findRoom(roomId);
    if (room) {
        room->lastMessageTimestamp = msg.timestamp;
        QString preview = msg.text.left(80);
        preview.replace(QLatin1Char('\n'), QLatin1Char(' '));
        room->lastMessagePreview = preview;
        room->unreadCount++;
        emit roomUpdated(*room);
    }
}
