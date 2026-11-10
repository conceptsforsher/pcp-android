#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BatchLambdaTest, Test1) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 0;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_0:matrix.org";
        ev["content"]["body"] = "Batch 10 test 0 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001000000 + e;
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

TEST(BatchLambdaTest, Test2) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 1;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_1:matrix.org";
        ev["content"]["body"] = "Batch 10 test 1 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001001000 + e;
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

TEST(BatchLambdaTest, Test3) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 2;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_2:matrix.org";
        ev["content"]["body"] = "Batch 10 test 2 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001002000 + e;
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

TEST(BatchLambdaTest, Test4) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 3;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_3:matrix.org";
        ev["content"]["body"] = "Batch 10 test 3 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001003000 + e;
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

TEST(BatchLambdaTest, Test5) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 4;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_4:matrix.org";
        ev["content"]["body"] = "Batch 10 test 4 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001004000 + e;
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

TEST(BatchLambdaTest, Test6) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 5;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_5:matrix.org";
        ev["content"]["body"] = "Batch 10 test 5 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001005000 + e;
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

TEST(BatchLambdaTest, Test7) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 6;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_6:matrix.org";
        ev["content"]["body"] = "Batch 10 test 6 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001006000 + e;
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

TEST(BatchLambdaTest, Test8) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 7;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_7:matrix.org";
        ev["content"]["body"] = "Batch 10 test 7 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001007000 + e;
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

TEST(BatchLambdaTest, Test9) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 8;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_8:matrix.org";
        ev["content"]["body"] = "Batch 10 test 8 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001008000 + e;
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

TEST(BatchLambdaTest, Test10) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 9;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_9:matrix.org";
        ev["content"]["body"] = "Batch 10 test 9 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001009000 + e;
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

TEST(BatchLambdaTest, Test11) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 10;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_10:matrix.org";
        ev["content"]["body"] = "Batch 10 test 10 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001010000 + e;
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

TEST(BatchLambdaTest, Test12) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 11;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_11:matrix.org";
        ev["content"]["body"] = "Batch 10 test 11 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001011000 + e;
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

TEST(BatchLambdaTest, Test13) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 12;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_12:matrix.org";
        ev["content"]["body"] = "Batch 10 test 12 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001012000 + e;
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

TEST(BatchLambdaTest, Test14) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 13;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_13:matrix.org";
        ev["content"]["body"] = "Batch 10 test 13 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001013000 + e;
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

TEST(BatchLambdaTest, Test15) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 14;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_14:matrix.org";
        ev["content"]["body"] = "Batch 10 test 14 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001014000 + e;
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

TEST(BatchLambdaTest, Test16) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 15;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_15:matrix.org";
        ev["content"]["body"] = "Batch 10 test 15 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001015000 + e;
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

TEST(BatchLambdaTest, Test17) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 16;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_16:matrix.org";
        ev["content"]["body"] = "Batch 10 test 16 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001016000 + e;
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

TEST(BatchLambdaTest, Test18) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 17;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_17:matrix.org";
        ev["content"]["body"] = "Batch 10 test 17 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001017000 + e;
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

TEST(BatchLambdaTest, Test19) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 18;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_18:matrix.org";
        ev["content"]["body"] = "Batch 10 test 18 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001018000 + e;
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

TEST(BatchLambdaTest, Test20) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 19;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_19:matrix.org";
        ev["content"]["body"] = "Batch 10 test 19 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001019000 + e;
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

TEST(BatchLambdaTest, Test21) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 20;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_20:matrix.org";
        ev["content"]["body"] = "Batch 10 test 20 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001020000 + e;
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

TEST(BatchLambdaTest, Test22) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 21;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_21:matrix.org";
        ev["content"]["body"] = "Batch 10 test 21 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001021000 + e;
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

TEST(BatchLambdaTest, Test23) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 22;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_22:matrix.org";
        ev["content"]["body"] = "Batch 10 test 22 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001022000 + e;
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

TEST(BatchLambdaTest, Test24) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 23;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_23:matrix.org";
        ev["content"]["body"] = "Batch 10 test 23 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001023000 + e;
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

TEST(BatchLambdaTest, Test25) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 24;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_24:matrix.org";
        ev["content"]["body"] = "Batch 10 test 24 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001024000 + e;
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

TEST(BatchLambdaTest, Test26) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 25;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_25:matrix.org";
        ev["content"]["body"] = "Batch 10 test 25 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001025000 + e;
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

TEST(BatchLambdaTest, Test27) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 26;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_26:matrix.org";
        ev["content"]["body"] = "Batch 10 test 26 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001026000 + e;
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

TEST(BatchLambdaTest, Test28) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 27;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_27:matrix.org";
        ev["content"]["body"] = "Batch 10 test 27 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001027000 + e;
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

TEST(BatchLambdaTest, Test29) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 28;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_28:matrix.org";
        ev["content"]["body"] = "Batch 10 test 28 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001028000 + e;
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

TEST(BatchLambdaTest, Test30) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 29;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_29:matrix.org";
        ev["content"]["body"] = "Batch 10 test 29 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001029000 + e;
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

TEST(BatchLambdaTest, Test31) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 30;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_30:matrix.org";
        ev["content"]["body"] = "Batch 10 test 30 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001030000 + e;
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

TEST(BatchLambdaTest, Test32) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 31;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_31:matrix.org";
        ev["content"]["body"] = "Batch 10 test 31 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001031000 + e;
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

TEST(BatchLambdaTest, Test33) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 32;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_32:matrix.org";
        ev["content"]["body"] = "Batch 10 test 32 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001032000 + e;
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

TEST(BatchLambdaTest, Test34) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 33;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_33:matrix.org";
        ev["content"]["body"] = "Batch 10 test 33 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001033000 + e;
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

TEST(BatchLambdaTest, Test35) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 34;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_34:matrix.org";
        ev["content"]["body"] = "Batch 10 test 34 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001034000 + e;
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

TEST(BatchLambdaTest, Test36) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 35;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_35:matrix.org";
        ev["content"]["body"] = "Batch 10 test 35 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001035000 + e;
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

TEST(BatchLambdaTest, Test37) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 36;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_36:matrix.org";
        ev["content"]["body"] = "Batch 10 test 36 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001036000 + e;
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

TEST(BatchLambdaTest, Test38) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 37;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_37:matrix.org";
        ev["content"]["body"] = "Batch 10 test 37 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001037000 + e;
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

TEST(BatchLambdaTest, Test39) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 38;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_38:matrix.org";
        ev["content"]["body"] = "Batch 10 test 38 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001038000 + e;
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

TEST(BatchLambdaTest, Test40) {
    json test;
    test["suite"] = "BatchLambda";
    test["index"] = 39;
    test["batch"] = 10;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(10) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_10_39:matrix.org";
        ev["content"]["body"] = "Batch 10 test 39 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600001039000 + e;
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
