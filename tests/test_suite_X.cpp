#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteX, T1) {
  json t; t["s"]="SuiteX"; t["n"]=0; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002300000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T2) {
  json t; t["s"]="SuiteX"; t["n"]=1; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002301000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T3) {
  json t; t["s"]="SuiteX"; t["n"]=2; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002302000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T4) {
  json t; t["s"]="SuiteX"; t["n"]=3; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002303000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T5) {
  json t; t["s"]="SuiteX"; t["n"]=4; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002304000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T6) {
  json t; t["s"]="SuiteX"; t["n"]=5; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002305000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T7) {
  json t; t["s"]="SuiteX"; t["n"]=6; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002306000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T8) {
  json t; t["s"]="SuiteX"; t["n"]=7; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002307000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T9) {
  json t; t["s"]="SuiteX"; t["n"]=8; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002308000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T10) {
  json t; t["s"]="SuiteX"; t["n"]=9; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002309000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T11) {
  json t; t["s"]="SuiteX"; t["n"]=10; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002310000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T12) {
  json t; t["s"]="SuiteX"; t["n"]=11; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002311000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T13) {
  json t; t["s"]="SuiteX"; t["n"]=12; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002312000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T14) {
  json t; t["s"]="SuiteX"; t["n"]=13; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002313000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T15) {
  json t; t["s"]="SuiteX"; t["n"]=14; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002314000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T16) {
  json t; t["s"]="SuiteX"; t["n"]=15; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002315000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T17) {
  json t; t["s"]="SuiteX"; t["n"]=16; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002316000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T18) {
  json t; t["s"]="SuiteX"; t["n"]=17; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002317000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T19) {
  json t; t["s"]="SuiteX"; t["n"]=18; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002318000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T20) {
  json t; t["s"]="SuiteX"; t["n"]=19; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002319000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T21) {
  json t; t["s"]="SuiteX"; t["n"]=20; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002320000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T22) {
  json t; t["s"]="SuiteX"; t["n"]=21; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002321000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T23) {
  json t; t["s"]="SuiteX"; t["n"]=22; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002322000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T24) {
  json t; t["s"]="SuiteX"; t["n"]=23; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002323000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T25) {
  json t; t["s"]="SuiteX"; t["n"]=24; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002324000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T26) {
  json t; t["s"]="SuiteX"; t["n"]=25; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002325000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T27) {
  json t; t["s"]="SuiteX"; t["n"]=26; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002326000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T28) {
  json t; t["s"]="SuiteX"; t["n"]=27; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002327000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T29) {
  json t; t["s"]="SuiteX"; t["n"]=28; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002328000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T30) {
  json t; t["s"]="SuiteX"; t["n"]=29; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002329000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T31) {
  json t; t["s"]="SuiteX"; t["n"]=30; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002330000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T32) {
  json t; t["s"]="SuiteX"; t["n"]=31; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002331000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T33) {
  json t; t["s"]="SuiteX"; t["n"]=32; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002332000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T34) {
  json t; t["s"]="SuiteX"; t["n"]=33; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002333000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T35) {
  json t; t["s"]="SuiteX"; t["n"]=34; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002334000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T36) {
  json t; t["s"]="SuiteX"; t["n"]=35; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002335000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T37) {
  json t; t["s"]="SuiteX"; t["n"]=36; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002336000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T38) {
  json t; t["s"]="SuiteX"; t["n"]=37; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002337000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T39) {
  json t; t["s"]="SuiteX"; t["n"]=38; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002338000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T40) {
  json t; t["s"]="SuiteX"; t["n"]=39; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002339000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T41) {
  json t; t["s"]="SuiteX"; t["n"]=40; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002340000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T42) {
  json t; t["s"]="SuiteX"; t["n"]=41; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002341000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T43) {
  json t; t["s"]="SuiteX"; t["n"]=42; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002342000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T44) {
  json t; t["s"]="SuiteX"; t["n"]=43; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002343000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T45) {
  json t; t["s"]="SuiteX"; t["n"]=44; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002344000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T46) {
  json t; t["s"]="SuiteX"; t["n"]=45; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002345000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T47) {
  json t; t["s"]="SuiteX"; t["n"]=46; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002346000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T48) {
  json t; t["s"]="SuiteX"; t["n"]=47; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002347000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T49) {
  json t; t["s"]="SuiteX"; t["n"]=48; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002348000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T50) {
  json t; t["s"]="SuiteX"; t["n"]=49; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002349000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T51) {
  json t; t["s"]="SuiteX"; t["n"]=50; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002350000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T52) {
  json t; t["s"]="SuiteX"; t["n"]=51; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002351000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T53) {
  json t; t["s"]="SuiteX"; t["n"]=52; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002352000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T54) {
  json t; t["s"]="SuiteX"; t["n"]=53; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002353000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T55) {
  json t; t["s"]="SuiteX"; t["n"]=54; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002354000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T56) {
  json t; t["s"]="SuiteX"; t["n"]=55; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002355000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T57) {
  json t; t["s"]="SuiteX"; t["n"]=56; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002356000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T58) {
  json t; t["s"]="SuiteX"; t["n"]=57; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002357000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T59) {
  json t; t["s"]="SuiteX"; t["n"]=58; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002358000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T60) {
  json t; t["s"]="SuiteX"; t["n"]=59; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002359000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T61) {
  json t; t["s"]="SuiteX"; t["n"]=60; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002360000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T62) {
  json t; t["s"]="SuiteX"; t["n"]=61; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002361000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T63) {
  json t; t["s"]="SuiteX"; t["n"]=62; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002362000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T64) {
  json t; t["s"]="SuiteX"; t["n"]=63; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002363000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T65) {
  json t; t["s"]="SuiteX"; t["n"]=64; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002364000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T66) {
  json t; t["s"]="SuiteX"; t["n"]=65; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002365000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T67) {
  json t; t["s"]="SuiteX"; t["n"]=66; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002366000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T68) {
  json t; t["s"]="SuiteX"; t["n"]=67; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002367000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T69) {
  json t; t["s"]="SuiteX"; t["n"]=68; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002368000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteX, T70) {
  json t; t["s"]="SuiteX"; t["n"]=69; t["b"]=23;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(23) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_23_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002369000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
