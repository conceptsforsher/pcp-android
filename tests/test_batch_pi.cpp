#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BPiTest, T1) {
    json t;
    t["s"] = "BPi";
    t["n"] = 0;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_0:m.org";
        ev["c"]["b"] = "B 0 t 0 e " + std::to_string(e);
        ev["ts"] = 1600000000000 + e;
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

TEST(BPiTest, T2) {
    json t;
    t["s"] = "BPi";
    t["n"] = 1;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_1:m.org";
        ev["c"]["b"] = "B 0 t 1 e " + std::to_string(e);
        ev["ts"] = 1600000001000 + e;
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

TEST(BPiTest, T3) {
    json t;
    t["s"] = "BPi";
    t["n"] = 2;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_2:m.org";
        ev["c"]["b"] = "B 0 t 2 e " + std::to_string(e);
        ev["ts"] = 1600000002000 + e;
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

TEST(BPiTest, T4) {
    json t;
    t["s"] = "BPi";
    t["n"] = 3;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_3:m.org";
        ev["c"]["b"] = "B 0 t 3 e " + std::to_string(e);
        ev["ts"] = 1600000003000 + e;
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

TEST(BPiTest, T5) {
    json t;
    t["s"] = "BPi";
    t["n"] = 4;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_4:m.org";
        ev["c"]["b"] = "B 0 t 4 e " + std::to_string(e);
        ev["ts"] = 1600000004000 + e;
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

TEST(BPiTest, T6) {
    json t;
    t["s"] = "BPi";
    t["n"] = 5;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_5:m.org";
        ev["c"]["b"] = "B 0 t 5 e " + std::to_string(e);
        ev["ts"] = 1600000005000 + e;
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

TEST(BPiTest, T7) {
    json t;
    t["s"] = "BPi";
    t["n"] = 6;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_6:m.org";
        ev["c"]["b"] = "B 0 t 6 e " + std::to_string(e);
        ev["ts"] = 1600000006000 + e;
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

TEST(BPiTest, T8) {
    json t;
    t["s"] = "BPi";
    t["n"] = 7;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_7:m.org";
        ev["c"]["b"] = "B 0 t 7 e " + std::to_string(e);
        ev["ts"] = 1600000007000 + e;
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

TEST(BPiTest, T9) {
    json t;
    t["s"] = "BPi";
    t["n"] = 8;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_8:m.org";
        ev["c"]["b"] = "B 0 t 8 e " + std::to_string(e);
        ev["ts"] = 1600000008000 + e;
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

TEST(BPiTest, T10) {
    json t;
    t["s"] = "BPi";
    t["n"] = 9;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_9:m.org";
        ev["c"]["b"] = "B 0 t 9 e " + std::to_string(e);
        ev["ts"] = 1600000009000 + e;
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

TEST(BPiTest, T11) {
    json t;
    t["s"] = "BPi";
    t["n"] = 10;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_10:m.org";
        ev["c"]["b"] = "B 0 t 10 e " + std::to_string(e);
        ev["ts"] = 1600000010000 + e;
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

TEST(BPiTest, T12) {
    json t;
    t["s"] = "BPi";
    t["n"] = 11;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_11:m.org";
        ev["c"]["b"] = "B 0 t 11 e " + std::to_string(e);
        ev["ts"] = 1600000011000 + e;
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

TEST(BPiTest, T13) {
    json t;
    t["s"] = "BPi";
    t["n"] = 12;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_12:m.org";
        ev["c"]["b"] = "B 0 t 12 e " + std::to_string(e);
        ev["ts"] = 1600000012000 + e;
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

TEST(BPiTest, T14) {
    json t;
    t["s"] = "BPi";
    t["n"] = 13;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_13:m.org";
        ev["c"]["b"] = "B 0 t 13 e " + std::to_string(e);
        ev["ts"] = 1600000013000 + e;
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

TEST(BPiTest, T15) {
    json t;
    t["s"] = "BPi";
    t["n"] = 14;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_14:m.org";
        ev["c"]["b"] = "B 0 t 14 e " + std::to_string(e);
        ev["ts"] = 1600000014000 + e;
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

TEST(BPiTest, T16) {
    json t;
    t["s"] = "BPi";
    t["n"] = 15;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_15:m.org";
        ev["c"]["b"] = "B 0 t 15 e " + std::to_string(e);
        ev["ts"] = 1600000015000 + e;
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

TEST(BPiTest, T17) {
    json t;
    t["s"] = "BPi";
    t["n"] = 16;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_16:m.org";
        ev["c"]["b"] = "B 0 t 16 e " + std::to_string(e);
        ev["ts"] = 1600000016000 + e;
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

TEST(BPiTest, T18) {
    json t;
    t["s"] = "BPi";
    t["n"] = 17;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_17:m.org";
        ev["c"]["b"] = "B 0 t 17 e " + std::to_string(e);
        ev["ts"] = 1600000017000 + e;
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

TEST(BPiTest, T19) {
    json t;
    t["s"] = "BPi";
    t["n"] = 18;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_18:m.org";
        ev["c"]["b"] = "B 0 t 18 e " + std::to_string(e);
        ev["ts"] = 1600000018000 + e;
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

TEST(BPiTest, T20) {
    json t;
    t["s"] = "BPi";
    t["n"] = 19;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_19:m.org";
        ev["c"]["b"] = "B 0 t 19 e " + std::to_string(e);
        ev["ts"] = 1600000019000 + e;
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

TEST(BPiTest, T21) {
    json t;
    t["s"] = "BPi";
    t["n"] = 20;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_20:m.org";
        ev["c"]["b"] = "B 0 t 20 e " + std::to_string(e);
        ev["ts"] = 1600000020000 + e;
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

TEST(BPiTest, T22) {
    json t;
    t["s"] = "BPi";
    t["n"] = 21;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_21:m.org";
        ev["c"]["b"] = "B 0 t 21 e " + std::to_string(e);
        ev["ts"] = 1600000021000 + e;
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

TEST(BPiTest, T23) {
    json t;
    t["s"] = "BPi";
    t["n"] = 22;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_22:m.org";
        ev["c"]["b"] = "B 0 t 22 e " + std::to_string(e);
        ev["ts"] = 1600000022000 + e;
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

TEST(BPiTest, T24) {
    json t;
    t["s"] = "BPi";
    t["n"] = 23;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_23:m.org";
        ev["c"]["b"] = "B 0 t 23 e " + std::to_string(e);
        ev["ts"] = 1600000023000 + e;
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

TEST(BPiTest, T25) {
    json t;
    t["s"] = "BPi";
    t["n"] = 24;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_24:m.org";
        ev["c"]["b"] = "B 0 t 24 e " + std::to_string(e);
        ev["ts"] = 1600000024000 + e;
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

TEST(BPiTest, T26) {
    json t;
    t["s"] = "BPi";
    t["n"] = 25;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_25:m.org";
        ev["c"]["b"] = "B 0 t 25 e " + std::to_string(e);
        ev["ts"] = 1600000025000 + e;
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

TEST(BPiTest, T27) {
    json t;
    t["s"] = "BPi";
    t["n"] = 26;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_26:m.org";
        ev["c"]["b"] = "B 0 t 26 e " + std::to_string(e);
        ev["ts"] = 1600000026000 + e;
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

TEST(BPiTest, T28) {
    json t;
    t["s"] = "BPi";
    t["n"] = 27;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_27:m.org";
        ev["c"]["b"] = "B 0 t 27 e " + std::to_string(e);
        ev["ts"] = 1600000027000 + e;
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

TEST(BPiTest, T29) {
    json t;
    t["s"] = "BPi";
    t["n"] = 28;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_28:m.org";
        ev["c"]["b"] = "B 0 t 28 e " + std::to_string(e);
        ev["ts"] = 1600000028000 + e;
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

TEST(BPiTest, T30) {
    json t;
    t["s"] = "BPi";
    t["n"] = 29;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_29:m.org";
        ev["c"]["b"] = "B 0 t 29 e " + std::to_string(e);
        ev["ts"] = 1600000029000 + e;
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

TEST(BPiTest, T31) {
    json t;
    t["s"] = "BPi";
    t["n"] = 30;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_30:m.org";
        ev["c"]["b"] = "B 0 t 30 e " + std::to_string(e);
        ev["ts"] = 1600000030000 + e;
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

TEST(BPiTest, T32) {
    json t;
    t["s"] = "BPi";
    t["n"] = 31;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_31:m.org";
        ev["c"]["b"] = "B 0 t 31 e " + std::to_string(e);
        ev["ts"] = 1600000031000 + e;
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

TEST(BPiTest, T33) {
    json t;
    t["s"] = "BPi";
    t["n"] = 32;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_32:m.org";
        ev["c"]["b"] = "B 0 t 32 e " + std::to_string(e);
        ev["ts"] = 1600000032000 + e;
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

TEST(BPiTest, T34) {
    json t;
    t["s"] = "BPi";
    t["n"] = 33;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_33:m.org";
        ev["c"]["b"] = "B 0 t 33 e " + std::to_string(e);
        ev["ts"] = 1600000033000 + e;
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

TEST(BPiTest, T35) {
    json t;
    t["s"] = "BPi";
    t["n"] = 34;
    t["b"] = 0;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(0) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_0_34:m.org";
        ev["c"]["b"] = "B 0 t 34 e " + std::to_string(e);
        ev["ts"] = 1600000034000 + e;
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
