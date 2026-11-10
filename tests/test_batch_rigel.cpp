#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BRigelTest, T1) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 0;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_0:m.org";
        ev["c"]["b"] = "B 13 t 0 e " + std::to_string(e);
        ev["ts"] = 1600001300000 + e;
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

TEST(BRigelTest, T2) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 1;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_1:m.org";
        ev["c"]["b"] = "B 13 t 1 e " + std::to_string(e);
        ev["ts"] = 1600001301000 + e;
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

TEST(BRigelTest, T3) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 2;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_2:m.org";
        ev["c"]["b"] = "B 13 t 2 e " + std::to_string(e);
        ev["ts"] = 1600001302000 + e;
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

TEST(BRigelTest, T4) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 3;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_3:m.org";
        ev["c"]["b"] = "B 13 t 3 e " + std::to_string(e);
        ev["ts"] = 1600001303000 + e;
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

TEST(BRigelTest, T5) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 4;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_4:m.org";
        ev["c"]["b"] = "B 13 t 4 e " + std::to_string(e);
        ev["ts"] = 1600001304000 + e;
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

TEST(BRigelTest, T6) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 5;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_5:m.org";
        ev["c"]["b"] = "B 13 t 5 e " + std::to_string(e);
        ev["ts"] = 1600001305000 + e;
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

TEST(BRigelTest, T7) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 6;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_6:m.org";
        ev["c"]["b"] = "B 13 t 6 e " + std::to_string(e);
        ev["ts"] = 1600001306000 + e;
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

TEST(BRigelTest, T8) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 7;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_7:m.org";
        ev["c"]["b"] = "B 13 t 7 e " + std::to_string(e);
        ev["ts"] = 1600001307000 + e;
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

TEST(BRigelTest, T9) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 8;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_8:m.org";
        ev["c"]["b"] = "B 13 t 8 e " + std::to_string(e);
        ev["ts"] = 1600001308000 + e;
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

TEST(BRigelTest, T10) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 9;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_9:m.org";
        ev["c"]["b"] = "B 13 t 9 e " + std::to_string(e);
        ev["ts"] = 1600001309000 + e;
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

TEST(BRigelTest, T11) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 10;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_10:m.org";
        ev["c"]["b"] = "B 13 t 10 e " + std::to_string(e);
        ev["ts"] = 1600001310000 + e;
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

TEST(BRigelTest, T12) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 11;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_11:m.org";
        ev["c"]["b"] = "B 13 t 11 e " + std::to_string(e);
        ev["ts"] = 1600001311000 + e;
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

TEST(BRigelTest, T13) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 12;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_12:m.org";
        ev["c"]["b"] = "B 13 t 12 e " + std::to_string(e);
        ev["ts"] = 1600001312000 + e;
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

TEST(BRigelTest, T14) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 13;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_13:m.org";
        ev["c"]["b"] = "B 13 t 13 e " + std::to_string(e);
        ev["ts"] = 1600001313000 + e;
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

TEST(BRigelTest, T15) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 14;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_14:m.org";
        ev["c"]["b"] = "B 13 t 14 e " + std::to_string(e);
        ev["ts"] = 1600001314000 + e;
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

TEST(BRigelTest, T16) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 15;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_15:m.org";
        ev["c"]["b"] = "B 13 t 15 e " + std::to_string(e);
        ev["ts"] = 1600001315000 + e;
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

TEST(BRigelTest, T17) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 16;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_16:m.org";
        ev["c"]["b"] = "B 13 t 16 e " + std::to_string(e);
        ev["ts"] = 1600001316000 + e;
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

TEST(BRigelTest, T18) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 17;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_17:m.org";
        ev["c"]["b"] = "B 13 t 17 e " + std::to_string(e);
        ev["ts"] = 1600001317000 + e;
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

TEST(BRigelTest, T19) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 18;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_18:m.org";
        ev["c"]["b"] = "B 13 t 18 e " + std::to_string(e);
        ev["ts"] = 1600001318000 + e;
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

TEST(BRigelTest, T20) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 19;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_19:m.org";
        ev["c"]["b"] = "B 13 t 19 e " + std::to_string(e);
        ev["ts"] = 1600001319000 + e;
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

TEST(BRigelTest, T21) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 20;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_20:m.org";
        ev["c"]["b"] = "B 13 t 20 e " + std::to_string(e);
        ev["ts"] = 1600001320000 + e;
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

TEST(BRigelTest, T22) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 21;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_21:m.org";
        ev["c"]["b"] = "B 13 t 21 e " + std::to_string(e);
        ev["ts"] = 1600001321000 + e;
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

TEST(BRigelTest, T23) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 22;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_22:m.org";
        ev["c"]["b"] = "B 13 t 22 e " + std::to_string(e);
        ev["ts"] = 1600001322000 + e;
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

TEST(BRigelTest, T24) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 23;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_23:m.org";
        ev["c"]["b"] = "B 13 t 23 e " + std::to_string(e);
        ev["ts"] = 1600001323000 + e;
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

TEST(BRigelTest, T25) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 24;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_24:m.org";
        ev["c"]["b"] = "B 13 t 24 e " + std::to_string(e);
        ev["ts"] = 1600001324000 + e;
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

TEST(BRigelTest, T26) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 25;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_25:m.org";
        ev["c"]["b"] = "B 13 t 25 e " + std::to_string(e);
        ev["ts"] = 1600001325000 + e;
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

TEST(BRigelTest, T27) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 26;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_26:m.org";
        ev["c"]["b"] = "B 13 t 26 e " + std::to_string(e);
        ev["ts"] = 1600001326000 + e;
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

TEST(BRigelTest, T28) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 27;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_27:m.org";
        ev["c"]["b"] = "B 13 t 27 e " + std::to_string(e);
        ev["ts"] = 1600001327000 + e;
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

TEST(BRigelTest, T29) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 28;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_28:m.org";
        ev["c"]["b"] = "B 13 t 28 e " + std::to_string(e);
        ev["ts"] = 1600001328000 + e;
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

TEST(BRigelTest, T30) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 29;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_29:m.org";
        ev["c"]["b"] = "B 13 t 29 e " + std::to_string(e);
        ev["ts"] = 1600001329000 + e;
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

TEST(BRigelTest, T31) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 30;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_30:m.org";
        ev["c"]["b"] = "B 13 t 30 e " + std::to_string(e);
        ev["ts"] = 1600001330000 + e;
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

TEST(BRigelTest, T32) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 31;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_31:m.org";
        ev["c"]["b"] = "B 13 t 31 e " + std::to_string(e);
        ev["ts"] = 1600001331000 + e;
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

TEST(BRigelTest, T33) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 32;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_32:m.org";
        ev["c"]["b"] = "B 13 t 32 e " + std::to_string(e);
        ev["ts"] = 1600001332000 + e;
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

TEST(BRigelTest, T34) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 33;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_33:m.org";
        ev["c"]["b"] = "B 13 t 33 e " + std::to_string(e);
        ev["ts"] = 1600001333000 + e;
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

TEST(BRigelTest, T35) {
    json t;
    t["s"] = "BRigel";
    t["n"] = 34;
    t["b"] = 13;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(13) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_13_34:m.org";
        ev["c"]["b"] = "B 13 t 34 e " + std::to_string(e);
        ev["ts"] = 1600001334000 + e;
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
