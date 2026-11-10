// PCP Test Suite
// Tests for the core SDK and application modules

#include <QTest>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QSignalSpy>
#include <QNetworkAccessManager>

#include <iostream>

// ===== Protocol Type Tests =====
class ProtocolTypeTests : public QObject
{
    Q_OBJECT

private slots:
    void testProtocolTypeToString()
    {
        QCOMPARE(pcp_chat::protocolTypeToString(
            pcp_chat::ProtocolType::Matrix), QString("matrix"));
        QCOMPARE(pcp_chat::protocolTypeToString(
            pcp_chat::ProtocolType::IRC), QString("irc"));
        QCOMPARE(pcp_chat::protocolTypeToString(
            pcp_chat::ProtocolType::Lemmy), QString("lemmy"));
        QCOMPARE(pcp_chat::protocolTypeToString(
            pcp_chat::ProtocolType::Unknown), QString("unknown"));
    }

    void testProtocolTypeFromString()
    {
        QCOMPARE(pcp_chat::protocolTypeFromString("matrix"),
                 pcp_chat::ProtocolType::Matrix);
        QCOMPARE(pcp_chat::protocolTypeFromString("irc"),
                 pcp_chat::ProtocolType::IRC);
        QCOMPARE(pcp_chat::protocolTypeFromString("lemmy"),
                 pcp_chat::ProtocolType::Lemmy);
        QCOMPARE(pcp_chat::protocolTypeFromString("invalid"),
                 pcp_chat::ProtocolType::Unknown);
    }

    void testProtocolTypeRoundTrip()
    {
        auto types = {pcp_chat::ProtocolType::Matrix,
                      pcp_chat::ProtocolType::IRC,
                      pcp_chat::ProtocolType::Lemmy};

        for (auto type : types) {
            QString str = pcp_chat::protocolTypeToString(type);
            QCOMPARE(pcp_chat::protocolTypeFromString(str), type);
        }
    }
};

// ===== Protocol Message Tests =====
class ProtocolMessageTests : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultMessage()
    {
        pcp_chat::ProtocolMessage msg;
        QCOMPARE(msg.protocol, pcp_chat::ProtocolType::Unknown);
        QCOMPARE(msg.type, pcp_chat::MessageType::Text);
        QVERIFY(!msg.isEncrypted());
        QVERIFY(!msg.isEdited());
        QVERIFY(!msg.isReply());
        QVERIFY(!msg.isInThread());
    }

    void testMessageReply()
    {
        pcp_chat::ProtocolMessage msg;
        msg.replyToId = "$event123";
        QVERIFY(msg.isReply());
    }

    void testMessageThread()
    {
        pcp_chat::ProtocolMessage msg;
        msg.threadRootId = "$thread456";
        QVERIFY(msg.isInThread());
    }

    void testMessageDisplayName()
    {
        pcp_chat::ProtocolMessage msg;
        msg.senderName = "Alice";
        msg.senderId = "@alice:matrix.org";
        QCOMPARE(msg.displayName(), QString("Alice"));

        pcp_chat::ProtocolMessage msg2;
        msg2.senderId = "@bob:matrix.org";
        QCOMPARE(msg2.displayName(), QString("@bob:matrix.org"));
    }
};

// ===== Protocol Room Tests =====
class ProtocolRoomTests : public QObject
{
    Q_OBJECT

private slots:
    void testRoomDefaults()
    {
        pcp_chat::ProtocolRoom room;
        QCOMPARE(room.protocol, pcp_chat::ProtocolType::Unknown);
        QCOMPARE(room.type, pcp_chat::ProtocolRoom::RoomType::Unknown);
        QCOMPARE(room.memberCount, 0);
        QCOMPARE(room.unreadCount, 0);
        QVERIFY(!room.isEncrypted);
        QVERIFY(!room.isDirect);
        QVERIFY(!room.isSpace());
    }

    void testRoomDisplayName()
    {
        pcp_chat::ProtocolRoom room;
        room.name = "Test Room";
        room.canonicalAlias = "#test:matrix.org";
        QCOMPARE(room.displayName(), QString("Test Room"));

        pcp_chat::ProtocolRoom room2;
        room2.canonicalAlias = "#onlyalias:matrix.org";
        QCOMPARE(room2.displayName(), QString("#onlyalias:matrix.org"));
    }

    void testSpaceRoom()
    {
        pcp_chat::ProtocolRoom room;
        room.type = pcp_chat::ProtocolRoom::RoomType::Space;
        QVERIFY(room.isSpace());
    }
};

// ===== String Utility Tests =====
class StringUtilTests : public QObject
{
    Q_OBJECT

private slots:
    void testEllipsize()
    {
        QCOMPARE(pcp_chat::util::ellipsize("Short", 10), QString("Short"));
        QCOMPARE(pcp_chat::util::ellipsize("This is a long text", 10),
                 QString("This is..."));
    }

    void testStripHtml()
    {
        QCOMPARE(pcp_chat::util::stripHtml("<b>Hello</b> <i>World</i>"),
                 QString("Hello World"));
    }

    void testSplitLines()
    {
        QStringList lines = pcp_chat::util::splitLines("line1\\nline2\\r\\nline3");
        QCOMPARE(lines.size(), 3);
        QCOMPARE(lines[0], QString("line1"));
        QCOMPARE(lines[1], QString("line2"));
        QCOMPARE(lines[2], QString("line3"));
    }

    void testIsValidMatrixId()
    {
        QVERIFY(pcp_chat::util::isValidMatrixId("@user:matrix.org"));
        QVERIFY(pcp_chat::util::isValidMatrixId("!room:matrix.org"));
        QVERIFY(pcp_chat::util::isValidMatrixId("#alias:matrix.org"));
        QVERIFY(!pcp_chat::util::isValidMatrixId("invalid"));
        QVERIFY(!pcp_chat::util::isValidMatrixId(""));
    }

    void testExtractServerName()
    {
        QCOMPARE(pcp_chat::util::extractServerName("@user:matrix.org"),
                 QString("matrix.org"));
        QCOMPARE(pcp_chat::util::extractServerName("@user:example.com:443"),
                 QString("443"));
    }

    void testFormatBytes()
    {
        QCOMPARE(pcp_chat::util::formatBytes(500), QString("500 B"));
        QCOMPARE(pcp_chat::util::formatBytes(1500), QString("1.5 KB"));
        QCOMPARE(pcp_chat::util::formatBytes(1500000), QString("1.4 MB"));
        QCOMPARE(pcp_chat::util::formatBytes(1500000000LL), QString("1.4 GB"));
    }
};

// ===== Date Utility Tests =====
class DateUtilTests : public QObject
{
    Q_OBJECT

private slots:
    void testFormatTimestamp()
    {
        QDateTime now = QDateTime::currentDateTime();
        QString today = pcp_chat::util::formatTimestamp(now);
        QVERIFY(!today.isEmpty());
    }

    void testFormatRelativeTime()
    {
        QDateTime now = QDateTime::currentDateTime();
        QDateTime tenSecAgo = now.addSecs(-10);
        QString relative = pcp_chat::util::formatRelativeTime(tenSecAgo);
        QVERIFY(relative.contains("ago"));
    }

    void testFormatDuration()
    {
        QCOMPARE(pcp_chat::util::formatDuration(5000), QString("0:05"));
        QCOMPARE(pcp_chat::util::formatDuration(65000), QString("1:05"));
        QCOMPARE(pcp_chat::util::formatDuration(3665000), QString("1:01:05"));
    }
};

// ===== Color Utility Tests =====
class ColorUtilTests : public QObject
{
    Q_OBJECT

private slots:
    void testColorFromUserId()
    {
        QColor color1 = pcp_chat::util::colorFromUserId("@alice:matrix.org");
        QColor color2 = pcp_chat::util::colorFromUserId("@alice:matrix.org");
        QVERIFY(color1.isValid());
        // Same user should get same color
        QCOMPARE(color1.name(), color2.name());
    }

    void testIsDarkColor()
    {
        QVERIFY(pcp_chat::util::isDarkColor(QColor("#000000")));
        QVERIFY(!pcp_chat::util::isDarkColor(QColor("#ffffff")));
    }

    void testTextColorForBackground()
    {
        QCOMPARE(pcp_chat::util::textColorForBackground(QColor("#000000")),
                 QColor("#ffffff"));
        QCOMPARE(pcp_chat::util::textColorForBackground(QColor("#ffffff")),
                 QColor("#212121"));
    }
};

// ===== JSON Utility Tests =====
class JsonUtilTests : public QObject
{
    Q_OBJECT

private slots:
    void testParseJson()
    {
        QByteArray valid = R"({"key": "value"})";
        auto result = pcp_chat::util::parseJson(valid);
        QVERIFY(result.has_value());
        QCOMPARE((*result)["key"].toString(), QString("value"));

        QByteArray invalid = "not json";
        auto badResult = pcp_chat::util::parseJson(invalid);
        QVERIFY(!badResult.has_value());
    }

    void testSafeObject()
    {
        QJsonObject obj;
        obj["nested"] = QJsonObject{{"key", "val"}};
        QJsonObject nested = pcp_chat::util::safeObject(obj["nested"]);
        QCOMPARE(nested["key"].toString(), QString("val"));

        QJsonObject empty = pcp_chat::util::safeObject(obj["nonexistent"]);
        QVERIFY(empty.isEmpty());
    }

    void testSafeArray()
    {
        QJsonObject obj;
        obj["list"] = QJsonArray{1, 2, 3};
        QJsonArray arr = pcp_chat::util::safeArray(obj["list"]);
        QCOMPARE(arr.size(), 3);
    }

    void testSafeString()
    {
        QJsonObject obj;
        obj["name"] = "test";
        QCOMPARE(pcp_chat::util::safeString(obj["name"]), QString("test"));
        QCOMPARE(pcp_chat::util::safeString(obj["missing"], "default"),
                 QString("default"));
    }

    void testSafeInt()
    {
        QJsonObject obj;
        obj["count"] = 42;
        QCOMPARE(pcp_chat::util::safeInt(obj["count"]), 42);
        QCOMPARE(pcp_chat::util::safeInt(obj["missing"], -1), -1);
    }

    void testCanonicalJson()
    {
        QJsonObject obj;
        obj["b"] = 2;
        obj["a"] = 1;
        QString canonical = pcp_chat::util::canonicalJson(obj);
        QVERIFY(canonical.contains("\"a\""));
        QVERIFY(canonical.contains("\"b\""));
    }

    void testMergeObjects()
    {
        QJsonObject base;
        base["a"] = 1;
        base["b"] = 2;
        QJsonObject overlay;
        overlay["b"] = 3;
        overlay["c"] = 4;

        QJsonObject merged = pcp_chat::util::mergeObjects(base, overlay);
        QCOMPARE(merged["a"].toInt(), 1);
        QCOMPARE(merged["b"].toInt(), 3);
        QCOMPARE(merged["c"].toInt(), 4);
    }
};

// ===== Network Utility Tests =====
class NetworkUtilTests : public QObject
{
    Q_OBJECT

private slots:
    void testIsValidUrl()
    {
        QVERIFY(pcp_chat::util::isValidUrl("https://matrix.org"));
        QVERIFY(pcp_chat::util::isValidUrl("http://localhost:8008"));
        QVERIFY(!pcp_chat::util::isValidUrl("not a url"));
        QVERIFY(!pcp_chat::util::isValidUrl(""));
    }

    void testIsMxcUrl()
    {
        QVERIFY(pcp_chat::util::isMxcUrl("mxc://matrix.org/abc123"));
        QVERIFY(!pcp_chat::util::isMxcUrl("https://matrix.org/abc123"));
    }

    void testParseMxcUrl()
    {
        auto [server, mediaId] = pcp_chat::util::parseMxcUrl(
            "mxc://matrix.org/abc123def");
        QCOMPARE(server, QString("matrix.org"));
        QCOMPARE(mediaId, QString("abc123def"));
    }

    void testMxcToHttp()
    {
        QString http = pcp_chat::util::mxcToHttp(
            "mxc://matrix.org/abc123", "https://matrix.org");
        QVERIFY(http.contains("_matrix/media/v3/download"));
        QVERIFY(http.contains("matrix.org"));
        QVERIFY(http.contains("abc123"));
    }
};

// ===== File Utility Tests =====
class FileUtilTests : public QObject
{
    Q_OBJECT

private slots:
    void testFileExtension()
    {
        QCOMPARE(pcp_chat::util::fileExtension("/path/to/file.jpg"),
                 QString("jpg"));
        QCOMPARE(pcp_chat::util::fileExtension("test.PNG"),
                 QString("png"));
    }

    void testFileName()
    {
        QCOMPARE(pcp_chat::util::fileName("/path/to/document.pdf"),
                 QString("document.pdf"));
    }

    void testSanitizeFilename()
    {
        QCOMPARE(pcp_chat::util::sanitizeFilename("file<name>:with?chars"),
                 QString("file_name__with_chars"));
    }
};

// ===== Matrix Push Rules Tests =====
class MatrixPushRulesTests : public QObject
{
    Q_OBJECT

private slots:
    void testParseRule()
    {
        QJsonObject ruleObj;
        ruleObj["rule_id"] = ".m.rule.contains_user_name";
        ruleObj["default"] = true;
        ruleObj["enabled"] = true;
        ruleObj["priority_class"] = 3;
        ruleObj["actions"] = QJsonArray{"notify", QJsonObject{{"set_tweak", "sound"}, {"value", "default"}}};

        auto rule = pcp_chat::MatrixPushRules::parseRule(ruleObj);
        QCOMPARE(rule.ruleId, QString(".m.rule.contains_user_name"));
        QVERIFY(rule.isDefault);
        QVERIFY(rule.enabled);
        QCOMPARE(rule.priority, 3);
        QVERIFY(rule.actions.contains(pcp_chat::PushAction::Notify));
        QCOMPARE(rule.tweaks["sound"], QString("default"));
    }

    void testSerializeRule()
    {
        pcp_chat::PushRule rule;
        rule.ruleId = ".m.rule.test";
        rule.actions.append(pcp_chat::PushAction::Notify);
        rule.tweaks["sound"] = "custom.wav";

        QJsonObject obj = pcp_chat::MatrixPushRules::serializeRule(rule);
        QCOMPARE(obj["rule_id"].toString(), QString(".m.rule.test"));
    }

    void testShouldNotify()
    {
        pcp_chat::PushRule notifyRule;
        notifyRule.actions.append(pcp_chat::PushAction::Notify);

        pcp_chat::MatrixPushRules rules;
        QVERIFY(rules.shouldNotify(notifyRule));

        pcp_chat::PushRule silentRule;
        silentRule.actions.append(pcp_chat::PushAction::DontNotify);
        QVERIFY(!rules.shouldNotify(silentRule));
    }
};

// ===== Matrix Session Tests =====
class MatrixSessionTests : public QObject
{
    Q_OBJECT

private slots:
    void testSessionDefaults()
    {
        QNetworkAccessManager network;
        pcp_chat::MatrixSession session(&network);
        QCOMPARE(session.protocolType(), pcp_chat::ProtocolType::Matrix);
        QVERIFY(!session.isConnected());
        QVERIFY(session.homeserver().isEmpty());
        QVERIFY(session.accessToken().isEmpty());
    }

    void testSessionConfiguration()
    {
        QNetworkAccessManager network;
        pcp_chat::MatrixSession session(&network);
        session.setHomeserver("https://matrix.org");
        session.setAccessToken("test_token");
        session.setDeviceId("test_device");

        QCOMPARE(session.homeserver(), QString("https://matrix.org"));
        QCOMPARE(session.accessToken(), QString("test_token"));
    }

    void testSessionSignals()
    {
        QNetworkAccessManager network;
        pcp_chat::MatrixSession session(&network);

        QSignalSpy connectedSpy(&session, &pcp_chat::MatrixSession::connected);
        QSignalSpy disconnectedSpy(&session, &pcp_chat::MatrixSession::disconnected);
        QSignalSpy errorSpy(&session, &pcp_chat::MatrixSession::connectionError);

        QCOMPARE(connectedSpy.count(), 0);
        QCOMPARE(disconnectedSpy.count(), 0);
        QCOMPARE(errorSpy.count(), 0);
    }
};

// ===== Theme Manager Tests =====
class ThemeManagerTests : public QObject
{
    Q_OBJECT

private slots:
    void testDefaultTheme()
    {
        pcp_chat::ThemeManager manager;
        QCOMPARE(manager.activeThemeName(), QString("Light"));
    }

    void testThemeSwitch()
    {
        pcp_chat::ThemeManager manager;
        QSignalSpy themeSpy(&manager, &pcp_chat::ThemeManager::themeChanged);

        manager.applyTheme(pcp_chat::ThemeManager::Theme::Dark);
        QCOMPARE(themeSpy.count(), 1);
        QCOMPARE(manager.activeThemeName(), QString("Dark"));
    }

    void testColorsAfterThemeSwitch()
    {
        pcp_chat::ThemeManager manager;
        manager.applyTheme(pcp_chat::ThemeManager::Theme::Dark);

        QColor bg = manager.backgroundColor();
        QVERIFY(pcp_chat::util::isDarkColor(bg));
    }
};

// ===== Main Test Runner =====
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);

    int failures = 0;

    auto runTest = [&](QObject *test) {
        int result = QTest::qExec(test, argc, argv);
        if (result != 0) failures++;
        delete test;
    };

    std::cout << "=== Protocol Type Tests ===" << std::endl;
    runTest(new ProtocolTypeTests());

    std::cout << "\n=== Protocol Message Tests ===" << std::endl;
    runTest(new ProtocolMessageTests());

    std::cout << "\n=== Protocol Room Tests ===" << std::endl;
    runTest(new ProtocolRoomTests());

    std::cout << "\n=== String Utility Tests ===" << std::endl;
    runTest(new StringUtilTests());

    std::cout << "\n=== Date Utility Tests ===" << std::endl;
    runTest(new DateUtilTests());

    std::cout << "\n=== Color Utility Tests ===" << std::endl;
    runTest(new ColorUtilTests());

    std::cout << "\n=== JSON Utility Tests ===" << std::endl;
    runTest(new JsonUtilTests());

    std::cout << "\n=== Network Utility Tests ===" << std::endl;
    runTest(new NetworkUtilTests());

    std::cout << "\n=== File Utility Tests ===" << std::endl;
    runTest(new FileUtilTests());

    std::cout << "\n=== Matrix Push Rules Tests ===" << std::endl;
    runTest(new MatrixPushRulesTests());

    std::cout << "\n=== Matrix Session Tests ===" << std::endl;
    runTest(new MatrixSessionTests());

    std::cout << "\n=== Theme Manager Tests ===" << std::endl;
    runTest(new ThemeManagerTests());

    std::cout << "\n============================" << std::endl;
    if (failures == 0) {
        std::cout << "All tests passed!" << std::endl;
        return 0;
    } else {
        std::cout << failures << " test suite(s) failed." << std::endl;
        return 1;
    }
}

#include "test_main.moc"
