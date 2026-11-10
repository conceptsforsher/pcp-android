#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(MentionsAndPillsTest, Scenario1) {
    // Test scenario 1 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_0";
    data["timestamp"] = 1600001400000;
    data["user_id"] = "@testuser_14_0:matrix.org";
    data["room_id"] = "!testroom_14_0:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997000000 + e * 1000;
        event["content"]["body"] = "Message body 14_0_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_0";
    data["state"]["topic"] = "Topic 14_0";
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

TEST(MentionsAndPillsTest, Scenario2) {
    // Test scenario 2 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_1";
    data["timestamp"] = 1600001401000;
    data["user_id"] = "@testuser_14_1:matrix.org";
    data["room_id"] = "!testroom_14_1:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997100000 + e * 1000;
        event["content"]["body"] = "Message body 14_1_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_1";
    data["state"]["topic"] = "Topic 14_1";
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

TEST(MentionsAndPillsTest, Scenario3) {
    // Test scenario 3 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_2";
    data["timestamp"] = 1600001402000;
    data["user_id"] = "@testuser_14_2:matrix.org";
    data["room_id"] = "!testroom_14_2:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997200000 + e * 1000;
        event["content"]["body"] = "Message body 14_2_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_2";
    data["state"]["topic"] = "Topic 14_2";
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

TEST(MentionsAndPillsTest, Scenario4) {
    // Test scenario 4 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_3";
    data["timestamp"] = 1600001403000;
    data["user_id"] = "@testuser_14_3:matrix.org";
    data["room_id"] = "!testroom_14_3:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997300000 + e * 1000;
        event["content"]["body"] = "Message body 14_3_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_3";
    data["state"]["topic"] = "Topic 14_3";
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

TEST(MentionsAndPillsTest, Scenario5) {
    // Test scenario 5 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_4";
    data["timestamp"] = 1600001404000;
    data["user_id"] = "@testuser_14_4:matrix.org";
    data["room_id"] = "!testroom_14_4:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997400000 + e * 1000;
        event["content"]["body"] = "Message body 14_4_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_4";
    data["state"]["topic"] = "Topic 14_4";
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

TEST(MentionsAndPillsTest, Scenario6) {
    // Test scenario 6 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_5";
    data["timestamp"] = 1600001405000;
    data["user_id"] = "@testuser_14_5:matrix.org";
    data["room_id"] = "!testroom_14_5:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997500000 + e * 1000;
        event["content"]["body"] = "Message body 14_5_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_5";
    data["state"]["topic"] = "Topic 14_5";
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

TEST(MentionsAndPillsTest, Scenario7) {
    // Test scenario 7 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_6";
    data["timestamp"] = 1600001406000;
    data["user_id"] = "@testuser_14_6:matrix.org";
    data["room_id"] = "!testroom_14_6:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997600000 + e * 1000;
        event["content"]["body"] = "Message body 14_6_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_6";
    data["state"]["topic"] = "Topic 14_6";
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

TEST(MentionsAndPillsTest, Scenario8) {
    // Test scenario 8 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_7";
    data["timestamp"] = 1600001407000;
    data["user_id"] = "@testuser_14_7:matrix.org";
    data["room_id"] = "!testroom_14_7:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997700000 + e * 1000;
        event["content"]["body"] = "Message body 14_7_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_7";
    data["state"]["topic"] = "Topic 14_7";
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

TEST(MentionsAndPillsTest, Scenario9) {
    // Test scenario 9 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_8";
    data["timestamp"] = 1600001408000;
    data["user_id"] = "@testuser_14_8:matrix.org";
    data["room_id"] = "!testroom_14_8:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997800000 + e * 1000;
        event["content"]["body"] = "Message body 14_8_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_8";
    data["state"]["topic"] = "Topic 14_8";
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

TEST(MentionsAndPillsTest, Scenario10) {
    // Test scenario 10 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_9";
    data["timestamp"] = 1600001409000;
    data["user_id"] = "@testuser_14_9:matrix.org";
    data["room_id"] = "!testroom_14_9:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997900000 + e * 1000;
        event["content"]["body"] = "Message body 14_9_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_9";
    data["state"]["topic"] = "Topic 14_9";
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

TEST(MentionsAndPillsTest, Scenario11) {
    // Test scenario 11 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_10";
    data["timestamp"] = 1600001410000;
    data["user_id"] = "@testuser_14_10:matrix.org";
    data["room_id"] = "!testroom_14_10:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998000000 + e * 1000;
        event["content"]["body"] = "Message body 14_10_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_10";
    data["state"]["topic"] = "Topic 14_10";
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

TEST(MentionsAndPillsTest, Scenario12) {
    // Test scenario 12 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_11";
    data["timestamp"] = 1600001411000;
    data["user_id"] = "@testuser_14_11:matrix.org";
    data["room_id"] = "!testroom_14_11:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998100000 + e * 1000;
        event["content"]["body"] = "Message body 14_11_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_11";
    data["state"]["topic"] = "Topic 14_11";
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

TEST(MentionsAndPillsTest, Scenario13) {
    // Test scenario 13 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_12";
    data["timestamp"] = 1600001412000;
    data["user_id"] = "@testuser_14_12:matrix.org";
    data["room_id"] = "!testroom_14_12:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998200000 + e * 1000;
        event["content"]["body"] = "Message body 14_12_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_12";
    data["state"]["topic"] = "Topic 14_12";
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

TEST(MentionsAndPillsTest, Scenario14) {
    // Test scenario 14 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_13";
    data["timestamp"] = 1600001413000;
    data["user_id"] = "@testuser_14_13:matrix.org";
    data["room_id"] = "!testroom_14_13:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998300000 + e * 1000;
        event["content"]["body"] = "Message body 14_13_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_13";
    data["state"]["topic"] = "Topic 14_13";
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

TEST(MentionsAndPillsTest, Scenario15) {
    // Test scenario 15 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_14";
    data["timestamp"] = 1600001414000;
    data["user_id"] = "@testuser_14_14:matrix.org";
    data["room_id"] = "!testroom_14_14:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998400000 + e * 1000;
        event["content"]["body"] = "Message body 14_14_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_14";
    data["state"]["topic"] = "Topic 14_14";
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

TEST(MentionsAndPillsTest, Scenario16) {
    // Test scenario 16 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_15";
    data["timestamp"] = 1600001415000;
    data["user_id"] = "@testuser_14_15:matrix.org";
    data["room_id"] = "!testroom_14_15:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998500000 + e * 1000;
        event["content"]["body"] = "Message body 14_15_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_15";
    data["state"]["topic"] = "Topic 14_15";
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

TEST(MentionsAndPillsTest, Scenario17) {
    // Test scenario 17 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_16";
    data["timestamp"] = 1600001416000;
    data["user_id"] = "@testuser_14_16:matrix.org";
    data["room_id"] = "!testroom_14_16:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998600000 + e * 1000;
        event["content"]["body"] = "Message body 14_16_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_16";
    data["state"]["topic"] = "Topic 14_16";
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

TEST(MentionsAndPillsTest, Scenario18) {
    // Test scenario 18 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_17";
    data["timestamp"] = 1600001417000;
    data["user_id"] = "@testuser_14_17:matrix.org";
    data["room_id"] = "!testroom_14_17:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998700000 + e * 1000;
        event["content"]["body"] = "Message body 14_17_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_17";
    data["state"]["topic"] = "Topic 14_17";
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

TEST(MentionsAndPillsTest, Scenario19) {
    // Test scenario 19 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_18";
    data["timestamp"] = 1600001418000;
    data["user_id"] = "@testuser_14_18:matrix.org";
    data["room_id"] = "!testroom_14_18:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998800000 + e * 1000;
        event["content"]["body"] = "Message body 14_18_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_18";
    data["state"]["topic"] = "Topic 14_18";
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

TEST(MentionsAndPillsTest, Scenario20) {
    // Test scenario 20 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_19";
    data["timestamp"] = 1600001419000;
    data["user_id"] = "@testuser_14_19:matrix.org";
    data["room_id"] = "!testroom_14_19:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998900000 + e * 1000;
        event["content"]["body"] = "Message body 14_19_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_19";
    data["state"]["topic"] = "Topic 14_19";
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

TEST(MentionsAndPillsTest, Scenario21) {
    // Test scenario 21 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_20";
    data["timestamp"] = 1600001420000;
    data["user_id"] = "@testuser_14_20:matrix.org";
    data["room_id"] = "!testroom_14_20:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999000000 + e * 1000;
        event["content"]["body"] = "Message body 14_20_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_20";
    data["state"]["topic"] = "Topic 14_20";
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

TEST(MentionsAndPillsTest, Scenario22) {
    // Test scenario 22 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_21";
    data["timestamp"] = 1600001421000;
    data["user_id"] = "@testuser_14_21:matrix.org";
    data["room_id"] = "!testroom_14_21:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999100000 + e * 1000;
        event["content"]["body"] = "Message body 14_21_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_21";
    data["state"]["topic"] = "Topic 14_21";
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

TEST(MentionsAndPillsTest, Scenario23) {
    // Test scenario 23 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_22";
    data["timestamp"] = 1600001422000;
    data["user_id"] = "@testuser_14_22:matrix.org";
    data["room_id"] = "!testroom_14_22:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999200000 + e * 1000;
        event["content"]["body"] = "Message body 14_22_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_22";
    data["state"]["topic"] = "Topic 14_22";
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

TEST(MentionsAndPillsTest, Scenario24) {
    // Test scenario 24 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_23";
    data["timestamp"] = 1600001423000;
    data["user_id"] = "@testuser_14_23:matrix.org";
    data["room_id"] = "!testroom_14_23:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999300000 + e * 1000;
        event["content"]["body"] = "Message body 14_23_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_23";
    data["state"]["topic"] = "Topic 14_23";
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

TEST(MentionsAndPillsTest, Scenario25) {
    // Test scenario 25 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_24";
    data["timestamp"] = 1600001424000;
    data["user_id"] = "@testuser_14_24:matrix.org";
    data["room_id"] = "!testroom_14_24:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999400000 + e * 1000;
        event["content"]["body"] = "Message body 14_24_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_24";
    data["state"]["topic"] = "Topic 14_24";
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

TEST(MentionsAndPillsTest, Scenario26) {
    // Test scenario 26 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_25";
    data["timestamp"] = 1600001425000;
    data["user_id"] = "@testuser_14_25:matrix.org";
    data["room_id"] = "!testroom_14_25:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999500000 + e * 1000;
        event["content"]["body"] = "Message body 14_25_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_25";
    data["state"]["topic"] = "Topic 14_25";
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

TEST(MentionsAndPillsTest, Scenario27) {
    // Test scenario 27 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_26";
    data["timestamp"] = 1600001426000;
    data["user_id"] = "@testuser_14_26:matrix.org";
    data["room_id"] = "!testroom_14_26:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999600000 + e * 1000;
        event["content"]["body"] = "Message body 14_26_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_26";
    data["state"]["topic"] = "Topic 14_26";
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

TEST(MentionsAndPillsTest, Scenario28) {
    // Test scenario 28 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_27";
    data["timestamp"] = 1600001427000;
    data["user_id"] = "@testuser_14_27:matrix.org";
    data["room_id"] = "!testroom_14_27:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999700000 + e * 1000;
        event["content"]["body"] = "Message body 14_27_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_27";
    data["state"]["topic"] = "Topic 14_27";
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

TEST(MentionsAndPillsTest, Scenario29) {
    // Test scenario 29 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_28";
    data["timestamp"] = 1600001428000;
    data["user_id"] = "@testuser_14_28:matrix.org";
    data["room_id"] = "!testroom_14_28:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999800000 + e * 1000;
        event["content"]["body"] = "Message body 14_28_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_28";
    data["state"]["topic"] = "Topic 14_28";
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

TEST(MentionsAndPillsTest, Scenario30) {
    // Test scenario 30 for MentionsAndPills
    json data;
    data["test_id"] = "MentionsAndPills_29";
    data["timestamp"] = 1600001429000;
    data["user_id"] = "@testuser_14_29:matrix.org";
    data["room_id"] = "!testroom_14_29:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(14) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999900000 + e * 1000;
        event["content"]["body"] = "Message body 14_29_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 14_29";
    data["state"]["topic"] = "Topic 14_29";
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
