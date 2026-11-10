#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteS, T1) {
  json t; t["s"]="SuiteS"; t["n"]=0; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001800000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T2) {
  json t; t["s"]="SuiteS"; t["n"]=1; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001801000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T3) {
  json t; t["s"]="SuiteS"; t["n"]=2; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001802000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T4) {
  json t; t["s"]="SuiteS"; t["n"]=3; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001803000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T5) {
  json t; t["s"]="SuiteS"; t["n"]=4; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001804000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T6) {
  json t; t["s"]="SuiteS"; t["n"]=5; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001805000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T7) {
  json t; t["s"]="SuiteS"; t["n"]=6; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001806000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T8) {
  json t; t["s"]="SuiteS"; t["n"]=7; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001807000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T9) {
  json t; t["s"]="SuiteS"; t["n"]=8; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001808000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T10) {
  json t; t["s"]="SuiteS"; t["n"]=9; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001809000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T11) {
  json t; t["s"]="SuiteS"; t["n"]=10; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001810000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T12) {
  json t; t["s"]="SuiteS"; t["n"]=11; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001811000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T13) {
  json t; t["s"]="SuiteS"; t["n"]=12; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001812000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T14) {
  json t; t["s"]="SuiteS"; t["n"]=13; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001813000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T15) {
  json t; t["s"]="SuiteS"; t["n"]=14; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001814000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T16) {
  json t; t["s"]="SuiteS"; t["n"]=15; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001815000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T17) {
  json t; t["s"]="SuiteS"; t["n"]=16; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001816000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T18) {
  json t; t["s"]="SuiteS"; t["n"]=17; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001817000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T19) {
  json t; t["s"]="SuiteS"; t["n"]=18; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001818000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T20) {
  json t; t["s"]="SuiteS"; t["n"]=19; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001819000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T21) {
  json t; t["s"]="SuiteS"; t["n"]=20; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001820000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T22) {
  json t; t["s"]="SuiteS"; t["n"]=21; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001821000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T23) {
  json t; t["s"]="SuiteS"; t["n"]=22; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001822000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T24) {
  json t; t["s"]="SuiteS"; t["n"]=23; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001823000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T25) {
  json t; t["s"]="SuiteS"; t["n"]=24; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001824000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T26) {
  json t; t["s"]="SuiteS"; t["n"]=25; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001825000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T27) {
  json t; t["s"]="SuiteS"; t["n"]=26; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001826000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T28) {
  json t; t["s"]="SuiteS"; t["n"]=27; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001827000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T29) {
  json t; t["s"]="SuiteS"; t["n"]=28; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001828000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T30) {
  json t; t["s"]="SuiteS"; t["n"]=29; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001829000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T31) {
  json t; t["s"]="SuiteS"; t["n"]=30; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001830000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T32) {
  json t; t["s"]="SuiteS"; t["n"]=31; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001831000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T33) {
  json t; t["s"]="SuiteS"; t["n"]=32; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001832000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T34) {
  json t; t["s"]="SuiteS"; t["n"]=33; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001833000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T35) {
  json t; t["s"]="SuiteS"; t["n"]=34; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001834000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T36) {
  json t; t["s"]="SuiteS"; t["n"]=35; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001835000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T37) {
  json t; t["s"]="SuiteS"; t["n"]=36; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001836000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T38) {
  json t; t["s"]="SuiteS"; t["n"]=37; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001837000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T39) {
  json t; t["s"]="SuiteS"; t["n"]=38; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001838000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T40) {
  json t; t["s"]="SuiteS"; t["n"]=39; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001839000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T41) {
  json t; t["s"]="SuiteS"; t["n"]=40; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001840000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T42) {
  json t; t["s"]="SuiteS"; t["n"]=41; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001841000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T43) {
  json t; t["s"]="SuiteS"; t["n"]=42; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001842000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T44) {
  json t; t["s"]="SuiteS"; t["n"]=43; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001843000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T45) {
  json t; t["s"]="SuiteS"; t["n"]=44; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001844000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T46) {
  json t; t["s"]="SuiteS"; t["n"]=45; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001845000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T47) {
  json t; t["s"]="SuiteS"; t["n"]=46; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001846000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T48) {
  json t; t["s"]="SuiteS"; t["n"]=47; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001847000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T49) {
  json t; t["s"]="SuiteS"; t["n"]=48; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001848000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T50) {
  json t; t["s"]="SuiteS"; t["n"]=49; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001849000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T51) {
  json t; t["s"]="SuiteS"; t["n"]=50; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001850000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T52) {
  json t; t["s"]="SuiteS"; t["n"]=51; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001851000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T53) {
  json t; t["s"]="SuiteS"; t["n"]=52; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001852000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T54) {
  json t; t["s"]="SuiteS"; t["n"]=53; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001853000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T55) {
  json t; t["s"]="SuiteS"; t["n"]=54; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001854000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T56) {
  json t; t["s"]="SuiteS"; t["n"]=55; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001855000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T57) {
  json t; t["s"]="SuiteS"; t["n"]=56; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001856000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T58) {
  json t; t["s"]="SuiteS"; t["n"]=57; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001857000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T59) {
  json t; t["s"]="SuiteS"; t["n"]=58; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001858000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T60) {
  json t; t["s"]="SuiteS"; t["n"]=59; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001859000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T61) {
  json t; t["s"]="SuiteS"; t["n"]=60; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001860000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T62) {
  json t; t["s"]="SuiteS"; t["n"]=61; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001861000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T63) {
  json t; t["s"]="SuiteS"; t["n"]=62; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001862000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T64) {
  json t; t["s"]="SuiteS"; t["n"]=63; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001863000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T65) {
  json t; t["s"]="SuiteS"; t["n"]=64; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001864000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T66) {
  json t; t["s"]="SuiteS"; t["n"]=65; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001865000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T67) {
  json t; t["s"]="SuiteS"; t["n"]=66; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001866000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T68) {
  json t; t["s"]="SuiteS"; t["n"]=67; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001867000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T69) {
  json t; t["s"]="SuiteS"; t["n"]=68; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001868000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteS, T70) {
  json t; t["s"]="SuiteS"; t["n"]=69; t["b"]=18;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(18) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_18_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001869000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
