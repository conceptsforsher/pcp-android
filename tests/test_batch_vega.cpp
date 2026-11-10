#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BVegaTest, T1) {
    json t;
    t["s"] = "BVega";
    t["n"] = 0;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_0:m.org";
        ev["c"]["b"] = "B 10 t 0 e " + std::to_string(e);
        ev["ts"] = 1600001000000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T2) {
    json t;
    t["s"] = "BVega";
    t["n"] = 1;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_1:m.org";
        ev["c"]["b"] = "B 10 t 1 e " + std::to_string(e);
        ev["ts"] = 1600001001000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T3) {
    json t;
    t["s"] = "BVega";
    t["n"] = 2;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_2:m.org";
        ev["c"]["b"] = "B 10 t 2 e " + std::to_string(e);
        ev["ts"] = 1600001002000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T4) {
    json t;
    t["s"] = "BVega";
    t["n"] = 3;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_3:m.org";
        ev["c"]["b"] = "B 10 t 3 e " + std::to_string(e);
        ev["ts"] = 1600001003000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T5) {
    json t;
    t["s"] = "BVega";
    t["n"] = 4;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_4:m.org";
        ev["c"]["b"] = "B 10 t 4 e " + std::to_string(e);
        ev["ts"] = 1600001004000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T6) {
    json t;
    t["s"] = "BVega";
    t["n"] = 5;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_5:m.org";
        ev["c"]["b"] = "B 10 t 5 e " + std::to_string(e);
        ev["ts"] = 1600001005000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T7) {
    json t;
    t["s"] = "BVega";
    t["n"] = 6;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_6:m.org";
        ev["c"]["b"] = "B 10 t 6 e " + std::to_string(e);
        ev["ts"] = 1600001006000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T8) {
    json t;
    t["s"] = "BVega";
    t["n"] = 7;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_7:m.org";
        ev["c"]["b"] = "B 10 t 7 e " + std::to_string(e);
        ev["ts"] = 1600001007000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T9) {
    json t;
    t["s"] = "BVega";
    t["n"] = 8;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_8:m.org";
        ev["c"]["b"] = "B 10 t 8 e " + std::to_string(e);
        ev["ts"] = 1600001008000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T10) {
    json t;
    t["s"] = "BVega";
    t["n"] = 9;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_9:m.org";
        ev["c"]["b"] = "B 10 t 9 e " + std::to_string(e);
        ev["ts"] = 1600001009000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T11) {
    json t;
    t["s"] = "BVega";
    t["n"] = 10;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_10:m.org";
        ev["c"]["b"] = "B 10 t 10 e " + std::to_string(e);
        ev["ts"] = 1600001010000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T12) {
    json t;
    t["s"] = "BVega";
    t["n"] = 11;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_11:m.org";
        ev["c"]["b"] = "B 10 t 11 e " + std::to_string(e);
        ev["ts"] = 1600001011000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T13) {
    json t;
    t["s"] = "BVega";
    t["n"] = 12;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_12:m.org";
        ev["c"]["b"] = "B 10 t 12 e " + std::to_string(e);
        ev["ts"] = 1600001012000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T14) {
    json t;
    t["s"] = "BVega";
    t["n"] = 13;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_13:m.org";
        ev["c"]["b"] = "B 10 t 13 e " + std::to_string(e);
        ev["ts"] = 1600001013000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T15) {
    json t;
    t["s"] = "BVega";
    t["n"] = 14;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_14:m.org";
        ev["c"]["b"] = "B 10 t 14 e " + std::to_string(e);
        ev["ts"] = 1600001014000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T16) {
    json t;
    t["s"] = "BVega";
    t["n"] = 15;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_15:m.org";
        ev["c"]["b"] = "B 10 t 15 e " + std::to_string(e);
        ev["ts"] = 1600001015000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T17) {
    json t;
    t["s"] = "BVega";
    t["n"] = 16;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_16:m.org";
        ev["c"]["b"] = "B 10 t 16 e " + std::to_string(e);
        ev["ts"] = 1600001016000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T18) {
    json t;
    t["s"] = "BVega";
    t["n"] = 17;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_17:m.org";
        ev["c"]["b"] = "B 10 t 17 e " + std::to_string(e);
        ev["ts"] = 1600001017000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T19) {
    json t;
    t["s"] = "BVega";
    t["n"] = 18;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_18:m.org";
        ev["c"]["b"] = "B 10 t 18 e " + std::to_string(e);
        ev["ts"] = 1600001018000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T20) {
    json t;
    t["s"] = "BVega";
    t["n"] = 19;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_19:m.org";
        ev["c"]["b"] = "B 10 t 19 e " + std::to_string(e);
        ev["ts"] = 1600001019000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T21) {
    json t;
    t["s"] = "BVega";
    t["n"] = 20;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_20:m.org";
        ev["c"]["b"] = "B 10 t 20 e " + std::to_string(e);
        ev["ts"] = 1600001020000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T22) {
    json t;
    t["s"] = "BVega";
    t["n"] = 21;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_21:m.org";
        ev["c"]["b"] = "B 10 t 21 e " + std::to_string(e);
        ev["ts"] = 1600001021000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T23) {
    json t;
    t["s"] = "BVega";
    t["n"] = 22;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_22:m.org";
        ev["c"]["b"] = "B 10 t 22 e " + std::to_string(e);
        ev["ts"] = 1600001022000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T24) {
    json t;
    t["s"] = "BVega";
    t["n"] = 23;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_23:m.org";
        ev["c"]["b"] = "B 10 t 23 e " + std::to_string(e);
        ev["ts"] = 1600001023000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T25) {
    json t;
    t["s"] = "BVega";
    t["n"] = 24;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_24:m.org";
        ev["c"]["b"] = "B 10 t 24 e " + std::to_string(e);
        ev["ts"] = 1600001024000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T26) {
    json t;
    t["s"] = "BVega";
    t["n"] = 25;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_25:m.org";
        ev["c"]["b"] = "B 10 t 25 e " + std::to_string(e);
        ev["ts"] = 1600001025000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T27) {
    json t;
    t["s"] = "BVega";
    t["n"] = 26;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_26:m.org";
        ev["c"]["b"] = "B 10 t 26 e " + std::to_string(e);
        ev["ts"] = 1600001026000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T28) {
    json t;
    t["s"] = "BVega";
    t["n"] = 27;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_27:m.org";
        ev["c"]["b"] = "B 10 t 27 e " + std::to_string(e);
        ev["ts"] = 1600001027000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T29) {
    json t;
    t["s"] = "BVega";
    t["n"] = 28;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_28:m.org";
        ev["c"]["b"] = "B 10 t 28 e " + std::to_string(e);
        ev["ts"] = 1600001028000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T30) {
    json t;
    t["s"] = "BVega";
    t["n"] = 29;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_29:m.org";
        ev["c"]["b"] = "B 10 t 29 e " + std::to_string(e);
        ev["ts"] = 1600001029000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T31) {
    json t;
    t["s"] = "BVega";
    t["n"] = 30;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_30:m.org";
        ev["c"]["b"] = "B 10 t 30 e " + std::to_string(e);
        ev["ts"] = 1600001030000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T32) {
    json t;
    t["s"] = "BVega";
    t["n"] = 31;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_31:m.org";
        ev["c"]["b"] = "B 10 t 31 e " + std::to_string(e);
        ev["ts"] = 1600001031000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T33) {
    json t;
    t["s"] = "BVega";
    t["n"] = 32;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_32:m.org";
        ev["c"]["b"] = "B 10 t 32 e " + std::to_string(e);
        ev["ts"] = 1600001032000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T34) {
    json t;
    t["s"] = "BVega";
    t["n"] = 33;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_33:m.org";
        ev["c"]["b"] = "B 10 t 33 e " + std::to_string(e);
        ev["ts"] = 1600001033000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = false;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

TEST(BVegaTest, T35) {
    json t;
    t["s"] = "BVega";
    t["n"] = 34;
    t["b"] = 10;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(10) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_10_34:m.org";
        ev["c"]["b"] = "B 10 t 34 e " + std::to_string(e);
        ev["ts"] = 1600001034000 + e;
        t["evs"].push_back(ev);
        ASSERT_FALSE(ev["eid"].empty());
    }
    ASSERT_EQ(t["evs"].size(), 5);
    t["st"]["init"] = true;
    t["st"]["sync"] = true;
    ASSERT_TRUE(t["st"]["init"]);
    auto s = t.dump();
    ASSERT_FALSE(s.empty());
    auto p = json::parse(s);
    ASSERT_EQ(p["s"], t["s"]);
    ASSERT_EQ(p["evs"].size(), 5);
}

}} // namespace pcp::test
