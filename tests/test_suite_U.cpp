#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteU, T1) {
  json t; t["s"]="SuiteU"; t["n"]=0; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002000000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T2) {
  json t; t["s"]="SuiteU"; t["n"]=1; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002001000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T3) {
  json t; t["s"]="SuiteU"; t["n"]=2; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002002000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T4) {
  json t; t["s"]="SuiteU"; t["n"]=3; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002003000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T5) {
  json t; t["s"]="SuiteU"; t["n"]=4; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002004000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T6) {
  json t; t["s"]="SuiteU"; t["n"]=5; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002005000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T7) {
  json t; t["s"]="SuiteU"; t["n"]=6; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002006000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T8) {
  json t; t["s"]="SuiteU"; t["n"]=7; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002007000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T9) {
  json t; t["s"]="SuiteU"; t["n"]=8; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002008000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T10) {
  json t; t["s"]="SuiteU"; t["n"]=9; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002009000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T11) {
  json t; t["s"]="SuiteU"; t["n"]=10; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002010000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T12) {
  json t; t["s"]="SuiteU"; t["n"]=11; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002011000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T13) {
  json t; t["s"]="SuiteU"; t["n"]=12; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002012000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T14) {
  json t; t["s"]="SuiteU"; t["n"]=13; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002013000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T15) {
  json t; t["s"]="SuiteU"; t["n"]=14; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002014000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T16) {
  json t; t["s"]="SuiteU"; t["n"]=15; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002015000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T17) {
  json t; t["s"]="SuiteU"; t["n"]=16; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002016000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T18) {
  json t; t["s"]="SuiteU"; t["n"]=17; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002017000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T19) {
  json t; t["s"]="SuiteU"; t["n"]=18; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002018000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T20) {
  json t; t["s"]="SuiteU"; t["n"]=19; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002019000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T21) {
  json t; t["s"]="SuiteU"; t["n"]=20; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002020000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T22) {
  json t; t["s"]="SuiteU"; t["n"]=21; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002021000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T23) {
  json t; t["s"]="SuiteU"; t["n"]=22; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002022000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T24) {
  json t; t["s"]="SuiteU"; t["n"]=23; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002023000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T25) {
  json t; t["s"]="SuiteU"; t["n"]=24; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002024000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T26) {
  json t; t["s"]="SuiteU"; t["n"]=25; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002025000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T27) {
  json t; t["s"]="SuiteU"; t["n"]=26; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002026000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T28) {
  json t; t["s"]="SuiteU"; t["n"]=27; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002027000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T29) {
  json t; t["s"]="SuiteU"; t["n"]=28; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002028000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T30) {
  json t; t["s"]="SuiteU"; t["n"]=29; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002029000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T31) {
  json t; t["s"]="SuiteU"; t["n"]=30; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002030000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T32) {
  json t; t["s"]="SuiteU"; t["n"]=31; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002031000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T33) {
  json t; t["s"]="SuiteU"; t["n"]=32; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002032000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T34) {
  json t; t["s"]="SuiteU"; t["n"]=33; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002033000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T35) {
  json t; t["s"]="SuiteU"; t["n"]=34; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002034000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T36) {
  json t; t["s"]="SuiteU"; t["n"]=35; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002035000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T37) {
  json t; t["s"]="SuiteU"; t["n"]=36; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002036000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T38) {
  json t; t["s"]="SuiteU"; t["n"]=37; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002037000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T39) {
  json t; t["s"]="SuiteU"; t["n"]=38; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002038000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T40) {
  json t; t["s"]="SuiteU"; t["n"]=39; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002039000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T41) {
  json t; t["s"]="SuiteU"; t["n"]=40; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002040000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T42) {
  json t; t["s"]="SuiteU"; t["n"]=41; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002041000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T43) {
  json t; t["s"]="SuiteU"; t["n"]=42; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002042000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T44) {
  json t; t["s"]="SuiteU"; t["n"]=43; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002043000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T45) {
  json t; t["s"]="SuiteU"; t["n"]=44; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002044000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T46) {
  json t; t["s"]="SuiteU"; t["n"]=45; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002045000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T47) {
  json t; t["s"]="SuiteU"; t["n"]=46; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002046000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T48) {
  json t; t["s"]="SuiteU"; t["n"]=47; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002047000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T49) {
  json t; t["s"]="SuiteU"; t["n"]=48; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002048000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T50) {
  json t; t["s"]="SuiteU"; t["n"]=49; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002049000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T51) {
  json t; t["s"]="SuiteU"; t["n"]=50; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002050000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T52) {
  json t; t["s"]="SuiteU"; t["n"]=51; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002051000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T53) {
  json t; t["s"]="SuiteU"; t["n"]=52; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002052000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T54) {
  json t; t["s"]="SuiteU"; t["n"]=53; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002053000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T55) {
  json t; t["s"]="SuiteU"; t["n"]=54; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002054000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T56) {
  json t; t["s"]="SuiteU"; t["n"]=55; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002055000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T57) {
  json t; t["s"]="SuiteU"; t["n"]=56; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002056000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T58) {
  json t; t["s"]="SuiteU"; t["n"]=57; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002057000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T59) {
  json t; t["s"]="SuiteU"; t["n"]=58; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002058000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T60) {
  json t; t["s"]="SuiteU"; t["n"]=59; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002059000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T61) {
  json t; t["s"]="SuiteU"; t["n"]=60; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002060000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T62) {
  json t; t["s"]="SuiteU"; t["n"]=61; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002061000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T63) {
  json t; t["s"]="SuiteU"; t["n"]=62; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002062000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T64) {
  json t; t["s"]="SuiteU"; t["n"]=63; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002063000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T65) {
  json t; t["s"]="SuiteU"; t["n"]=64; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002064000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T66) {
  json t; t["s"]="SuiteU"; t["n"]=65; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002065000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T67) {
  json t; t["s"]="SuiteU"; t["n"]=66; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002066000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T68) {
  json t; t["s"]="SuiteU"; t["n"]=67; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002067000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T69) {
  json t; t["s"]="SuiteU"; t["n"]=68; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002068000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteU, T70) {
  json t; t["s"]="SuiteU"; t["n"]=69; t["b"]=20;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(20) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_20_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002069000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
