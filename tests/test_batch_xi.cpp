#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BatchXiTest, Test1) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 0;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_0:matrix.org";
        ev["content"]["body"] = "Batch 13 test 0 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001300000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test2) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 1;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_1:matrix.org";
        ev["content"]["body"] = "Batch 13 test 1 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001301000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test3) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 2;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_2:matrix.org";
        ev["content"]["body"] = "Batch 13 test 2 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001302000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test4) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 3;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_3:matrix.org";
        ev["content"]["body"] = "Batch 13 test 3 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001303000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test5) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 4;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_4:matrix.org";
        ev["content"]["body"] = "Batch 13 test 4 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001304000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test6) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 5;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_5:matrix.org";
        ev["content"]["body"] = "Batch 13 test 5 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001305000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test7) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 6;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_6:matrix.org";
        ev["content"]["body"] = "Batch 13 test 6 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001306000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test8) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 7;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_7:matrix.org";
        ev["content"]["body"] = "Batch 13 test 7 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001307000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test9) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 8;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_8:matrix.org";
        ev["content"]["body"] = "Batch 13 test 8 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001308000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test10) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 9;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_9:matrix.org";
        ev["content"]["body"] = "Batch 13 test 9 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001309000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test11) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 10;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_10:matrix.org";
        ev["content"]["body"] = "Batch 13 test 10 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001310000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test12) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 11;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_11:matrix.org";
        ev["content"]["body"] = "Batch 13 test 11 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001311000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test13) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 12;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_12:matrix.org";
        ev["content"]["body"] = "Batch 13 test 12 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001312000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test14) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 13;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_13:matrix.org";
        ev["content"]["body"] = "Batch 13 test 13 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001313000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test15) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 14;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_14:matrix.org";
        ev["content"]["body"] = "Batch 13 test 14 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001314000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test16) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 15;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_15:matrix.org";
        ev["content"]["body"] = "Batch 13 test 15 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001315000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test17) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 16;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_16:matrix.org";
        ev["content"]["body"] = "Batch 13 test 16 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001316000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test18) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 17;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_17:matrix.org";
        ev["content"]["body"] = "Batch 13 test 17 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001317000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test19) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 18;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_18:matrix.org";
        ev["content"]["body"] = "Batch 13 test 18 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001318000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test20) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 19;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_19:matrix.org";
        ev["content"]["body"] = "Batch 13 test 19 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001319000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test21) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 20;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_20:matrix.org";
        ev["content"]["body"] = "Batch 13 test 20 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001320000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test22) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 21;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_21:matrix.org";
        ev["content"]["body"] = "Batch 13 test 21 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001321000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test23) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 22;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_22:matrix.org";
        ev["content"]["body"] = "Batch 13 test 22 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001322000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test24) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 23;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_23:matrix.org";
        ev["content"]["body"] = "Batch 13 test 23 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001323000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test25) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 24;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_24:matrix.org";
        ev["content"]["body"] = "Batch 13 test 24 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001324000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test26) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 25;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_25:matrix.org";
        ev["content"]["body"] = "Batch 13 test 25 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001325000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test27) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 26;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_26:matrix.org";
        ev["content"]["body"] = "Batch 13 test 26 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001326000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test28) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 27;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_27:matrix.org";
        ev["content"]["body"] = "Batch 13 test 27 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001327000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test29) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 28;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_28:matrix.org";
        ev["content"]["body"] = "Batch 13 test 28 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001328000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test30) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 29;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_29:matrix.org";
        ev["content"]["body"] = "Batch 13 test 29 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001329000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test31) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 30;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_30:matrix.org";
        ev["content"]["body"] = "Batch 13 test 30 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001330000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test32) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 31;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_31:matrix.org";
        ev["content"]["body"] = "Batch 13 test 31 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001331000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test33) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 32;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_32:matrix.org";
        ev["content"]["body"] = "Batch 13 test 32 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001332000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test34) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 33;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_33:matrix.org";
        ev["content"]["body"] = "Batch 13 test 33 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001333000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test35) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 34;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_34:matrix.org";
        ev["content"]["body"] = "Batch 13 test 34 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001334000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test36) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 35;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_35:matrix.org";
        ev["content"]["body"] = "Batch 13 test 35 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001335000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test37) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 36;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_36:matrix.org";
        ev["content"]["body"] = "Batch 13 test 36 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001336000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test38) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 37;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_37:matrix.org";
        ev["content"]["body"] = "Batch 13 test 37 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001337000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test39) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 38;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_38:matrix.org";
        ev["content"]["body"] = "Batch 13 test 38 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001338000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = true;
    test["state"]["encrypted"] = false;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

TEST(BatchXiTest, Test40) {
    json test;
    test["suite"] = "BatchXi";
    test["index"] = 39;
    test["batch"] = 13;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(13) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_13_39:matrix.org";
        ev["content"]["body"] = "Batch 13 test 39 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001339000 + e;
        test["events"].push_back(ev);
        ASSERT_FALSE(ev["event_id"].empty());
    }
    ASSERT_EQ(test["events"].size(), 4);
    
    test["state"]["initialized"] = true;
    test["state"]["synced"] = false;
    test["state"]["encrypted"] = true;
    ASSERT_TRUE(test["state"]["initialized"]);
    
    std::string jsonStr = test.dump();
    ASSERT_FALSE(jsonStr.empty());
    json parsed = json::parse(jsonStr);
    ASSERT_EQ(parsed["suite"], test["suite"]);
    ASSERT_EQ(parsed["events"].size(), 4);
}

}} // namespace pcp::test
