#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(BOrionTest, T1) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 0;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_0:m.org";
        ev["c"]["b"] = "B 9 t 0 e " + std::to_string(e);
        ev["ts"] = 1600000900000 + e;
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

TEST(BOrionTest, T2) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 1;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_1:m.org";
        ev["c"]["b"] = "B 9 t 1 e " + std::to_string(e);
        ev["ts"] = 1600000901000 + e;
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

TEST(BOrionTest, T3) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 2;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_2:m.org";
        ev["c"]["b"] = "B 9 t 2 e " + std::to_string(e);
        ev["ts"] = 1600000902000 + e;
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

TEST(BOrionTest, T4) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 3;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_3:m.org";
        ev["c"]["b"] = "B 9 t 3 e " + std::to_string(e);
        ev["ts"] = 1600000903000 + e;
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

TEST(BOrionTest, T5) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 4;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_4:m.org";
        ev["c"]["b"] = "B 9 t 4 e " + std::to_string(e);
        ev["ts"] = 1600000904000 + e;
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

TEST(BOrionTest, T6) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 5;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_5:m.org";
        ev["c"]["b"] = "B 9 t 5 e " + std::to_string(e);
        ev["ts"] = 1600000905000 + e;
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

TEST(BOrionTest, T7) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 6;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_6:m.org";
        ev["c"]["b"] = "B 9 t 6 e " + std::to_string(e);
        ev["ts"] = 1600000906000 + e;
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

TEST(BOrionTest, T8) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 7;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_7:m.org";
        ev["c"]["b"] = "B 9 t 7 e " + std::to_string(e);
        ev["ts"] = 1600000907000 + e;
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

TEST(BOrionTest, T9) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 8;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_8:m.org";
        ev["c"]["b"] = "B 9 t 8 e " + std::to_string(e);
        ev["ts"] = 1600000908000 + e;
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

TEST(BOrionTest, T10) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 9;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_9:m.org";
        ev["c"]["b"] = "B 9 t 9 e " + std::to_string(e);
        ev["ts"] = 1600000909000 + e;
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

TEST(BOrionTest, T11) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 10;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_10:m.org";
        ev["c"]["b"] = "B 9 t 10 e " + std::to_string(e);
        ev["ts"] = 1600000910000 + e;
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

TEST(BOrionTest, T12) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 11;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_11:m.org";
        ev["c"]["b"] = "B 9 t 11 e " + std::to_string(e);
        ev["ts"] = 1600000911000 + e;
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

TEST(BOrionTest, T13) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 12;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_12:m.org";
        ev["c"]["b"] = "B 9 t 12 e " + std::to_string(e);
        ev["ts"] = 1600000912000 + e;
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

TEST(BOrionTest, T14) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 13;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_13:m.org";
        ev["c"]["b"] = "B 9 t 13 e " + std::to_string(e);
        ev["ts"] = 1600000913000 + e;
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

TEST(BOrionTest, T15) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 14;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_14:m.org";
        ev["c"]["b"] = "B 9 t 14 e " + std::to_string(e);
        ev["ts"] = 1600000914000 + e;
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

TEST(BOrionTest, T16) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 15;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_15:m.org";
        ev["c"]["b"] = "B 9 t 15 e " + std::to_string(e);
        ev["ts"] = 1600000915000 + e;
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

TEST(BOrionTest, T17) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 16;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_16:m.org";
        ev["c"]["b"] = "B 9 t 16 e " + std::to_string(e);
        ev["ts"] = 1600000916000 + e;
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

TEST(BOrionTest, T18) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 17;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_17:m.org";
        ev["c"]["b"] = "B 9 t 17 e " + std::to_string(e);
        ev["ts"] = 1600000917000 + e;
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

TEST(BOrionTest, T19) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 18;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_18:m.org";
        ev["c"]["b"] = "B 9 t 18 e " + std::to_string(e);
        ev["ts"] = 1600000918000 + e;
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

TEST(BOrionTest, T20) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 19;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_19:m.org";
        ev["c"]["b"] = "B 9 t 19 e " + std::to_string(e);
        ev["ts"] = 1600000919000 + e;
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

TEST(BOrionTest, T21) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 20;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_20:m.org";
        ev["c"]["b"] = "B 9 t 20 e " + std::to_string(e);
        ev["ts"] = 1600000920000 + e;
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

TEST(BOrionTest, T22) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 21;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_21:m.org";
        ev["c"]["b"] = "B 9 t 21 e " + std::to_string(e);
        ev["ts"] = 1600000921000 + e;
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

TEST(BOrionTest, T23) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 22;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_22:m.org";
        ev["c"]["b"] = "B 9 t 22 e " + std::to_string(e);
        ev["ts"] = 1600000922000 + e;
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

TEST(BOrionTest, T24) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 23;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_23:m.org";
        ev["c"]["b"] = "B 9 t 23 e " + std::to_string(e);
        ev["ts"] = 1600000923000 + e;
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

TEST(BOrionTest, T25) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 24;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_24:m.org";
        ev["c"]["b"] = "B 9 t 24 e " + std::to_string(e);
        ev["ts"] = 1600000924000 + e;
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

TEST(BOrionTest, T26) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 25;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_25:m.org";
        ev["c"]["b"] = "B 9 t 25 e " + std::to_string(e);
        ev["ts"] = 1600000925000 + e;
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

TEST(BOrionTest, T27) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 26;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_26:m.org";
        ev["c"]["b"] = "B 9 t 26 e " + std::to_string(e);
        ev["ts"] = 1600000926000 + e;
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

TEST(BOrionTest, T28) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 27;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_27:m.org";
        ev["c"]["b"] = "B 9 t 27 e " + std::to_string(e);
        ev["ts"] = 1600000927000 + e;
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

TEST(BOrionTest, T29) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 28;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_28:m.org";
        ev["c"]["b"] = "B 9 t 28 e " + std::to_string(e);
        ev["ts"] = 1600000928000 + e;
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

TEST(BOrionTest, T30) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 29;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_29:m.org";
        ev["c"]["b"] = "B 9 t 29 e " + std::to_string(e);
        ev["ts"] = 1600000929000 + e;
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

TEST(BOrionTest, T31) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 30;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_30:m.org";
        ev["c"]["b"] = "B 9 t 30 e " + std::to_string(e);
        ev["ts"] = 1600000930000 + e;
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

TEST(BOrionTest, T32) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 31;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_31:m.org";
        ev["c"]["b"] = "B 9 t 31 e " + std::to_string(e);
        ev["ts"] = 1600000931000 + e;
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

TEST(BOrionTest, T33) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 32;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_32:m.org";
        ev["c"]["b"] = "B 9 t 32 e " + std::to_string(e);
        ev["ts"] = 1600000932000 + e;
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

TEST(BOrionTest, T34) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 33;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_33:m.org";
        ev["c"]["b"] = "B 9 t 33 e " + std::to_string(e);
        ev["ts"] = 1600000933000 + e;
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

TEST(BOrionTest, T35) {
    json t;
    t["s"] = "BOrion";
    t["n"] = 34;
    t["b"] = 9;
    for (int e = 0; e < 5; ++e) {
        json ev;
        ev["eid"] = "$b" + std::to_string(9) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m.org";
        ev["t"] = "m.room.message";
        ev["s"] = "@bu_9_34:m.org";
        ev["c"]["b"] = "B 9 t 34 e " + std::to_string(e);
        ev["ts"] = 1600000934000 + e;
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
