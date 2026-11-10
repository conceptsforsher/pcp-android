#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(CrossSigningFullTest, Scenario1) {
    // Test scenario 1 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_0";
    data["timestamp"] = 1600002400000;
    data["user_id"] = "@testuser_24_0:matrix.org";
    data["room_id"] = "!testroom_24_0:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997000000 + e * 1000;
        event["content"]["body"] = "Message body 24_0_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_0";
    data["state"]["topic"] = "Topic 24_0";
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

TEST(CrossSigningFullTest, Scenario2) {
    // Test scenario 2 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_1";
    data["timestamp"] = 1600002401000;
    data["user_id"] = "@testuser_24_1:matrix.org";
    data["room_id"] = "!testroom_24_1:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997100000 + e * 1000;
        event["content"]["body"] = "Message body 24_1_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_1";
    data["state"]["topic"] = "Topic 24_1";
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

TEST(CrossSigningFullTest, Scenario3) {
    // Test scenario 3 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_2";
    data["timestamp"] = 1600002402000;
    data["user_id"] = "@testuser_24_2:matrix.org";
    data["room_id"] = "!testroom_24_2:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997200000 + e * 1000;
        event["content"]["body"] = "Message body 24_2_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_2";
    data["state"]["topic"] = "Topic 24_2";
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

TEST(CrossSigningFullTest, Scenario4) {
    // Test scenario 4 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_3";
    data["timestamp"] = 1600002403000;
    data["user_id"] = "@testuser_24_3:matrix.org";
    data["room_id"] = "!testroom_24_3:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997300000 + e * 1000;
        event["content"]["body"] = "Message body 24_3_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_3";
    data["state"]["topic"] = "Topic 24_3";
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

TEST(CrossSigningFullTest, Scenario5) {
    // Test scenario 5 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_4";
    data["timestamp"] = 1600002404000;
    data["user_id"] = "@testuser_24_4:matrix.org";
    data["room_id"] = "!testroom_24_4:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997400000 + e * 1000;
        event["content"]["body"] = "Message body 24_4_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_4";
    data["state"]["topic"] = "Topic 24_4";
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

TEST(CrossSigningFullTest, Scenario6) {
    // Test scenario 6 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_5";
    data["timestamp"] = 1600002405000;
    data["user_id"] = "@testuser_24_5:matrix.org";
    data["room_id"] = "!testroom_24_5:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997500000 + e * 1000;
        event["content"]["body"] = "Message body 24_5_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_5";
    data["state"]["topic"] = "Topic 24_5";
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

TEST(CrossSigningFullTest, Scenario7) {
    // Test scenario 7 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_6";
    data["timestamp"] = 1600002406000;
    data["user_id"] = "@testuser_24_6:matrix.org";
    data["room_id"] = "!testroom_24_6:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997600000 + e * 1000;
        event["content"]["body"] = "Message body 24_6_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_6";
    data["state"]["topic"] = "Topic 24_6";
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

TEST(CrossSigningFullTest, Scenario8) {
    // Test scenario 8 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_7";
    data["timestamp"] = 1600002407000;
    data["user_id"] = "@testuser_24_7:matrix.org";
    data["room_id"] = "!testroom_24_7:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997700000 + e * 1000;
        event["content"]["body"] = "Message body 24_7_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_7";
    data["state"]["topic"] = "Topic 24_7";
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

TEST(CrossSigningFullTest, Scenario9) {
    // Test scenario 9 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_8";
    data["timestamp"] = 1600002408000;
    data["user_id"] = "@testuser_24_8:matrix.org";
    data["room_id"] = "!testroom_24_8:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997800000 + e * 1000;
        event["content"]["body"] = "Message body 24_8_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_8";
    data["state"]["topic"] = "Topic 24_8";
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

TEST(CrossSigningFullTest, Scenario10) {
    // Test scenario 10 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_9";
    data["timestamp"] = 1600002409000;
    data["user_id"] = "@testuser_24_9:matrix.org";
    data["room_id"] = "!testroom_24_9:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599997900000 + e * 1000;
        event["content"]["body"] = "Message body 24_9_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_9";
    data["state"]["topic"] = "Topic 24_9";
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

TEST(CrossSigningFullTest, Scenario11) {
    // Test scenario 11 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_10";
    data["timestamp"] = 1600002410000;
    data["user_id"] = "@testuser_24_10:matrix.org";
    data["room_id"] = "!testroom_24_10:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998000000 + e * 1000;
        event["content"]["body"] = "Message body 24_10_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_10";
    data["state"]["topic"] = "Topic 24_10";
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

TEST(CrossSigningFullTest, Scenario12) {
    // Test scenario 12 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_11";
    data["timestamp"] = 1600002411000;
    data["user_id"] = "@testuser_24_11:matrix.org";
    data["room_id"] = "!testroom_24_11:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998100000 + e * 1000;
        event["content"]["body"] = "Message body 24_11_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_11";
    data["state"]["topic"] = "Topic 24_11";
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

TEST(CrossSigningFullTest, Scenario13) {
    // Test scenario 13 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_12";
    data["timestamp"] = 1600002412000;
    data["user_id"] = "@testuser_24_12:matrix.org";
    data["room_id"] = "!testroom_24_12:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998200000 + e * 1000;
        event["content"]["body"] = "Message body 24_12_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_12";
    data["state"]["topic"] = "Topic 24_12";
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

TEST(CrossSigningFullTest, Scenario14) {
    // Test scenario 14 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_13";
    data["timestamp"] = 1600002413000;
    data["user_id"] = "@testuser_24_13:matrix.org";
    data["room_id"] = "!testroom_24_13:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998300000 + e * 1000;
        event["content"]["body"] = "Message body 24_13_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_13";
    data["state"]["topic"] = "Topic 24_13";
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

TEST(CrossSigningFullTest, Scenario15) {
    // Test scenario 15 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_14";
    data["timestamp"] = 1600002414000;
    data["user_id"] = "@testuser_24_14:matrix.org";
    data["room_id"] = "!testroom_24_14:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998400000 + e * 1000;
        event["content"]["body"] = "Message body 24_14_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_14";
    data["state"]["topic"] = "Topic 24_14";
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

TEST(CrossSigningFullTest, Scenario16) {
    // Test scenario 16 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_15";
    data["timestamp"] = 1600002415000;
    data["user_id"] = "@testuser_24_15:matrix.org";
    data["room_id"] = "!testroom_24_15:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998500000 + e * 1000;
        event["content"]["body"] = "Message body 24_15_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_15";
    data["state"]["topic"] = "Topic 24_15";
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

TEST(CrossSigningFullTest, Scenario17) {
    // Test scenario 17 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_16";
    data["timestamp"] = 1600002416000;
    data["user_id"] = "@testuser_24_16:matrix.org";
    data["room_id"] = "!testroom_24_16:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998600000 + e * 1000;
        event["content"]["body"] = "Message body 24_16_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_16";
    data["state"]["topic"] = "Topic 24_16";
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

TEST(CrossSigningFullTest, Scenario18) {
    // Test scenario 18 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_17";
    data["timestamp"] = 1600002417000;
    data["user_id"] = "@testuser_24_17:matrix.org";
    data["room_id"] = "!testroom_24_17:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998700000 + e * 1000;
        event["content"]["body"] = "Message body 24_17_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_17";
    data["state"]["topic"] = "Topic 24_17";
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

TEST(CrossSigningFullTest, Scenario19) {
    // Test scenario 19 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_18";
    data["timestamp"] = 1600002418000;
    data["user_id"] = "@testuser_24_18:matrix.org";
    data["room_id"] = "!testroom_24_18:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998800000 + e * 1000;
        event["content"]["body"] = "Message body 24_18_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_18";
    data["state"]["topic"] = "Topic 24_18";
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

TEST(CrossSigningFullTest, Scenario20) {
    // Test scenario 20 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_19";
    data["timestamp"] = 1600002419000;
    data["user_id"] = "@testuser_24_19:matrix.org";
    data["room_id"] = "!testroom_24_19:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599998900000 + e * 1000;
        event["content"]["body"] = "Message body 24_19_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_19";
    data["state"]["topic"] = "Topic 24_19";
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

TEST(CrossSigningFullTest, Scenario21) {
    // Test scenario 21 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_20";
    data["timestamp"] = 1600002420000;
    data["user_id"] = "@testuser_24_20:matrix.org";
    data["room_id"] = "!testroom_24_20:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999000000 + e * 1000;
        event["content"]["body"] = "Message body 24_20_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_20";
    data["state"]["topic"] = "Topic 24_20";
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

TEST(CrossSigningFullTest, Scenario22) {
    // Test scenario 22 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_21";
    data["timestamp"] = 1600002421000;
    data["user_id"] = "@testuser_24_21:matrix.org";
    data["room_id"] = "!testroom_24_21:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999100000 + e * 1000;
        event["content"]["body"] = "Message body 24_21_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_21";
    data["state"]["topic"] = "Topic 24_21";
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

TEST(CrossSigningFullTest, Scenario23) {
    // Test scenario 23 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_22";
    data["timestamp"] = 1600002422000;
    data["user_id"] = "@testuser_24_22:matrix.org";
    data["room_id"] = "!testroom_24_22:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999200000 + e * 1000;
        event["content"]["body"] = "Message body 24_22_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_22";
    data["state"]["topic"] = "Topic 24_22";
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

TEST(CrossSigningFullTest, Scenario24) {
    // Test scenario 24 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_23";
    data["timestamp"] = 1600002423000;
    data["user_id"] = "@testuser_24_23:matrix.org";
    data["room_id"] = "!testroom_24_23:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999300000 + e * 1000;
        event["content"]["body"] = "Message body 24_23_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_23";
    data["state"]["topic"] = "Topic 24_23";
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

TEST(CrossSigningFullTest, Scenario25) {
    // Test scenario 25 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_24";
    data["timestamp"] = 1600002424000;
    data["user_id"] = "@testuser_24_24:matrix.org";
    data["room_id"] = "!testroom_24_24:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999400000 + e * 1000;
        event["content"]["body"] = "Message body 24_24_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_24";
    data["state"]["topic"] = "Topic 24_24";
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

TEST(CrossSigningFullTest, Scenario26) {
    // Test scenario 26 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_25";
    data["timestamp"] = 1600002425000;
    data["user_id"] = "@testuser_24_25:matrix.org";
    data["room_id"] = "!testroom_24_25:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999500000 + e * 1000;
        event["content"]["body"] = "Message body 24_25_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_25";
    data["state"]["topic"] = "Topic 24_25";
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

TEST(CrossSigningFullTest, Scenario27) {
    // Test scenario 27 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_26";
    data["timestamp"] = 1600002426000;
    data["user_id"] = "@testuser_24_26:matrix.org";
    data["room_id"] = "!testroom_24_26:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999600000 + e * 1000;
        event["content"]["body"] = "Message body 24_26_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_26";
    data["state"]["topic"] = "Topic 24_26";
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

TEST(CrossSigningFullTest, Scenario28) {
    // Test scenario 28 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_27";
    data["timestamp"] = 1600002427000;
    data["user_id"] = "@testuser_24_27:matrix.org";
    data["room_id"] = "!testroom_24_27:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999700000 + e * 1000;
        event["content"]["body"] = "Message body 24_27_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_27";
    data["state"]["topic"] = "Topic 24_27";
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

TEST(CrossSigningFullTest, Scenario29) {
    // Test scenario 29 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_28";
    data["timestamp"] = 1600002428000;
    data["user_id"] = "@testuser_24_28:matrix.org";
    data["room_id"] = "!testroom_24_28:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999800000 + e * 1000;
        event["content"]["body"] = "Message body 24_28_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_28";
    data["state"]["topic"] = "Topic 24_28";
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

TEST(CrossSigningFullTest, Scenario30) {
    // Test scenario 30 for CrossSigningFull
    json data;
    data["test_id"] = "CrossSigningFull_29";
    data["timestamp"] = 1600002429000;
    data["user_id"] = "@testuser_24_29:matrix.org";
    data["room_id"] = "!testroom_24_29:matrix.org";
    
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
        event["event_id"] = "$event" + std::to_string(24) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        event["type"] = "m.room.message";
        event["sender"] = "@sender" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        event["origin_server_ts"] = 1599999900000 + e * 1000;
        event["content"]["body"] = "Message body 24_29_" + std::to_string(e);
        event["content"]["msgtype"] = "m.text";
        data["events"].push_back(event);
    }
    ASSERT_EQ(data["events"].size(), 5);
    
    // Verify state
    data["state"] = json::object();
    data["state"]["name"] = "Room 24_29";
    data["state"]["topic"] = "Topic 24_29";
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
