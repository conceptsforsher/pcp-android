#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteW, T1) {
  json t; t["s"]="SuiteW"; t["n"]=0; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002200000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T2) {
  json t; t["s"]="SuiteW"; t["n"]=1; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002201000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T3) {
  json t; t["s"]="SuiteW"; t["n"]=2; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002202000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T4) {
  json t; t["s"]="SuiteW"; t["n"]=3; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002203000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T5) {
  json t; t["s"]="SuiteW"; t["n"]=4; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002204000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T6) {
  json t; t["s"]="SuiteW"; t["n"]=5; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002205000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T7) {
  json t; t["s"]="SuiteW"; t["n"]=6; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002206000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T8) {
  json t; t["s"]="SuiteW"; t["n"]=7; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002207000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T9) {
  json t; t["s"]="SuiteW"; t["n"]=8; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002208000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T10) {
  json t; t["s"]="SuiteW"; t["n"]=9; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002209000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T11) {
  json t; t["s"]="SuiteW"; t["n"]=10; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002210000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T12) {
  json t; t["s"]="SuiteW"; t["n"]=11; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002211000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T13) {
  json t; t["s"]="SuiteW"; t["n"]=12; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002212000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T14) {
  json t; t["s"]="SuiteW"; t["n"]=13; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002213000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T15) {
  json t; t["s"]="SuiteW"; t["n"]=14; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002214000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T16) {
  json t; t["s"]="SuiteW"; t["n"]=15; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002215000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T17) {
  json t; t["s"]="SuiteW"; t["n"]=16; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002216000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T18) {
  json t; t["s"]="SuiteW"; t["n"]=17; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002217000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T19) {
  json t; t["s"]="SuiteW"; t["n"]=18; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002218000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T20) {
  json t; t["s"]="SuiteW"; t["n"]=19; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002219000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T21) {
  json t; t["s"]="SuiteW"; t["n"]=20; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002220000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T22) {
  json t; t["s"]="SuiteW"; t["n"]=21; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002221000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T23) {
  json t; t["s"]="SuiteW"; t["n"]=22; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002222000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T24) {
  json t; t["s"]="SuiteW"; t["n"]=23; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002223000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T25) {
  json t; t["s"]="SuiteW"; t["n"]=24; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002224000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T26) {
  json t; t["s"]="SuiteW"; t["n"]=25; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002225000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T27) {
  json t; t["s"]="SuiteW"; t["n"]=26; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002226000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T28) {
  json t; t["s"]="SuiteW"; t["n"]=27; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002227000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T29) {
  json t; t["s"]="SuiteW"; t["n"]=28; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002228000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T30) {
  json t; t["s"]="SuiteW"; t["n"]=29; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002229000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T31) {
  json t; t["s"]="SuiteW"; t["n"]=30; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002230000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T32) {
  json t; t["s"]="SuiteW"; t["n"]=31; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002231000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T33) {
  json t; t["s"]="SuiteW"; t["n"]=32; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002232000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T34) {
  json t; t["s"]="SuiteW"; t["n"]=33; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002233000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T35) {
  json t; t["s"]="SuiteW"; t["n"]=34; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002234000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T36) {
  json t; t["s"]="SuiteW"; t["n"]=35; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002235000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T37) {
  json t; t["s"]="SuiteW"; t["n"]=36; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002236000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T38) {
  json t; t["s"]="SuiteW"; t["n"]=37; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002237000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T39) {
  json t; t["s"]="SuiteW"; t["n"]=38; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002238000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T40) {
  json t; t["s"]="SuiteW"; t["n"]=39; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002239000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T41) {
  json t; t["s"]="SuiteW"; t["n"]=40; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002240000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T42) {
  json t; t["s"]="SuiteW"; t["n"]=41; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002241000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T43) {
  json t; t["s"]="SuiteW"; t["n"]=42; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002242000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T44) {
  json t; t["s"]="SuiteW"; t["n"]=43; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002243000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T45) {
  json t; t["s"]="SuiteW"; t["n"]=44; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002244000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T46) {
  json t; t["s"]="SuiteW"; t["n"]=45; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002245000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T47) {
  json t; t["s"]="SuiteW"; t["n"]=46; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002246000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T48) {
  json t; t["s"]="SuiteW"; t["n"]=47; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002247000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T49) {
  json t; t["s"]="SuiteW"; t["n"]=48; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002248000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T50) {
  json t; t["s"]="SuiteW"; t["n"]=49; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002249000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T51) {
  json t; t["s"]="SuiteW"; t["n"]=50; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002250000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T52) {
  json t; t["s"]="SuiteW"; t["n"]=51; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002251000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T53) {
  json t; t["s"]="SuiteW"; t["n"]=52; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002252000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T54) {
  json t; t["s"]="SuiteW"; t["n"]=53; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002253000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T55) {
  json t; t["s"]="SuiteW"; t["n"]=54; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002254000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T56) {
  json t; t["s"]="SuiteW"; t["n"]=55; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002255000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T57) {
  json t; t["s"]="SuiteW"; t["n"]=56; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002256000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T58) {
  json t; t["s"]="SuiteW"; t["n"]=57; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002257000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T59) {
  json t; t["s"]="SuiteW"; t["n"]=58; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002258000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T60) {
  json t; t["s"]="SuiteW"; t["n"]=59; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002259000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T61) {
  json t; t["s"]="SuiteW"; t["n"]=60; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002260000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T62) {
  json t; t["s"]="SuiteW"; t["n"]=61; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002261000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T63) {
  json t; t["s"]="SuiteW"; t["n"]=62; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002262000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T64) {
  json t; t["s"]="SuiteW"; t["n"]=63; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002263000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T65) {
  json t; t["s"]="SuiteW"; t["n"]=64; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002264000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T66) {
  json t; t["s"]="SuiteW"; t["n"]=65; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002265000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T67) {
  json t; t["s"]="SuiteW"; t["n"]=66; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002266000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T68) {
  json t; t["s"]="SuiteW"; t["n"]=67; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002267000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T69) {
  json t; t["s"]="SuiteW"; t["n"]=68; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002268000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteW, T70) {
  json t; t["s"]="SuiteW"; t["n"]=69; t["b"]=22;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(22) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_22_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002269000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
