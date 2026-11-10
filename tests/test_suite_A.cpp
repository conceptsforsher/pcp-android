#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteA, T1) {
  json t; t["s"]="SuiteA"; t["n"]=0; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000000000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T2) {
  json t; t["s"]="SuiteA"; t["n"]=1; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000001000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T3) {
  json t; t["s"]="SuiteA"; t["n"]=2; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000002000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T4) {
  json t; t["s"]="SuiteA"; t["n"]=3; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000003000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T5) {
  json t; t["s"]="SuiteA"; t["n"]=4; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000004000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T6) {
  json t; t["s"]="SuiteA"; t["n"]=5; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000005000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T7) {
  json t; t["s"]="SuiteA"; t["n"]=6; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000006000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T8) {
  json t; t["s"]="SuiteA"; t["n"]=7; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000007000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T9) {
  json t; t["s"]="SuiteA"; t["n"]=8; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000008000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T10) {
  json t; t["s"]="SuiteA"; t["n"]=9; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000009000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T11) {
  json t; t["s"]="SuiteA"; t["n"]=10; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000010000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T12) {
  json t; t["s"]="SuiteA"; t["n"]=11; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000011000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T13) {
  json t; t["s"]="SuiteA"; t["n"]=12; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000012000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T14) {
  json t; t["s"]="SuiteA"; t["n"]=13; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000013000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T15) {
  json t; t["s"]="SuiteA"; t["n"]=14; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000014000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T16) {
  json t; t["s"]="SuiteA"; t["n"]=15; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000015000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T17) {
  json t; t["s"]="SuiteA"; t["n"]=16; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000016000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T18) {
  json t; t["s"]="SuiteA"; t["n"]=17; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000017000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T19) {
  json t; t["s"]="SuiteA"; t["n"]=18; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000018000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T20) {
  json t; t["s"]="SuiteA"; t["n"]=19; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000019000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T21) {
  json t; t["s"]="SuiteA"; t["n"]=20; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000020000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T22) {
  json t; t["s"]="SuiteA"; t["n"]=21; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000021000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T23) {
  json t; t["s"]="SuiteA"; t["n"]=22; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000022000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T24) {
  json t; t["s"]="SuiteA"; t["n"]=23; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000023000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T25) {
  json t; t["s"]="SuiteA"; t["n"]=24; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000024000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T26) {
  json t; t["s"]="SuiteA"; t["n"]=25; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000025000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T27) {
  json t; t["s"]="SuiteA"; t["n"]=26; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000026000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T28) {
  json t; t["s"]="SuiteA"; t["n"]=27; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000027000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T29) {
  json t; t["s"]="SuiteA"; t["n"]=28; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000028000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T30) {
  json t; t["s"]="SuiteA"; t["n"]=29; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000029000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T31) {
  json t; t["s"]="SuiteA"; t["n"]=30; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000030000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T32) {
  json t; t["s"]="SuiteA"; t["n"]=31; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000031000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T33) {
  json t; t["s"]="SuiteA"; t["n"]=32; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000032000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T34) {
  json t; t["s"]="SuiteA"; t["n"]=33; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000033000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T35) {
  json t; t["s"]="SuiteA"; t["n"]=34; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000034000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T36) {
  json t; t["s"]="SuiteA"; t["n"]=35; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000035000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T37) {
  json t; t["s"]="SuiteA"; t["n"]=36; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000036000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T38) {
  json t; t["s"]="SuiteA"; t["n"]=37; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000037000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T39) {
  json t; t["s"]="SuiteA"; t["n"]=38; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000038000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T40) {
  json t; t["s"]="SuiteA"; t["n"]=39; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000039000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T41) {
  json t; t["s"]="SuiteA"; t["n"]=40; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000040000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T42) {
  json t; t["s"]="SuiteA"; t["n"]=41; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000041000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T43) {
  json t; t["s"]="SuiteA"; t["n"]=42; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000042000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T44) {
  json t; t["s"]="SuiteA"; t["n"]=43; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000043000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T45) {
  json t; t["s"]="SuiteA"; t["n"]=44; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000044000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T46) {
  json t; t["s"]="SuiteA"; t["n"]=45; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000045000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T47) {
  json t; t["s"]="SuiteA"; t["n"]=46; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000046000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T48) {
  json t; t["s"]="SuiteA"; t["n"]=47; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000047000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T49) {
  json t; t["s"]="SuiteA"; t["n"]=48; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000048000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T50) {
  json t; t["s"]="SuiteA"; t["n"]=49; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000049000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T51) {
  json t; t["s"]="SuiteA"; t["n"]=50; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000050000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T52) {
  json t; t["s"]="SuiteA"; t["n"]=51; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000051000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T53) {
  json t; t["s"]="SuiteA"; t["n"]=52; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000052000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T54) {
  json t; t["s"]="SuiteA"; t["n"]=53; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000053000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T55) {
  json t; t["s"]="SuiteA"; t["n"]=54; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000054000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T56) {
  json t; t["s"]="SuiteA"; t["n"]=55; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000055000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T57) {
  json t; t["s"]="SuiteA"; t["n"]=56; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000056000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T58) {
  json t; t["s"]="SuiteA"; t["n"]=57; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000057000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T59) {
  json t; t["s"]="SuiteA"; t["n"]=58; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000058000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T60) {
  json t; t["s"]="SuiteA"; t["n"]=59; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000059000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T61) {
  json t; t["s"]="SuiteA"; t["n"]=60; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000060000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T62) {
  json t; t["s"]="SuiteA"; t["n"]=61; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000061000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T63) {
  json t; t["s"]="SuiteA"; t["n"]=62; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000062000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T64) {
  json t; t["s"]="SuiteA"; t["n"]=63; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000063000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T65) {
  json t; t["s"]="SuiteA"; t["n"]=64; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000064000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T66) {
  json t; t["s"]="SuiteA"; t["n"]=65; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000065000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T67) {
  json t; t["s"]="SuiteA"; t["n"]=66; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000066000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T68) {
  json t; t["s"]="SuiteA"; t["n"]=67; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000067000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T69) {
  json t; t["s"]="SuiteA"; t["n"]=68; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000068000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteA, T70) {
  json t; t["s"]="SuiteA"; t["n"]=69; t["b"]=0;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(0) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_0_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000069000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
