#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BatchGammaTest, Test1) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 0;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_0:matrix.org";
        ev["content"]["body"] = "Batch 2 test 0 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000200000 + e;
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

TEST(BatchGammaTest, Test2) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 1;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_1:matrix.org";
        ev["content"]["body"] = "Batch 2 test 1 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000201000 + e;
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

TEST(BatchGammaTest, Test3) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 2;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_2:matrix.org";
        ev["content"]["body"] = "Batch 2 test 2 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000202000 + e;
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

TEST(BatchGammaTest, Test4) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 3;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_3:matrix.org";
        ev["content"]["body"] = "Batch 2 test 3 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000203000 + e;
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

TEST(BatchGammaTest, Test5) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 4;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_4:matrix.org";
        ev["content"]["body"] = "Batch 2 test 4 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000204000 + e;
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

TEST(BatchGammaTest, Test6) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 5;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_5:matrix.org";
        ev["content"]["body"] = "Batch 2 test 5 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000205000 + e;
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

TEST(BatchGammaTest, Test7) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 6;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_6:matrix.org";
        ev["content"]["body"] = "Batch 2 test 6 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000206000 + e;
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

TEST(BatchGammaTest, Test8) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 7;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_7:matrix.org";
        ev["content"]["body"] = "Batch 2 test 7 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000207000 + e;
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

TEST(BatchGammaTest, Test9) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 8;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_8:matrix.org";
        ev["content"]["body"] = "Batch 2 test 8 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000208000 + e;
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

TEST(BatchGammaTest, Test10) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 9;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_9:matrix.org";
        ev["content"]["body"] = "Batch 2 test 9 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000209000 + e;
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

TEST(BatchGammaTest, Test11) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 10;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_10:matrix.org";
        ev["content"]["body"] = "Batch 2 test 10 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000210000 + e;
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

TEST(BatchGammaTest, Test12) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 11;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_11:matrix.org";
        ev["content"]["body"] = "Batch 2 test 11 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000211000 + e;
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

TEST(BatchGammaTest, Test13) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 12;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_12:matrix.org";
        ev["content"]["body"] = "Batch 2 test 12 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000212000 + e;
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

TEST(BatchGammaTest, Test14) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 13;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_13:matrix.org";
        ev["content"]["body"] = "Batch 2 test 13 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000213000 + e;
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

TEST(BatchGammaTest, Test15) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 14;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_14:matrix.org";
        ev["content"]["body"] = "Batch 2 test 14 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000214000 + e;
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

TEST(BatchGammaTest, Test16) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 15;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_15:matrix.org";
        ev["content"]["body"] = "Batch 2 test 15 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000215000 + e;
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

TEST(BatchGammaTest, Test17) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 16;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_16:matrix.org";
        ev["content"]["body"] = "Batch 2 test 16 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000216000 + e;
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

TEST(BatchGammaTest, Test18) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 17;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_17:matrix.org";
        ev["content"]["body"] = "Batch 2 test 17 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000217000 + e;
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

TEST(BatchGammaTest, Test19) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 18;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_18:matrix.org";
        ev["content"]["body"] = "Batch 2 test 18 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000218000 + e;
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

TEST(BatchGammaTest, Test20) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 19;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_19:matrix.org";
        ev["content"]["body"] = "Batch 2 test 19 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000219000 + e;
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

TEST(BatchGammaTest, Test21) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 20;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_20:matrix.org";
        ev["content"]["body"] = "Batch 2 test 20 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000220000 + e;
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

TEST(BatchGammaTest, Test22) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 21;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_21:matrix.org";
        ev["content"]["body"] = "Batch 2 test 21 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000221000 + e;
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

TEST(BatchGammaTest, Test23) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 22;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_22:matrix.org";
        ev["content"]["body"] = "Batch 2 test 22 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000222000 + e;
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

TEST(BatchGammaTest, Test24) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 23;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_23:matrix.org";
        ev["content"]["body"] = "Batch 2 test 23 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000223000 + e;
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

TEST(BatchGammaTest, Test25) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 24;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_24:matrix.org";
        ev["content"]["body"] = "Batch 2 test 24 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000224000 + e;
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

TEST(BatchGammaTest, Test26) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 25;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_25:matrix.org";
        ev["content"]["body"] = "Batch 2 test 25 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000225000 + e;
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

TEST(BatchGammaTest, Test27) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 26;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_26:matrix.org";
        ev["content"]["body"] = "Batch 2 test 26 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000226000 + e;
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

TEST(BatchGammaTest, Test28) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 27;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_27:matrix.org";
        ev["content"]["body"] = "Batch 2 test 27 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000227000 + e;
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

TEST(BatchGammaTest, Test29) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 28;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_28:matrix.org";
        ev["content"]["body"] = "Batch 2 test 28 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000228000 + e;
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

TEST(BatchGammaTest, Test30) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 29;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_29:matrix.org";
        ev["content"]["body"] = "Batch 2 test 29 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000229000 + e;
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

TEST(BatchGammaTest, Test31) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 30;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_30:matrix.org";
        ev["content"]["body"] = "Batch 2 test 30 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000230000 + e;
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

TEST(BatchGammaTest, Test32) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 31;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_31:matrix.org";
        ev["content"]["body"] = "Batch 2 test 31 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000231000 + e;
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

TEST(BatchGammaTest, Test33) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 32;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_32:matrix.org";
        ev["content"]["body"] = "Batch 2 test 32 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000232000 + e;
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

TEST(BatchGammaTest, Test34) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 33;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_33:matrix.org";
        ev["content"]["body"] = "Batch 2 test 33 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000233000 + e;
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

TEST(BatchGammaTest, Test35) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 34;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_34:matrix.org";
        ev["content"]["body"] = "Batch 2 test 34 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000234000 + e;
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

TEST(BatchGammaTest, Test36) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 35;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_35:matrix.org";
        ev["content"]["body"] = "Batch 2 test 35 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000235000 + e;
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

TEST(BatchGammaTest, Test37) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 36;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_36:matrix.org";
        ev["content"]["body"] = "Batch 2 test 36 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000236000 + e;
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

TEST(BatchGammaTest, Test38) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 37;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_37:matrix.org";
        ev["content"]["body"] = "Batch 2 test 37 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000237000 + e;
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

TEST(BatchGammaTest, Test39) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 38;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_38:matrix.org";
        ev["content"]["body"] = "Batch 2 test 38 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000238000 + e;
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

TEST(BatchGammaTest, Test40) {
    json test;
    test["suite"] = "BatchGamma";
    test["index"] = 39;
    test["batch"] = 2;
    
    for (int e = 0; e < 4; ++e) {
        json ev;
        ev["event_id"] = "$batch" + std::to_string(2) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":matrix.org";
        ev["type"] = "m.room.message";
        ev["sender"] = "@batchuser_2_39:matrix.org";
        ev["content"]["body"] = "Batch 2 test 39 event " + std::to_string(e);
        ev["content"]["msgtype"] = "m.text";
        ev["origin_server_ts"] = 1600000239000 + e;
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
