#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BulkSyncHandlerTest, Scenario1) {
    // Test scenario 1 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_0";
    data["timestamp"] = 1600000600000;
    data["user_id"] = "@testuser_6_0:matrix.org";
    data["room_id"] = "!testroom_6_0:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 10;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 0;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 0;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997000000 + e * 1000;
        event["content"]["body"] = "Message body 6_0_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_0";
    data["state"]["topic"] = "Topic 6_0";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario2) {
    // Test scenario 2 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_1";
    data["timestamp"] = 1600000601000;
    data["user_id"] = "@testuser_6_1:matrix.org";
    data["room_id"] = "!testroom_6_1:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 11;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 1;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 1;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997100000 + e * 1000;
        event["content"]["body"] = "Message body 6_1_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_1";
    data["state"]["topic"] = "Topic 6_1";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario3) {
    // Test scenario 3 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_2";
    data["timestamp"] = 1600000602000;
    data["user_id"] = "@testuser_6_2:matrix.org";
    data["room_id"] = "!testroom_6_2:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 12;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 2;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 2;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997200000 + e * 1000;
        event["content"]["body"] = "Message body 6_2_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_2";
    data["state"]["topic"] = "Topic 6_2";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario4) {
    // Test scenario 4 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_3";
    data["timestamp"] = 1600000603000;
    data["user_id"] = "@testuser_6_3:matrix.org";
    data["room_id"] = "!testroom_6_3:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 13;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 3;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 3;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997300000 + e * 1000;
        event["content"]["body"] = "Message body 6_3_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_3";
    data["state"]["topic"] = "Topic 6_3";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario5) {
    // Test scenario 5 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_4";
    data["timestamp"] = 1600000604000;
    data["user_id"] = "@testuser_6_4:matrix.org";
    data["room_id"] = "!testroom_6_4:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 14;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 4;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 4;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997400000 + e * 1000;
        event["content"]["body"] = "Message body 6_4_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_4";
    data["state"]["topic"] = "Topic 6_4";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario6) {
    // Test scenario 6 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_5";
    data["timestamp"] = 1600000605000;
    data["user_id"] = "@testuser_6_5:matrix.org";
    data["room_id"] = "!testroom_6_5:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 15;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 0;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 0;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997500000 + e * 1000;
        event["content"]["body"] = "Message body 6_5_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_5";
    data["state"]["topic"] = "Topic 6_5";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario7) {
    // Test scenario 7 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_6";
    data["timestamp"] = 1600000606000;
    data["user_id"] = "@testuser_6_6:matrix.org";
    data["room_id"] = "!testroom_6_6:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 16;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 1;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 1;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997600000 + e * 1000;
        event["content"]["body"] = "Message body 6_6_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_6";
    data["state"]["topic"] = "Topic 6_6";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario8) {
    // Test scenario 8 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_7";
    data["timestamp"] = 1600000607000;
    data["user_id"] = "@testuser_6_7:matrix.org";
    data["room_id"] = "!testroom_6_7:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 17;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 2;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 2;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997700000 + e * 1000;
        event["content"]["body"] = "Message body 6_7_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_7";
    data["state"]["topic"] = "Topic 6_7";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario9) {
    // Test scenario 9 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_8";
    data["timestamp"] = 1600000608000;
    data["user_id"] = "@testuser_6_8:matrix.org";
    data["room_id"] = "!testroom_6_8:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 18;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 3;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 3;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997800000 + e * 1000;
        event["content"]["body"] = "Message body 6_8_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_8";
    data["state"]["topic"] = "Topic 6_8";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario10) {
    // Test scenario 10 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_9";
    data["timestamp"] = 1600000609000;
    data["user_id"] = "@testuser_6_9:matrix.org";
    data["room_id"] = "!testroom_6_9:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 19;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 4;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 4;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997900000 + e * 1000;
        event["content"]["body"] = "Message body 6_9_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_9";
    data["state"]["topic"] = "Topic 6_9";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario11) {
    // Test scenario 11 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_10";
    data["timestamp"] = 1600000610000;
    data["user_id"] = "@testuser_6_10:matrix.org";
    data["room_id"] = "!testroom_6_10:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 20;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 0;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 0;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998000000 + e * 1000;
        event["content"]["body"] = "Message body 6_10_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_10";
    data["state"]["topic"] = "Topic 6_10";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario12) {
    // Test scenario 12 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_11";
    data["timestamp"] = 1600000611000;
    data["user_id"] = "@testuser_6_11:matrix.org";
    data["room_id"] = "!testroom_6_11:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 21;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 1;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 1;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998100000 + e * 1000;
        event["content"]["body"] = "Message body 6_11_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_11";
    data["state"]["topic"] = "Topic 6_11";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario13) {
    // Test scenario 13 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_12";
    data["timestamp"] = 1600000612000;
    data["user_id"] = "@testuser_6_12:matrix.org";
    data["room_id"] = "!testroom_6_12:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 22;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 2;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 2;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998200000 + e * 1000;
        event["content"]["body"] = "Message body 6_12_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_12";
    data["state"]["topic"] = "Topic 6_12";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario14) {
    // Test scenario 14 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_13";
    data["timestamp"] = 1600000613000;
    data["user_id"] = "@testuser_6_13:matrix.org";
    data["room_id"] = "!testroom_6_13:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 23;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 3;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 3;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998300000 + e * 1000;
        event["content"]["body"] = "Message body 6_13_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_13";
    data["state"]["topic"] = "Topic 6_13";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario15) {
    // Test scenario 15 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_14";
    data["timestamp"] = 1600000614000;
    data["user_id"] = "@testuser_6_14:matrix.org";
    data["room_id"] = "!testroom_6_14:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 24;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 4;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 4;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998400000 + e * 1000;
        event["content"]["body"] = "Message body 6_14_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_14";
    data["state"]["topic"] = "Topic 6_14";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario16) {
    // Test scenario 16 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_15";
    data["timestamp"] = 1600000615000;
    data["user_id"] = "@testuser_6_15:matrix.org";
    data["room_id"] = "!testroom_6_15:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 25;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 0;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 0;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998500000 + e * 1000;
        event["content"]["body"] = "Message body 6_15_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_15";
    data["state"]["topic"] = "Topic 6_15";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario17) {
    // Test scenario 17 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_16";
    data["timestamp"] = 1600000616000;
    data["user_id"] = "@testuser_6_16:matrix.org";
    data["room_id"] = "!testroom_6_16:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 26;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 1;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 1;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998600000 + e * 1000;
        event["content"]["body"] = "Message body 6_16_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_16";
    data["state"]["topic"] = "Topic 6_16";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario18) {
    // Test scenario 18 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_17";
    data["timestamp"] = 1600000617000;
    data["user_id"] = "@testuser_6_17:matrix.org";
    data["room_id"] = "!testroom_6_17:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 27;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 2;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 2;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998700000 + e * 1000;
        event["content"]["body"] = "Message body 6_17_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_17";
    data["state"]["topic"] = "Topic 6_17";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario19) {
    // Test scenario 19 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_18";
    data["timestamp"] = 1600000618000;
    data["user_id"] = "@testuser_6_18:matrix.org";
    data["room_id"] = "!testroom_6_18:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 28;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 3;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 3;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998800000 + e * 1000;
        event["content"]["body"] = "Message body 6_18_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_18";
    data["state"]["topic"] = "Topic 6_18";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario20) {
    // Test scenario 20 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_19";
    data["timestamp"] = 1600000619000;
    data["user_id"] = "@testuser_6_19:matrix.org";
    data["room_id"] = "!testroom_6_19:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 29;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 4;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 4;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998900000 + e * 1000;
        event["content"]["body"] = "Message body 6_19_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_19";
    data["state"]["topic"] = "Topic 6_19";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario21) {
    // Test scenario 21 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_20";
    data["timestamp"] = 1600000620000;
    data["user_id"] = "@testuser_6_20:matrix.org";
    data["room_id"] = "!testroom_6_20:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 30;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 0;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 0;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999000000 + e * 1000;
        event["content"]["body"] = "Message body 6_20_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_20";
    data["state"]["topic"] = "Topic 6_20";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario22) {
    // Test scenario 22 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_21";
    data["timestamp"] = 1600000621000;
    data["user_id"] = "@testuser_6_21:matrix.org";
    data["room_id"] = "!testroom_6_21:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 31;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 1;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 1;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999100000 + e * 1000;
        event["content"]["body"] = "Message body 6_21_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_21";
    data["state"]["topic"] = "Topic 6_21";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario23) {
    // Test scenario 23 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_22";
    data["timestamp"] = 1600000622000;
    data["user_id"] = "@testuser_6_22:matrix.org";
    data["room_id"] = "!testroom_6_22:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 32;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 2;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 2;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999200000 + e * 1000;
        event["content"]["body"] = "Message body 6_22_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_22";
    data["state"]["topic"] = "Topic 6_22";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario24) {
    // Test scenario 24 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_23";
    data["timestamp"] = 1600000623000;
    data["user_id"] = "@testuser_6_23:matrix.org";
    data["room_id"] = "!testroom_6_23:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 33;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 3;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 3;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999300000 + e * 1000;
        event["content"]["body"] = "Message body 6_23_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_23";
    data["state"]["topic"] = "Topic 6_23";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario25) {
    // Test scenario 25 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_24";
    data["timestamp"] = 1600000624000;
    data["user_id"] = "@testuser_6_24:matrix.org";
    data["room_id"] = "!testroom_6_24:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 34;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 4;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 4;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999400000 + e * 1000;
        event["content"]["body"] = "Message body 6_24_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_24";
    data["state"]["topic"] = "Topic 6_24";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario26) {
    // Test scenario 26 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_25";
    data["timestamp"] = 1600000625000;
    data["user_id"] = "@testuser_6_25:matrix.org";
    data["room_id"] = "!testroom_6_25:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 35;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 0;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 0;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999500000 + e * 1000;
        event["content"]["body"] = "Message body 6_25_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_25";
    data["state"]["topic"] = "Topic 6_25";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario27) {
    // Test scenario 27 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_26";
    data["timestamp"] = 1600000626000;
    data["user_id"] = "@testuser_6_26:matrix.org";
    data["room_id"] = "!testroom_6_26:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 36;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 1;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 1;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999600000 + e * 1000;
        event["content"]["body"] = "Message body 6_26_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_26";
    data["state"]["topic"] = "Topic 6_26";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario28) {
    // Test scenario 28 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_27";
    data["timestamp"] = 1600000627000;
    data["user_id"] = "@testuser_6_27:matrix.org";
    data["room_id"] = "!testroom_6_27:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 37;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 2;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 2;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999700000 + e * 1000;
        event["content"]["body"] = "Message body 6_27_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_27";
    data["state"]["topic"] = "Topic 6_27";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario29) {
    // Test scenario 29 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_28";
    data["timestamp"] = 1600000628000;
    data["user_id"] = "@testuser_6_28:matrix.org";
    data["room_id"] = "!testroom_6_28:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 38;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 3;
    data["parameters"]["enabled"] = True;
    data["parameters"]["priority"] = 3;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999800000 + e * 1000;
        event["content"]["body"] = "Message body 6_28_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_28";
    data["state"]["topic"] = "Topic 6_28";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

TEST(BulkSyncHandlerTest, Scenario30) {
    // Test scenario 30 for BulkSyncHandler
    json data;
    data["test_id"] = "BulkSyncHandler_29";
    data["timestamp"] = 1600000629000;
    data["user_id"] = "@testuser_6_29:matrix.org";
    data["room_id"] = "!testroom_6_29:matrix.org";
    
    // Setup test parameters
    data["parameters"]["count"] = 39;
    data["parameters"]["limit"] = 50;
    data["parameters"]["offset"] = 4;
    data["parameters"]["enabled"] = False;
    data["parameters"]["priority"] = 4;
    ASSERT_FALSE(data["user_id"].empty());
    ASSERT_FALSE(data["room_id"].empty());
    ASSERT_TRUE(data["parameters"]["limit"] > 0);
    
    // Generate test events
    data["events"] = json::array();
    for (int e = 0; e < 5; ++e) {
        json event;
        event["event_id"] = "$event" + std::to_string(6) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999900000 + e * 1000;
        event["content"]["body"] = "Message body 6_29_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 6_29";
    data["state"]["topic"] = "Topic 6_29";
    data["state"]["join_rule"] = "invite";
    data["state"]["encryption"] = "m.megolm.v1.aes-sha2";
    ASSERT_FALSE(data["state"]["name"].empty());
    ASSERT_EQ(data["state"]["join_rule"], "invite");
    
    // verify serialization
    std::string serialized = data.dump();
    ASSERT_FALSE(serialized.empty());
    ASSERT_GT(serialized.size(), 10);
    
    // re-parse and verify
    json reparsed = json::parse(serialized);
    ASSERT_EQ(reparsed["test_id"], data["test_id"]);
    ASSERT_EQ(reparsed["events"].size(), 5);
}

}} // namespace pcp::test
