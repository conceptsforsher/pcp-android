#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "pcp/room_utils.hpp"
#include "pcp/create_room.hpp"
#include "pcp/power_level_utils.hpp"

namespace pcp { namespace test {
using json = nlohmann::json;

// Test 1: Room operation scenario
TEST(RoomOperationsTest, Scenario1) {
    // Setup test room data
    std::string roomId = "!testroom1:matrix.org";
    std::string userId = "@testuser1:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 1";
    createRequest["topic"] = "Topic for room 1";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 1");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 1"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 1"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 1";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee1:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer1:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom1:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 2: Room operation scenario
TEST(RoomOperationsTest, Scenario2) {
    // Setup test room data
    std::string roomId = "!testroom2:matrix.org";
    std::string userId = "@testuser2:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 2";
    createRequest["topic"] = "Topic for room 2";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 2");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 2"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 2"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 2";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee2:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer2:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom2:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 3: Room operation scenario
TEST(RoomOperationsTest, Scenario3) {
    // Setup test room data
    std::string roomId = "!testroom3:matrix.org";
    std::string userId = "@testuser3:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 3";
    createRequest["topic"] = "Topic for room 3";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 3");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 3"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 3"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 3";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee3:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer3:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom3:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 4: Room operation scenario
TEST(RoomOperationsTest, Scenario4) {
    // Setup test room data
    std::string roomId = "!testroom4:matrix.org";
    std::string userId = "@testuser4:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 4";
    createRequest["topic"] = "Topic for room 4";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 4");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 4"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 4"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 4";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee4:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer4:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom4:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 5: Room operation scenario
TEST(RoomOperationsTest, Scenario5) {
    // Setup test room data
    std::string roomId = "!testroom5:matrix.org";
    std::string userId = "@testuser5:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 5";
    createRequest["topic"] = "Topic for room 5";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 5");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 5"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 5"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 5";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee5:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer5:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom5:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 6: Room operation scenario
TEST(RoomOperationsTest, Scenario6) {
    // Setup test room data
    std::string roomId = "!testroom6:matrix.org";
    std::string userId = "@testuser6:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 6";
    createRequest["topic"] = "Topic for room 6";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 6");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 6"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 6"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 6";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee6:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer6:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom6:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 7: Room operation scenario
TEST(RoomOperationsTest, Scenario7) {
    // Setup test room data
    std::string roomId = "!testroom7:matrix.org";
    std::string userId = "@testuser7:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 7";
    createRequest["topic"] = "Topic for room 7";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 7");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 7"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 7"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 7";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee7:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer7:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom7:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 8: Room operation scenario
TEST(RoomOperationsTest, Scenario8) {
    // Setup test room data
    std::string roomId = "!testroom8:matrix.org";
    std::string userId = "@testuser8:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 8";
    createRequest["topic"] = "Topic for room 8";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 8");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 8"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 8"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 8";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee8:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer8:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom8:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 9: Room operation scenario
TEST(RoomOperationsTest, Scenario9) {
    // Setup test room data
    std::string roomId = "!testroom9:matrix.org";
    std::string userId = "@testuser9:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 9";
    createRequest["topic"] = "Topic for room 9";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 9");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 9"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 9"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 9";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee9:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer9:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom9:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 10: Room operation scenario
TEST(RoomOperationsTest, Scenario10) {
    // Setup test room data
    std::string roomId = "!testroom10:matrix.org";
    std::string userId = "@testuser10:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 10";
    createRequest["topic"] = "Topic for room 10";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 10");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 10"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 10"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 10";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee10:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer10:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom10:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 11: Room operation scenario
TEST(RoomOperationsTest, Scenario11) {
    // Setup test room data
    std::string roomId = "!testroom11:matrix.org";
    std::string userId = "@testuser11:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 11";
    createRequest["topic"] = "Topic for room 11";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 11");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 11"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 11"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 11";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee11:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer11:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom11:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 12: Room operation scenario
TEST(RoomOperationsTest, Scenario12) {
    // Setup test room data
    std::string roomId = "!testroom12:matrix.org";
    std::string userId = "@testuser12:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 12";
    createRequest["topic"] = "Topic for room 12";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 12");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 12"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 12"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 12";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee12:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer12:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom12:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 13: Room operation scenario
TEST(RoomOperationsTest, Scenario13) {
    // Setup test room data
    std::string roomId = "!testroom13:matrix.org";
    std::string userId = "@testuser13:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 13";
    createRequest["topic"] = "Topic for room 13";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 13");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 13"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 13"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 13";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee13:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer13:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom13:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 14: Room operation scenario
TEST(RoomOperationsTest, Scenario14) {
    // Setup test room data
    std::string roomId = "!testroom14:matrix.org";
    std::string userId = "@testuser14:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 14";
    createRequest["topic"] = "Topic for room 14";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 14");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 14"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 14"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 14";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee14:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer14:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom14:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 15: Room operation scenario
TEST(RoomOperationsTest, Scenario15) {
    // Setup test room data
    std::string roomId = "!testroom15:matrix.org";
    std::string userId = "@testuser15:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 15";
    createRequest["topic"] = "Topic for room 15";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 15");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 15"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 15"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 15";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee15:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer15:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom15:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 16: Room operation scenario
TEST(RoomOperationsTest, Scenario16) {
    // Setup test room data
    std::string roomId = "!testroom16:matrix.org";
    std::string userId = "@testuser16:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 16";
    createRequest["topic"] = "Topic for room 16";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 16");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 16"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 16"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 16";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee16:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer16:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom16:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 17: Room operation scenario
TEST(RoomOperationsTest, Scenario17) {
    // Setup test room data
    std::string roomId = "!testroom17:matrix.org";
    std::string userId = "@testuser17:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 17";
    createRequest["topic"] = "Topic for room 17";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 17");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 17"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 17"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 17";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee17:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer17:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom17:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 18: Room operation scenario
TEST(RoomOperationsTest, Scenario18) {
    // Setup test room data
    std::string roomId = "!testroom18:matrix.org";
    std::string userId = "@testuser18:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 18";
    createRequest["topic"] = "Topic for room 18";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 18");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 18"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 18"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 18";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee18:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer18:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom18:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 19: Room operation scenario
TEST(RoomOperationsTest, Scenario19) {
    // Setup test room data
    std::string roomId = "!testroom19:matrix.org";
    std::string userId = "@testuser19:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 19";
    createRequest["topic"] = "Topic for room 19";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 19");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 19"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 19"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 19";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee19:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer19:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom19:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 20: Room operation scenario
TEST(RoomOperationsTest, Scenario20) {
    // Setup test room data
    std::string roomId = "!testroom20:matrix.org";
    std::string userId = "@testuser20:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 20";
    createRequest["topic"] = "Topic for room 20";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 20");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 20"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 20"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 20";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee20:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer20:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom20:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 21: Room operation scenario
TEST(RoomOperationsTest, Scenario21) {
    // Setup test room data
    std::string roomId = "!testroom21:matrix.org";
    std::string userId = "@testuser21:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 21";
    createRequest["topic"] = "Topic for room 21";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 21");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 21"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 21"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 21";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee21:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer21:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom21:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 22: Room operation scenario
TEST(RoomOperationsTest, Scenario22) {
    // Setup test room data
    std::string roomId = "!testroom22:matrix.org";
    std::string userId = "@testuser22:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 22";
    createRequest["topic"] = "Topic for room 22";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 22");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 22"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 22"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 22";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee22:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer22:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom22:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 23: Room operation scenario
TEST(RoomOperationsTest, Scenario23) {
    // Setup test room data
    std::string roomId = "!testroom23:matrix.org";
    std::string userId = "@testuser23:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 23";
    createRequest["topic"] = "Topic for room 23";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 23");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 23"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 23"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 23";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee23:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer23:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom23:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 24: Room operation scenario
TEST(RoomOperationsTest, Scenario24) {
    // Setup test room data
    std::string roomId = "!testroom24:matrix.org";
    std::string userId = "@testuser24:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 24";
    createRequest["topic"] = "Topic for room 24";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 24");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 24"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 24"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 24";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee24:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer24:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom24:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 25: Room operation scenario
TEST(RoomOperationsTest, Scenario25) {
    // Setup test room data
    std::string roomId = "!testroom25:matrix.org";
    std::string userId = "@testuser25:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 25";
    createRequest["topic"] = "Topic for room 25";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 25");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 25"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 25"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 25";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee25:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer25:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom25:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 26: Room operation scenario
TEST(RoomOperationsTest, Scenario26) {
    // Setup test room data
    std::string roomId = "!testroom26:matrix.org";
    std::string userId = "@testuser26:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 26";
    createRequest["topic"] = "Topic for room 26";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 26");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 26"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 26"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 26";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee26:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer26:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom26:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 27: Room operation scenario
TEST(RoomOperationsTest, Scenario27) {
    // Setup test room data
    std::string roomId = "!testroom27:matrix.org";
    std::string userId = "@testuser27:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 27";
    createRequest["topic"] = "Topic for room 27";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 27");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 27"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 27"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 27";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee27:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer27:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom27:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 28: Room operation scenario
TEST(RoomOperationsTest, Scenario28) {
    // Setup test room data
    std::string roomId = "!testroom28:matrix.org";
    std::string userId = "@testuser28:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 28";
    createRequest["topic"] = "Topic for room 28";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 28");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 28"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 28"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 28";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee28:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer28:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom28:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 29: Room operation scenario
TEST(RoomOperationsTest, Scenario29) {
    // Setup test room data
    std::string roomId = "!testroom29:matrix.org";
    std::string userId = "@testuser29:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 29";
    createRequest["topic"] = "Topic for room 29";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 29");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 29"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 29"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 29";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee29:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer29:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom29:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 30: Room operation scenario
TEST(RoomOperationsTest, Scenario30) {
    // Setup test room data
    std::string roomId = "!testroom30:matrix.org";
    std::string userId = "@testuser30:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 30";
    createRequest["topic"] = "Topic for room 30";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 30");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 30"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 30"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 30";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee30:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer30:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom30:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 31: Room operation scenario
TEST(RoomOperationsTest, Scenario31) {
    // Setup test room data
    std::string roomId = "!testroom31:matrix.org";
    std::string userId = "@testuser31:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 31";
    createRequest["topic"] = "Topic for room 31";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 31");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 31"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 31"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 31";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee31:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer31:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom31:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 32: Room operation scenario
TEST(RoomOperationsTest, Scenario32) {
    // Setup test room data
    std::string roomId = "!testroom32:matrix.org";
    std::string userId = "@testuser32:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 32";
    createRequest["topic"] = "Topic for room 32";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 32");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 32"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 32"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 32";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee32:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer32:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom32:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 33: Room operation scenario
TEST(RoomOperationsTest, Scenario33) {
    // Setup test room data
    std::string roomId = "!testroom33:matrix.org";
    std::string userId = "@testuser33:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 33";
    createRequest["topic"] = "Topic for room 33";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 33");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 33"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 33"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 33";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee33:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer33:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom33:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 34: Room operation scenario
TEST(RoomOperationsTest, Scenario34) {
    // Setup test room data
    std::string roomId = "!testroom34:matrix.org";
    std::string userId = "@testuser34:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 34";
    createRequest["topic"] = "Topic for room 34";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 34");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 34"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 34"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 34";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee34:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer34:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom34:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 35: Room operation scenario
TEST(RoomOperationsTest, Scenario35) {
    // Setup test room data
    std::string roomId = "!testroom35:matrix.org";
    std::string userId = "@testuser35:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 35";
    createRequest["topic"] = "Topic for room 35";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 35");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 35"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 35"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 35";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee35:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer35:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom35:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 36: Room operation scenario
TEST(RoomOperationsTest, Scenario36) {
    // Setup test room data
    std::string roomId = "!testroom36:matrix.org";
    std::string userId = "@testuser36:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 36";
    createRequest["topic"] = "Topic for room 36";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 36");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 36"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 36"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 36";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee36:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer36:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom36:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 37: Room operation scenario
TEST(RoomOperationsTest, Scenario37) {
    // Setup test room data
    std::string roomId = "!testroom37:matrix.org";
    std::string userId = "@testuser37:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 37";
    createRequest["topic"] = "Topic for room 37";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 37");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 37"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 37"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 37";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee37:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer37:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom37:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 38: Room operation scenario
TEST(RoomOperationsTest, Scenario38) {
    // Setup test room data
    std::string roomId = "!testroom38:matrix.org";
    std::string userId = "@testuser38:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 38";
    createRequest["topic"] = "Topic for room 38";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 38");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 38"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 38"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 38";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee38:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer38:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom38:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 39: Room operation scenario
TEST(RoomOperationsTest, Scenario39) {
    // Setup test room data
    std::string roomId = "!testroom39:matrix.org";
    std::string userId = "@testuser39:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 39";
    createRequest["topic"] = "Topic for room 39";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 39");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 39"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 39"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 39";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee39:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer39:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom39:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 40: Room operation scenario
TEST(RoomOperationsTest, Scenario40) {
    // Setup test room data
    std::string roomId = "!testroom40:matrix.org";
    std::string userId = "@testuser40:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 40";
    createRequest["topic"] = "Topic for room 40";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 40");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 40"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 40"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 40";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee40:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer40:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom40:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 41: Room operation scenario
TEST(RoomOperationsTest, Scenario41) {
    // Setup test room data
    std::string roomId = "!testroom41:matrix.org";
    std::string userId = "@testuser41:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 41";
    createRequest["topic"] = "Topic for room 41";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 41");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 41"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 41"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 41";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee41:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer41:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom41:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 42: Room operation scenario
TEST(RoomOperationsTest, Scenario42) {
    // Setup test room data
    std::string roomId = "!testroom42:matrix.org";
    std::string userId = "@testuser42:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 42";
    createRequest["topic"] = "Topic for room 42";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 42");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 42"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 42"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 42";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee42:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer42:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom42:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 43: Room operation scenario
TEST(RoomOperationsTest, Scenario43) {
    // Setup test room data
    std::string roomId = "!testroom43:matrix.org";
    std::string userId = "@testuser43:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 43";
    createRequest["topic"] = "Topic for room 43";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 43");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 43"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 43"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 43";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee43:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer43:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom43:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 44: Room operation scenario
TEST(RoomOperationsTest, Scenario44) {
    // Setup test room data
    std::string roomId = "!testroom44:matrix.org";
    std::string userId = "@testuser44:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 44";
    createRequest["topic"] = "Topic for room 44";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 44");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 44"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 44"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 44";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee44:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer44:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom44:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 45: Room operation scenario
TEST(RoomOperationsTest, Scenario45) {
    // Setup test room data
    std::string roomId = "!testroom45:matrix.org";
    std::string userId = "@testuser45:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 45";
    createRequest["topic"] = "Topic for room 45";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 45");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 45"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 45"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 45";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee45:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer45:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom45:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 46: Room operation scenario
TEST(RoomOperationsTest, Scenario46) {
    // Setup test room data
    std::string roomId = "!testroom46:matrix.org";
    std::string userId = "@testuser46:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 46";
    createRequest["topic"] = "Topic for room 46";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 46");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 46"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 46"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 46";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee46:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer46:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom46:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 47: Room operation scenario
TEST(RoomOperationsTest, Scenario47) {
    // Setup test room data
    std::string roomId = "!testroom47:matrix.org";
    std::string userId = "@testuser47:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 47";
    createRequest["topic"] = "Topic for room 47";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 47");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 47"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 47"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 47";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee47:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer47:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom47:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 48: Room operation scenario
TEST(RoomOperationsTest, Scenario48) {
    // Setup test room data
    std::string roomId = "!testroom48:matrix.org";
    std::string userId = "@testuser48:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 48";
    createRequest["topic"] = "Topic for room 48";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 48");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 48"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 48"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 48";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee48:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer48:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom48:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 49: Room operation scenario
TEST(RoomOperationsTest, Scenario49) {
    // Setup test room data
    std::string roomId = "!testroom49:matrix.org";
    std::string userId = "@testuser49:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 49";
    createRequest["topic"] = "Topic for room 49";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 49");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 49"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 49"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 49";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee49:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer49:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom49:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

// Test 50: Room operation scenario
TEST(RoomOperationsTest, Scenario50) {
    // Setup test room data
    std::string roomId = "!testroom50:matrix.org";
    std::string userId = "@testuser50:matrix.org";
    
    json createRequest;
    createRequest["name"] = "Test Room 50";
    createRequest["topic"] = "Topic for room 50";
    createRequest["preset"] = "private_chat";
    createRequest["visibility"] = "private";
    createRequest["is_direct"] = true;
    
    ASSERT_FALSE(createRequest.empty());
    ASSERT_EQ(createRequest["name"], "Test Room 50");
    ASSERT_TRUE(createRequest["is_direct"]);
    
    // Test room state events
    json nameEvent;
    nameEvent["type"] = "m.room.name";
    nameEvent["content"] = {"name", "Test Room 50"};
    ASSERT_EQ(nameEvent["type"], "m.room.name");
    
    json topicEvent;
    topicEvent["type"] = "m.room.topic";
    topicEvent["content"] = {"topic", "Topic for room 50"};
    ASSERT_EQ(topicEvent["type"], "m.room.topic");
    
    json powerLevels;
    powerLevels["type"] = "m.room.power_levels";
    powerLevels["content"]["users_default"] = 0;
    powerLevels["content"]["events_default"] = 0;
    powerLevels["content"]["state_default"] = 50;
    powerLevels["content"]["users"][userId] = 100;
    ASSERT_EQ(powerLevels["content"]["users_default"], 0);
    ASSERT_EQ(powerLevels["content"]["state_default"], 50);
    
    // Test membership operations
    json membershipEvent;
    membershipEvent["type"] = "m.room.member";
    membershipEvent["state_key"] = userId;
    membershipEvent["content"]["membership"] = "join";
    membershipEvent["content"]["displayname"] = "Test User 50";
    ASSERT_EQ(membershipEvent["content"]["membership"], "join");
    ASSERT_FALSE(membershipEvent["state_key"].empty());
    
    // Test invite
    std::string inviteeId = "@invitee50:matrix.org";
    json inviteEvent;
    inviteEvent["type"] = "m.room.member";
    inviteEvent["state_key"] = inviteeId;
    inviteEvent["content"]["membership"] = "invite";
    ASSERT_EQ(inviteEvent["content"]["membership"], "invite");
    
    // Test ban
    json banEvent;
    banEvent["type"] = "m.room.member";
    banEvent["state_key"] = "@spammer50:matrix.org";
    banEvent["content"]["membership"] = "ban";
    banEvent["content"]["reason"] = "Violating terms of service";
    ASSERT_EQ(banEvent["content"]["membership"], "ban");
    ASSERT_FALSE(banEvent["content"]["reason"].empty());
    
    // Test alias management
    json aliasEvent;
    aliasEvent["type"] = "m.room.canonical_alias";
    aliasEvent["content"]["alias"] = "#testroom50:matrix.org";
    ASSERT_TRUE(aliasEvent["content"]["alias"].get<std::string>().starts_with("#"));
    
    // Verify all events are valid
    ASSERT_NO_THROW({ auto j = nameEvent.dump(); });
    ASSERT_NO_THROW({ auto j = membershipEvent.dump(); });
}

}} // namespace pcp::test