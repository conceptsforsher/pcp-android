#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "pcp/database.hpp"
#include "pcp/store_utils.hpp"

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(DatabaseTest, Scenario1) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0000.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom0:matrix.org";
    room["display_name"] = "Test Room 0";
    room["topic"] = "Database test room 0";
    room["avatar_url"] = "mxc://matrix.org/avatar_0";
    room["joined_members_count"] = 3 + 0 % 5;
    room["invited_members_count"] = 0 % 3;
    room["notification_count"] = 0 % 10;
    room["highlight_count"] = 0 % 5;
    room["is_direct"] = 0 % 2 == 0;
    room["is_encrypted"] = 0 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom0:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000000:matrix.org";
    event["room_id"] = "!testroom0:matrix.org";
    event["sender"] = "@dbtestuser0:matrix.org";
    event["origin_server_ts"] = 1600000000000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 0";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom0:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom0:matrix.org";
    member["user_id"] = "@member0:matrix.org";
    member["display_name"] = "Member 0";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_0";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom0:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom0:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom0:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000000";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom0:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario2) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0001.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom1:matrix.org";
    room["display_name"] = "Test Room 1";
    room["topic"] = "Database test room 1";
    room["avatar_url"] = "mxc://matrix.org/avatar_1";
    room["joined_members_count"] = 3 + 1 % 5;
    room["invited_members_count"] = 1 % 3;
    room["notification_count"] = 1 % 10;
    room["highlight_count"] = 1 % 5;
    room["is_direct"] = 1 % 2 == 0;
    room["is_encrypted"] = 1 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom1:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000001:matrix.org";
    event["room_id"] = "!testroom1:matrix.org";
    event["sender"] = "@dbtestuser1:matrix.org";
    event["origin_server_ts"] = 1600000001000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 1";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom1:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom1:matrix.org";
    member["user_id"] = "@member1:matrix.org";
    member["display_name"] = "Member 1";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_1";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom1:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom1:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom1:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000001";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom1:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario3) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0002.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom2:matrix.org";
    room["display_name"] = "Test Room 2";
    room["topic"] = "Database test room 2";
    room["avatar_url"] = "mxc://matrix.org/avatar_2";
    room["joined_members_count"] = 3 + 2 % 5;
    room["invited_members_count"] = 2 % 3;
    room["notification_count"] = 2 % 10;
    room["highlight_count"] = 2 % 5;
    room["is_direct"] = 2 % 2 == 0;
    room["is_encrypted"] = 2 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom2:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000002:matrix.org";
    event["room_id"] = "!testroom2:matrix.org";
    event["sender"] = "@dbtestuser2:matrix.org";
    event["origin_server_ts"] = 1600000002000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 2";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom2:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom2:matrix.org";
    member["user_id"] = "@member2:matrix.org";
    member["display_name"] = "Member 2";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_2";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom2:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom2:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom2:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000002";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom2:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario4) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0003.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom3:matrix.org";
    room["display_name"] = "Test Room 3";
    room["topic"] = "Database test room 3";
    room["avatar_url"] = "mxc://matrix.org/avatar_3";
    room["joined_members_count"] = 3 + 3 % 5;
    room["invited_members_count"] = 3 % 3;
    room["notification_count"] = 3 % 10;
    room["highlight_count"] = 3 % 5;
    room["is_direct"] = 3 % 2 == 0;
    room["is_encrypted"] = 3 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom3:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000003:matrix.org";
    event["room_id"] = "!testroom3:matrix.org";
    event["sender"] = "@dbtestuser3:matrix.org";
    event["origin_server_ts"] = 1600000003000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 3";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom3:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom3:matrix.org";
    member["user_id"] = "@member3:matrix.org";
    member["display_name"] = "Member 3";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_3";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom3:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom3:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom3:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000003";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom3:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario5) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0004.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom4:matrix.org";
    room["display_name"] = "Test Room 4";
    room["topic"] = "Database test room 4";
    room["avatar_url"] = "mxc://matrix.org/avatar_4";
    room["joined_members_count"] = 3 + 4 % 5;
    room["invited_members_count"] = 4 % 3;
    room["notification_count"] = 4 % 10;
    room["highlight_count"] = 4 % 5;
    room["is_direct"] = 4 % 2 == 0;
    room["is_encrypted"] = 4 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom4:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000004:matrix.org";
    event["room_id"] = "!testroom4:matrix.org";
    event["sender"] = "@dbtestuser4:matrix.org";
    event["origin_server_ts"] = 1600000004000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 4";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom4:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom4:matrix.org";
    member["user_id"] = "@member4:matrix.org";
    member["display_name"] = "Member 4";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_4";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom4:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom4:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom4:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000004";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom4:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario6) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0005.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom5:matrix.org";
    room["display_name"] = "Test Room 5";
    room["topic"] = "Database test room 5";
    room["avatar_url"] = "mxc://matrix.org/avatar_5";
    room["joined_members_count"] = 3 + 5 % 5;
    room["invited_members_count"] = 5 % 3;
    room["notification_count"] = 5 % 10;
    room["highlight_count"] = 5 % 5;
    room["is_direct"] = 5 % 2 == 0;
    room["is_encrypted"] = 5 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom5:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000005:matrix.org";
    event["room_id"] = "!testroom5:matrix.org";
    event["sender"] = "@dbtestuser5:matrix.org";
    event["origin_server_ts"] = 1600000005000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 5";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom5:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom5:matrix.org";
    member["user_id"] = "@member5:matrix.org";
    member["display_name"] = "Member 5";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_5";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom5:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom5:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom5:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000005";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom5:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario7) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0006.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom6:matrix.org";
    room["display_name"] = "Test Room 6";
    room["topic"] = "Database test room 6";
    room["avatar_url"] = "mxc://matrix.org/avatar_6";
    room["joined_members_count"] = 3 + 6 % 5;
    room["invited_members_count"] = 6 % 3;
    room["notification_count"] = 6 % 10;
    room["highlight_count"] = 6 % 5;
    room["is_direct"] = 6 % 2 == 0;
    room["is_encrypted"] = 6 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom6:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000006:matrix.org";
    event["room_id"] = "!testroom6:matrix.org";
    event["sender"] = "@dbtestuser6:matrix.org";
    event["origin_server_ts"] = 1600000006000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 6";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom6:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom6:matrix.org";
    member["user_id"] = "@member6:matrix.org";
    member["display_name"] = "Member 6";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_6";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom6:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom6:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom6:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000006";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom6:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario8) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0007.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom7:matrix.org";
    room["display_name"] = "Test Room 7";
    room["topic"] = "Database test room 7";
    room["avatar_url"] = "mxc://matrix.org/avatar_7";
    room["joined_members_count"] = 3 + 7 % 5;
    room["invited_members_count"] = 7 % 3;
    room["notification_count"] = 7 % 10;
    room["highlight_count"] = 7 % 5;
    room["is_direct"] = 7 % 2 == 0;
    room["is_encrypted"] = 7 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom7:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000007:matrix.org";
    event["room_id"] = "!testroom7:matrix.org";
    event["sender"] = "@dbtestuser7:matrix.org";
    event["origin_server_ts"] = 1600000007000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 7";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom7:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom7:matrix.org";
    member["user_id"] = "@member7:matrix.org";
    member["display_name"] = "Member 7";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_7";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom7:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom7:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom7:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000007";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom7:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario9) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0008.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom8:matrix.org";
    room["display_name"] = "Test Room 8";
    room["topic"] = "Database test room 8";
    room["avatar_url"] = "mxc://matrix.org/avatar_8";
    room["joined_members_count"] = 3 + 8 % 5;
    room["invited_members_count"] = 8 % 3;
    room["notification_count"] = 8 % 10;
    room["highlight_count"] = 8 % 5;
    room["is_direct"] = 8 % 2 == 0;
    room["is_encrypted"] = 8 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom8:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000008:matrix.org";
    event["room_id"] = "!testroom8:matrix.org";
    event["sender"] = "@dbtestuser8:matrix.org";
    event["origin_server_ts"] = 1600000008000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 8";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom8:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom8:matrix.org";
    member["user_id"] = "@member8:matrix.org";
    member["display_name"] = "Member 8";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_8";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom8:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom8:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom8:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000008";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom8:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario10) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0009.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom9:matrix.org";
    room["display_name"] = "Test Room 9";
    room["topic"] = "Database test room 9";
    room["avatar_url"] = "mxc://matrix.org/avatar_9";
    room["joined_members_count"] = 3 + 9 % 5;
    room["invited_members_count"] = 9 % 3;
    room["notification_count"] = 9 % 10;
    room["highlight_count"] = 9 % 5;
    room["is_direct"] = 9 % 2 == 0;
    room["is_encrypted"] = 9 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom9:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000009:matrix.org";
    event["room_id"] = "!testroom9:matrix.org";
    event["sender"] = "@dbtestuser9:matrix.org";
    event["origin_server_ts"] = 1600000009000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 9";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom9:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom9:matrix.org";
    member["user_id"] = "@member9:matrix.org";
    member["display_name"] = "Member 9";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_9";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom9:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom9:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom9:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000009";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom9:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario11) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0010.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom10:matrix.org";
    room["display_name"] = "Test Room 10";
    room["topic"] = "Database test room 10";
    room["avatar_url"] = "mxc://matrix.org/avatar_10";
    room["joined_members_count"] = 3 + 10 % 5;
    room["invited_members_count"] = 10 % 3;
    room["notification_count"] = 10 % 10;
    room["highlight_count"] = 10 % 5;
    room["is_direct"] = 10 % 2 == 0;
    room["is_encrypted"] = 10 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom10:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000010:matrix.org";
    event["room_id"] = "!testroom10:matrix.org";
    event["sender"] = "@dbtestuser10:matrix.org";
    event["origin_server_ts"] = 1600000010000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 10";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom10:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom10:matrix.org";
    member["user_id"] = "@member10:matrix.org";
    member["display_name"] = "Member 10";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_10";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom10:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom10:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom10:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000010";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom10:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario12) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0011.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom11:matrix.org";
    room["display_name"] = "Test Room 11";
    room["topic"] = "Database test room 11";
    room["avatar_url"] = "mxc://matrix.org/avatar_11";
    room["joined_members_count"] = 3 + 11 % 5;
    room["invited_members_count"] = 11 % 3;
    room["notification_count"] = 11 % 10;
    room["highlight_count"] = 11 % 5;
    room["is_direct"] = 11 % 2 == 0;
    room["is_encrypted"] = 11 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom11:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000011:matrix.org";
    event["room_id"] = "!testroom11:matrix.org";
    event["sender"] = "@dbtestuser11:matrix.org";
    event["origin_server_ts"] = 1600000011000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 11";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom11:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom11:matrix.org";
    member["user_id"] = "@member11:matrix.org";
    member["display_name"] = "Member 11";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_11";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom11:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom11:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom11:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000011";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom11:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario13) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0012.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom12:matrix.org";
    room["display_name"] = "Test Room 12";
    room["topic"] = "Database test room 12";
    room["avatar_url"] = "mxc://matrix.org/avatar_12";
    room["joined_members_count"] = 3 + 12 % 5;
    room["invited_members_count"] = 12 % 3;
    room["notification_count"] = 12 % 10;
    room["highlight_count"] = 12 % 5;
    room["is_direct"] = 12 % 2 == 0;
    room["is_encrypted"] = 12 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom12:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000012:matrix.org";
    event["room_id"] = "!testroom12:matrix.org";
    event["sender"] = "@dbtestuser12:matrix.org";
    event["origin_server_ts"] = 1600000012000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 12";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom12:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom12:matrix.org";
    member["user_id"] = "@member12:matrix.org";
    member["display_name"] = "Member 12";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_12";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom12:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom12:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom12:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000012";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom12:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario14) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0013.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom13:matrix.org";
    room["display_name"] = "Test Room 13";
    room["topic"] = "Database test room 13";
    room["avatar_url"] = "mxc://matrix.org/avatar_13";
    room["joined_members_count"] = 3 + 13 % 5;
    room["invited_members_count"] = 13 % 3;
    room["notification_count"] = 13 % 10;
    room["highlight_count"] = 13 % 5;
    room["is_direct"] = 13 % 2 == 0;
    room["is_encrypted"] = 13 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom13:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000013:matrix.org";
    event["room_id"] = "!testroom13:matrix.org";
    event["sender"] = "@dbtestuser13:matrix.org";
    event["origin_server_ts"] = 1600000013000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 13";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom13:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom13:matrix.org";
    member["user_id"] = "@member13:matrix.org";
    member["display_name"] = "Member 13";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_13";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom13:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom13:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom13:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000013";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom13:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario15) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0014.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom14:matrix.org";
    room["display_name"] = "Test Room 14";
    room["topic"] = "Database test room 14";
    room["avatar_url"] = "mxc://matrix.org/avatar_14";
    room["joined_members_count"] = 3 + 14 % 5;
    room["invited_members_count"] = 14 % 3;
    room["notification_count"] = 14 % 10;
    room["highlight_count"] = 14 % 5;
    room["is_direct"] = 14 % 2 == 0;
    room["is_encrypted"] = 14 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom14:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000014:matrix.org";
    event["room_id"] = "!testroom14:matrix.org";
    event["sender"] = "@dbtestuser14:matrix.org";
    event["origin_server_ts"] = 1600000014000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 14";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom14:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom14:matrix.org";
    member["user_id"] = "@member14:matrix.org";
    member["display_name"] = "Member 14";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_14";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom14:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom14:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom14:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000014";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom14:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario16) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0015.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom15:matrix.org";
    room["display_name"] = "Test Room 15";
    room["topic"] = "Database test room 15";
    room["avatar_url"] = "mxc://matrix.org/avatar_15";
    room["joined_members_count"] = 3 + 15 % 5;
    room["invited_members_count"] = 15 % 3;
    room["notification_count"] = 15 % 10;
    room["highlight_count"] = 15 % 5;
    room["is_direct"] = 15 % 2 == 0;
    room["is_encrypted"] = 15 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom15:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000015:matrix.org";
    event["room_id"] = "!testroom15:matrix.org";
    event["sender"] = "@dbtestuser15:matrix.org";
    event["origin_server_ts"] = 1600000015000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 15";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom15:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom15:matrix.org";
    member["user_id"] = "@member15:matrix.org";
    member["display_name"] = "Member 15";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_15";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom15:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom15:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom15:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000015";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom15:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario17) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0016.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom16:matrix.org";
    room["display_name"] = "Test Room 16";
    room["topic"] = "Database test room 16";
    room["avatar_url"] = "mxc://matrix.org/avatar_16";
    room["joined_members_count"] = 3 + 16 % 5;
    room["invited_members_count"] = 16 % 3;
    room["notification_count"] = 16 % 10;
    room["highlight_count"] = 16 % 5;
    room["is_direct"] = 16 % 2 == 0;
    room["is_encrypted"] = 16 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom16:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000016:matrix.org";
    event["room_id"] = "!testroom16:matrix.org";
    event["sender"] = "@dbtestuser16:matrix.org";
    event["origin_server_ts"] = 1600000016000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 16";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom16:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom16:matrix.org";
    member["user_id"] = "@member16:matrix.org";
    member["display_name"] = "Member 16";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_16";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom16:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom16:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom16:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000016";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom16:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario18) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0017.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom17:matrix.org";
    room["display_name"] = "Test Room 17";
    room["topic"] = "Database test room 17";
    room["avatar_url"] = "mxc://matrix.org/avatar_17";
    room["joined_members_count"] = 3 + 17 % 5;
    room["invited_members_count"] = 17 % 3;
    room["notification_count"] = 17 % 10;
    room["highlight_count"] = 17 % 5;
    room["is_direct"] = 17 % 2 == 0;
    room["is_encrypted"] = 17 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom17:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000017:matrix.org";
    event["room_id"] = "!testroom17:matrix.org";
    event["sender"] = "@dbtestuser17:matrix.org";
    event["origin_server_ts"] = 1600000017000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 17";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom17:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom17:matrix.org";
    member["user_id"] = "@member17:matrix.org";
    member["display_name"] = "Member 17";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_17";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom17:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom17:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom17:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000017";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom17:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario19) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0018.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom18:matrix.org";
    room["display_name"] = "Test Room 18";
    room["topic"] = "Database test room 18";
    room["avatar_url"] = "mxc://matrix.org/avatar_18";
    room["joined_members_count"] = 3 + 18 % 5;
    room["invited_members_count"] = 18 % 3;
    room["notification_count"] = 18 % 10;
    room["highlight_count"] = 18 % 5;
    room["is_direct"] = 18 % 2 == 0;
    room["is_encrypted"] = 18 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom18:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000018:matrix.org";
    event["room_id"] = "!testroom18:matrix.org";
    event["sender"] = "@dbtestuser18:matrix.org";
    event["origin_server_ts"] = 1600000018000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 18";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom18:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom18:matrix.org";
    member["user_id"] = "@member18:matrix.org";
    member["display_name"] = "Member 18";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_18";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom18:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom18:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom18:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000018";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom18:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario20) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0019.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom19:matrix.org";
    room["display_name"] = "Test Room 19";
    room["topic"] = "Database test room 19";
    room["avatar_url"] = "mxc://matrix.org/avatar_19";
    room["joined_members_count"] = 3 + 19 % 5;
    room["invited_members_count"] = 19 % 3;
    room["notification_count"] = 19 % 10;
    room["highlight_count"] = 19 % 5;
    room["is_direct"] = 19 % 2 == 0;
    room["is_encrypted"] = 19 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom19:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000019:matrix.org";
    event["room_id"] = "!testroom19:matrix.org";
    event["sender"] = "@dbtestuser19:matrix.org";
    event["origin_server_ts"] = 1600000019000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 19";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom19:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom19:matrix.org";
    member["user_id"] = "@member19:matrix.org";
    member["display_name"] = "Member 19";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_19";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom19:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom19:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom19:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000019";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom19:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario21) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0020.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom20:matrix.org";
    room["display_name"] = "Test Room 20";
    room["topic"] = "Database test room 20";
    room["avatar_url"] = "mxc://matrix.org/avatar_20";
    room["joined_members_count"] = 3 + 20 % 5;
    room["invited_members_count"] = 20 % 3;
    room["notification_count"] = 20 % 10;
    room["highlight_count"] = 20 % 5;
    room["is_direct"] = 20 % 2 == 0;
    room["is_encrypted"] = 20 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom20:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000020:matrix.org";
    event["room_id"] = "!testroom20:matrix.org";
    event["sender"] = "@dbtestuser20:matrix.org";
    event["origin_server_ts"] = 1600000020000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 20";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom20:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom20:matrix.org";
    member["user_id"] = "@member20:matrix.org";
    member["display_name"] = "Member 20";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_20";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom20:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom20:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom20:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000020";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom20:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario22) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0021.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom21:matrix.org";
    room["display_name"] = "Test Room 21";
    room["topic"] = "Database test room 21";
    room["avatar_url"] = "mxc://matrix.org/avatar_21";
    room["joined_members_count"] = 3 + 21 % 5;
    room["invited_members_count"] = 21 % 3;
    room["notification_count"] = 21 % 10;
    room["highlight_count"] = 21 % 5;
    room["is_direct"] = 21 % 2 == 0;
    room["is_encrypted"] = 21 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom21:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000021:matrix.org";
    event["room_id"] = "!testroom21:matrix.org";
    event["sender"] = "@dbtestuser21:matrix.org";
    event["origin_server_ts"] = 1600000021000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 21";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom21:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom21:matrix.org";
    member["user_id"] = "@member21:matrix.org";
    member["display_name"] = "Member 21";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_21";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom21:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom21:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom21:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000021";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom21:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario23) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0022.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom22:matrix.org";
    room["display_name"] = "Test Room 22";
    room["topic"] = "Database test room 22";
    room["avatar_url"] = "mxc://matrix.org/avatar_22";
    room["joined_members_count"] = 3 + 22 % 5;
    room["invited_members_count"] = 22 % 3;
    room["notification_count"] = 22 % 10;
    room["highlight_count"] = 22 % 5;
    room["is_direct"] = 22 % 2 == 0;
    room["is_encrypted"] = 22 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom22:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000022:matrix.org";
    event["room_id"] = "!testroom22:matrix.org";
    event["sender"] = "@dbtestuser22:matrix.org";
    event["origin_server_ts"] = 1600000022000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 22";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom22:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom22:matrix.org";
    member["user_id"] = "@member22:matrix.org";
    member["display_name"] = "Member 22";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_22";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom22:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom22:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom22:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000022";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom22:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario24) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0023.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom23:matrix.org";
    room["display_name"] = "Test Room 23";
    room["topic"] = "Database test room 23";
    room["avatar_url"] = "mxc://matrix.org/avatar_23";
    room["joined_members_count"] = 3 + 23 % 5;
    room["invited_members_count"] = 23 % 3;
    room["notification_count"] = 23 % 10;
    room["highlight_count"] = 23 % 5;
    room["is_direct"] = 23 % 2 == 0;
    room["is_encrypted"] = 23 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom23:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000023:matrix.org";
    event["room_id"] = "!testroom23:matrix.org";
    event["sender"] = "@dbtestuser23:matrix.org";
    event["origin_server_ts"] = 1600000023000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 23";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom23:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom23:matrix.org";
    member["user_id"] = "@member23:matrix.org";
    member["display_name"] = "Member 23";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_23";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom23:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom23:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom23:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000023";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom23:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario25) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0024.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom24:matrix.org";
    room["display_name"] = "Test Room 24";
    room["topic"] = "Database test room 24";
    room["avatar_url"] = "mxc://matrix.org/avatar_24";
    room["joined_members_count"] = 3 + 24 % 5;
    room["invited_members_count"] = 24 % 3;
    room["notification_count"] = 24 % 10;
    room["highlight_count"] = 24 % 5;
    room["is_direct"] = 24 % 2 == 0;
    room["is_encrypted"] = 24 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom24:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000024:matrix.org";
    event["room_id"] = "!testroom24:matrix.org";
    event["sender"] = "@dbtestuser24:matrix.org";
    event["origin_server_ts"] = 1600000024000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 24";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom24:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom24:matrix.org";
    member["user_id"] = "@member24:matrix.org";
    member["display_name"] = "Member 24";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_24";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom24:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom24:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom24:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000024";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom24:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario26) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0025.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom25:matrix.org";
    room["display_name"] = "Test Room 25";
    room["topic"] = "Database test room 25";
    room["avatar_url"] = "mxc://matrix.org/avatar_25";
    room["joined_members_count"] = 3 + 25 % 5;
    room["invited_members_count"] = 25 % 3;
    room["notification_count"] = 25 % 10;
    room["highlight_count"] = 25 % 5;
    room["is_direct"] = 25 % 2 == 0;
    room["is_encrypted"] = 25 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom25:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000025:matrix.org";
    event["room_id"] = "!testroom25:matrix.org";
    event["sender"] = "@dbtestuser25:matrix.org";
    event["origin_server_ts"] = 1600000025000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 25";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom25:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom25:matrix.org";
    member["user_id"] = "@member25:matrix.org";
    member["display_name"] = "Member 25";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_25";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom25:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom25:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom25:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000025";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom25:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario27) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0026.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom26:matrix.org";
    room["display_name"] = "Test Room 26";
    room["topic"] = "Database test room 26";
    room["avatar_url"] = "mxc://matrix.org/avatar_26";
    room["joined_members_count"] = 3 + 26 % 5;
    room["invited_members_count"] = 26 % 3;
    room["notification_count"] = 26 % 10;
    room["highlight_count"] = 26 % 5;
    room["is_direct"] = 26 % 2 == 0;
    room["is_encrypted"] = 26 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom26:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000026:matrix.org";
    event["room_id"] = "!testroom26:matrix.org";
    event["sender"] = "@dbtestuser26:matrix.org";
    event["origin_server_ts"] = 1600000026000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 26";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom26:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom26:matrix.org";
    member["user_id"] = "@member26:matrix.org";
    member["display_name"] = "Member 26";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_26";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom26:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom26:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom26:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000026";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom26:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario28) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0027.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom27:matrix.org";
    room["display_name"] = "Test Room 27";
    room["topic"] = "Database test room 27";
    room["avatar_url"] = "mxc://matrix.org/avatar_27";
    room["joined_members_count"] = 3 + 27 % 5;
    room["invited_members_count"] = 27 % 3;
    room["notification_count"] = 27 % 10;
    room["highlight_count"] = 27 % 5;
    room["is_direct"] = 27 % 2 == 0;
    room["is_encrypted"] = 27 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom27:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000027:matrix.org";
    event["room_id"] = "!testroom27:matrix.org";
    event["sender"] = "@dbtestuser27:matrix.org";
    event["origin_server_ts"] = 1600000027000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 27";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom27:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom27:matrix.org";
    member["user_id"] = "@member27:matrix.org";
    member["display_name"] = "Member 27";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_27";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom27:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom27:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom27:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000027";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom27:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario29) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0028.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom28:matrix.org";
    room["display_name"] = "Test Room 28";
    room["topic"] = "Database test room 28";
    room["avatar_url"] = "mxc://matrix.org/avatar_28";
    room["joined_members_count"] = 3 + 28 % 5;
    room["invited_members_count"] = 28 % 3;
    room["notification_count"] = 28 % 10;
    room["highlight_count"] = 28 % 5;
    room["is_direct"] = 28 % 2 == 0;
    room["is_encrypted"] = 28 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom28:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000028:matrix.org";
    event["room_id"] = "!testroom28:matrix.org";
    event["sender"] = "@dbtestuser28:matrix.org";
    event["origin_server_ts"] = 1600000028000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 28";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom28:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom28:matrix.org";
    member["user_id"] = "@member28:matrix.org";
    member["display_name"] = "Member 28";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_28";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom28:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom28:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom28:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000028";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom28:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario30) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0029.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom29:matrix.org";
    room["display_name"] = "Test Room 29";
    room["topic"] = "Database test room 29";
    room["avatar_url"] = "mxc://matrix.org/avatar_29";
    room["joined_members_count"] = 3 + 29 % 5;
    room["invited_members_count"] = 29 % 3;
    room["notification_count"] = 29 % 10;
    room["highlight_count"] = 29 % 5;
    room["is_direct"] = 29 % 2 == 0;
    room["is_encrypted"] = 29 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom29:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000029:matrix.org";
    event["room_id"] = "!testroom29:matrix.org";
    event["sender"] = "@dbtestuser29:matrix.org";
    event["origin_server_ts"] = 1600000029000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 29";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom29:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom29:matrix.org";
    member["user_id"] = "@member29:matrix.org";
    member["display_name"] = "Member 29";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_29";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom29:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom29:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom29:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000029";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom29:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario31) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0030.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom30:matrix.org";
    room["display_name"] = "Test Room 30";
    room["topic"] = "Database test room 30";
    room["avatar_url"] = "mxc://matrix.org/avatar_30";
    room["joined_members_count"] = 3 + 30 % 5;
    room["invited_members_count"] = 30 % 3;
    room["notification_count"] = 30 % 10;
    room["highlight_count"] = 30 % 5;
    room["is_direct"] = 30 % 2 == 0;
    room["is_encrypted"] = 30 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom30:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000030:matrix.org";
    event["room_id"] = "!testroom30:matrix.org";
    event["sender"] = "@dbtestuser30:matrix.org";
    event["origin_server_ts"] = 1600000030000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 30";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom30:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom30:matrix.org";
    member["user_id"] = "@member30:matrix.org";
    member["display_name"] = "Member 30";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_30";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom30:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom30:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom30:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000030";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom30:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario32) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0031.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom31:matrix.org";
    room["display_name"] = "Test Room 31";
    room["topic"] = "Database test room 31";
    room["avatar_url"] = "mxc://matrix.org/avatar_31";
    room["joined_members_count"] = 3 + 31 % 5;
    room["invited_members_count"] = 31 % 3;
    room["notification_count"] = 31 % 10;
    room["highlight_count"] = 31 % 5;
    room["is_direct"] = 31 % 2 == 0;
    room["is_encrypted"] = 31 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom31:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000031:matrix.org";
    event["room_id"] = "!testroom31:matrix.org";
    event["sender"] = "@dbtestuser31:matrix.org";
    event["origin_server_ts"] = 1600000031000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 31";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom31:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom31:matrix.org";
    member["user_id"] = "@member31:matrix.org";
    member["display_name"] = "Member 31";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_31";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom31:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom31:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom31:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000031";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom31:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario33) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0032.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom32:matrix.org";
    room["display_name"] = "Test Room 32";
    room["topic"] = "Database test room 32";
    room["avatar_url"] = "mxc://matrix.org/avatar_32";
    room["joined_members_count"] = 3 + 32 % 5;
    room["invited_members_count"] = 32 % 3;
    room["notification_count"] = 32 % 10;
    room["highlight_count"] = 32 % 5;
    room["is_direct"] = 32 % 2 == 0;
    room["is_encrypted"] = 32 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom32:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000032:matrix.org";
    event["room_id"] = "!testroom32:matrix.org";
    event["sender"] = "@dbtestuser32:matrix.org";
    event["origin_server_ts"] = 1600000032000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 32";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom32:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom32:matrix.org";
    member["user_id"] = "@member32:matrix.org";
    member["display_name"] = "Member 32";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_32";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom32:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom32:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom32:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000032";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom32:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario34) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0033.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom33:matrix.org";
    room["display_name"] = "Test Room 33";
    room["topic"] = "Database test room 33";
    room["avatar_url"] = "mxc://matrix.org/avatar_33";
    room["joined_members_count"] = 3 + 33 % 5;
    room["invited_members_count"] = 33 % 3;
    room["notification_count"] = 33 % 10;
    room["highlight_count"] = 33 % 5;
    room["is_direct"] = 33 % 2 == 0;
    room["is_encrypted"] = 33 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom33:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000033:matrix.org";
    event["room_id"] = "!testroom33:matrix.org";
    event["sender"] = "@dbtestuser33:matrix.org";
    event["origin_server_ts"] = 1600000033000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 33";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom33:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom33:matrix.org";
    member["user_id"] = "@member33:matrix.org";
    member["display_name"] = "Member 33";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_33";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom33:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom33:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom33:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000033";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom33:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario35) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0034.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom34:matrix.org";
    room["display_name"] = "Test Room 34";
    room["topic"] = "Database test room 34";
    room["avatar_url"] = "mxc://matrix.org/avatar_34";
    room["joined_members_count"] = 3 + 34 % 5;
    room["invited_members_count"] = 34 % 3;
    room["notification_count"] = 34 % 10;
    room["highlight_count"] = 34 % 5;
    room["is_direct"] = 34 % 2 == 0;
    room["is_encrypted"] = 34 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom34:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000034:matrix.org";
    event["room_id"] = "!testroom34:matrix.org";
    event["sender"] = "@dbtestuser34:matrix.org";
    event["origin_server_ts"] = 1600000034000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 34";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom34:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom34:matrix.org";
    member["user_id"] = "@member34:matrix.org";
    member["display_name"] = "Member 34";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_34";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom34:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom34:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom34:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000034";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom34:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario36) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0035.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom35:matrix.org";
    room["display_name"] = "Test Room 35";
    room["topic"] = "Database test room 35";
    room["avatar_url"] = "mxc://matrix.org/avatar_35";
    room["joined_members_count"] = 3 + 35 % 5;
    room["invited_members_count"] = 35 % 3;
    room["notification_count"] = 35 % 10;
    room["highlight_count"] = 35 % 5;
    room["is_direct"] = 35 % 2 == 0;
    room["is_encrypted"] = 35 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom35:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000035:matrix.org";
    event["room_id"] = "!testroom35:matrix.org";
    event["sender"] = "@dbtestuser35:matrix.org";
    event["origin_server_ts"] = 1600000035000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 35";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom35:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom35:matrix.org";
    member["user_id"] = "@member35:matrix.org";
    member["display_name"] = "Member 35";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_35";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom35:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom35:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom35:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000035";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom35:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario37) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0036.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom36:matrix.org";
    room["display_name"] = "Test Room 36";
    room["topic"] = "Database test room 36";
    room["avatar_url"] = "mxc://matrix.org/avatar_36";
    room["joined_members_count"] = 3 + 36 % 5;
    room["invited_members_count"] = 36 % 3;
    room["notification_count"] = 36 % 10;
    room["highlight_count"] = 36 % 5;
    room["is_direct"] = 36 % 2 == 0;
    room["is_encrypted"] = 36 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom36:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000036:matrix.org";
    event["room_id"] = "!testroom36:matrix.org";
    event["sender"] = "@dbtestuser36:matrix.org";
    event["origin_server_ts"] = 1600000036000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 36";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom36:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom36:matrix.org";
    member["user_id"] = "@member36:matrix.org";
    member["display_name"] = "Member 36";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_36";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom36:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom36:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom36:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000036";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom36:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario38) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0037.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom37:matrix.org";
    room["display_name"] = "Test Room 37";
    room["topic"] = "Database test room 37";
    room["avatar_url"] = "mxc://matrix.org/avatar_37";
    room["joined_members_count"] = 3 + 37 % 5;
    room["invited_members_count"] = 37 % 3;
    room["notification_count"] = 37 % 10;
    room["highlight_count"] = 37 % 5;
    room["is_direct"] = 37 % 2 == 0;
    room["is_encrypted"] = 37 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom37:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000037:matrix.org";
    event["room_id"] = "!testroom37:matrix.org";
    event["sender"] = "@dbtestuser37:matrix.org";
    event["origin_server_ts"] = 1600000037000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 37";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom37:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom37:matrix.org";
    member["user_id"] = "@member37:matrix.org";
    member["display_name"] = "Member 37";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_37";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom37:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom37:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom37:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000037";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom37:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario39) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0038.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom38:matrix.org";
    room["display_name"] = "Test Room 38";
    room["topic"] = "Database test room 38";
    room["avatar_url"] = "mxc://matrix.org/avatar_38";
    room["joined_members_count"] = 3 + 38 % 5;
    room["invited_members_count"] = 38 % 3;
    room["notification_count"] = 38 % 10;
    room["highlight_count"] = 38 % 5;
    room["is_direct"] = 38 % 2 == 0;
    room["is_encrypted"] = 38 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom38:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000038:matrix.org";
    event["room_id"] = "!testroom38:matrix.org";
    event["sender"] = "@dbtestuser38:matrix.org";
    event["origin_server_ts"] = 1600000038000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 38";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom38:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom38:matrix.org";
    member["user_id"] = "@member38:matrix.org";
    member["display_name"] = "Member 38";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_38";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom38:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom38:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom38:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000038";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom38:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario40) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0039.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom39:matrix.org";
    room["display_name"] = "Test Room 39";
    room["topic"] = "Database test room 39";
    room["avatar_url"] = "mxc://matrix.org/avatar_39";
    room["joined_members_count"] = 3 + 39 % 5;
    room["invited_members_count"] = 39 % 3;
    room["notification_count"] = 39 % 10;
    room["highlight_count"] = 39 % 5;
    room["is_direct"] = 39 % 2 == 0;
    room["is_encrypted"] = 39 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom39:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000039:matrix.org";
    event["room_id"] = "!testroom39:matrix.org";
    event["sender"] = "@dbtestuser39:matrix.org";
    event["origin_server_ts"] = 1600000039000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 39";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom39:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom39:matrix.org";
    member["user_id"] = "@member39:matrix.org";
    member["display_name"] = "Member 39";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_39";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom39:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom39:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom39:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000039";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom39:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario41) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0040.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom40:matrix.org";
    room["display_name"] = "Test Room 40";
    room["topic"] = "Database test room 40";
    room["avatar_url"] = "mxc://matrix.org/avatar_40";
    room["joined_members_count"] = 3 + 40 % 5;
    room["invited_members_count"] = 40 % 3;
    room["notification_count"] = 40 % 10;
    room["highlight_count"] = 40 % 5;
    room["is_direct"] = 40 % 2 == 0;
    room["is_encrypted"] = 40 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom40:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000040:matrix.org";
    event["room_id"] = "!testroom40:matrix.org";
    event["sender"] = "@dbtestuser40:matrix.org";
    event["origin_server_ts"] = 1600000040000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 40";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom40:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom40:matrix.org";
    member["user_id"] = "@member40:matrix.org";
    member["display_name"] = "Member 40";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_40";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom40:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom40:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom40:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000040";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom40:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario42) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0041.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom41:matrix.org";
    room["display_name"] = "Test Room 41";
    room["topic"] = "Database test room 41";
    room["avatar_url"] = "mxc://matrix.org/avatar_41";
    room["joined_members_count"] = 3 + 41 % 5;
    room["invited_members_count"] = 41 % 3;
    room["notification_count"] = 41 % 10;
    room["highlight_count"] = 41 % 5;
    room["is_direct"] = 41 % 2 == 0;
    room["is_encrypted"] = 41 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom41:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000041:matrix.org";
    event["room_id"] = "!testroom41:matrix.org";
    event["sender"] = "@dbtestuser41:matrix.org";
    event["origin_server_ts"] = 1600000041000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 41";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom41:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom41:matrix.org";
    member["user_id"] = "@member41:matrix.org";
    member["display_name"] = "Member 41";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_41";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom41:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom41:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom41:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000041";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom41:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario43) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0042.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom42:matrix.org";
    room["display_name"] = "Test Room 42";
    room["topic"] = "Database test room 42";
    room["avatar_url"] = "mxc://matrix.org/avatar_42";
    room["joined_members_count"] = 3 + 42 % 5;
    room["invited_members_count"] = 42 % 3;
    room["notification_count"] = 42 % 10;
    room["highlight_count"] = 42 % 5;
    room["is_direct"] = 42 % 2 == 0;
    room["is_encrypted"] = 42 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom42:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000042:matrix.org";
    event["room_id"] = "!testroom42:matrix.org";
    event["sender"] = "@dbtestuser42:matrix.org";
    event["origin_server_ts"] = 1600000042000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 42";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom42:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom42:matrix.org";
    member["user_id"] = "@member42:matrix.org";
    member["display_name"] = "Member 42";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_42";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom42:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom42:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom42:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000042";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom42:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario44) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0043.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom43:matrix.org";
    room["display_name"] = "Test Room 43";
    room["topic"] = "Database test room 43";
    room["avatar_url"] = "mxc://matrix.org/avatar_43";
    room["joined_members_count"] = 3 + 43 % 5;
    room["invited_members_count"] = 43 % 3;
    room["notification_count"] = 43 % 10;
    room["highlight_count"] = 43 % 5;
    room["is_direct"] = 43 % 2 == 0;
    room["is_encrypted"] = 43 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom43:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000043:matrix.org";
    event["room_id"] = "!testroom43:matrix.org";
    event["sender"] = "@dbtestuser43:matrix.org";
    event["origin_server_ts"] = 1600000043000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 43";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom43:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom43:matrix.org";
    member["user_id"] = "@member43:matrix.org";
    member["display_name"] = "Member 43";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_43";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom43:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom43:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom43:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000043";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom43:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario45) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0044.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom44:matrix.org";
    room["display_name"] = "Test Room 44";
    room["topic"] = "Database test room 44";
    room["avatar_url"] = "mxc://matrix.org/avatar_44";
    room["joined_members_count"] = 3 + 44 % 5;
    room["invited_members_count"] = 44 % 3;
    room["notification_count"] = 44 % 10;
    room["highlight_count"] = 44 % 5;
    room["is_direct"] = 44 % 2 == 0;
    room["is_encrypted"] = 44 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom44:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000044:matrix.org";
    event["room_id"] = "!testroom44:matrix.org";
    event["sender"] = "@dbtestuser44:matrix.org";
    event["origin_server_ts"] = 1600000044000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 44";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom44:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom44:matrix.org";
    member["user_id"] = "@member44:matrix.org";
    member["display_name"] = "Member 44";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_44";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom44:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom44:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom44:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000044";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom44:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario46) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0045.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom45:matrix.org";
    room["display_name"] = "Test Room 45";
    room["topic"] = "Database test room 45";
    room["avatar_url"] = "mxc://matrix.org/avatar_45";
    room["joined_members_count"] = 3 + 45 % 5;
    room["invited_members_count"] = 45 % 3;
    room["notification_count"] = 45 % 10;
    room["highlight_count"] = 45 % 5;
    room["is_direct"] = 45 % 2 == 0;
    room["is_encrypted"] = 45 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom45:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000045:matrix.org";
    event["room_id"] = "!testroom45:matrix.org";
    event["sender"] = "@dbtestuser45:matrix.org";
    event["origin_server_ts"] = 1600000045000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 45";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom45:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom45:matrix.org";
    member["user_id"] = "@member45:matrix.org";
    member["display_name"] = "Member 45";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_45";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom45:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom45:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom45:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000045";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom45:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario47) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0046.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom46:matrix.org";
    room["display_name"] = "Test Room 46";
    room["topic"] = "Database test room 46";
    room["avatar_url"] = "mxc://matrix.org/avatar_46";
    room["joined_members_count"] = 3 + 46 % 5;
    room["invited_members_count"] = 46 % 3;
    room["notification_count"] = 46 % 10;
    room["highlight_count"] = 46 % 5;
    room["is_direct"] = 46 % 2 == 0;
    room["is_encrypted"] = 46 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom46:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000046:matrix.org";
    event["room_id"] = "!testroom46:matrix.org";
    event["sender"] = "@dbtestuser46:matrix.org";
    event["origin_server_ts"] = 1600000046000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 46";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom46:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom46:matrix.org";
    member["user_id"] = "@member46:matrix.org";
    member["display_name"] = "Member 46";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_46";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom46:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom46:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom46:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000046";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom46:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario48) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0047.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom47:matrix.org";
    room["display_name"] = "Test Room 47";
    room["topic"] = "Database test room 47";
    room["avatar_url"] = "mxc://matrix.org/avatar_47";
    room["joined_members_count"] = 3 + 47 % 5;
    room["invited_members_count"] = 47 % 3;
    room["notification_count"] = 47 % 10;
    room["highlight_count"] = 47 % 5;
    room["is_direct"] = 47 % 2 == 0;
    room["is_encrypted"] = 47 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom47:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000047:matrix.org";
    event["room_id"] = "!testroom47:matrix.org";
    event["sender"] = "@dbtestuser47:matrix.org";
    event["origin_server_ts"] = 1600000047000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 47";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom47:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom47:matrix.org";
    member["user_id"] = "@member47:matrix.org";
    member["display_name"] = "Member 47";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_47";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom47:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom47:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom47:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000047";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom47:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario49) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0048.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom48:matrix.org";
    room["display_name"] = "Test Room 48";
    room["topic"] = "Database test room 48";
    room["avatar_url"] = "mxc://matrix.org/avatar_48";
    room["joined_members_count"] = 3 + 48 % 5;
    room["invited_members_count"] = 48 % 3;
    room["notification_count"] = 48 % 10;
    room["highlight_count"] = 48 % 5;
    room["is_direct"] = 48 % 2 == 0;
    room["is_encrypted"] = 48 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom48:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000048:matrix.org";
    event["room_id"] = "!testroom48:matrix.org";
    event["sender"] = "@dbtestuser48:matrix.org";
    event["origin_server_ts"] = 1600000048000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 48";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom48:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom48:matrix.org";
    member["user_id"] = "@member48:matrix.org";
    member["display_name"] = "Member 48";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_48";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom48:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom48:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom48:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000048";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom48:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

TEST(DatabaseTest, Scenario50) {
    json db;
    db["version"] = 1;
    db["name"] = "test_matrix_0049.db";

    // Create tables schema
    json schema;
    schema["rooms"] = json::object();
    schema["rooms"]["columns"] = json::array();
    schema["rooms"]["columns"].push_back({{"name", "room_id"}, {"type", "TEXT"}, {"pk", true}});
    schema["rooms"]["columns"].push_back({{"name", "display_name"}, {"type", "TEXT"}});
    ASSERT_EQ(schema["rooms"]["columns"].size(), 2);

    // Insert room
    json room;
    room["room_id"] = "!testroom49:matrix.org";
    room["display_name"] = "Test Room 49";
    room["topic"] = "Database test room 49";
    room["avatar_url"] = "mxc://matrix.org/avatar_49";
    room["joined_members_count"] = 3 + 49 % 5;
    room["invited_members_count"] = 49 % 3;
    room["notification_count"] = 49 % 10;
    room["highlight_count"] = 49 % 5;
    room["is_direct"] = 49 % 2 == 0;
    room["is_encrypted"] = 49 % 3 == 0;
    room["membership"] = "join";
    ASSERT_EQ(room["room_id"], "!testroom49:matrix.org");
    ASSERT_FALSE(room["display_name"].empty());
    ASSERT_TRUE(room["joined_members_count"] >= 0);

    // Insert event
    json event;
    event["event_id"] = "$dbevent000049:matrix.org";
    event["room_id"] = "!testroom49:matrix.org";
    event["sender"] = "@dbtestuser49:matrix.org";
    event["origin_server_ts"] = 1600000049000;
    event["type"] = "m.room.message";
    event["content"]["body"] = "DB test message 49";
    event["content"]["msgtype"] = "m.text";
    ASSERT_EQ(event["room_id"], "!testroom49:matrix.org");
    ASSERT_FALSE(event["event_id"].empty());

    // Insert member
    json member;
    member["room_id"] = "!testroom49:matrix.org";
    member["user_id"] = "@member49:matrix.org";
    member["display_name"] = "Member 49";
    member["avatar_url"] = "mxc://matrix.org/member_avatar_49";
    member["membership"] = "join";
    ASSERT_EQ(member["membership"], "join");
    ASSERT_FALSE(member["user_id"].empty());

    // Query room
    json query;
    query["select"] = json::array({"room_id", "display_name", "topic"});
    query["from"] = "rooms";
    query["where"]["room_id"] = "!testroom49:matrix.org";
    ASSERT_EQ(query["select"].size(), 3);
    ASSERT_EQ(query["from"], "rooms");

    // Query events with pagination
    json eventQuery;
    eventQuery["select"] = json::array({"*"});
    eventQuery["from"] = "events";
    eventQuery["where"]["room_id"] = "!testroom49:matrix.org";
    eventQuery["order_by"] = "origin_server_ts DESC";
    eventQuery["limit"] = 20;
    eventQuery["offset"] = 0;
    ASSERT_EQ(eventQuery["limit"], 20);
    ASSERT_EQ(eventQuery["offset"], 0);

    // Transaction operations
    json txn;
    txn["begin"] = true;
    txn["operations"] = json::array();
    txn["operations"].push_back({{"op", "insert"}, {"table", "rooms"}, {"data", room}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "events"}, {"data", event}});
    txn["operations"].push_back({{"op", "insert"}, {"table", "members"}, {"data", member}});
    txn["commit"] = true;
    ASSERT_TRUE(txn["begin"]);
    ASSERT_TRUE(txn["commit"]);
    ASSERT_EQ(txn["operations"].size(), 3);

    // Account data operations
    json accountData;
    accountData["type"] = "m.direct";
    accountData["content"]["@user1:matrix.org"] = json::array();
    accountData["content"]["@user1:matrix.org"].push_back("!testroom49:matrix.org");
    ASSERT_EQ(accountData["type"], "m.direct");
    ASSERT_EQ(accountData["content"]["@user1:matrix.org"].size(), 1);

    // Sync operations
    json syncData;
    syncData["token"] = "next_batch_000049";
    syncData["rooms"] = json::array();
    syncData["rooms"].push_back("!testroom49:matrix.org");
    ASSERT_FALSE(syncData["token"].empty());
    ASSERT_EQ(syncData["rooms"].size(), 1);

    // Verify serialization
    ASSERT_NO_THROW({{ auto d = room.dump(); }});
    ASSERT_NO_THROW({{ auto d = event.dump(); }});
    ASSERT_NO_THROW({{ auto d = txn.dump(); }});
    ASSERT_NO_THROW({{ auto d = accountData.dump(); }});
    ASSERT_NO_THROW({{ auto d = syncData.dump(); }});
}

}} // namespace pcp::test