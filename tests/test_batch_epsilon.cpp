#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BatchEpsilonTest, Test1) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 0;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_0:matrix.org";
        ev["content"]["body"] = "Batch 4 test 0 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000400000 + e;
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

TEST(BatchEpsilonTest, Test2) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 1;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_1:matrix.org";
        ev["content"]["body"] = "Batch 4 test 1 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000401000 + e;
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

TEST(BatchEpsilonTest, Test3) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 2;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_2:matrix.org";
        ev["content"]["body"] = "Batch 4 test 2 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000402000 + e;
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

TEST(BatchEpsilonTest, Test4) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 3;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_3:matrix.org";
        ev["content"]["body"] = "Batch 4 test 3 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000403000 + e;
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

TEST(BatchEpsilonTest, Test5) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 4;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_4:matrix.org";
        ev["content"]["body"] = "Batch 4 test 4 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000404000 + e;
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

TEST(BatchEpsilonTest, Test6) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 5;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_5:matrix.org";
        ev["content"]["body"] = "Batch 4 test 5 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000405000 + e;
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

TEST(BatchEpsilonTest, Test7) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 6;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_6:matrix.org";
        ev["content"]["body"] = "Batch 4 test 6 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000406000 + e;
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

TEST(BatchEpsilonTest, Test8) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 7;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_7:matrix.org";
        ev["content"]["body"] = "Batch 4 test 7 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000407000 + e;
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

TEST(BatchEpsilonTest, Test9) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 8;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_8:matrix.org";
        ev["content"]["body"] = "Batch 4 test 8 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000408000 + e;
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

TEST(BatchEpsilonTest, Test10) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 9;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_9:matrix.org";
        ev["content"]["body"] = "Batch 4 test 9 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000409000 + e;
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

TEST(BatchEpsilonTest, Test11) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 10;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_10:matrix.org";
        ev["content"]["body"] = "Batch 4 test 10 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000410000 + e;
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

TEST(BatchEpsilonTest, Test12) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 11;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_11:matrix.org";
        ev["content"]["body"] = "Batch 4 test 11 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000411000 + e;
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

TEST(BatchEpsilonTest, Test13) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 12;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_12:matrix.org";
        ev["content"]["body"] = "Batch 4 test 12 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000412000 + e;
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

TEST(BatchEpsilonTest, Test14) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 13;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_13:matrix.org";
        ev["content"]["body"] = "Batch 4 test 13 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000413000 + e;
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

TEST(BatchEpsilonTest, Test15) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 14;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_14:matrix.org";
        ev["content"]["body"] = "Batch 4 test 14 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000414000 + e;
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

TEST(BatchEpsilonTest, Test16) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 15;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_15:matrix.org";
        ev["content"]["body"] = "Batch 4 test 15 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000415000 + e;
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

TEST(BatchEpsilonTest, Test17) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 16;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_16:matrix.org";
        ev["content"]["body"] = "Batch 4 test 16 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000416000 + e;
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

TEST(BatchEpsilonTest, Test18) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 17;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_17:matrix.org";
        ev["content"]["body"] = "Batch 4 test 17 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000417000 + e;
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

TEST(BatchEpsilonTest, Test19) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 18;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_18:matrix.org";
        ev["content"]["body"] = "Batch 4 test 18 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000418000 + e;
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

TEST(BatchEpsilonTest, Test20) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 19;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_19:matrix.org";
        ev["content"]["body"] = "Batch 4 test 19 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000419000 + e;
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

TEST(BatchEpsilonTest, Test21) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 20;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_20:matrix.org";
        ev["content"]["body"] = "Batch 4 test 20 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000420000 + e;
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

TEST(BatchEpsilonTest, Test22) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 21;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_21:matrix.org";
        ev["content"]["body"] = "Batch 4 test 21 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000421000 + e;
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

TEST(BatchEpsilonTest, Test23) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 22;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_22:matrix.org";
        ev["content"]["body"] = "Batch 4 test 22 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000422000 + e;
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

TEST(BatchEpsilonTest, Test24) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 23;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_23:matrix.org";
        ev["content"]["body"] = "Batch 4 test 23 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000423000 + e;
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

TEST(BatchEpsilonTest, Test25) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 24;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_24:matrix.org";
        ev["content"]["body"] = "Batch 4 test 24 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000424000 + e;
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

TEST(BatchEpsilonTest, Test26) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 25;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_25:matrix.org";
        ev["content"]["body"] = "Batch 4 test 25 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000425000 + e;
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

TEST(BatchEpsilonTest, Test27) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 26;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_26:matrix.org";
        ev["content"]["body"] = "Batch 4 test 26 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000426000 + e;
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

TEST(BatchEpsilonTest, Test28) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 27;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_27:matrix.org";
        ev["content"]["body"] = "Batch 4 test 27 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000427000 + e;
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

TEST(BatchEpsilonTest, Test29) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 28;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_28:matrix.org";
        ev["content"]["body"] = "Batch 4 test 28 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000428000 + e;
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

TEST(BatchEpsilonTest, Test30) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 29;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_29:matrix.org";
        ev["content"]["body"] = "Batch 4 test 29 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000429000 + e;
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

TEST(BatchEpsilonTest, Test31) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 30;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_30:matrix.org";
        ev["content"]["body"] = "Batch 4 test 30 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000430000 + e;
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

TEST(BatchEpsilonTest, Test32) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 31;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_31:matrix.org";
        ev["content"]["body"] = "Batch 4 test 31 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000431000 + e;
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

TEST(BatchEpsilonTest, Test33) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 32;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_32:matrix.org";
        ev["content"]["body"] = "Batch 4 test 32 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000432000 + e;
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

TEST(BatchEpsilonTest, Test34) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 33;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_33:matrix.org";
        ev["content"]["body"] = "Batch 4 test 33 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000433000 + e;
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

TEST(BatchEpsilonTest, Test35) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 34;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_34:matrix.org";
        ev["content"]["body"] = "Batch 4 test 34 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000434000 + e;
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

TEST(BatchEpsilonTest, Test36) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 35;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_35:matrix.org";
        ev["content"]["body"] = "Batch 4 test 35 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000435000 + e;
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

TEST(BatchEpsilonTest, Test37) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 36;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_36:matrix.org";
        ev["content"]["body"] = "Batch 4 test 36 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000436000 + e;
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

TEST(BatchEpsilonTest, Test38) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 37;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_37:matrix.org";
        ev["content"]["body"] = "Batch 4 test 37 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000437000 + e;
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

TEST(BatchEpsilonTest, Test39) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 38;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_38:matrix.org";
        ev["content"]["body"] = "Batch 4 test 38 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000438000 + e;
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

TEST(BatchEpsilonTest, Test40) {
    json test;
    test["suite"] = "BatchEpsilon";
    test["index"] = 39;
    test["batch"] = 4;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(4) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_4_39:matrix.org";
        ev["content"]["body"] = "Batch 4 test 39 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000439000 + e;
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
