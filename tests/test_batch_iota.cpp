#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BatchIotaTest, Test1) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 0;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_0:matrix.org";
        ev["content"]["body"] = "Batch 8 test 0 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000800000 + e;
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

TEST(BatchIotaTest, Test2) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 1;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_1:matrix.org";
        ev["content"]["body"] = "Batch 8 test 1 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000801000 + e;
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

TEST(BatchIotaTest, Test3) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 2;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_2:matrix.org";
        ev["content"]["body"] = "Batch 8 test 2 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000802000 + e;
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

TEST(BatchIotaTest, Test4) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 3;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_3:matrix.org";
        ev["content"]["body"] = "Batch 8 test 3 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000803000 + e;
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

TEST(BatchIotaTest, Test5) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 4;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_4:matrix.org";
        ev["content"]["body"] = "Batch 8 test 4 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000804000 + e;
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

TEST(BatchIotaTest, Test6) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 5;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_5:matrix.org";
        ev["content"]["body"] = "Batch 8 test 5 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000805000 + e;
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

TEST(BatchIotaTest, Test7) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 6;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_6:matrix.org";
        ev["content"]["body"] = "Batch 8 test 6 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000806000 + e;
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

TEST(BatchIotaTest, Test8) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 7;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_7:matrix.org";
        ev["content"]["body"] = "Batch 8 test 7 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000807000 + e;
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

TEST(BatchIotaTest, Test9) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 8;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_8:matrix.org";
        ev["content"]["body"] = "Batch 8 test 8 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000808000 + e;
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

TEST(BatchIotaTest, Test10) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 9;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_9:matrix.org";
        ev["content"]["body"] = "Batch 8 test 9 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000809000 + e;
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

TEST(BatchIotaTest, Test11) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 10;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_10:matrix.org";
        ev["content"]["body"] = "Batch 8 test 10 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000810000 + e;
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

TEST(BatchIotaTest, Test12) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 11;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_11:matrix.org";
        ev["content"]["body"] = "Batch 8 test 11 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000811000 + e;
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

TEST(BatchIotaTest, Test13) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 12;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_12:matrix.org";
        ev["content"]["body"] = "Batch 8 test 12 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000812000 + e;
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

TEST(BatchIotaTest, Test14) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 13;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_13:matrix.org";
        ev["content"]["body"] = "Batch 8 test 13 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000813000 + e;
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

TEST(BatchIotaTest, Test15) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 14;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_14:matrix.org";
        ev["content"]["body"] = "Batch 8 test 14 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000814000 + e;
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

TEST(BatchIotaTest, Test16) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 15;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_15:matrix.org";
        ev["content"]["body"] = "Batch 8 test 15 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000815000 + e;
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

TEST(BatchIotaTest, Test17) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 16;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_16:matrix.org";
        ev["content"]["body"] = "Batch 8 test 16 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000816000 + e;
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

TEST(BatchIotaTest, Test18) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 17;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_17:matrix.org";
        ev["content"]["body"] = "Batch 8 test 17 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000817000 + e;
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

TEST(BatchIotaTest, Test19) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 18;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_18:matrix.org";
        ev["content"]["body"] = "Batch 8 test 18 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000818000 + e;
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

TEST(BatchIotaTest, Test20) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 19;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_19:matrix.org";
        ev["content"]["body"] = "Batch 8 test 19 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000819000 + e;
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

TEST(BatchIotaTest, Test21) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 20;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_20:matrix.org";
        ev["content"]["body"] = "Batch 8 test 20 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000820000 + e;
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

TEST(BatchIotaTest, Test22) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 21;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_21:matrix.org";
        ev["content"]["body"] = "Batch 8 test 21 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000821000 + e;
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

TEST(BatchIotaTest, Test23) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 22;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_22:matrix.org";
        ev["content"]["body"] = "Batch 8 test 22 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000822000 + e;
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

TEST(BatchIotaTest, Test24) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 23;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_23:matrix.org";
        ev["content"]["body"] = "Batch 8 test 23 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000823000 + e;
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

TEST(BatchIotaTest, Test25) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 24;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_24:matrix.org";
        ev["content"]["body"] = "Batch 8 test 24 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000824000 + e;
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

TEST(BatchIotaTest, Test26) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 25;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_25:matrix.org";
        ev["content"]["body"] = "Batch 8 test 25 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000825000 + e;
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

TEST(BatchIotaTest, Test27) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 26;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_26:matrix.org";
        ev["content"]["body"] = "Batch 8 test 26 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000826000 + e;
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

TEST(BatchIotaTest, Test28) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 27;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_27:matrix.org";
        ev["content"]["body"] = "Batch 8 test 27 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000827000 + e;
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

TEST(BatchIotaTest, Test29) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 28;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_28:matrix.org";
        ev["content"]["body"] = "Batch 8 test 28 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000828000 + e;
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

TEST(BatchIotaTest, Test30) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 29;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_29:matrix.org";
        ev["content"]["body"] = "Batch 8 test 29 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000829000 + e;
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

TEST(BatchIotaTest, Test31) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 30;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_30:matrix.org";
        ev["content"]["body"] = "Batch 8 test 30 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000830000 + e;
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

TEST(BatchIotaTest, Test32) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 31;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_31:matrix.org";
        ev["content"]["body"] = "Batch 8 test 31 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000831000 + e;
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

TEST(BatchIotaTest, Test33) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 32;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_32:matrix.org";
        ev["content"]["body"] = "Batch 8 test 32 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000832000 + e;
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

TEST(BatchIotaTest, Test34) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 33;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_33:matrix.org";
        ev["content"]["body"] = "Batch 8 test 33 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000833000 + e;
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

TEST(BatchIotaTest, Test35) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 34;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_34:matrix.org";
        ev["content"]["body"] = "Batch 8 test 34 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000834000 + e;
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

TEST(BatchIotaTest, Test36) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 35;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_35:matrix.org";
        ev["content"]["body"] = "Batch 8 test 35 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000835000 + e;
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

TEST(BatchIotaTest, Test37) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 36;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_36:matrix.org";
        ev["content"]["body"] = "Batch 8 test 36 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000836000 + e;
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

TEST(BatchIotaTest, Test38) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 37;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_37:matrix.org";
        ev["content"]["body"] = "Batch 8 test 37 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000837000 + e;
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

TEST(BatchIotaTest, Test39) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 38;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_38:matrix.org";
        ev["content"]["body"] = "Batch 8 test 38 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000838000 + e;
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

TEST(BatchIotaTest, Test40) {
    json test;
    test["suite"] = "BatchIota";
    test["index"] = 39;
    test["batch"] = 8;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(8) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_8_39:matrix.org";
        ev["content"]["body"] = "Batch 8 test 39 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000839000 + e;
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
