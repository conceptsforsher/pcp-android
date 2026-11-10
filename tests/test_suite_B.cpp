#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteB, T1) {
  json t; t["s"]="SuiteB"; t["n"]=0; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000100000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T2) {
  json t; t["s"]="SuiteB"; t["n"]=1; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000101000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T3) {
  json t; t["s"]="SuiteB"; t["n"]=2; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000102000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T4) {
  json t; t["s"]="SuiteB"; t["n"]=3; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000103000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T5) {
  json t; t["s"]="SuiteB"; t["n"]=4; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000104000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T6) {
  json t; t["s"]="SuiteB"; t["n"]=5; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000105000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T7) {
  json t; t["s"]="SuiteB"; t["n"]=6; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000106000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T8) {
  json t; t["s"]="SuiteB"; t["n"]=7; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000107000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T9) {
  json t; t["s"]="SuiteB"; t["n"]=8; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000108000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T10) {
  json t; t["s"]="SuiteB"; t["n"]=9; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000109000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T11) {
  json t; t["s"]="SuiteB"; t["n"]=10; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000110000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T12) {
  json t; t["s"]="SuiteB"; t["n"]=11; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000111000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T13) {
  json t; t["s"]="SuiteB"; t["n"]=12; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000112000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T14) {
  json t; t["s"]="SuiteB"; t["n"]=13; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000113000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T15) {
  json t; t["s"]="SuiteB"; t["n"]=14; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000114000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T16) {
  json t; t["s"]="SuiteB"; t["n"]=15; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000115000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T17) {
  json t; t["s"]="SuiteB"; t["n"]=16; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000116000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T18) {
  json t; t["s"]="SuiteB"; t["n"]=17; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000117000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T19) {
  json t; t["s"]="SuiteB"; t["n"]=18; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000118000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T20) {
  json t; t["s"]="SuiteB"; t["n"]=19; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000119000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T21) {
  json t; t["s"]="SuiteB"; t["n"]=20; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000120000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T22) {
  json t; t["s"]="SuiteB"; t["n"]=21; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000121000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T23) {
  json t; t["s"]="SuiteB"; t["n"]=22; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000122000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T24) {
  json t; t["s"]="SuiteB"; t["n"]=23; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000123000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T25) {
  json t; t["s"]="SuiteB"; t["n"]=24; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000124000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T26) {
  json t; t["s"]="SuiteB"; t["n"]=25; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000125000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T27) {
  json t; t["s"]="SuiteB"; t["n"]=26; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000126000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T28) {
  json t; t["s"]="SuiteB"; t["n"]=27; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000127000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T29) {
  json t; t["s"]="SuiteB"; t["n"]=28; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000128000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T30) {
  json t; t["s"]="SuiteB"; t["n"]=29; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000129000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T31) {
  json t; t["s"]="SuiteB"; t["n"]=30; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000130000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T32) {
  json t; t["s"]="SuiteB"; t["n"]=31; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000131000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T33) {
  json t; t["s"]="SuiteB"; t["n"]=32; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000132000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T34) {
  json t; t["s"]="SuiteB"; t["n"]=33; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000133000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T35) {
  json t; t["s"]="SuiteB"; t["n"]=34; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000134000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T36) {
  json t; t["s"]="SuiteB"; t["n"]=35; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000135000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T37) {
  json t; t["s"]="SuiteB"; t["n"]=36; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000136000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T38) {
  json t; t["s"]="SuiteB"; t["n"]=37; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000137000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T39) {
  json t; t["s"]="SuiteB"; t["n"]=38; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000138000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T40) {
  json t; t["s"]="SuiteB"; t["n"]=39; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000139000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T41) {
  json t; t["s"]="SuiteB"; t["n"]=40; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000140000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T42) {
  json t; t["s"]="SuiteB"; t["n"]=41; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000141000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T43) {
  json t; t["s"]="SuiteB"; t["n"]=42; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000142000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T44) {
  json t; t["s"]="SuiteB"; t["n"]=43; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000143000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T45) {
  json t; t["s"]="SuiteB"; t["n"]=44; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000144000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T46) {
  json t; t["s"]="SuiteB"; t["n"]=45; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000145000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T47) {
  json t; t["s"]="SuiteB"; t["n"]=46; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000146000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T48) {
  json t; t["s"]="SuiteB"; t["n"]=47; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000147000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T49) {
  json t; t["s"]="SuiteB"; t["n"]=48; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000148000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T50) {
  json t; t["s"]="SuiteB"; t["n"]=49; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000149000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T51) {
  json t; t["s"]="SuiteB"; t["n"]=50; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000150000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T52) {
  json t; t["s"]="SuiteB"; t["n"]=51; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000151000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T53) {
  json t; t["s"]="SuiteB"; t["n"]=52; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000152000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T54) {
  json t; t["s"]="SuiteB"; t["n"]=53; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000153000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T55) {
  json t; t["s"]="SuiteB"; t["n"]=54; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000154000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T56) {
  json t; t["s"]="SuiteB"; t["n"]=55; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000155000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T57) {
  json t; t["s"]="SuiteB"; t["n"]=56; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000156000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T58) {
  json t; t["s"]="SuiteB"; t["n"]=57; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000157000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T59) {
  json t; t["s"]="SuiteB"; t["n"]=58; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000158000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T60) {
  json t; t["s"]="SuiteB"; t["n"]=59; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000159000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T61) {
  json t; t["s"]="SuiteB"; t["n"]=60; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000160000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T62) {
  json t; t["s"]="SuiteB"; t["n"]=61; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000161000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T63) {
  json t; t["s"]="SuiteB"; t["n"]=62; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000162000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T64) {
  json t; t["s"]="SuiteB"; t["n"]=63; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000163000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T65) {
  json t; t["s"]="SuiteB"; t["n"]=64; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000164000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T66) {
  json t; t["s"]="SuiteB"; t["n"]=65; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000165000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T67) {
  json t; t["s"]="SuiteB"; t["n"]=66; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000166000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T68) {
  json t; t["s"]="SuiteB"; t["n"]=67; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000167000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T69) {
  json t; t["s"]="SuiteB"; t["n"]=68; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000168000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteB, T70) {
  json t; t["s"]="SuiteB"; t["n"]=69; t["b"]=1;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(1) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_1_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000169000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
