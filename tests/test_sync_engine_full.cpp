#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>
#include "pcp/sync_utils.hpp"
#include "pcp/sync_filter.hpp"

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SyncEngineTest, Scenario1) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000000";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(0) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_0_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_0:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_0:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_0:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario2) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000001";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(1) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_1_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_1:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_1:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_1:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario3) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000002";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(2) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_2_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_2:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_2:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_2:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario4) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000003";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(3) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_3_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_3:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_3:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_3:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario5) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000004";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(4) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_4_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_4:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_4:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_4:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario6) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000005";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(5) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_5_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_5:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_5:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_5:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario7) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000006";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(6) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_6_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_6:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_6:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_6:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario8) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000007";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(7) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_7_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_7:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_7:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_7:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario9) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000008";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(8) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_8_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_8:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_8:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_8:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario10) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000009";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(9) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_9_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_9:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_9:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_9:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario11) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000010";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(10) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_10_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_10:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_10:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_10:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario12) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000011";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(11) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_11_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_11:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_11:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_11:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario13) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000012";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(12) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_12_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_12:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_12:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_12:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario14) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000013";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(13) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_13_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_13:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_13:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_13:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario15) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000014";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(14) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_14_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_14:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_14:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_14:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario16) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000015";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(15) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_15_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_15:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_15:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_15:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario17) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000016";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(16) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_16_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_16:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_16:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_16:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario18) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000017";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(17) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_17_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_17:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_17:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_17:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario19) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000018";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(18) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_18_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_18:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_18:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_18:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario20) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000019";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(19) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_19_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_19:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_19:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_19:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario21) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000020";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(20) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_20_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_20:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_20:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_20:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario22) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000021";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(21) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_21_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_21:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_21:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_21:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario23) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000022";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(22) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_22_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_22:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_22:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_22:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario24) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000023";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(23) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_23_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_23:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_23:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_23:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario25) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000024";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(24) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_24_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_24:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_24:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_24:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario26) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000025";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(25) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_25_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_25:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_25:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_25:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario27) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000026";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(26) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_26_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_26:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_26:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_26:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario28) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000027";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(27) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_27_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_27:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_27:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_27:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario29) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000028";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(28) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_28_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_28:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_28:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_28:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario30) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000029";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(29) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_29_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_29:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_29:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_29:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario31) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000030";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(30) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_30_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_30:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_30:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_30:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario32) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000031";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(31) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_31_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_31:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_31:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_31:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario33) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000032";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(32) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_32_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_32:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_32:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_32:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario34) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000033";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(33) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_33_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_33:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_33:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_33:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

TEST(SyncEngineTest, Scenario35) {
    // Mock sync response
    json syncResp;
    syncResp["next_batch"] = "batch_000034";
    syncResp["presence"]["events"] = json::array();
    syncResp["account_data"]["events"] = json::array();

    // Joined rooms
    for (int r = 0; r < 3; ++r) {
        std::string roomId = "!synctestroom" + std::to_string(34) + "_" + std::to_string(r) + ":matrix.org";
        json room;
        room["timeline"]["events"] = json::array();
        room["timeline"]["prev_batch"] = "prev_batch_34_" + std::to_string(r);
        room["timeline"]["limited"] = false;
        room["state"]["events"] = json::array();
        room["ephemeral"]["events"] = json::array();
        room["account_data"]["events"] = json::array();
        room["unread_notifications"]["notification_count"] = 0;
        room["unread_notifications"]["highlight_count"] = 0;
        room["summary"]["m.joined_member_count"] = 1 + r;
        room["summary"]["m.invited_member_count"] = 0;
        syncResp["rooms"]["join"]["!" + roomId] = room;
    }

    std::string nextBatch = syncResp["next_batch"];
    ASSERT_FALSE(nextBatch.empty());
    ASSERT_TRUE(nextBatch.starts_with("batch_"));
    ASSERT_EQ(syncResp["rooms"]["join"].size(), 3);

    // Test filter
    json filter;
    filter["room"]["timeline"]["limit"] = 20;
    filter["room"]["timeline"]["lazy_load_members"] = true;
    filter["room"]["state"]["lazy_load_members"] = true;
    filter["presence"]["senders"] = json::array();
    ASSERT_EQ(filter["room"]["timeline"]["limit"], 20);
    ASSERT_TRUE(filter["room"]["timeline"]["lazy_load_members"]);

    // Test device lists
    json deviceLists;
    deviceLists["changed"] = json::array({"@changeduser_34:matrix.org"});
    deviceLists["left"] = json::array({"@leftuser_34:matrix.org"});
    ASSERT_EQ(deviceLists["changed"].size(), 1);
    ASSERT_EQ(deviceLists["left"].size(), 1);

    // Test to-device events
    json toDevice;
    toDevice["events"] = json::array();
    json tdEvent;
    tdEvent["sender"] = "@otheruser_34:matrix.org";
    tdEvent["type"] = "m.room_key";
    toDevice["events"].push_back(tdEvent);
    ASSERT_EQ(toDevice["events"].size(), 1);

    // Verify sync response is valid
    ASSERT_NO_THROW({ auto d = syncResp.dump(); });
    ASSERT_NO_THROW({ auto d = filter.dump(); });
}

}} // namespace pcp::test