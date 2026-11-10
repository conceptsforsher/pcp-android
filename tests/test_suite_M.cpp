#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteM, T1) {
  json t; t["s"]="SuiteM"; t["n"]=0; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001200000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T2) {
  json t; t["s"]="SuiteM"; t["n"]=1; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001201000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T3) {
  json t; t["s"]="SuiteM"; t["n"]=2; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001202000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T4) {
  json t; t["s"]="SuiteM"; t["n"]=3; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001203000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T5) {
  json t; t["s"]="SuiteM"; t["n"]=4; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001204000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T6) {
  json t; t["s"]="SuiteM"; t["n"]=5; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001205000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T7) {
  json t; t["s"]="SuiteM"; t["n"]=6; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001206000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T8) {
  json t; t["s"]="SuiteM"; t["n"]=7; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001207000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T9) {
  json t; t["s"]="SuiteM"; t["n"]=8; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001208000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T10) {
  json t; t["s"]="SuiteM"; t["n"]=9; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001209000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T11) {
  json t; t["s"]="SuiteM"; t["n"]=10; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001210000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T12) {
  json t; t["s"]="SuiteM"; t["n"]=11; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001211000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T13) {
  json t; t["s"]="SuiteM"; t["n"]=12; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001212000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T14) {
  json t; t["s"]="SuiteM"; t["n"]=13; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001213000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T15) {
  json t; t["s"]="SuiteM"; t["n"]=14; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001214000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T16) {
  json t; t["s"]="SuiteM"; t["n"]=15; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001215000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T17) {
  json t; t["s"]="SuiteM"; t["n"]=16; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001216000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T18) {
  json t; t["s"]="SuiteM"; t["n"]=17; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001217000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T19) {
  json t; t["s"]="SuiteM"; t["n"]=18; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001218000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T20) {
  json t; t["s"]="SuiteM"; t["n"]=19; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001219000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T21) {
  json t; t["s"]="SuiteM"; t["n"]=20; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001220000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T22) {
  json t; t["s"]="SuiteM"; t["n"]=21; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001221000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T23) {
  json t; t["s"]="SuiteM"; t["n"]=22; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001222000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T24) {
  json t; t["s"]="SuiteM"; t["n"]=23; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001223000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T25) {
  json t; t["s"]="SuiteM"; t["n"]=24; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001224000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T26) {
  json t; t["s"]="SuiteM"; t["n"]=25; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001225000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T27) {
  json t; t["s"]="SuiteM"; t["n"]=26; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001226000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T28) {
  json t; t["s"]="SuiteM"; t["n"]=27; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001227000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T29) {
  json t; t["s"]="SuiteM"; t["n"]=28; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001228000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T30) {
  json t; t["s"]="SuiteM"; t["n"]=29; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001229000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T31) {
  json t; t["s"]="SuiteM"; t["n"]=30; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001230000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T32) {
  json t; t["s"]="SuiteM"; t["n"]=31; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001231000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T33) {
  json t; t["s"]="SuiteM"; t["n"]=32; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001232000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T34) {
  json t; t["s"]="SuiteM"; t["n"]=33; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001233000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T35) {
  json t; t["s"]="SuiteM"; t["n"]=34; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001234000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T36) {
  json t; t["s"]="SuiteM"; t["n"]=35; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001235000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T37) {
  json t; t["s"]="SuiteM"; t["n"]=36; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001236000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T38) {
  json t; t["s"]="SuiteM"; t["n"]=37; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001237000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T39) {
  json t; t["s"]="SuiteM"; t["n"]=38; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001238000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T40) {
  json t; t["s"]="SuiteM"; t["n"]=39; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001239000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T41) {
  json t; t["s"]="SuiteM"; t["n"]=40; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001240000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T42) {
  json t; t["s"]="SuiteM"; t["n"]=41; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001241000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T43) {
  json t; t["s"]="SuiteM"; t["n"]=42; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001242000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T44) {
  json t; t["s"]="SuiteM"; t["n"]=43; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001243000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T45) {
  json t; t["s"]="SuiteM"; t["n"]=44; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001244000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T46) {
  json t; t["s"]="SuiteM"; t["n"]=45; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001245000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T47) {
  json t; t["s"]="SuiteM"; t["n"]=46; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001246000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T48) {
  json t; t["s"]="SuiteM"; t["n"]=47; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001247000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T49) {
  json t; t["s"]="SuiteM"; t["n"]=48; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001248000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T50) {
  json t; t["s"]="SuiteM"; t["n"]=49; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001249000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T51) {
  json t; t["s"]="SuiteM"; t["n"]=50; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001250000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T52) {
  json t; t["s"]="SuiteM"; t["n"]=51; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001251000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T53) {
  json t; t["s"]="SuiteM"; t["n"]=52; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001252000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T54) {
  json t; t["s"]="SuiteM"; t["n"]=53; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001253000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T55) {
  json t; t["s"]="SuiteM"; t["n"]=54; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001254000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T56) {
  json t; t["s"]="SuiteM"; t["n"]=55; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001255000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T57) {
  json t; t["s"]="SuiteM"; t["n"]=56; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001256000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T58) {
  json t; t["s"]="SuiteM"; t["n"]=57; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001257000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T59) {
  json t; t["s"]="SuiteM"; t["n"]=58; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001258000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T60) {
  json t; t["s"]="SuiteM"; t["n"]=59; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001259000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T61) {
  json t; t["s"]="SuiteM"; t["n"]=60; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001260000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T62) {
  json t; t["s"]="SuiteM"; t["n"]=61; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001261000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T63) {
  json t; t["s"]="SuiteM"; t["n"]=62; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001262000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T64) {
  json t; t["s"]="SuiteM"; t["n"]=63; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001263000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T65) {
  json t; t["s"]="SuiteM"; t["n"]=64; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001264000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T66) {
  json t; t["s"]="SuiteM"; t["n"]=65; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001265000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T67) {
  json t; t["s"]="SuiteM"; t["n"]=66; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001266000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T68) {
  json t; t["s"]="SuiteM"; t["n"]=67; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001267000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T69) {
  json t; t["s"]="SuiteM"; t["n"]=68; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001268000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteM, T70) {
  json t; t["s"]="SuiteM"; t["n"]=69; t["b"]=12;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(12) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_12_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001269000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
