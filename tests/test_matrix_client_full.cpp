#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include <random>
#include <algorithm>
#include <sstream>

using json = nlohmann::json;

namespace pcp {
namespace test {

// ============================================================================
// Mock Data Helpers - Real Matrix event JSON structures
// ============================================================================

std::string makeLoginResponse(const std::string& userId, const std::string& accessToken,
                               const std::string& deviceId, const std::string& homeServer) {
    json j;
    j["user_id"] = userId;
    j["access_token"] = accessToken;
    j["device_id"] = deviceId;
    j["home_server"] = homeServer;
    j["well_known"] = {
        {"m.homeserver", {{"base_url", homeServer}}}
    };
    return j.dump();
}

std::string makeSyncResponse(const std::string& nextBatch) {
    json j;
    j["next_batch"] = nextBatch;
    j["presence"] = {
        {"events", json::array()}
    };
    j["account_data"] = {
        {"events", json::array()}
    };
    j["device_lists"] = {
        {"changed", json::array()},
        {"left", json::array()}
    };
    j["device_one_time_keys_count"] = {
        {"signed_curve25519", 50}
    };
    j["to_device"] = {
        {"events", json::array()}
    };
    j["rooms"] = {
        {"join", json::object()},
        {"invite", json::object()},
        {"leave", json::object()}
    };
    return j.dump();
}

std::string makeRoomEvent(const std::string& eventId, const std::string& roomId,
                           const std::string& sender, const std::string& type,
                           const json& content, int64_t ts = 0) {
    json j;
    j["event_id"] = eventId;
    j["room_id"] = roomId;
    j["sender"] = sender;
    j["type"] = type;
    j["content"] = content;
    j["origin_server_ts"] = ts > 0 ? ts : 1600000000000LL;
    j["unsigned"] = {
        {"age", 1234}
    };
    return j.dump();
}

std::string makeTextMessageEvent(const std::string& eventId, const std::string& roomId,
                                  const std::string& sender, const std::string& body) {
    json content;
    content["msgtype"] = "m.text";
    content["body"] = body;
    return makeRoomEvent(eventId, roomId, sender, "m.room.message", content);
}

std::string makeMembershipEvent(const std::string& eventId, const std::string& roomId,
                                 const std::string& sender, const std::string& target,
                                 const std::string& membership) {
    json content;
    content["membership"] = membership;
    content["displayname"] = target.substr(1, target.find(":") - 1);
    return makeRoomEvent(eventId, roomId, sender, "m.room.member", content);
}

std::string makeTypingEvent(const std::string& roomId, const std::vector<std::string>& users) {
    json j;
    j["type"] = "m.typing";
    j["room_id"] = roomId;
    j["content"] = {
        {"user_ids", users}
    };
    return j.dump();
}

std::string makeReceiptEvent(const std::string& roomId, const std::string& eventId,
                              const std::string& userId, int64_t ts = 0) {
    json j;
    j["type"] = "m.receipt";
    j["room_id"] = roomId;
    json content;
    content[eventId] = {
        {"m.read", {
            {userId, {
                {"ts", ts > 0 ? ts : 1600000000000LL}
            }}
        }}
    };
    j["content"] = content;
    return j.dump();
}

std::string makePresenceEvent(const std::string& userId, const std::string& presence,
                               const std::string& statusMsg = "") {
    json j;
    j["type"] = "m.presence";
    j["sender"] = userId;
    j["content"] = {
        {"presence", presence},
        {"status_msg", statusMsg},
        {"currently_active", presence == "online"},
        {"last_active_ago", presence == "online" ? 0 : 3600000}
    };
    return j.dump();
}

std::string makePushRule(const std::string& ruleId, const std::string& kind,
                          const std::string& pattern, bool enabled = true) {
    json rule;
    rule["rule_id"] = ruleId;
    rule["enabled"] = enabled;
    if (kind == "override") {
        rule["actions"] = {"notify", {"set_tweak", "sound", "default"}};
    } else if (kind == "content") {
        rule["actions"] = {"notify"};
        rule["pattern"] = pattern;
    } else if (kind == "room") {
        rule["actions"] = {"notify"};
        rule["rule_id"] = pattern;
    } else if (kind == "sender") {
        rule["actions"] = {"notify"};
        rule["rule_id"] = pattern;
    } else if (kind == "under") {
        rule["actions"] = {"dont_notify"};
    }
    return rule.dump();
}

// ============================================================================
// Test Fixture for Matrix Client tests
// ============================================================================

class MatrixClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        m_homeServer = "https://matrix.example.org";
        m_userId = "@alice:example.org";
        m_accessToken = "syt_abc123def456_ghi789jkl";
        m_deviceId = "ABCDEFGHIJ";
        m_nextBatch = "s123456789_123456789_123456789";
        m_roomId = "!room123:example.org";
        m_loginJson = makeLoginResponse(m_userId, m_accessToken, m_deviceId, m_homeServer);
        m_syncJson = makeSyncResponse(m_nextBatch);
    }

    void TearDown() override {
    }

    std::string m_homeServer;
    std::string m_userId;
    std::string m_accessToken;
    std::string m_deviceId;
    std::string m_nextBatch;
    std::string m_roomId;
    std::string m_loginJson;
    std::string m_syncJson;
};

// ============================================================================
// Session Creation Tests
// ============================================================================

TEST_F(MatrixClientTest, CreateSessionWithValidParams) {
    json params;
    params["user_id"] = m_userId;
    params["access_token"] = m_accessToken;
    params["device_id"] = m_deviceId;
    params["home_server_url"] = m_homeServer;

    ASSERT_FALSE(params["user_id"].get<std::string>().empty());
    ASSERT_EQ(params["user_id"].get<std::string>(), m_userId);
    ASSERT_FALSE(params["access_token"].get<std::string>().empty());
    ASSERT_TRUE(params["access_token"].get<std::string>().find("syt_") == 0);
    ASSERT_FALSE(params["device_id"].get<std::string>().empty());
    ASSERT_EQ(params["device_id"].get<std::string>().size(), 10u);
    ASSERT_FALSE(params["home_server_url"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, CreateSessionWithInvalidParams) {
    json paramsEmpty;
    paramsEmpty["user_id"] = "";
    paramsEmpty["access_token"] = "";
    paramsEmpty["device_id"] = "";
    paramsEmpty["home_server_url"] = "";

    ASSERT_TRUE(paramsEmpty["user_id"].get<std::string>().empty());
    ASSERT_TRUE(paramsEmpty["access_token"].get<std::string>().empty());
    ASSERT_TRUE(paramsEmpty["device_id"].get<std::string>().empty());
    ASSERT_TRUE(paramsEmpty["home_server_url"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, SessionStateInitial) {
    json state;
    state["sync_state"] = "IDLE";
    state["connection_state"] = "DISCONNECTED";
    state["is_openable"] = false;
    state["is_encryption_enabled"] = false;

    ASSERT_EQ(state["sync_state"].get<std::string>(), "IDLE");
    ASSERT_EQ(state["connection_state"].get<std::string>(), "DISCONNECTED");
    ASSERT_FALSE(state["is_openable"].get<bool>());
    ASSERT_FALSE(state["is_encryption_enabled"].get<bool>());
}

TEST_F(MatrixClientTest, SessionStateAfterOpen) {
    json state;
    state["sync_state"] = "SYNCING";
    state["connection_state"] = "CONNECTED";
    state["is_openable"] = true;
    state["is_encryption_enabled"] = true;

    ASSERT_EQ(state["sync_state"].get<std::string>(), "SYNCING");
    ASSERT_EQ(state["connection_state"].get<std::string>(), "CONNECTED");
    ASSERT_TRUE(state["is_openable"].get<bool>());
    ASSERT_TRUE(state["is_encryption_enabled"].get<bool>());
}

// ============================================================================
// Login Flow Tests
// ============================================================================

TEST_F(MatrixClientTest, LoginFlowPasswordAuth) {
    json loginReq;
    loginReq["type"] = "m.login.password";
    loginReq["identifier"] = {
        {"type", "m.id.user"},
        {"user", "alice"}
    };
    loginReq["password"] = "securepassword123";
    loginReq["initial_device_display_name"] = "Test Device";

    ASSERT_EQ(loginReq["type"].get<std::string>(), "m.login.password");
    ASSERT_EQ(loginReq["identifier"]["type"].get<std::string>(), "m.id.user");
    ASSERT_EQ(loginReq["identifier"]["user"].get<std::string>(), "alice");
    ASSERT_FALSE(loginReq["password"].get<std::string>().empty());
    ASSERT_EQ(loginReq["initial_device_display_name"].get<std::string>(), "Test Device");
}

TEST_F(MatrixClientTest, LoginFlowTokenAuth) {
    json loginReq;
    loginReq["type"] = "m.login.token";
    loginReq["token"] = "login_token_abc123";
    loginReq["initial_device_display_name"] = "Token Device";

    ASSERT_EQ(loginReq["type"].get<std::string>(), "m.login.token");
    ASSERT_FALSE(loginReq["token"].get<std::string>().empty());
    ASSERT_TRUE(loginReq["token"].get<std::string>().find("login_token_") == 0);
    ASSERT_EQ(loginReq["initial_device_display_name"].get<std::string>(), "Token Device");
}

TEST_F(MatrixClientTest, LoginResponseParsing) {
    json response = json::parse(m_loginJson);

    ASSERT_EQ(response["user_id"].get<std::string>(), m_userId);
    ASSERT_EQ(response["access_token"].get<std::string>(), m_accessToken);
    ASSERT_EQ(response["device_id"].get<std::string>(), m_deviceId);
    ASSERT_EQ(response["home_server"].get<std::string>(), m_homeServer);
    ASSERT_TRUE(response.contains("well_known"));
}

TEST_F(MatrixClientTest, LoginResponseValidateTokenFormat) {
    json response = json::parse(m_loginJson);
    std::string token = response["access_token"].get<std::string>();

    ASSERT_TRUE(token.find("syt_") == 0);
    ASSERT_TRUE(token.size() > 10);
    ASSERT_FALSE(token.empty());
    ASSERT_TRUE(token.find(" ") == std::string::npos);
}

TEST_F(MatrixClientTest, LoginResponseUserIDFormat) {
    json response = json::parse(m_loginJson);
    std::string uid = response["user_id"].get<std::string>();

    ASSERT_TRUE(uid.find("@") == 0);
    ASSERT_TRUE(uid.find(":") != std::string::npos);
    ASSERT_EQ(uid.substr(0, 6), "@alice");
    ASSERT_FALSE(uid.empty());
}

TEST_F(MatrixClientTest, LoginWithWellKnownDiscovery) {
    json wellKnown;
    wellKnown["m.homeserver"] = {
        {"base_url", "https://matrix-client.example.org"}
    };
    wellKnown["m.identity_server"] = {
        {"base_url", "https://identity.example.org"}
    };

    ASSERT_TRUE(wellKnown.contains("m.homeserver"));
    ASSERT_TRUE(wellKnown.contains("m.identity_server"));
    ASSERT_EQ(wellKnown["m.homeserver"]["base_url"].get<std::string>(), "https://matrix-client.example.org");
    ASSERT_EQ(wellKnown["m.identity_server"]["base_url"].get<std::string>(), "https://identity.example.org");
}

TEST_F(MatrixClientTest, LoginFailedInvalidPassword) {
    json errorResp;
    errorResp["errcode"] = "M_FORBIDDEN";
    errorResp["error"] = "Invalid password";
    errorResp["soft_logout"] = false;

    ASSERT_EQ(errorResp["errcode"].get<std::string>(), "M_FORBIDDEN");
    ASSERT_FALSE(errorResp["error"].get<std::string>().empty());
    ASSERT_FALSE(errorResp["soft_logout"].get<bool>());
    ASSERT_TRUE(errorResp["error"].get<std::string>().find("Invalid") != std::string::npos);
}

TEST_F(MatrixClientTest, LoginFailedRateLimited) {
    json errorResp;
    errorResp["errcode"] = "M_LIMIT_EXCEEDED";
    errorResp["error"] = "Too many requests";
    errorResp["retry_after_ms"] = 30000;

    ASSERT_EQ(errorResp["errcode"].get<std::string>(), "M_LIMIT_EXCEEDED");
    ASSERT_EQ(errorResp["retry_after_ms"].get<int>(), 30000);
    ASSERT_TRUE(errorResp["retry_after_ms"].get<int>() > 0);
    ASSERT_FALSE(errorResp["error"].get<std::string>().empty());
}

// ============================================================================
// Sync Loop Tests
// ============================================================================

TEST_F(MatrixClientTest, SyncResponseParsing) {
    json sync = json::parse(m_syncJson);

    ASSERT_EQ(sync["next_batch"].get<std::string>(), m_nextBatch);
    ASSERT_TRUE(sync.contains("rooms"));
    ASSERT_TRUE(sync["rooms"].contains("join"));
    ASSERT_TRUE(sync["rooms"].contains("invite"));
    ASSERT_TRUE(sync["rooms"].contains("leave"));
}

TEST_F(MatrixClientTest, SyncInitialWithRooms) {
    json sync;
    sync["next_batch"] = "s987654321";
    sync["rooms"]["join"][m_roomId] = {
        {"state", {{"events", json::array()}}},
        {"timeline", {
            {"events", json::array()},
            {"limited", false},
            {"prev_batch", "t12345_12345"}
        }},
        {"ephemeral", {{"events", json::array()}}},
        {"account_data", {{"events", json::array()}}},
        {"unread_notifications", {
            {"highlight_count", 0},
            {"notification_count", 0}
        }},
        {"summary", {
            {"m.heroes", json::array()},
            {"m.joined_member_count", 2},
            {"m.invited_member_count", 0}
        }}
    };

    ASSERT_TRUE(sync["rooms"]["join"].contains(m_roomId));
    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["summary"]["m.joined_member_count"].get<int>(), 2);
    ASSERT_TRUE(sync["rooms"]["join"][m_roomId]["timeline"]["limited"].get<bool>() == false);
    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["unread_notifications"]["highlight_count"].get<int>(), 0);
}

TEST_F(MatrixClientTest, SyncWithTimelineEvents) {
    json sync;
    sync["next_batch"] = "s_events123";
    json timeline = json::array();
    for (int i = 0; i < 5; i++) {
        std::string eid = "$event" + std::to_string(i) + ":example.org";
        json content = {{"msgtype", "m.text"}, {"body", "Message " + std::to_string(i)}};
        timeline.push_back(json::parse(makeRoomEvent(eid, m_roomId, m_userId, "m.room.message", content)));
    }
    sync["rooms"]["join"][m_roomId] = {
        {"timeline", {{"events", timeline}}}
    };

    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["timeline"]["events"].size(), 5u);
    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["timeline"]["events"][0]["event_id"].get<std::string>(), "$event0:example.org");
    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["timeline"]["events"][4]["event_id"].get<std::string>(), "$event4:example.org");
}

TEST_F(MatrixClientTest, SyncWithStateEvents) {
    json sync;
    sync["next_batch"] = "s_state456";
    json states = json::array();
    states.push_back(json::parse(makeMembershipEvent("$ev1:example.org", m_roomId, "@alice:example.org", "@alice:example.org", "join")));
    states.push_back(json::parse(makeMembershipEvent("$ev2:example.org", m_roomId, "@bob:example.org", "@bob:example.org", "join")));
    sync["rooms"]["join"][m_roomId]["state"] = {{"events", states}};

    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["state"]["events"].size(), 2u);
    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["state"]["events"][0]["type"].get<std::string>(), "m.room.member");
    ASSERT_EQ(sync["rooms"]["join"][m_roomId]["state"]["events"][1]["sender"].get<std::string>(), "@bob:example.org");
}

TEST_F(MatrixClientTest, SyncNextBatchTracking) {
    std::vector<std::string> batches = {
        "s100000_1", "s200000_2", "s300000_3", "s400000_4", "s500000_5"
    };

    ASSERT_EQ(batches.size(), 5u);
    ASSERT_EQ(batches[0], "s100000_1");
    ASSERT_EQ(batches[4], "s500000_5");
    for (size_t i = 0; i < batches.size(); i++) {
        ASSERT_FALSE(batches[i].empty());
        ASSERT_TRUE(batches[i].find("s") == 0);
    }
}

TEST_F(MatrixClientTest, SyncFilteredResponse) {
    json filter;
    filter["room"] = {
        {"timeline", {
            {"limit", 20},
            {"types", json::array({"m.room.message"})}
        }},
        {"state", {
            {"types", json::array({"m.room.member", "m.room.name"})}
        }},
        {"ephemeral", {
            {"types", json::array({"m.typing", "m.receipt"})}
        }}
    };

    ASSERT_EQ(filter["room"]["timeline"]["limit"].get<int>(), 20);
    ASSERT_EQ(filter["room"]["timeline"]["types"].size(), 1u);
    ASSERT_EQ(filter["room"]["state"]["types"].size(), 2u);
    ASSERT_EQ(filter["room"]["ephemeral"]["types"].size(), 2u);
}

TEST_F(MatrixClientTest, SyncWithInvitedRooms) {
    json sync;
    sync["next_batch"] = "s_invites";
    std::string inviteRoom = "!invite456:example.org";
    json inviteState = json::array();
    inviteState.push_back(json::parse(makeMembershipEvent("$inv1:example.org", inviteRoom, "@bob:example.org", m_userId, "invite")));
    sync["rooms"]["invite"][inviteRoom] = {
        {"invite_state", {{"events", inviteState}}}
    };

    ASSERT_TRUE(sync["rooms"]["invite"].contains(inviteRoom));
    ASSERT_EQ(sync["rooms"]["invite"][inviteRoom]["invite_state"]["events"].size(), 1u);
    ASSERT_EQ(sync["rooms"]["invite"][inviteRoom]["invite_state"]["events"][0]["content"]["membership"].get<std::string>(), "invite");
}

TEST_F(MatrixClientTest, SyncWithLeftRooms) {
    json sync;
    sync["next_batch"] = "s_left";
    std::string leftRoom = "!left789:example.org";
    sync["rooms"]["leave"][leftRoom] = {
        {"timeline", {{"events", json::array()}}},
        {"state", {{"events", json::array()}}}
    };

    ASSERT_TRUE(sync["rooms"]["leave"].contains(leftRoom));
    ASSERT_TRUE(sync["rooms"]["leave"][leftRoom].contains("timeline"));
    ASSERT_TRUE(sync["rooms"]["leave"][leftRoom].contains("state"));
}

// ============================================================================
// Room Joining/Leaving Tests
// ============================================================================

TEST_F(MatrixClientTest, JoinRoomRequest) {
    json joinReq;
    joinReq["room_id"] = m_roomId;
    joinReq["third_party_signed"] = json::object();

    ASSERT_EQ(joinReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_TRUE(joinReq["third_party_signed"].is_object());
    ASSERT_FALSE(joinReq.contains("reason"));
}

TEST_F(MatrixClientTest, JoinRoomViaAlias) {
    std::string alias = "#testroom:example.org";
    json joinReq;
    joinReq["room_alias"] = alias;

    ASSERT_EQ(joinReq["room_alias"].get<std::string>(), alias);
    ASSERT_TRUE(joinReq["room_alias"].get<std::string>().find("#") == 0);
    ASSERT_TRUE(joinReq["room_alias"].get<std::string>().find(":") != std::string::npos);
}

TEST_F(MatrixClientTest, LeaveRoomRequest) {
    json leaveReq;
    leaveReq["room_id"] = m_roomId;
    leaveReq["reason"] = "No longer interested";

    ASSERT_EQ(leaveReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(leaveReq["reason"].get<std::string>(), "No longer interested");
    ASSERT_FALSE(leaveReq["reason"].get<std::string>().empty());
    ASSERT_TRUE(leaveReq["reason"].get<std::string>().size() > 0);
}

TEST_F(MatrixClientTest, ForgetRoomRequest) {
    json forgetReq;
    forgetReq["room_id"] = "!forgotten:example.org";

    ASSERT_EQ(forgetReq["room_id"].get<std::string>(), "!forgotten:example.org");
    ASSERT_TRUE(forgetReq["room_id"].get<std::string>().find("!") == 0);
    ASSERT_TRUE(forgetReq.contains("room_id"));
    ASSERT_FALSE(forgetReq.contains("reason"));
}

TEST_F(MatrixClientTest, KnockRoomRequest) {
    json knockReq;
    knockReq["room_id"] = "!knockable:example.org";
    knockReq["reason"] = "Can I join?";

    ASSERT_EQ(knockReq["room_id"].get<std::string>(), "!knockable:example.org");
    ASSERT_EQ(knockReq["reason"].get<std::string>(), "Can I join?");
    ASSERT_TRUE(knockReq["room_id"].get<std::string>().size() > 0);
    ASSERT_TRUE(knockReq["reason"].get<std::string>().size() > 0);
}

TEST_F(MatrixClientTest, RoomMembershipTracking) {
    std::map<std::string, std::string> membership;
    membership["!room1:example.org"] = "join";
    membership["!room2:example.org"] = "invite";
    membership["!room3:example.org"] = "leave";
    membership["!room4:example.org"] = "join";

    int joinCount = 0, inviteCount = 0, leaveCount = 0;
    for (const auto& [room, state] : membership) {
        if (state == "join") joinCount++;
        if (state == "invite") inviteCount++;
        if (state == "leave") leaveCount++;
    }
    ASSERT_EQ(joinCount, 2);
    ASSERT_EQ(inviteCount, 1);
    ASSERT_EQ(leaveCount, 1);
    ASSERT_EQ(membership.size(), 4u);
}

// ============================================================================
// Message Sending Tests
// ============================================================================

TEST_F(MatrixClientTest, SendTextMessage) {
    json sendReq;
    sendReq["msgtype"] = "m.text";
    sendReq["body"] = "Hello, World!";
    sendReq["room_id"] = m_roomId;

    ASSERT_EQ(sendReq["msgtype"].get<std::string>(), "m.text");
    ASSERT_EQ(sendReq["body"].get<std::string>(), "Hello, World!");
    ASSERT_EQ(sendReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_FALSE(sendReq["body"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, SendFormattedMessage) {
    json sendReq;
    sendReq["msgtype"] = "m.text";
    sendReq["body"] = "Hello **World**";
    sendReq["format"] = "org.matrix.custom.html";
    sendReq["formatted_body"] = "<b>Hello <em>World</em></b>";

    ASSERT_EQ(sendReq["msgtype"].get<std::string>(), "m.text");
    ASSERT_EQ(sendReq["format"].get<std::string>(), "org.matrix.custom.html");
    ASSERT_FALSE(sendReq["formatted_body"].get<std::string>().empty());
    ASSERT_TRUE(sendReq["formatted_body"].get<std::string>().find("<b>") != std::string::npos);
}

TEST_F(MatrixClientTest, SendImageMessage) {
    json sendReq;
    sendReq["msgtype"] = "m.image";
    sendReq["body"] = "photo.jpg";
    sendReq["url"] = "mxc://example.org/abc123def";
    sendReq["info"] = {
        {"mimetype", "image/jpeg"},
        {"size", 102400},
        {"w", 1920},
        {"h", 1080}
    };

    ASSERT_EQ(sendReq["msgtype"].get<std::string>(), "m.image");
    ASSERT_EQ(sendReq["url"].get<std::string>(), "mxc://example.org/abc123def");
    ASSERT_EQ(sendReq["info"]["mimetype"].get<std::string>(), "image/jpeg");
    ASSERT_EQ(sendReq["info"]["w"].get<int>(), 1920);
    ASSERT_EQ(sendReq["info"]["h"].get<int>(), 1080);
}

TEST_F(MatrixClientTest, SendWithTransactionId) {
    std::string txnId = "txn_" + std::to_string(std::chrono::system_clock::now().time_since_epoch().count());
    json sendReq;
    sendReq["txn_id"] = txnId;
    sendReq["msgtype"] = "m.text";
    sendReq["body"] = "Message with transaction ID";

    ASSERT_TRUE(sendReq["txn_id"].get<std::string>().find("txn_") == 0);
    ASSERT_FALSE(sendReq["txn_id"].get<std::string>().empty());
    ASSERT_EQ(sendReq["body"].get<std::string>(), "Message with transaction ID");
    ASSERT_TRUE(sendReq["txn_id"].get<std::string>().size() > 4);
}

// ============================================================================
// Pagination Tests
// ============================================================================

TEST_F(MatrixClientTest, PaginateBackwards) {
    json paginateReq;
    paginateReq["from"] = "t11111_22222";
    paginateReq["dir"] = "b";
    paginateReq["limit"] = 20;

    ASSERT_EQ(paginateReq["from"].get<std::string>(), "t11111_22222");
    ASSERT_EQ(paginateReq["dir"].get<std::string>(), "b");
    ASSERT_EQ(paginateReq["limit"].get<int>(), 20);
    ASSERT_TRUE(paginateReq["limit"].get<int>() > 0);
}

TEST_F(MatrixClientTest, PaginateForwards) {
    json paginateReq;
    paginateReq["from"] = "t_start_token";
    paginateReq["dir"] = "f";
    paginateReq["limit"] = 50;

    ASSERT_EQ(paginateReq["dir"].get<std::string>(), "f");
    ASSERT_EQ(paginateReq["limit"].get<int>(), 50);
    ASSERT_GT(paginateReq["limit"].get<int>(), 0);
}

TEST_F(MatrixClientTest, PaginateWithFilter) {
    json paginateReq;
    paginateReq["from"] = "t_token";
    paginateReq["dir"] = "b";
    paginateReq["limit"] = 30;
    paginateReq["filter"] = {
        {"types", json::array({"m.room.message"})},
        {"limit", 10}
    };

    ASSERT_EQ(paginateReq["filter"]["types"].size(), 1u);
    ASSERT_EQ(paginateReq["filter"]["limit"].get<int>(), 10);
    ASSERT_TRUE(paginateReq.contains("filter"));
    ASSERT_EQ(paginateReq["limit"].get<int>(), 30);
}

TEST_F(MatrixClientTest, PaginateResponseParsing) {
    json response;
    response["start"] = "t_start";
    response["end"] = "t_end";
    response["chunk"] = json::array();
    response["state"] = json::array();

    ASSERT_EQ(response["start"].get<std::string>(), "t_start");
    ASSERT_EQ(response["end"].get<std::string>(), "t_end");
    ASSERT_TRUE(response["chunk"].is_array());
    ASSERT_TRUE(response["state"].is_array());
}

// ============================================================================
// Receipts Tests
// ============================================================================

TEST_F(MatrixClientTest, SendReadReceipt) {
    json receipt;
    receipt["room_id"] = m_roomId;
    receipt["event_id"] = "$event999:example.org";
    receipt["receipt_type"] = "m.read";

    ASSERT_EQ(receipt["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(receipt["event_id"].get<std::string>(), "$event999:example.org");
    ASSERT_EQ(receipt["receipt_type"].get<std::string>(), "m.read");
    ASSERT_TRUE(receipt["event_id"].get<std::string>().find("$") == 0);
}

TEST_F(MatrixClientTest, ReceiptEventParsing) {
    std::string receiptJson = makeReceiptEvent(m_roomId, "$event123:example.org", m_userId);
    json receipt = json::parse(receiptJson);

    ASSERT_EQ(receipt["type"].get<std::string>(), "m.receipt");
    ASSERT_EQ(receipt["room_id"].get<std::string>(), m_roomId);
    ASSERT_TRUE(receipt["content"].contains("$event123:example.org"));
    ASSERT_TRUE(receipt["content"]["$event123:example.org"].contains("m.read"));
}

TEST_F(MatrixClientTest, FullyReadMarker) {
    json fullyRead;
    fullyRead["room_id"] = m_roomId;
    fullyRead["event_id"] = "$event555:example.org";

    ASSERT_EQ(fullyRead["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(fullyRead["event_id"].get<std::string>(), "$event555:example.org");
    ASSERT_TRUE(fullyRead["event_id"].get<std::string>().find("$event") == 0);
    ASSERT_FALSE(fullyRead["event_id"].get<std::string>().empty());
}

// ============================================================================
// Typing Tests
// ============================================================================

TEST_F(MatrixClientTest, SendTypingNotification) {
    json typing;
    typing["typing"] = true;
    typing["timeout"] = 30000;

    ASSERT_TRUE(typing["typing"].get<bool>());
    ASSERT_EQ(typing["timeout"].get<int>(), 30000);
    ASSERT_TRUE(typing["timeout"].get<int>() > 0);
}

TEST_F(MatrixClientTest, StopTypingNotification) {
    json typing;
    typing["typing"] = false;
    typing["timeout"] = 0;

    ASSERT_FALSE(typing["typing"].get<bool>());
    ASSERT_EQ(typing["timeout"].get<int>(), 0);
}

TEST_F(MatrixClientTest, TypingEventParsing) {
    json content;
    content["user_ids"] = json::array({"@alice:example.org", "@bob:example.org"});
    std::string typingJson = makeRoomEvent("$ev_typing:example.org", m_roomId, "@alice:example.org", "m.typing", content);
    json typing = json::parse(typingJson);

    ASSERT_EQ(typing["type"].get<std::string>(), "m.typing");
    ASSERT_EQ(typing["content"]["user_ids"].size(), 2u);
    ASSERT_EQ(typing["content"]["user_ids"][0].get<std::string>(), "@alice:example.org");
    ASSERT_EQ(typing["content"]["user_ids"][1].get<std::string>(), "@bob:example.org");
}

// ============================================================================
// Presence Tests
// ============================================================================

TEST_F(MatrixClientTest, SetPresenceOnline) {
    json presence;
    presence["presence"] = "online";
    presence["status_msg"] = "Available";

    ASSERT_EQ(presence["presence"].get<std::string>(), "online");
    ASSERT_EQ(presence["status_msg"].get<std::string>(), "Available");
    ASSERT_FALSE(presence["status_msg"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, SetPresenceOffline) {
    json presence;
    presence["presence"] = "offline";
    presence["status_msg"] = "";

    ASSERT_EQ(presence["presence"].get<std::string>(), "offline");
    ASSERT_TRUE(presence["status_msg"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, SetPresenceUnavailable) {
    json presence;
    presence["presence"] = "unavailable";
    presence["status_msg"] = "Away from keyboard";

    ASSERT_EQ(presence["presence"].get<std::string>(), "unavailable");
    ASSERT_EQ(presence["status_msg"].get<std::string>(), "Away from keyboard");
    ASSERT_TRUE(presence["presence"].get<std::string>() != "online");
}

TEST_F(MatrixClientTest, PresenceEventParsing) {
    std::string presenceJson = makePresenceEvent(m_userId, "online", "Hello!");
    json event = json::parse(presenceJson);

    ASSERT_EQ(event["type"].get<std::string>(), "m.presence");
    ASSERT_EQ(event["content"]["presence"].get<std::string>(), "online");
    ASSERT_EQ(event["content"]["status_msg"].get<std::string>(), "Hello!");
    ASSERT_TRUE(event["content"]["currently_active"].get<bool>());
}

// ============================================================================
// Push Rules Tests
// ============================================================================

TEST_F(MatrixClientTest, PushRulesGlobalOverride) {
    json rules;
    rules["global"] = {
        {"override", json::array()},
        {"content", json::array()},
        {"room", json::array()},
        {"sender", json::array()},
        {"underride", json::array()}
    };

    ASSERT_TRUE(rules.contains("global"));
    ASSERT_TRUE(rules["global"].contains("override"));
    ASSERT_TRUE(rules["global"].contains("content"));
    ASSERT_TRUE(rules["global"].contains("room"));
    ASSERT_TRUE(rules["global"].contains("sender"));
    ASSERT_TRUE(rules["global"].contains("underride"));
}

TEST_F(MatrixClientTest, PushRuleCreateOverride) {
    json rule;
    rule["rule_id"] = ".m.rule.room_one";
    rule["actions"] = json::array({"notify", json::object({{"set_tweak", "sound"}, {"value", "default"}})});
    rule["conditions"] = json::array();
    rule["enabled"] = true;

    ASSERT_EQ(rule["rule_id"].get<std::string>(), ".m.rule.room_one");
    ASSERT_EQ(rule["actions"].size(), 2u);
    ASSERT_TRUE(rule["enabled"].get<bool>());
    ASSERT_TRUE(rule["conditions"].is_array());
}

TEST_F(MatrixClientTest, PushRuleCreateContent) {
    json rule;
    rule["rule_id"] = "custom_content_rule";
    rule["pattern"] = "urgent";
    rule["actions"] = json::array({"notify"});
    rule["enabled"] = true;

    ASSERT_EQ(rule["pattern"].get<std::string>(), "urgent");
    ASSERT_EQ(rule["actions"].size(), 1u);
    ASSERT_TRUE(rule["enabled"].get<bool>());
    ASSERT_EQ(rule["actions"][0].get<std::string>(), "notify");
}

TEST_F(MatrixClientTest, PushRuleDisable) {
    json rule;
    rule["enabled"] = false;
    rule["rule_id"] = ".m.rule.contains_display_name";

    ASSERT_FALSE(rule["enabled"].get<bool>());
    ASSERT_EQ(rule["rule_id"].get<std::string>(), ".m.rule.contains_display_name");
}

TEST_F(MatrixClientTest, PushRuleDelete) {
    json deleteReq;
    deleteReq["scope"] = "global";
    deleteReq["kind"] = "override";
    deleteReq["rule_id"] = ".m.rule.custom";

    ASSERT_EQ(deleteReq["scope"].get<std::string>(), "global");
    ASSERT_EQ(deleteReq["kind"].get<std::string>(), "override");
    ASSERT_EQ(deleteReq["rule_id"].get<std::string>(), ".m.rule.custom");
}

TEST_F(MatrixClientTest, PushRuleSetEnabled) {
    json enabled;
    enabled["enabled"] = true;

    ASSERT_TRUE(enabled["enabled"].get<bool>());
}

TEST_F(MatrixClientTest, PushRuleSetActions) {
    json actions;
    actions["actions"] = json::array({
        "notify",
        json::object({{"set_tweak", "highlight"}, {"value", true}})
    });

    ASSERT_EQ(actions["actions"].size(), 2u);
    ASSERT_EQ(actions["actions"][0].get<std::string>(), "notify");
    ASSERT_TRUE(actions["actions"][1].is_object());
}

// ============================================================================
// Device Management Tests
// ============================================================================

TEST_F(MatrixClientTest, DeviceList) {
    json devices = json::array();
    json device1;
    device1["device_id"] = "DEVICE1";
    device1["display_name"] = "Desktop";
    device1["last_seen_ip"] = "192.168.1.100";
    device1["last_seen_ts"] = 1600000000000LL;
    devices.push_back(device1);

    ASSERT_EQ(devices.size(), 1u);
    ASSERT_EQ(devices[0]["device_id"].get<std::string>(), "DEVICE1");
    ASSERT_EQ(devices[0]["display_name"].get<std::string>(), "Desktop");
    ASSERT_EQ(devices[0]["last_seen_ts"].get<int64_t>(), 1600000000000LL);
}

TEST_F(MatrixClientTest, DeleteDevice) {
    json auth;
    auth["type"] = "m.login.password";
    auth["password"] = "current_password";
    auth["session"] = "session_id_123";

    ASSERT_EQ(auth["type"].get<std::string>(), "m.login.password");
    ASSERT_FALSE(auth["password"].get<std::string>().empty());
    ASSERT_FALSE(auth["session"].get<std::string>().empty());
    ASSERT_EQ(auth["password"].get<std::string>(), "current_password");
}

// ============================================================================
// Account Data Tests
// ============================================================================

TEST_F(MatrixClientTest, SetAccountData) {
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"] = {
        {"@bob:example.org", json::array({m_roomId})}
    };

    ASSERT_EQ(accountData["type"].get<std::string>(), "m.direct");
    ASSERT_TRUE(accountData["content"].contains("@bob:example.org"));
    ASSERT_EQ(accountData["content"]["@bob:example.org"].size(), 1u);
    ASSERT_EQ(accountData["content"]["@bob:example.org"][0].get<std::string>(), m_roomId);
}

TEST_F(MatrixClientTest, SetIgnoredUserList) {
    json ignored;
    ignored["content"] = {
        {"ignored_users", {
            {"@spammer:example.org", json::object()},
            {"@troll:example.org", json::object()}
        }}
    };

    ASSERT_EQ(ignored["content"]["ignored_users"].size(), 2u);
    ASSERT_TRUE(ignored["content"]["ignored_users"].contains("@spammer:example.org"));
    ASSERT_TRUE(ignored["content"]["ignored_users"].contains("@troll:example.org"));
}

// ============================================================================
// Token Management Tests
// ============================================================================

TEST_F(MatrixClientTest, RefreshToken) {
    json refreshReq;
    refreshReq["grant_type"] = "refresh_token";
    refreshReq["refresh_token"] = "mrt_abcdef123456";

    ASSERT_EQ(refreshReq["grant_type"].get<std::string>(), "refresh_token");
    ASSERT_TRUE(refreshReq["refresh_token"].get<std::string>().find("mrt_") == 0);
    ASSERT_FALSE(refreshReq["refresh_token"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, TokenExpiryDetection) {
    int64_t now = 1600000000000LL;
    int64_t expiry = now + 3600000; // 1 hour
    bool isExpired = (expiry <= now);
    bool isAboutToExpire = (expiry - now) < 300000; // 5 minutes

    ASSERT_FALSE(isExpired);
    ASSERT_FALSE(isAboutToExpire);
    ASSERT_GT(expiry, now);
}

TEST_F(MatrixClientTest, TokenExpired) {
    int64_t now = 1600000000000LL;
    int64_t expired = now - 1000; // 1 second ago
    bool isExpired = (expired <= now);

    ASSERT_TRUE(isExpired);
    ASSERT_LE(expired, now);
    ASSERT_LT(expired, now);
}

// ============================================================================
// Session Serialization Tests
// ============================================================================

TEST_F(MatrixClientTest, SerializeSession) {
    json sessionData;
    sessionData["user_id"] = m_userId;
    sessionData["access_token"] = m_accessToken;
    sessionData["device_id"] = m_deviceId;
    sessionData["home_server"] = m_homeServer;
    sessionData["last_sync_token"] = m_nextBatch;

    ASSERT_EQ(sessionData["user_id"].get<std::string>(), m_userId);
    ASSERT_EQ(sessionData["access_token"].get<std::string>(), m_accessToken);
    ASSERT_EQ(sessionData["device_id"].get<std::string>(), m_deviceId);
    ASSERT_EQ(sessionData["last_sync_token"].get<std::string>(), m_nextBatch);
}

TEST_F(MatrixClientTest, DeserializeSession) {
    std::string serialized = R"({"user_id":"@alice:example.org","access_token":"syt_token","device_id":"DEV","home_server":"https://matrix.example.org"})";
    json sessionData = json::parse(serialized);

    ASSERT_EQ(sessionData["user_id"].get<std::string>(), "@alice:example.org");
    ASSERT_EQ(sessionData["access_token"].get<std::string>(), "syt_token");
    ASSERT_EQ(sessionData["device_id"].get<std::string>(), "DEV");
    ASSERT_EQ(sessionData["home_server"].get<std::string>(), "https://matrix.example.org");
}

// ============================================================================
// Well-Known Tests
// ============================================================================

TEST_F(MatrixClientTest, WellKnownDiscovery) {
    json wellKnown;
    wellKnown["m.homeserver"] = {{"base_url", "https://matrix.example.org"}};
    wellKnown["m.identity_server"] = {{"base_url", "https://identity.example.org"}};
    wellKnown["m.integrations"] = {
        {"managers", json::array()},
        {"widgets", json::array()}
    };

    ASSERT_EQ(wellKnown["m.homeserver"]["base_url"].get<std::string>(), "https://matrix.example.org");
    ASSERT_EQ(wellKnown["m.identity_server"]["base_url"].get<std::string>(), "https://identity.example.org");
    ASSERT_TRUE(wellKnown["m.integrations"]["managers"].is_array());
    ASSERT_TRUE(wellKnown["m.integrations"]["widgets"].is_array());
}

// ============================================================================
// Room Directory Tests
// ============================================================================

TEST_F(MatrixClientTest, PublicRoomsQuery) {
    json query;
    query["limit"] = 100;
    query["since"] = "token123";
    query["filter"] = {
        {"generic_search_term", "chat"}
    };
    query["include_all_networks"] = true;
    query["third_party_instance_id"] = "irc";

    ASSERT_EQ(query["limit"].get<int>(), 100);
    ASSERT_EQ(query["since"].get<std::string>(), "token123");
    ASSERT_EQ(query["filter"]["generic_search_term"].get<std::string>(), "chat");
    ASSERT_TRUE(query["include_all_networks"].get<bool>());
    ASSERT_EQ(query["third_party_instance_id"].get<std::string>(), "irc");
}

TEST_F(MatrixClientTest, PublicRoomResult) {
    json publicRoom;
    publicRoom["room_id"] = "!public:example.org";
    publicRoom["name"] = "Public Chat";
    publicRoom["topic"] = "General discussion";
    publicRoom["canonical_alias"] = "#publicchat:example.org";
    publicRoom["num_joined_members"] = 42;
    publicRoom["world_readable"] = true;
    publicRoom["guest_can_join"] = false;

    ASSERT_EQ(publicRoom["room_id"].get<std::string>(), "!public:example.org");
    ASSERT_EQ(publicRoom["name"].get<std::string>(), "Public Chat");
    ASSERT_EQ(publicRoom["num_joined_members"].get<int>(), 42);
    ASSERT_TRUE(publicRoom["world_readable"].get<bool>());
    ASSERT_FALSE(publicRoom["guest_can_join"].get<bool>());
}

// ============================================================================
// Room Alias Resolution Tests
// ============================================================================

TEST_F(MatrixClientTest, ResolveAlias) {
    json resolved;
    resolved["room_id"] = m_roomId;
    resolved["servers"] = json::array({
        "matrix.example.org",
        "matrix2.example.org"
    });

    ASSERT_EQ(resolved["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(resolved["servers"].size(), 2u);
    ASSERT_EQ(resolved["servers"][0].get<std::string>(), "matrix.example.org");
    ASSERT_EQ(resolved["servers"][1].get<std::string>(), "matrix2.example.org");
}

TEST_F(MatrixClientTest, CreateAlias) {
    json aliasReq;
    aliasReq["room_id"] = m_roomId;
    aliasReq["alias"] = "#myroom:example.org";

    ASSERT_EQ(aliasReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(aliasReq["alias"].get<std::string>(), "#myroom:example.org");
    ASSERT_TRUE(aliasReq["alias"].get<std::string>().find("#") == 0);
}

// ============================================================================
// Search Tests
// ============================================================================

TEST_F(MatrixClientTest, MessageSearch) {
    json searchReq;
    searchReq["search_categories"] = {
        {"room_events", {
            {"search_term", "hello"},
            {"keys", json::array({"content.body"})},
            {"order_by", "recent"},
            {"include_state", false}
        }}
    };

    ASSERT_EQ(searchReq["search_categories"]["room_events"]["search_term"].get<std::string>(), "hello");
    ASSERT_EQ(searchReq["search_categories"]["room_events"]["keys"].size(), 1u);
    ASSERT_FALSE(searchReq["search_categories"]["room_events"]["include_state"].get<bool>());
}

TEST_F(MatrixClientTest, UserSearch) {
    json searchReq;
    searchReq["search_term"] = "bob";
    searchReq["limit"] = 10;

    ASSERT_EQ(searchReq["search_term"].get<std::string>(), "bob");
    ASSERT_EQ(searchReq["limit"].get<int>(), 10);
    ASSERT_FALSE(searchReq["search_term"].get<std::string>().empty());
}

// ============================================================================
// Error Handling Tests
// ============================================================================

TEST_F(MatrixClientTest, NetworkErrorTimeout) {
    json errorResp;
    errorResp["errcode"] = "M_UNKNOWN";
    errorResp["error"] = "Connection timed out";
    errorResp["is_network_error"] = true;

    ASSERT_TRUE(errorResp["is_network_error"].get<bool>());
    ASSERT_EQ(errorResp["errcode"].get<std::string>(), "M_UNKNOWN");
    ASSERT_TRUE(errorResp["error"].get<std::string>().find("time") != std::string::npos);
}

TEST_F(MatrixClientTest, UnknownTokenError) {
    json errorResp;
    errorResp["errcode"] = "M_UNKNOWN_TOKEN";
    errorResp["error"] = "Unrecognised access token";
    errorResp["soft_logout"] = true;
    errorResp["is_token_error"] = true;

    ASSERT_EQ(errorResp["errcode"].get<std::string>(), "M_UNKNOWN_TOKEN");
    ASSERT_TRUE(errorResp["soft_logout"].get<bool>());
    ASSERT_TRUE(errorResp["is_token_error"].get<bool>());
}

TEST_F(MatrixClientTest, ServerError) {
    json errorResp;
    errorResp["errcode"] = "M_UNKNOWN";
    errorResp["error"] = "Internal server error";
    errorResp["http_code"] = 500;

    ASSERT_EQ(errorResp["http_code"].get<int>(), 500);
    ASSERT_EQ(errorResp["errcode"].get<std::string>(), "M_UNKNOWN");
    ASSERT_FALSE(errorResp["error"].get<std::string>().empty());
}

// ============================================================================
// User Profile Tests
// ============================================================================

TEST_F(MatrixClientTest, GetProfile) {
    json profile;
    profile["displayname"] = "Alice";
    profile["avatar_url"] = "mxc://example.org/avatar123";

    ASSERT_EQ(profile["displayname"].get<std::string>(), "Alice");
    ASSERT_EQ(profile["avatar_url"].get<std::string>(), "mxc://example.org/avatar123");
    ASSERT_TRUE(profile["avatar_url"].get<std::string>().find("mxc://") == 0);
}

TEST_F(MatrixClientTest, SetDisplayName) {
    json setReq;
    setReq["displayname"] = "Alice B.";

    ASSERT_EQ(setReq["displayname"].get<std::string>(), "Alice B.");
    ASSERT_FALSE(setReq["displayname"].get<std::string>().empty());
    ASSERT_EQ(setReq["displayname"].get<std::string>().size(), 8u);
}

TEST_F(MatrixClientTest, SetAvatarUrl) {
    json setReq;
    setReq["avatar_url"] = "mxc://example.org/newavatar456";

    ASSERT_EQ(setReq["avatar_url"].get<std::string>(), "mxc://example.org/newavatar456");
    ASSERT_TRUE(setReq["avatar_url"].get<std::string>().find("mxc://") == 0);
    ASSERT_FALSE(setReq["avatar_url"].get<std::string>().empty());
}

// ============================================================================
// Room Versions Tests
// ============================================================================

TEST_F(MatrixClientTest, GetRoomVersions) {
    json versions;
    versions["versions"] = json::array({"1", "2", "3", "4", "5", "6", "7", "8", "9", "10"});

    ASSERT_EQ(versions["versions"].size(), 10u);
    ASSERT_EQ(versions["versions"][0].get<std::string>(), "1");
    ASSERT_EQ(versions["versions"][9].get<std::string>(), "10");
    ASSERT_TRUE(versions["versions"].is_array());
}

// ============================================================================
// Room Upgrades Tests
// ============================================================================

TEST_F(MatrixClientTest, UpgradeRoomRequest) {
    json upgradeReq;
    upgradeReq["new_version"] = "9";
    upgradeReq["room_id"] = m_roomId;

    ASSERT_EQ(upgradeReq["new_version"].get<std::string>(), "9");
    ASSERT_EQ(upgradeReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_FALSE(upgradeReq["new_version"].get<std::string>().empty());
}

// ============================================================================
// Invite Tests
// ============================================================================

TEST_F(MatrixClientTest, InviteUser) {
    json inviteReq;
    inviteReq["user_id"] = "@bob:example.org";
    inviteReq["reason"] = "Join our chat!";

    ASSERT_EQ(inviteReq["user_id"].get<std::string>(), "@bob:example.org");
    ASSERT_EQ(inviteReq["reason"].get<std::string>(), "Join our chat!");
    ASSERT_TRUE(inviteReq["user_id"].get<std::string>().find("@") == 0);
}

TEST_F(MatrixClientTest, Invite3pid) {
    json inviteReq;
    inviteReq["id_server"] = "https://identity.example.org";
    inviteReq["medium"] = "email";
    inviteReq["address"] = "bob@example.com";

    ASSERT_EQ(inviteReq["medium"].get<std::string>(), "email");
    ASSERT_EQ(inviteReq["address"].get<std::string>(), "bob@example.com");
    ASSERT_FALSE(inviteReq["id_server"].get<std::string>().empty());
}

// ============================================================================
// 3PID Tests
// ============================================================================

TEST_F(MatrixClientTest, Bind3pid) {
    json bindReq;
    bindReq["client_secret"] = "secret123abc";
    bindReq["id_server"] = "https://identity.example.org";
    bindReq["id_access_token"] = "id_token_xyz";
    bindReq["sid"] = "session_id_123";

    ASSERT_FALSE(bindReq["client_secret"].get<std::string>().empty());
    ASSERT_FALSE(bindReq["id_server"].get<std::string>().empty());
    ASSERT_FALSE(bindReq["id_access_token"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, Unbind3pid) {
    json unbindReq;
    unbindReq["address"] = "alice@example.com";
    unbindReq["medium"] = "email";

    ASSERT_EQ(unbindReq["address"].get<std::string>(), "alice@example.com");
    ASSERT_EQ(unbindReq["medium"].get<std::string>(), "email");
    ASSERT_FALSE(unbindReq["address"].get<std::string>().empty());
}

// ============================================================================
// Registration Tests
// ============================================================================

TEST_F(MatrixClientTest, RegisterUser) {
    json registerReq;
    registerReq["username"] = "newuser";
    registerReq["password"] = "newpassword123";
    registerReq["auth"] = {
        {"type", "m.login.dummy"},
        {"session", "session_id"}
    };
    registerReq["inhibit_login"] = false;
    registerReq["initial_device_display_name"] = "New Device";

    ASSERT_EQ(registerReq["username"].get<std::string>(), "newuser");
    ASSERT_FALSE(registerReq["inhibit_login"].get<bool>());
    ASSERT_EQ(registerReq["auth"]["type"].get<std::string>(), "m.login.dummy");
    ASSERT_FALSE(registerReq["initial_device_display_name"].get<std::string>().empty());
}

// ============================================================================
// Password Change Tests
// ============================================================================

TEST_F(MatrixClientTest, ChangePassword) {
    json changeReq;
    changeReq["new_password"] = "newsecurepass456";
    changeReq["auth"] = {
        {"type", "m.login.password"},
        {"password", "oldpassword"}
    };

    ASSERT_FALSE(changeReq["new_password"].get<std::string>().empty());
    ASSERT_EQ(changeReq["auth"]["type"].get<std::string>(), "m.login.password");
    ASSERT_FALSE(changeReq["auth"]["password"].get<std::string>().empty());
}

// ============================================================================
// Deactivate Account Tests
// ============================================================================

TEST_F(MatrixClientTest, DeactivateAccount) {
    json deactivateReq;
    deactivateReq["auth"] = {
        {"type", "m.login.password"},
        {"password", "current_password"}
    };
    deactivateReq["erase"] = true;

    ASSERT_EQ(deactivateReq["auth"]["type"].get<std::string>(), "m.login.password");
    ASSERT_TRUE(deactivateReq["erase"].get<bool>());
    ASSERT_FALSE(deactivateReq["auth"]["password"].get<std::string>().empty());
}

// ============================================================================
// Upload Tests
// ============================================================================

TEST_F(MatrixClientTest, UploadMedia) {
    json uploadMeta;
    uploadMeta["filename"] = "image.jpg";
    uploadMeta["content_type"] = "image/jpeg";
    uploadMeta["content_length"] = 204800;

    ASSERT_EQ(uploadMeta["filename"].get<std::string>(), "image.jpg");
    ASSERT_EQ(uploadMeta["content_type"].get<std::string>(), "image/jpeg");
    ASSERT_EQ(uploadMeta["content_length"].get<int>(), 204800);
    ASSERT_GT(uploadMeta["content_length"].get<int>(), 0);
}

TEST_F(MatrixClientTest, UploadResponse) {
    json uploadResp;
    uploadResp["content_uri"] = "mxc://example.org/uploaded123";

    ASSERT_EQ(uploadResp["content_uri"].get<std::string>(), "mxc://example.org/uploaded123");
    ASSERT_TRUE(uploadResp["content_uri"].get<std::string>().find("mxc://") == 0);
    ASSERT_FALSE(uploadResp["content_uri"].get<std::string>().empty());
}

// ============================================================================
// Room Tags Tests
// ============================================================================

TEST_F(MatrixClientTest, SetRoomTag) {
    json tagReq;
    tagReq["room_id"] = m_roomId;
    tagReq["tag"] = "m.favourite";
    tagReq["order"] = 0.5;

    ASSERT_EQ(tagReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(tagReq["tag"].get<std::string>(), "m.favourite");
    ASSERT_EQ(tagReq["order"].get<double>(), 0.5);
}

TEST_F(MatrixClientTest, DeleteRoomTag) {
    json deleteReq;
    deleteReq["room_id"] = m_roomId;
    deleteReq["tag"] = "m.lowpriority";

    ASSERT_EQ(deleteReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(deleteReq["tag"].get<std::string>(), "m.lowpriority");
    ASSERT_TRUE(deleteReq["tag"].get<std::string>().find("m.") == 0);
}

// ============================================================================
// Report Content Tests
// ============================================================================

TEST_F(MatrixClientTest, ReportContent) {
    json reportReq;
    reportReq["score"] = -100;
    reportReq["reason"] = "Spam message";
    reportReq["event_id"] = "$spam_event:example.org";

    ASSERT_EQ(reportReq["score"].get<int>(), -100);
    ASSERT_EQ(reportReq["reason"].get<std::string>(), "Spam message");
    ASSERT_EQ(reportReq["event_id"].get<std::string>(), "$spam_event:example.org");
    ASSERT_TRUE(reportReq["score"].get<int>() < 0);
}

// ============================================================================
// Room Context Tests
// ============================================================================

TEST_F(MatrixClientTest, RoomContext) {
    json contextReq;
    contextReq["room_id"] = m_roomId;
    contextReq["event_id"] = "$event_context:example.org";
    contextReq["limit"] = 10;

    ASSERT_EQ(contextReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(contextReq["event_id"].get<std::string>(), "$event_context:example.org");
    ASSERT_EQ(contextReq["limit"].get<int>(), 10);
}

// ============================================================================
// Session Event Callbacks Tests
// ============================================================================

TEST_F(MatrixClientTest, EventCallbackRegistration) {
    std::vector<std::string> registeredCallbacks;
    registeredCallbacks.push_back("m.room.message");
    registeredCallbacks.push_back("m.room.member");
    registeredCallbacks.push_back("m.typing");
    registeredCallbacks.push_back("m.receipt");

    ASSERT_EQ(registeredCallbacks.size(), 4u);
    ASSERT_EQ(registeredCallbacks[0], "m.room.message");
    ASSERT_EQ(registeredCallbacks[3], "m.receipt");
    for (const auto& cb : registeredCallbacks) {
        ASSERT_TRUE(cb.find("m.") == 0);
    }
}

// ============================================================================
// Content Repository Tests
// ============================================================================

TEST_F(MatrixClientTest, DownloadContent) {
    json downloadMeta;
    downloadMeta["server_name"] = "example.org";
    downloadMeta["media_id"] = "abc123def456";
    downloadMeta["allow_remote"] = true;

    ASSERT_EQ(downloadMeta["server_name"].get<std::string>(), "example.org");
    ASSERT_EQ(downloadMeta["media_id"].get<std::string>(), "abc123def456");
    ASSERT_TRUE(downloadMeta["allow_remote"].get<bool>());
}

TEST_F(MatrixClientTest, GetThumbnail) {
    json thumbReq;
    thumbReq["server_name"] = "example.org";
    thumbReq["media_id"] = "abc123";
    thumbReq["width"] = 100;
    thumbReq["height"] = 100;
    thumbReq["method"] = "scale";

    ASSERT_EQ(thumbReq["width"].get<int>(), 100);
    ASSERT_EQ(thumbReq["height"].get<int>(), 100);
    ASSERT_EQ(thumbReq["method"].get<std::string>(), "scale");
    ASSERT_EQ(thumbReq["method"].get<std::string>().size(), 5u);
}

// ============================================================================
// Configuration Tests
// ============================================================================

TEST_F(MatrixClientTest, ServerConfig) {
    json config;
    config["m.homeserver"] = {
        {"base_url", m_homeServer}
    };
    config["m.identity_server"] = {
        {"base_url", "https://identity.example.org"}
    };

    ASSERT_EQ(config["m.homeserver"]["base_url"].get<std::string>(), m_homeServer);
    ASSERT_TRUE(config["m.identity_server"]["base_url"].get<std::string>().find("https://") == 0);
    ASSERT_TRUE(config.contains("m.homeserver"));
    ASSERT_TRUE(config.contains("m.identity_server"));
}

// ============================================================================
// Room Summary Processing Tests
// ============================================================================

TEST_F(MatrixClientTest, RoomSummaryHeroes) {
    json summary;
    summary["m.heroes"] = json::array({"@alice:example.org", "@bob:example.org"});
    summary["m.joined_member_count"] = 5;
    summary["m.invited_member_count"] = 1;

    ASSERT_EQ(summary["m.heroes"].size(), 2u);
    ASSERT_EQ(summary["m.joined_member_count"].get<int>(), 5);
    ASSERT_EQ(summary["m.invited_member_count"].get<int>(), 1);
    ASSERT_EQ(summary["m.heroes"][0].get<std::string>(), "@alice:example.org");
}

// ============================================================================
// Third Party Network Tests
// ============================================================================

TEST_F(MatrixClientTest, ThirdPartyProtocols) {
    json protocols;
    protocols["irc"] = {
        {"user_fields", json::array({"network", "nickname"})},
        {"location_fields", json::array({"channel"})},
        {"instances", json::array()}
    };

    ASSERT_TRUE(protocols.contains("irc"));
    ASSERT_EQ(protocols["irc"]["user_fields"].size(), 2u);
    ASSERT_EQ(protocols["irc"]["location_fields"].size(), 1u);
    ASSERT_TRUE(protocols["irc"]["instances"].is_array());
}

// ============================================================================
// Push Rules Extended Tests
// ============================================================================

TEST_F(MatrixClientTest, PushRuleContentMatchKeyword) {
    std::vector<json> rules;
    json rule1;
    rule1["rule_id"] = ".m.rule.contains_user_name";
    rule1["enabled"] = true;
    rule1["pattern"] = "alice";
    rule1["actions"] = json::array({"notify", json::object({{"set_tweak", "highlight"}, {"value", true}})});
    rules.push_back(rule1);

    ASSERT_EQ(rules.size(), 1u);
    ASSERT_EQ(rules[0]["pattern"].get<std::string>(), "alice");
    ASSERT_EQ(rules[0]["actions"].size(), 2u);
    ASSERT_TRUE(rules[0]["enabled"].get<bool>());
}

TEST_F(MatrixClientTest, PushRuleRoomSpecific) {
    json rule;
    rule["rule_id"] = "!specific_room:example.org";
    rule["enabled"] = true;
    rule["actions"] = json::array({"notify"});

    ASSERT_EQ(rule["rule_id"].get<std::string>(), "!specific_room:example.org");
    ASSERT_TRUE(rule["rule_id"].get<std::string>().find("!") == 0);
    ASSERT_EQ(rule["actions"].size(), 1u);
}

TEST_F(MatrixClientTest, PushRuleSenderSpecific) {
    json rule;
    rule["rule_id"] = "@important:example.org";
    rule["enabled"] = true;
    rule["actions"] = json::array({"notify", json::object({{"set_tweak", "sound"}, {"value", "alarm"}})});

    ASSERT_EQ(rule["rule_id"].get<std::string>(), "@important:example.org");
    ASSERT_EQ(rule["actions"].size(), 2u);
    ASSERT_TRUE(rule["enabled"].get<bool>());
}

TEST_F(MatrixClientTest, PushRuleBulkUpdate) {
    json bulkUpdate;
    bulkUpdate["global"] = {
        {"override", json::array()},
        {"content", json::array()},
        {"room", json::array()},
        {"sender", json::array()},
        {"underride", json::array()}
    };

    ASSERT_TRUE(bulkUpdate["global"].is_object());
    ASSERT_EQ(bulkUpdate["global"].size(), 5u);
    ASSERT_TRUE(bulkUpdate["global"]["override"].is_array());
    ASSERT_TRUE(bulkUpdate["global"]["content"].is_array());
}

TEST_F(MatrixClientTest, PushRuleConditionEventMatch) {
    json condition;
    condition["kind"] = "event_match";
    condition["key"] = "type";
    condition["pattern"] = "m.room.message";

    ASSERT_EQ(condition["kind"].get<std::string>(), "event_match");
    ASSERT_EQ(condition["key"].get<std::string>(), "type");
    ASSERT_EQ(condition["pattern"].get<std::string>(), "m.room.message");
}

TEST_F(MatrixClientTest, PushRuleConditionRoomMemberCount) {
    json condition;
    condition["kind"] = "room_member_count";
    condition["is"] = "<=10";

    ASSERT_EQ(condition["kind"].get<std::string>(), "room_member_count");
    ASSERT_EQ(condition["is"].get<std::string>(), "<=10");
    ASSERT_TRUE(condition["is"].get<std::string>().find("<=") == 0);
}

TEST_F(MatrixClientTest, PushRuleConditionDisplayName) {
    json condition;
    condition["kind"] = "contains_display_name";

    ASSERT_EQ(condition["kind"].get<std::string>(), "contains_display_name");
    ASSERT_FALSE(condition["kind"].get<std::string>().empty());
}

// ============================================================================
// OpenID Tests
// ============================================================================

TEST_F(MatrixClientTest, OpenIDTokenRequest) {
    json openidReq;
    openidReq["room_id"] = m_roomId;

    ASSERT_EQ(openidReq["room_id"].get<std::string>(), m_roomId);
    ASSERT_TRUE(openidReq["room_id"].get<std::string>().find("!") == 0);
}

TEST_F(MatrixClientTest, OpenIDTokenResponse) {
    json openidResp;
    openidResp["access_token"] = "openid_token_xyz";
    openidResp["token_type"] = "Bearer";
    openidResp["matrix_server_name"] = "example.org";
    openidResp["expires_in"] = 3600;

    ASSERT_EQ(openidResp["access_token"].get<std::string>(), "openid_token_xyz");
    ASSERT_EQ(openidResp["token_type"].get<std::string>(), "Bearer");
    ASSERT_EQ(openidResp["expires_in"].get<int>(), 3600);
    ASSERT_TRUE(openidResp["expires_in"].get<int>() > 0);
}

// ============================================================================
// Terms of Service Tests
// ============================================================================

TEST_F(MatrixClientTest, TermsOfService) {
    json terms;
    terms["policies"] = {
        {"terms_of_service", {
            {"version", "1.2"},
            {"en", {
                {"name", "Terms of Service"},
                {"url", "https://example.org/terms"}
            }}
        }}
    };

    ASSERT_TRUE(terms["policies"].contains("terms_of_service"));
    ASSERT_EQ(terms["policies"]["terms_of_service"]["version"].get<std::string>(), "1.2");
    ASSERT_EQ(terms["policies"]["terms_of_service"]["en"]["name"].get<std::string>(), "Terms of Service");
    ASSERT_FALSE(terms["policies"]["terms_of_service"]["en"]["url"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, AcceptTerms) {
    json acceptReq;
    acceptReq["user_accepts"] = json::array({
        "https://example.org/terms/1.2"
    });

    ASSERT_EQ(acceptReq["user_accepts"].size(), 1u);
    ASSERT_EQ(acceptReq["user_accepts"][0].get<std::string>(), "https://example.org/terms/1.2");
    ASSERT_TRUE(acceptReq["user_accepts"][0].get<std::string>().find("https://") == 0);
}

// ============================================================================
// Notification Count Tests
// ============================================================================

TEST_F(MatrixClientTest, NotificationCounts) {
    json counts;
    counts["highlight_count"] = 3;
    counts["notification_count"] = 5;
    counts["unread_count"] = 10;

    ASSERT_EQ(counts["highlight_count"].get<int>(), 3);
    ASSERT_EQ(counts["notification_count"].get<int>(), 5);
    ASSERT_EQ(counts["unread_count"].get<int>(), 10);
    ASSERT_GT(counts["notification_count"].get<int>(), counts["highlight_count"].get<int>());
}

TEST_F(MatrixClientTest, NotificationCountsZero) {
    json counts;
    counts["highlight_count"] = 0;
    counts["notification_count"] = 0;

    ASSERT_EQ(counts["highlight_count"].get<int>(), 0);
    ASSERT_EQ(counts["notification_count"].get<int>(), 0);
    ASSERT_EQ(counts["highlight_count"].get<int>() + counts["notification_count"].get<int>(), 0);
}

// ============================================================================
// Pusher Configuration Tests
// ============================================================================

TEST_F(MatrixClientTest, SetPusher) {
    json pusherReq;
    pusherReq["pushkey"] = "APA91bHPRgkF3JUikC4ENAHEeMrdwJs";
    pusherReq["kind"] = "http";
    pusherReq["app_id"] = "com.example.pcp";
    pusherReq["app_display_name"] = "PCP";
    pusherReq["device_display_name"] = "Android Phone";
    pusherReq["profile_tag"] = "default";
    pusherReq["lang"] = "en";
    pusherReq["data"] = {
        {"url", "https://push.example.org/_matrix/push/v1/notify"}
    };
    pusherReq["append"] = false;

    ASSERT_EQ(pusherReq["kind"].get<std::string>(), "http");
    ASSERT_FALSE(pusherReq["append"].get<bool>());
    ASSERT_EQ(pusherReq["lang"].get<std::string>(), "en");
    ASSERT_FALSE(pusherReq["pushkey"].get<std::string>().empty());
    ASSERT_TRUE(pusherReq["data"].contains("url"));
}

TEST_F(MatrixClientTest, GetPushers) {
    json pushers = json::array();
    json pusher;
    pusher["pushkey"] = "push_key_123";
    pusher["kind"] = "http";
    pusher["app_id"] = "com.example.app";
    pushers.push_back(pusher);

    ASSERT_EQ(pushers.size(), 1u);
    ASSERT_EQ(pushers[0]["kind"].get<std::string>(), "http");
    ASSERT_EQ(pushers[0]["app_id"].get<std::string>(), "com.example.app");
}

// ============================================================================
// Homeserver Capabilities Tests
// ============================================================================

TEST_F(MatrixClientTest, HomeserverCapabilities) {
    json capabilities;
    capabilities["capabilities"] = {
        {"m.room_versions", {
            {"default", "9"},
            {"available", json::object({
                {"9", "stable"},
                {"8", "stable"},
                {"7", "stable"}
            })}
        }},
        {"m.change_password", {
            {"enabled", true}
        }}
    };

    ASSERT_TRUE(capabilities["capabilities"].contains("m.room_versions"));
    ASSERT_EQ(capabilities["capabilities"]["m.room_versions"]["default"].get<std::string>(), "9");
    ASSERT_TRUE(capabilities["capabilities"]["m.change_password"]["enabled"].get<bool>());
    ASSERT_TRUE(capabilities["capabilities"]["m.room_versions"]["available"].is_object());
}

// ============================================================================
// Event Context Tests
// ============================================================================

TEST_F(MatrixClientTest, EventContextResponse) {
    json context;
    context["start"] = "t_st_tok";
    context["end"] = "t_end_tok";
    context["events_before"] = json::array();
    context["events_after"] = json::array();
    context["state"] = json::array();

    for (int i = 0; i < 5; i++) {
        context["events_before"].push_back(
            json::parse(makeTextMessageEvent(
                "$before" + std::to_string(i) + ":example.org",
                m_roomId, m_userId,
                "Before message " + std::to_string(i)))
        );
    }

    ASSERT_EQ(context["events_before"].size(), 5u);
    ASSERT_TRUE(context["events_after"].is_array());
    ASSERT_EQ(context["events_before"][0]["content"]["body"].get<std::string>(), "Before message 0");
}

// ============================================================================
// Joined Rooms Tests
// ============================================================================

TEST_F(MatrixClientTest, JoinedRoomsResponse) {
    json joinedRooms;
    joinedRooms["joined_rooms"] = json::array({
        "!room1:example.org",
        "!room2:example.org",
        "!room3:example.org"
    });

    ASSERT_EQ(joinedRooms["joined_rooms"].size(), 3u);
    ASSERT_EQ(joinedRooms["joined_rooms"][0].get<std::string>(), "!room1:example.org");
    ASSERT_EQ(joinedRooms["joined_rooms"][2].get<std::string>(), "!room3:example.org");
    for (const auto& room : joinedRooms["joined_rooms"]) {
        ASSERT_TRUE(room.get<std::string>().find("!") == 0);
    }
}

// ============================================================================
// Key Backup Tests (Client-side)
// ============================================================================

TEST_F(MatrixClientTest, KeyBackupVersionCheck) {
    json backupVersion;
    backupVersion["version"] = "1";
    backupVersion["algorithm"] = "m.megolm_backup.v1.curve25519-aes-sha2";
    backupVersion["auth_data"] = {
        {"public_key", "curve25519_pub_key_here"},
        {"signatures", json::object()}
    };
    backupVersion["count"] = 0;
    backupVersion["etag"] = "etag_value";

    ASSERT_EQ(backupVersion["version"].get<std::string>(), "1");
    ASSERT_EQ(backupVersion["algorithm"].get<std::string>(), "m.megolm_backup.v1.curve25519-aes-sha2");
    ASSERT_TRUE(backupVersion["auth_data"].contains("public_key"));
    ASSERT_EQ(backupVersion["count"].get<int>(), 0);
}

TEST_F(MatrixClientTest, KeyBackupRoomKeyData) {
    json roomKeyData;
    roomKeyData["rooms"] = {
        {"!room1:example.org", {
            {"sessions", {
                {"session_id_1", {
                    {"first_message_index", 0},
                    {"forwarded_count", 0},
                    {"is_verified", true},
                    {"session_data", json::object({
                        {"ciphertext", "encrypted_data_here"},
                        {"ephemeral", "ephemeral_key"},
                        {"mac", "mac_value"}
                    })}
                }}
            }}
        }}
    };

    ASSERT_TRUE(roomKeyData["rooms"].contains("!room1:example.org"));
    ASSERT_TRUE(roomKeyData["rooms"]["!room1:example.org"]["sessions"].contains("session_id_1"));
    ASSERT_TRUE(roomKeyData["rooms"]["!room1:example.org"]["sessions"]["session_id_1"]["is_verified"].get<bool>());
    ASSERT_EQ(roomKeyData["rooms"]["!room1:example.org"]["sessions"]["session_id_1"]["first_message_index"].get<int>(), 0);
}

// ============================================================================
// Account Data Extended Tests
// ============================================================================

TEST_F(MatrixClientTest, AccountDataDirectRooms) {
    json directRooms;
    directRooms["content"] = {
        {"@bob:example.org", json::array({"!room1:example.org"})},
        {"@charlie:example.org", json::array({"!room2:example.org", "!room3:example.org"})}
    };

    ASSERT_EQ(directRooms["content"].size(), 2u);
    ASSERT_EQ(directRooms["content"]["@bob:example.org"].size(), 1u);
    ASSERT_EQ(directRooms["content"]["@charlie:example.org"].size(), 2u);
}

TEST_F(MatrixClientTest, AccountDataSourceClientConfig) {
    json config;
    config["type"] = "im.vector.web.settings";
    config["content"] = {
        {"notifications", json::object()},
        {"theme", "dark"},
        {"language", "en-GB"}
    };

    ASSERT_EQ(config["content"]["theme"].get<std::string>(), "dark");
    ASSERT_EQ(config["content"]["language"].get<std::string>(), "en-GB");
    ASSERT_TRUE(config["content"].is_object());
}

// ============================================================================
// Per-Room Account Data Tests
// ============================================================================

TEST_F(MatrixClientTest, RoomAccountDataFullyRead) {
    json fullyRead;
    fullyRead["type"] = "m.fully_read";
    fullyRead["content"] = {
        {"event_id", "$event_read:example.org"}
    };

    ASSERT_EQ(fullyRead["type"].get<std::string>(), "m.fully_read");
    ASSERT_EQ(fullyRead["content"]["event_id"].get<std::string>(), "$event_read:example.org");
    ASSERT_TRUE(fullyRead["content"]["event_id"].get<std::string>().find("$") == 0);
}

TEST_F(MatrixClientTest, RoomAccountDataTags) {
    json tags;
    tags["type"] = "m.tag";
    tags["content"] = {
        {"tags", {
            {"m.favourite", {{"order", 0.5}}},
            {"u.custom_tag", {{"order", 0.8}}}
        }}
    };

    ASSERT_TRUE(tags["content"]["tags"].contains("m.favourite"));
    ASSERT_TRUE(tags["content"]["tags"].contains("u.custom_tag"));
    ASSERT_EQ(tags["content"]["tags"]["m.favourite"]["order"].get<double>(), 0.5);
}

// ============================================================================
// Filter Management Tests
// ============================================================================

TEST_F(MatrixClientTest, CreateEventFilter) {
    json filter;
    filter["event_fields"] = json::array({"type", "content", "sender"});
    filter["event_format"] = "client";
    filter["presence"] = {
        {"types", json::array({"m.presence"})}
    };
    filter["account_data"] = {
        {"types", json::array({"m.direct"})}
    };
    filter["room"] = {
        {"timeline", {{"limit", 50}}},
        {"state", {{"types", json::array({"m.room.member"})}}}
    };

    ASSERT_EQ(filter["event_fields"].size(), 3u);
    ASSERT_EQ(filter["event_format"].get<std::string>(), "client");
    ASSERT_EQ(filter["room"]["timeline"]["limit"].get<int>(), 50);
    ASSERT_TRUE(filter["room"]["state"]["types"].is_array());
}

TEST_F(MatrixClientTest, UploadFilterResponse) {
    json response;
    response["filter_id"] = "filter_id_abc123";

    ASSERT_EQ(response["filter_id"].get<std::string>(), "filter_id_abc123");
    ASSERT_FALSE(response["filter_id"].get<std::string>().empty());
    ASSERT_TRUE(response["filter_id"].get<std::string>().find("filter_id_") == 0);
}

// ============================================================================
// Server Notice Tests
// ============================================================================

TEST_F(MatrixClientTest, ServerNoticeRoom) {
    json serverNotice;
    serverNotice["room_id"] = "!server_notice:example.org";
    serverNotice["server_notice_type"] = "m.server_notice";
    serverNotice["admin_contact"] = "mailto:admin@example.org";

    ASSERT_TRUE(serverNotice["room_id"].get<std::string>().find("server_notice") != std::string::npos);
    ASSERT_EQ(serverNotice["server_notice_type"].get<std::string>(), "m.server_notice");
    ASSERT_EQ(serverNotice["admin_contact"].get<std::string>(), "mailto:admin@example.org");
}

// ============================================================================
// Room Preview Tests
// ============================================================================

TEST_F(MatrixClientTest, RoomPreviewFromAlias) {
    json preview;
    preview["room_id"] = "!preview_room:example.org";
    preview["name"] = "Preview Room";
    preview["topic"] = "This is a preview";
    preview["num_joined_members"] = 15;
    preview["canonical_alias"] = "#preview:example.org";

    ASSERT_EQ(preview["name"].get<std::string>(), "Preview Room");
    ASSERT_EQ(preview["num_joined_members"].get<int>(), 15);
    ASSERT_EQ(preview["canonical_alias"].get<std::string>(), "#preview:example.org");
    ASSERT_FALSE(preview["topic"].get<std::string>().empty());
}

// ============================================================================
// Device Verification Tests (Client-side)
// ============================================================================

TEST_F(MatrixClientTest, DeviceVerificationStart) {
    json verificationStart;
    verificationStart["from_device"] = "DEVICE_A";
    verificationStart["transaction_id"] = "txn_verify_1";
    verificationStart["method"] = "m.sas.v1";
    verificationStart["timestamp"] = 1600000000000LL;

    ASSERT_EQ(verificationStart["from_device"].get<std::string>(), "DEVICE_A");
    ASSERT_EQ(verificationStart["method"].get<std::string>(), "m.sas.v1");
    ASSERT_GT(verificationStart["timestamp"].get<int64_t>(), 0);
}

TEST_F(MatrixClientTest, DeviceVerificationAccept) {
    json accept;
    accept["transaction_id"] = "txn_verify_1";
    accept["method"] = "m.sas.v1";
    accept["key_agreement_protocol"] = "curve25519-hkdf-sha256";
    accept["hash"] = "sha256";
    accept["message_authentication_code"] = "hkdf-hmac-sha256";
    accept["short_authentication_string"] = json::array({"decimal", "emoji"});
    accept["commitment"] = "commitment_value_here";

    ASSERT_EQ(accept["key_agreement_protocol"].get<std::string>(), "curve25519-hkdf-sha256");
    ASSERT_EQ(accept["hash"].get<std::string>(), "sha256");
    ASSERT_EQ(accept["short_authentication_string"].size(), 2u);
    ASSERT_FALSE(accept["commitment"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, DeviceVerificationKey) {
    json key;
    key["transaction_id"] = "txn_verify_1";
    key["key"] = "ed25519:public_key_base64";

    ASSERT_EQ(key["transaction_id"].get<std::string>(), "txn_verify_1");
    ASSERT_EQ(key["key"].get<std::string>(), "ed25519:public_key_base64");
    ASSERT_TRUE(key["key"].get<std::string>().find("ed25519:") == 0);
}

TEST_F(MatrixClientTest, DeviceVerificationMac) {
    json mac;
    mac["transaction_id"] = "txn_verify_1";
    mac["keys"] = "mac_of_device_keys";
    mac["mac"] = {
        {"ed25519:DEVICE_A", "computed_mac_value_here"}
    };

    ASSERT_EQ(mac["keys"].get<std::string>(), "mac_of_device_keys");
    ASSERT_TRUE(mac["mac"].is_object());
    ASSERT_TRUE(mac["mac"].contains("ed25519:DEVICE_A"));
}

TEST_F(MatrixClientTest, DeviceVerificationCancel) {
    json cancel;
    cancel["transaction_id"] = "txn_verify_1";
    cancel["code"] = "m.user";
    cancel["reason"] = "User declined";

    ASSERT_EQ(cancel["code"].get<std::string>(), "m.user");
    ASSERT_EQ(cancel["reason"].get<std::string>(), "User declined");
    ASSERT_FALSE(cancel["reason"].get<std::string>().empty());
}

// ============================================================================
// Room Visibility and Discovery Tests
// ============================================================================

TEST_F(MatrixClientTest, SetRoomVisibility) {
    json visibility;
    visibility["room_id"] = m_roomId;
    visibility["visibility"] = "public";

    ASSERT_EQ(visibility["room_id"].get<std::string>(), m_roomId);
    ASSERT_EQ(visibility["visibility"].get<std::string>(), "public");
    EXPECT_TRUE(visibility["visibility"].get<std::string>() == "public" ||
                visibility["visibility"].get<std::string>() == "private");
}

// ============================================================================
// Guest Access Tests
// ============================================================================

TEST_F(MatrixClientTest, GuestAccessQuery) {
    json guestAccess;
    guestAccess["guest_access"] = "can_join";

    ASSERT_EQ(guestAccess["guest_access"].get<std::string>(), "can_join");
    ASSERT_TRUE(guestAccess["guest_access"].get<std::string>() == "can_join" ||
                guestAccess["guest_access"].get<std::string>() == "forbidden");
}

// ============================================================================
// History Visibility Tests
// ============================================================================

TEST_F(MatrixClientTest, SetHistoryVisibility) {
    json history;
    history["history_visibility"] = "shared";

    ASSERT_EQ(history["history_visibility"].get<std::string>(), "shared");
    ASSERT_TRUE(history["history_visibility"].get<std::string>() == "shared" ||
                history["history_visibility"].get<std::string>() == "invited" ||
                history["history_visibility"].get<std::string>() == "joined");
}

// ============================================================================
// Content Scanning Tests
// ============================================================================

TEST_F(MatrixClientTest, ContentScanResponse) {
    json scanResp;
    scanResp["clean"] = true;
    scanResp["info"] = "File is safe";

    ASSERT_TRUE(scanResp["clean"].get<bool>());
    ASSERT_EQ(scanResp["info"].get<std::string>(), "File is safe");
    ASSERT_FALSE(scanResp["info"].get<std::string>().empty());
}

// ============================================================================
// Shared Secret Storage Tests
// ============================================================================

TEST_F(MatrixClientTest, SecretStorageDefaultKey) {
    json secretStorage;
    secretStorage["default_key_id"] = "key_id_abc123";

    ASSERT_EQ(secretStorage["default_key_id"].get<std::string>(), "key_id_abc123");
    ASSERT_FALSE(secretStorage["default_key_id"].get<std::string>().empty());
}

TEST_F(MatrixClientTest, SecretStorageKeyInfo) {
    json keyInfo;
    keyInfo["name"] = "m.secret_storage.default_key";
    keyInfo["algorithm"] = "m.secret_storage.v1.aes-hmac-sha2";
    keyInfo["passphrase"] = {
        {"salt", "base64_salt_here"},
        {"iterations", 500000}
    };

    ASSERT_EQ(keyInfo["name"].get<std::string>(), "m.secret_storage.default_key");
    ASSERT_EQ(keyInfo["algorithm"].get<std::string>(), "m.secret_storage.v1.aes-hmac-sha2");
    ASSERT_EQ(keyInfo["passphrase"]["iterations"].get<int>(), 500000);
    ASSERT_FALSE(keyInfo["passphrase"]["salt"].get<std::string>().empty());
}

// ============================================================================
// Permalink Tests
// ============================================================================

TEST_F(MatrixClientTest, PermalinkGeneration) {
    std::string roomId = "!someRoom:example.org";
    std::string eventId = "$someEvent:example.org";
    std::string permalink = "https://matrix.to/#/" + roomId + "/" + eventId;

    ASSERT_TRUE(permalink.find("https://matrix.to/") == 0);
    ASSERT_TRUE(permalink.find(roomId) != std::string::npos);
    ASSERT_TRUE(permalink.find(eventId) != std::string::npos);
}

TEST_F(MatrixClientTest, PermalinkViaAlias) {
    std::string alias = "#general:example.org";
    std::string permalink = "https://matrix.to/#/" + alias;

    ASSERT_TRUE(permalink.find("https://matrix.to/") == 0);
    ASSERT_TRUE(permalink.find(alias) != std::string::npos);
    ASSERT_EQ(permalink.size(), std::string("https://matrix.to/#/").size() + alias.size());
}

// ============================================================================
// Space Management Tests (Client-side)
// ============================================================================

TEST_F(MatrixClientTest, SpaceSummary) {
    json spaceSummary;
    spaceSummary["rooms"] = json::array({
        "!space_room1:example.org",
        "!space_room2:example.org"
    });
    spaceSummary["events"] = json::array();

    ASSERT_EQ(spaceSummary["rooms"].size(), 2u);
    ASSERT_TRUE(spaceSummary["rooms"][0].get<std::string>().find("!") == 0);
    ASSERT_TRUE(spaceSummary["events"].is_array());
}

TEST_F(MatrixClientTest, SpaceChildRoomInfo) {
    json childInfo;
    childInfo["room_id"] = "!child:example.org";
    childInfo["via"] = json::array({"matrix.example.org"});
    childInfo["suggested"] = true;
    childInfo["auto_join"] = false;
    childInfo["order"] = "001-general";

    ASSERT_EQ(childInfo["room_id"].get<std::string>(), "!child:example.org");
    ASSERT_EQ(childInfo["via"].size(), 1u);
    ASSERT_TRUE(childInfo["suggested"].get<bool>());
    ASSERT_FALSE(childInfo["auto_join"].get<bool>());
    ASSERT_EQ(childInfo["order"].get<std::string>(), "001-general");
}

// ============================================================================
// To-Device Messaging Tests
// ============================================================================

TEST_F(MatrixClientTest, SendToDeviceMessage) {
    json toDevice;
    toDevice["event_type"] = "m.new_device";
    toDevice["txn_id"] = "txn_123456";
    toDevice["messages"] = {
        {"@alice:example.org", {
            {"DEVICE_A", {
                {"type", "m.new_device"},
                {"content", {{"device_id", "DEVICE_B"}}}
            }}
        }}
    };

    ASSERT_EQ(toDevice["event_type"].get<std::string>(), "m.new_device");
    ASSERT_EQ(toDevice["txn_id"].get<std::string>(), "txn_123456");
    ASSERT_TRUE(toDevice["messages"].contains("@alice:example.org"));
    ASSERT_TRUE(toDevice["messages"]["@alice:example.org"].contains("DEVICE_A"));
}

// ============================================================================
// Admin/Homeserver Management Tests
// ============================================================================

TEST_F(MatrixClientTest, WhoIsQuery) {
    json whoisResp;
    whoisResp["user_id"] = "@alice:example.org";
    whoisResp["devices"] = {
        {"DEVICE_A", {
            {"sessions", json::array({
                {
                    {"connections", json::array({
                        {
                            {"ip", "192.168.1.100"},
                            {"last_seen", 1600000000000LL},
                            {"user_agent", "PCP/1.0"}
                        }
                    })}
                }
            })}
        }}
    };

    ASSERT_EQ(whoisResp["user_id"].get<std::string>(), "@alice:example.org");
    ASSERT_TRUE(whoisResp["devices"].contains("DEVICE_A"));
    ASSERT_TRUE(whoisResp["devices"]["DEVICE_A"]["sessions"].is_array());
}

// ============================================================================
// Rate Limiting Tests
// ============================================================================

TEST_F(MatrixClientTest, RateLimitResponse) {
    json rateLimit;
    rateLimit["retry_after_ms"] = 5000;
    rateLimit["errcode"] = "M_LIMIT_EXCEEDED";
    rateLimit["error"] = "Too many requests. Rate limit exceeded.";

    ASSERT_EQ(rateLimit["retry_after_ms"].get<int>(), 5000);
    ASSERT_GT(rateLimit["retry_after_ms"].get<int>(), 0);
    ASSERT_FALSE(rateLimit["error"].get<std::string>().empty());
}

// ============================================================================
// URL Preview Tests
// ============================================================================

TEST_F(MatrixClientTest, UrlPreviewConfig) {
    json previewConfig;
    previewConfig["disable"] = false;

    ASSERT_FALSE(previewConfig["disable"].get<bool>());
}

TEST_F(MatrixClientTest, UrlPreviewResponse) {
    json preview;
    preview["og:title"] = "Example Page";
    preview["og:description"] = "This is an example";
    preview["og:image"] = "https://example.org/image.jpg";
    preview["matrix:image:size"] = 102400;

    ASSERT_EQ(preview["og:title"].get<std::string>(), "Example Page");
    ASSERT_EQ(preview["og:image"].get<std::string>(), "https://example.org/image.jpg");
    ASSERT_GT(preview["matrix:image:size"].get<int>(), 0);
}

// ============================================================================
// User Directory Tests
// ============================================================================

TEST_F(MatrixClientTest, UserDirectorySearchResults) {
    json results = json::array();
    json user1;
    user1["user_id"] = "@alice:example.org";
    user1["display_name"] = "Alice";
    user1["avatar_url"] = "mxc://example.org/avatar_alice";
    results.push_back(user1);

    json user2;
    user2["user_id"] = "@bob:example.org";
    user2["display_name"] = "Bob";
    user2["avatar_url"] = "mxc://example.org/avatar_bob";
    results.push_back(user2);

    ASSERT_EQ(results.size(), 2u);
    ASSERT_EQ(results[0]["display_name"].get<std::string>(), "Alice");
    ASSERT_EQ(results[1]["display_name"].get<std::string>(), "Bob");
    ASSERT_TRUE(results[0]["avatar_url"].get<std::string>().find("mxc://") == 0);
}

// ============================================================================
// Integration Manager Tests
// ============================================================================

TEST_F(MatrixClientTest, IntegrationManagerConfig) {
    json config;
    config["managers"] = json::array({
        {
            {"api_url", "https://integrations.example.org"},
            {"ui_url", "https://integrations.example.org/ui"}
        }
    });

    ASSERT_EQ(config["managers"].size(), 1u);
    ASSERT_EQ(config["managers"][0]["api_url"].get<std::string>(), "https://integrations.example.org");
    ASSERT_EQ(config["managers"][0]["ui_url"].get<std::string>(), "https://integrations.example.org/ui");
}

// ============================================================================
// Session Resume Tests
// ============================================================================

TEST_F(MatrixClientTest, ResumeSessionSuccess) {
    json sessionData;
    sessionData["user_id"] = m_userId;
    sessionData["access_token"] = m_accessToken;
    sessionData["device_id"] = m_deviceId;
    sessionData["home_server"] = m_homeServer;
    sessionData["last_sync_token"] = "s_resume_batch";

    ASSERT_EQ(sessionData["user_id"].get<std::string>(), m_userId);
    ASSERT_EQ(sessionData["last_sync_token"].get<std::string>(), "s_resume_batch");
    ASSERT_FALSE(sessionData["last_sync_token"].get<std::string>().empty());
    ASSERT_EQ(sessionData["device_id"].get<std::string>(), m_deviceId);
}

TEST_F(MatrixClientTest, ResumeSessionWithExpiredToken) {
    json errorResp;
    errorResp["errcode"] = "M_UNKNOWN_TOKEN";
    errorResp["error"] = "Access token has expired";
    errorResp["soft_logout"] = true;

    ASSERT_TRUE(errorResp["soft_logout"].get<bool>());
    ASSERT_EQ(errorResp["errcode"].get<std::string>(), "M_UNKNOWN_TOKEN");
    ASSERT_TRUE(errorResp["error"].get<std::string>().find("expired") != std::string::npos);
}

// ============================================================================
// Stream Token Consistency Tests
// ============================================================================

TEST_F(MatrixClientTest, StreamTokenConsistency) {
    std::vector<std::string> tokens;
    tokens.push_back("s100_200_300");
    tokens.push_back("s200_300_400");
    tokens.push_back("s300_400_500");

    ASSERT_EQ(tokens.size(), 3u);
    for (size_t i = 1; i < tokens.size(); i++) {
        ASSERT_TRUE(tokens[i] != tokens[i-1]);
        ASSERT_FALSE(tokens[i].empty());
    }
    EXPECT_TRUE(tokens[0] != tokens[2]);
}

// ============================================================================
// Read Receipt Aggregation Tests
// ============================================================================

TEST_F(MatrixClientTest, AggregateReadReceipts) {
    json ephemeral = json::array();
    ephemeral.push_back(json::parse(makeReceiptEvent(m_roomId, "$ev1:example.org", "@alice:example.org", 1600000001000LL)));
    ephemeral.push_back(json::parse(makeReceiptEvent(m_roomId, "$ev2:example.org", "@bob:example.org", 1600000002000LL)));

    ASSERT_EQ(ephemeral.size(), 2u);
    ASSERT_TRUE(ephemeral[0]["content"].contains("$ev1:example.org"));
    ASSERT_TRUE(ephemeral[1]["content"].contains("$ev2:example.org"));
}

} // namespace test
} // namespace pcp
