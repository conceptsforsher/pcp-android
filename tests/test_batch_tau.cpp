#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BTauTest, T1) {
    json t;
    t["s"] = "BTau";
    t["n"] = 0;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_0:m.org";
        ev["c"]["b"] = "B 3 t 0 e " + std::to_string(e);
        ev["ts"] = 1600000300000 + e;
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

TEST(BTauTest, T2) {
    json t;
    t["s"] = "BTau";
    t["n"] = 1;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_1:m.org";
        ev["c"]["b"] = "B 3 t 1 e " + std::to_string(e);
        ev["ts"] = 1600000301000 + e;
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

TEST(BTauTest, T3) {
    json t;
    t["s"] = "BTau";
    t["n"] = 2;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_2:m.org";
        ev["c"]["b"] = "B 3 t 2 e " + std::to_string(e);
        ev["ts"] = 1600000302000 + e;
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

TEST(BTauTest, T4) {
    json t;
    t["s"] = "BTau";
    t["n"] = 3;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_3:m.org";
        ev["c"]["b"] = "B 3 t 3 e " + std::to_string(e);
        ev["ts"] = 1600000303000 + e;
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

TEST(BTauTest, T5) {
    json t;
    t["s"] = "BTau";
    t["n"] = 4;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_4:m.org";
        ev["c"]["b"] = "B 3 t 4 e " + std::to_string(e);
        ev["ts"] = 1600000304000 + e;
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

TEST(BTauTest, T6) {
    json t;
    t["s"] = "BTau";
    t["n"] = 5;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_5:m.org";
        ev["c"]["b"] = "B 3 t 5 e " + std::to_string(e);
        ev["ts"] = 1600000305000 + e;
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

TEST(BTauTest, T7) {
    json t;
    t["s"] = "BTau";
    t["n"] = 6;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_6:m.org";
        ev["c"]["b"] = "B 3 t 6 e " + std::to_string(e);
        ev["ts"] = 1600000306000 + e;
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

TEST(BTauTest, T8) {
    json t;
    t["s"] = "BTau";
    t["n"] = 7;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_7:m.org";
        ev["c"]["b"] = "B 3 t 7 e " + std::to_string(e);
        ev["ts"] = 1600000307000 + e;
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

TEST(BTauTest, T9) {
    json t;
    t["s"] = "BTau";
    t["n"] = 8;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_8:m.org";
        ev["c"]["b"] = "B 3 t 8 e " + std::to_string(e);
        ev["ts"] = 1600000308000 + e;
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

TEST(BTauTest, T10) {
    json t;
    t["s"] = "BTau";
    t["n"] = 9;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_9:m.org";
        ev["c"]["b"] = "B 3 t 9 e " + std::to_string(e);
        ev["ts"] = 1600000309000 + e;
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

TEST(BTauTest, T11) {
    json t;
    t["s"] = "BTau";
    t["n"] = 10;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_10:m.org";
        ev["c"]["b"] = "B 3 t 10 e " + std::to_string(e);
        ev["ts"] = 1600000310000 + e;
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

TEST(BTauTest, T12) {
    json t;
    t["s"] = "BTau";
    t["n"] = 11;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_11:m.org";
        ev["c"]["b"] = "B 3 t 11 e " + std::to_string(e);
        ev["ts"] = 1600000311000 + e;
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

TEST(BTauTest, T13) {
    json t;
    t["s"] = "BTau";
    t["n"] = 12;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_12:m.org";
        ev["c"]["b"] = "B 3 t 12 e " + std::to_string(e);
        ev["ts"] = 1600000312000 + e;
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

TEST(BTauTest, T14) {
    json t;
    t["s"] = "BTau";
    t["n"] = 13;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_13:m.org";
        ev["c"]["b"] = "B 3 t 13 e " + std::to_string(e);
        ev["ts"] = 1600000313000 + e;
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

TEST(BTauTest, T15) {
    json t;
    t["s"] = "BTau";
    t["n"] = 14;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_14:m.org";
        ev["c"]["b"] = "B 3 t 14 e " + std::to_string(e);
        ev["ts"] = 1600000314000 + e;
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

TEST(BTauTest, T16) {
    json t;
    t["s"] = "BTau";
    t["n"] = 15;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_15:m.org";
        ev["c"]["b"] = "B 3 t 15 e " + std::to_string(e);
        ev["ts"] = 1600000315000 + e;
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

TEST(BTauTest, T17) {
    json t;
    t["s"] = "BTau";
    t["n"] = 16;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_16:m.org";
        ev["c"]["b"] = "B 3 t 16 e " + std::to_string(e);
        ev["ts"] = 1600000316000 + e;
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

TEST(BTauTest, T18) {
    json t;
    t["s"] = "BTau";
    t["n"] = 17;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_17:m.org";
        ev["c"]["b"] = "B 3 t 17 e " + std::to_string(e);
        ev["ts"] = 1600000317000 + e;
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

TEST(BTauTest, T19) {
    json t;
    t["s"] = "BTau";
    t["n"] = 18;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_18:m.org";
        ev["c"]["b"] = "B 3 t 18 e " + std::to_string(e);
        ev["ts"] = 1600000318000 + e;
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

TEST(BTauTest, T20) {
    json t;
    t["s"] = "BTau";
    t["n"] = 19;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_19:m.org";
        ev["c"]["b"] = "B 3 t 19 e " + std::to_string(e);
        ev["ts"] = 1600000319000 + e;
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

TEST(BTauTest, T21) {
    json t;
    t["s"] = "BTau";
    t["n"] = 20;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_20:m.org";
        ev["c"]["b"] = "B 3 t 20 e " + std::to_string(e);
        ev["ts"] = 1600000320000 + e;
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

TEST(BTauTest, T22) {
    json t;
    t["s"] = "BTau";
    t["n"] = 21;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_21:m.org";
        ev["c"]["b"] = "B 3 t 21 e " + std::to_string(e);
        ev["ts"] = 1600000321000 + e;
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

TEST(BTauTest, T23) {
    json t;
    t["s"] = "BTau";
    t["n"] = 22;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_22:m.org";
        ev["c"]["b"] = "B 3 t 22 e " + std::to_string(e);
        ev["ts"] = 1600000322000 + e;
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

TEST(BTauTest, T24) {
    json t;
    t["s"] = "BTau";
    t["n"] = 23;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_23:m.org";
        ev["c"]["b"] = "B 3 t 23 e " + std::to_string(e);
        ev["ts"] = 1600000323000 + e;
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

TEST(BTauTest, T25) {
    json t;
    t["s"] = "BTau";
    t["n"] = 24;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_24:m.org";
        ev["c"]["b"] = "B 3 t 24 e " + std::to_string(e);
        ev["ts"] = 1600000324000 + e;
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

TEST(BTauTest, T26) {
    json t;
    t["s"] = "BTau";
    t["n"] = 25;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_25:m.org";
        ev["c"]["b"] = "B 3 t 25 e " + std::to_string(e);
        ev["ts"] = 1600000325000 + e;
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

TEST(BTauTest, T27) {
    json t;
    t["s"] = "BTau";
    t["n"] = 26;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_26:m.org";
        ev["c"]["b"] = "B 3 t 26 e " + std::to_string(e);
        ev["ts"] = 1600000326000 + e;
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

TEST(BTauTest, T28) {
    json t;
    t["s"] = "BTau";
    t["n"] = 27;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_27:m.org";
        ev["c"]["b"] = "B 3 t 27 e " + std::to_string(e);
        ev["ts"] = 1600000327000 + e;
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

TEST(BTauTest, T29) {
    json t;
    t["s"] = "BTau";
    t["n"] = 28;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_28:m.org";
        ev["c"]["b"] = "B 3 t 28 e " + std::to_string(e);
        ev["ts"] = 1600000328000 + e;
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

TEST(BTauTest, T30) {
    json t;
    t["s"] = "BTau";
    t["n"] = 29;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_29:m.org";
        ev["c"]["b"] = "B 3 t 29 e " + std::to_string(e);
        ev["ts"] = 1600000329000 + e;
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

TEST(BTauTest, T31) {
    json t;
    t["s"] = "BTau";
    t["n"] = 30;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_30:m.org";
        ev["c"]["b"] = "B 3 t 30 e " + std::to_string(e);
        ev["ts"] = 1600000330000 + e;
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

TEST(BTauTest, T32) {
    json t;
    t["s"] = "BTau";
    t["n"] = 31;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_31:m.org";
        ev["c"]["b"] = "B 3 t 31 e " + std::to_string(e);
        ev["ts"] = 1600000331000 + e;
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

TEST(BTauTest, T33) {
    json t;
    t["s"] = "BTau";
    t["n"] = 32;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_32:m.org";
        ev["c"]["b"] = "B 3 t 32 e " + std::to_string(e);
        ev["ts"] = 1600000332000 + e;
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

TEST(BTauTest, T34) {
    json t;
    t["s"] = "BTau";
    t["n"] = 33;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_33:m.org";
        ev["c"]["b"] = "B 3 t 33 e " + std::to_string(e);
        ev["ts"] = 1600000333000 + e;
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

TEST(BTauTest, T35) {
    json t;
    t["s"] = "BTau";
    t["n"] = 34;
    t["b"] = 3;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(3) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_3_34:m.org";
        ev["c"]["b"] = "B 3 t 34 e " + std::to_string(e);
        ev["ts"] = 1600000334000 + e;
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
