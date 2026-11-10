#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteC, T1) {
  json t; t["s"]="SuiteC"; t["n"]=0; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000200000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T2) {
  json t; t["s"]="SuiteC"; t["n"]=1; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000201000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T3) {
  json t; t["s"]="SuiteC"; t["n"]=2; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000202000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T4) {
  json t; t["s"]="SuiteC"; t["n"]=3; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000203000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T5) {
  json t; t["s"]="SuiteC"; t["n"]=4; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000204000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T6) {
  json t; t["s"]="SuiteC"; t["n"]=5; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000205000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T7) {
  json t; t["s"]="SuiteC"; t["n"]=6; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000206000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T8) {
  json t; t["s"]="SuiteC"; t["n"]=7; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000207000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T9) {
  json t; t["s"]="SuiteC"; t["n"]=8; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000208000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T10) {
  json t; t["s"]="SuiteC"; t["n"]=9; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000209000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T11) {
  json t; t["s"]="SuiteC"; t["n"]=10; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000210000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T12) {
  json t; t["s"]="SuiteC"; t["n"]=11; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000211000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T13) {
  json t; t["s"]="SuiteC"; t["n"]=12; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000212000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T14) {
  json t; t["s"]="SuiteC"; t["n"]=13; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000213000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T15) {
  json t; t["s"]="SuiteC"; t["n"]=14; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000214000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T16) {
  json t; t["s"]="SuiteC"; t["n"]=15; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000215000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T17) {
  json t; t["s"]="SuiteC"; t["n"]=16; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000216000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T18) {
  json t; t["s"]="SuiteC"; t["n"]=17; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000217000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T19) {
  json t; t["s"]="SuiteC"; t["n"]=18; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000218000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T20) {
  json t; t["s"]="SuiteC"; t["n"]=19; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000219000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T21) {
  json t; t["s"]="SuiteC"; t["n"]=20; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000220000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T22) {
  json t; t["s"]="SuiteC"; t["n"]=21; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000221000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T23) {
  json t; t["s"]="SuiteC"; t["n"]=22; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000222000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T24) {
  json t; t["s"]="SuiteC"; t["n"]=23; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000223000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T25) {
  json t; t["s"]="SuiteC"; t["n"]=24; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000224000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T26) {
  json t; t["s"]="SuiteC"; t["n"]=25; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000225000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T27) {
  json t; t["s"]="SuiteC"; t["n"]=26; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000226000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T28) {
  json t; t["s"]="SuiteC"; t["n"]=27; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000227000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T29) {
  json t; t["s"]="SuiteC"; t["n"]=28; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000228000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T30) {
  json t; t["s"]="SuiteC"; t["n"]=29; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000229000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T31) {
  json t; t["s"]="SuiteC"; t["n"]=30; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000230000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T32) {
  json t; t["s"]="SuiteC"; t["n"]=31; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000231000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T33) {
  json t; t["s"]="SuiteC"; t["n"]=32; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000232000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T34) {
  json t; t["s"]="SuiteC"; t["n"]=33; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000233000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T35) {
  json t; t["s"]="SuiteC"; t["n"]=34; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000234000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T36) {
  json t; t["s"]="SuiteC"; t["n"]=35; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000235000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T37) {
  json t; t["s"]="SuiteC"; t["n"]=36; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000236000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T38) {
  json t; t["s"]="SuiteC"; t["n"]=37; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000237000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T39) {
  json t; t["s"]="SuiteC"; t["n"]=38; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000238000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T40) {
  json t; t["s"]="SuiteC"; t["n"]=39; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000239000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T41) {
  json t; t["s"]="SuiteC"; t["n"]=40; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000240000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T42) {
  json t; t["s"]="SuiteC"; t["n"]=41; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000241000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T43) {
  json t; t["s"]="SuiteC"; t["n"]=42; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000242000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T44) {
  json t; t["s"]="SuiteC"; t["n"]=43; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000243000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T45) {
  json t; t["s"]="SuiteC"; t["n"]=44; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000244000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T46) {
  json t; t["s"]="SuiteC"; t["n"]=45; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000245000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T47) {
  json t; t["s"]="SuiteC"; t["n"]=46; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000246000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T48) {
  json t; t["s"]="SuiteC"; t["n"]=47; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000247000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T49) {
  json t; t["s"]="SuiteC"; t["n"]=48; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000248000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T50) {
  json t; t["s"]="SuiteC"; t["n"]=49; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000249000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T51) {
  json t; t["s"]="SuiteC"; t["n"]=50; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000250000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T52) {
  json t; t["s"]="SuiteC"; t["n"]=51; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000251000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T53) {
  json t; t["s"]="SuiteC"; t["n"]=52; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000252000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T54) {
  json t; t["s"]="SuiteC"; t["n"]=53; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000253000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T55) {
  json t; t["s"]="SuiteC"; t["n"]=54; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000254000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T56) {
  json t; t["s"]="SuiteC"; t["n"]=55; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000255000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T57) {
  json t; t["s"]="SuiteC"; t["n"]=56; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000256000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T58) {
  json t; t["s"]="SuiteC"; t["n"]=57; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000257000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T59) {
  json t; t["s"]="SuiteC"; t["n"]=58; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000258000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T60) {
  json t; t["s"]="SuiteC"; t["n"]=59; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000259000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T61) {
  json t; t["s"]="SuiteC"; t["n"]=60; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000260000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T62) {
  json t; t["s"]="SuiteC"; t["n"]=61; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000261000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T63) {
  json t; t["s"]="SuiteC"; t["n"]=62; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000262000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T64) {
  json t; t["s"]="SuiteC"; t["n"]=63; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000263000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T65) {
  json t; t["s"]="SuiteC"; t["n"]=64; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000264000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T66) {
  json t; t["s"]="SuiteC"; t["n"]=65; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000265000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T67) {
  json t; t["s"]="SuiteC"; t["n"]=66; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000266000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T68) {
  json t; t["s"]="SuiteC"; t["n"]=67; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000267000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T69) {
  json t; t["s"]="SuiteC"; t["n"]=68; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000268000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteC, T70) {
  json t; t["s"]="SuiteC"; t["n"]=69; t["b"]=2;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(2) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_2_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000269000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
