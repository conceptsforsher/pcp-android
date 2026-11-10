#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(EX1Test, T1) {
  json t; t["s"] = "EX1"; t["n"] = 0;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000000000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T2) {
  json t; t["s"] = "EX1"; t["n"] = 1;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000001000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T3) {
  json t; t["s"] = "EX1"; t["n"] = 2;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000002000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T4) {
  json t; t["s"] = "EX1"; t["n"] = 3;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000003000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T5) {
  json t; t["s"] = "EX1"; t["n"] = 4;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000004000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T6) {
  json t; t["s"] = "EX1"; t["n"] = 5;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000005000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T7) {
  json t; t["s"] = "EX1"; t["n"] = 6;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000006000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T8) {
  json t; t["s"] = "EX1"; t["n"] = 7;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000007000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T9) {
  json t; t["s"] = "EX1"; t["n"] = 8;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000008000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T10) {
  json t; t["s"] = "EX1"; t["n"] = 9;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000009000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T11) {
  json t; t["s"] = "EX1"; t["n"] = 10;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000010000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T12) {
  json t; t["s"] = "EX1"; t["n"] = 11;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000011000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T13) {
  json t; t["s"] = "EX1"; t["n"] = 12;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000012000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T14) {
  json t; t["s"] = "EX1"; t["n"] = 13;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000013000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T15) {
  json t; t["s"] = "EX1"; t["n"] = 14;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000014000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T16) {
  json t; t["s"] = "EX1"; t["n"] = 15;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000015000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T17) {
  json t; t["s"] = "EX1"; t["n"] = 16;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000016000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T18) {
  json t; t["s"] = "EX1"; t["n"] = 17;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000017000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T19) {
  json t; t["s"] = "EX1"; t["n"] = 18;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000018000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T20) {
  json t; t["s"] = "EX1"; t["n"] = 19;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000019000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T21) {
  json t; t["s"] = "EX1"; t["n"] = 20;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000020000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T22) {
  json t; t["s"] = "EX1"; t["n"] = 21;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000021000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T23) {
  json t; t["s"] = "EX1"; t["n"] = 22;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000022000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T24) {
  json t; t["s"] = "EX1"; t["n"] = 23;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000023000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T25) {
  json t; t["s"] = "EX1"; t["n"] = 24;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000024000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T26) {
  json t; t["s"] = "EX1"; t["n"] = 25;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000025000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T27) {
  json t; t["s"] = "EX1"; t["n"] = 26;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000026000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T28) {
  json t; t["s"] = "EX1"; t["n"] = 27;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000027000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T29) {
  json t; t["s"] = "EX1"; t["n"] = 28;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000028000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T30) {
  json t; t["s"] = "EX1"; t["n"] = 29;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000029000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T31) {
  json t; t["s"] = "EX1"; t["n"] = 30;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000030000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T32) {
  json t; t["s"] = "EX1"; t["n"] = 31;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000031000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T33) {
  json t; t["s"] = "EX1"; t["n"] = 32;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000032000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T34) {
  json t; t["s"] = "EX1"; t["n"] = 33;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000033000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

TEST(EX1Test, T35) {
  json t; t["s"] = "EX1"; t["n"] = 34;
  for (int e = 0; e < 6; ++e) {
    json v; v["eid"] = "$x" + std::to_string(1) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["c"]["b"] = "B " + std::to_string(e); v["ts"] = 1600000034000 + e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(), 6);
  auto sd = t.dump(); ASSERT_FALSE(sd.empty());
  auto p = json::parse(sd); ASSERT_EQ(p["evs"].size(), 6);
}

}}
