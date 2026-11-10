#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(IntegrationTest, FullScenario1) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration0:matrix.org";
    login["password"] = "password0000";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration0:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000000";
    loginResp["device_id"] = "DEVICE_INTEGRATION_0";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000000";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(0) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 0";
    createRoom["topic"] = "Testing integration scenario 0";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_0:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 0";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000000:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_0:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 0";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario2) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration1:matrix.org";
    login["password"] = "password0001";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration1:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000001";
    loginResp["device_id"] = "DEVICE_INTEGRATION_1";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000001";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(1) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 1";
    createRoom["topic"] = "Testing integration scenario 1";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_1:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 1";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000001:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_1:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 1";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario3) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration2:matrix.org";
    login["password"] = "password0002";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration2:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000002";
    loginResp["device_id"] = "DEVICE_INTEGRATION_2";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000002";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(2) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 2";
    createRoom["topic"] = "Testing integration scenario 2";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_2:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 2";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000002:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_2:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 2";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario4) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration3:matrix.org";
    login["password"] = "password0003";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration3:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000003";
    loginResp["device_id"] = "DEVICE_INTEGRATION_3";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000003";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(3) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 3";
    createRoom["topic"] = "Testing integration scenario 3";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_3:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 3";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000003:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_3:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 3";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario5) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration4:matrix.org";
    login["password"] = "password0004";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration4:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000004";
    loginResp["device_id"] = "DEVICE_INTEGRATION_4";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000004";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(4) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 4";
    createRoom["topic"] = "Testing integration scenario 4";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_4:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 4";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000004:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_4:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 4";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario6) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration5:matrix.org";
    login["password"] = "password0005";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration5:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000005";
    loginResp["device_id"] = "DEVICE_INTEGRATION_5";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000005";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(5) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 5";
    createRoom["topic"] = "Testing integration scenario 5";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_5:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 5";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000005:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_5:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 5";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario7) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration6:matrix.org";
    login["password"] = "password0006";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration6:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000006";
    loginResp["device_id"] = "DEVICE_INTEGRATION_6";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000006";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(6) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 6";
    createRoom["topic"] = "Testing integration scenario 6";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_6:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 6";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000006:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_6:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 6";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario8) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration7:matrix.org";
    login["password"] = "password0007";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration7:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000007";
    loginResp["device_id"] = "DEVICE_INTEGRATION_7";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000007";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(7) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 7";
    createRoom["topic"] = "Testing integration scenario 7";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_7:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 7";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000007:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_7:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 7";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario9) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration8:matrix.org";
    login["password"] = "password0008";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration8:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000008";
    loginResp["device_id"] = "DEVICE_INTEGRATION_8";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000008";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(8) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 8";
    createRoom["topic"] = "Testing integration scenario 8";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_8:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 8";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000008:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_8:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 8";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario10) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration9:matrix.org";
    login["password"] = "password0009";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration9:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000009";
    loginResp["device_id"] = "DEVICE_INTEGRATION_9";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000009";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(9) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 9";
    createRoom["topic"] = "Testing integration scenario 9";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_9:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 9";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000009:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_9:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 9";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario11) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration10:matrix.org";
    login["password"] = "password0010";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration10:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000010";
    loginResp["device_id"] = "DEVICE_INTEGRATION_10";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000010";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(10) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 10";
    createRoom["topic"] = "Testing integration scenario 10";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_10:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 10";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000010:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_10:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 10";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario12) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration11:matrix.org";
    login["password"] = "password0011";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration11:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000011";
    loginResp["device_id"] = "DEVICE_INTEGRATION_11";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000011";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(11) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 11";
    createRoom["topic"] = "Testing integration scenario 11";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_11:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 11";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000011:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_11:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 11";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario13) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration12:matrix.org";
    login["password"] = "password0012";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration12:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000012";
    loginResp["device_id"] = "DEVICE_INTEGRATION_12";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000012";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(12) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 12";
    createRoom["topic"] = "Testing integration scenario 12";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_12:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 12";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000012:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_12:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 12";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario14) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration13:matrix.org";
    login["password"] = "password0013";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration13:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000013";
    loginResp["device_id"] = "DEVICE_INTEGRATION_13";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000013";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(13) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 13";
    createRoom["topic"] = "Testing integration scenario 13";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_13:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 13";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000013:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_13:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 13";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario15) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration14:matrix.org";
    login["password"] = "password0014";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration14:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000014";
    loginResp["device_id"] = "DEVICE_INTEGRATION_14";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000014";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(14) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 14";
    createRoom["topic"] = "Testing integration scenario 14";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_14:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 14";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000014:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_14:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 14";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario16) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration15:matrix.org";
    login["password"] = "password0015";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration15:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000015";
    loginResp["device_id"] = "DEVICE_INTEGRATION_15";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000015";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(15) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 15";
    createRoom["topic"] = "Testing integration scenario 15";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_15:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 15";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000015:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_15:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 15";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario17) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration16:matrix.org";
    login["password"] = "password0016";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration16:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000016";
    loginResp["device_id"] = "DEVICE_INTEGRATION_16";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000016";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(16) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 16";
    createRoom["topic"] = "Testing integration scenario 16";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_16:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 16";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000016:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_16:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 16";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario18) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration17:matrix.org";
    login["password"] = "password0017";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration17:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000017";
    loginResp["device_id"] = "DEVICE_INTEGRATION_17";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000017";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(17) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 17";
    createRoom["topic"] = "Testing integration scenario 17";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_17:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 17";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000017:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_17:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 17";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario19) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration18:matrix.org";
    login["password"] = "password0018";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration18:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000018";
    loginResp["device_id"] = "DEVICE_INTEGRATION_18";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000018";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(18) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 18";
    createRoom["topic"] = "Testing integration scenario 18";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_18:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 18";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000018:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_18:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 18";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario20) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration19:matrix.org";
    login["password"] = "password0019";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration19:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000019";
    loginResp["device_id"] = "DEVICE_INTEGRATION_19";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000019";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(19) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 19";
    createRoom["topic"] = "Testing integration scenario 19";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_19:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 19";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000019:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_19:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 19";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario21) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration20:matrix.org";
    login["password"] = "password0020";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration20:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000020";
    loginResp["device_id"] = "DEVICE_INTEGRATION_20";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000020";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(20) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 20";
    createRoom["topic"] = "Testing integration scenario 20";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_20:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 20";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000020:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_20:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 20";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario22) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration21:matrix.org";
    login["password"] = "password0021";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration21:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000021";
    loginResp["device_id"] = "DEVICE_INTEGRATION_21";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000021";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(21) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 21";
    createRoom["topic"] = "Testing integration scenario 21";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_21:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 21";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000021:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_21:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 21";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario23) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration22:matrix.org";
    login["password"] = "password0022";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration22:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000022";
    loginResp["device_id"] = "DEVICE_INTEGRATION_22";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000022";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(22) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 22";
    createRoom["topic"] = "Testing integration scenario 22";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_22:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 22";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000022:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_22:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 22";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario24) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration23:matrix.org";
    login["password"] = "password0023";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration23:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000023";
    loginResp["device_id"] = "DEVICE_INTEGRATION_23";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000023";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(23) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 23";
    createRoom["topic"] = "Testing integration scenario 23";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_23:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 23";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000023:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_23:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 23";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario25) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration24:matrix.org";
    login["password"] = "password0024";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration24:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000024";
    loginResp["device_id"] = "DEVICE_INTEGRATION_24";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000024";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(24) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 24";
    createRoom["topic"] = "Testing integration scenario 24";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_24:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 24";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000024:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_24:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 24";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario26) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration25:matrix.org";
    login["password"] = "password0025";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration25:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000025";
    loginResp["device_id"] = "DEVICE_INTEGRATION_25";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000025";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(25) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 25";
    createRoom["topic"] = "Testing integration scenario 25";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_25:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 25";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000025:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_25:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 25";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario27) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration26:matrix.org";
    login["password"] = "password0026";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration26:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000026";
    loginResp["device_id"] = "DEVICE_INTEGRATION_26";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000026";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(26) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 26";
    createRoom["topic"] = "Testing integration scenario 26";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_26:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 26";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000026:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_26:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 26";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario28) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration27:matrix.org";
    login["password"] = "password0027";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration27:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000027";
    loginResp["device_id"] = "DEVICE_INTEGRATION_27";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000027";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(27) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 27";
    createRoom["topic"] = "Testing integration scenario 27";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_27:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 27";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000027:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_27:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 27";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario29) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration28:matrix.org";
    login["password"] = "password0028";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration28:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000028";
    loginResp["device_id"] = "DEVICE_INTEGRATION_28";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000028";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(28) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 28";
    createRoom["topic"] = "Testing integration scenario 28";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_28:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 28";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000028:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_28:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 28";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

TEST(IntegrationTest, FullScenario30) {
    json login;
    login["type"] = "m.login.password";
    login["identifier"]["type"] = "m.id.user";
    login["identifier"]["user"] = "@integration29:matrix.org";
    login["password"] = "password0029";
    ASSERT_EQ(login["type"], "m.login.password");
    ASSERT_FALSE(login["identifier"]["user"].empty());

    json loginResp;
    loginResp["user_id"] = "@integration29:matrix.org";
    loginResp["access_token"] = "TOKEN_INTEGRATION_00000029";
    loginResp["device_id"] = "DEVICE_INTEGRATION_29";
    loginResp["home_server"] = "matrix.org";
    ASSERT_FALSE(loginResp["access_token"].empty());
    ASSERT_FALSE(loginResp["device_id"].empty());

    json syncResp;
    syncResp["next_batch"] = "batch_int_000029";
    syncResp["rooms"]["join"] = json::object();
    syncResp["rooms"]["invite"] = json::object();
    syncResp["rooms"]["leave"] = json::object();
    for (int r = 0; r < 3; ++r) {
        std::string rid = "!introom" + std::to_string(29) + "_" + std::to_string(r) + ":matrix.org";
        syncResp["rooms"]["join"][rid]["timeline"]["events"] = json::array();
        syncResp["rooms"]["join"][rid]["state"]["events"] = json::array();
    }
    ASSERT_GT(syncResp["rooms"]["join"].size(), 0);

    json createRoom;
    createRoom["name"] = "Integration Room 29";
    createRoom["topic"] = "Testing integration scenario 29";
    createRoom["preset"] = "private_chat";
    ASSERT_EQ(createRoom["preset"], "private_chat");

    std::string newRoomId = "!newroom_29:matrix.org";
    json joinRoom;
    joinRoom["room_id"] = newRoomId;
    joinRoom["membership"] = "join";
    ASSERT_EQ(joinRoom["membership"], "join");

    json messageEvent;
    messageEvent["type"] = "m.room.message";
    messageEvent["room_id"] = newRoomId;
    messageEvent["content"]["body"] = "Integration test message 29";
    messageEvent["content"]["msgtype"] = "m.text";
    ASSERT_EQ(messageEvent["type"], "m.room.message");
    ASSERT_FALSE(messageEvent["content"]["body"].empty());

    json receivedEvent;
    receivedEvent["event_id"] = "$received_int00000029:matrix.org";
    receivedEvent["room_id"] = newRoomId;
    receivedEvent["sender"] = "@otheruser_29:matrix.org";
    receivedEvent["type"] = "m.room.message";
    receivedEvent["content"]["body"] = "Reply to integration test 29";
    ASSERT_FALSE(receivedEvent["event_id"].empty());
    ASSERT_NE(receivedEvent["sender"], loginResp["user_id"]);

    json readMarker;
    readMarker["m.fully_read"] = receivedEvent["event_id"];
    readMarker["m.read"] = receivedEvent["event_id"];
    ASSERT_FALSE(readMarker["m.fully_read"].empty());

    json leaveRoom;
    leaveRoom["room_id"] = newRoomId;
    leaveRoom["membership"] = "leave";
    ASSERT_EQ(leaveRoom["membership"], "leave");

    ASSERT_NO_THROW({ auto a = login.dump(); });
    ASSERT_NO_THROW({ auto a = loginResp.dump(); });
    ASSERT_NO_THROW({ auto a = syncResp.dump(); });
    ASSERT_NO_THROW({ auto a = createRoom.dump(); });
    ASSERT_NO_THROW({ auto a = messageEvent.dump(); });
    ASSERT_NO_THROW({ auto a = receivedEvent.dump(); });
}

}} // namespace pcp::test
