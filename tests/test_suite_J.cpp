#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteJ, T1) {
  json t; t["s"]="SuiteJ"; t["n"]=0; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000900000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T2) {
  json t; t["s"]="SuiteJ"; t["n"]=1; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000901000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T3) {
  json t; t["s"]="SuiteJ"; t["n"]=2; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000902000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T4) {
  json t; t["s"]="SuiteJ"; t["n"]=3; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000903000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T5) {
  json t; t["s"]="SuiteJ"; t["n"]=4; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000904000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T6) {
  json t; t["s"]="SuiteJ"; t["n"]=5; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000905000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T7) {
  json t; t["s"]="SuiteJ"; t["n"]=6; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000906000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T8) {
  json t; t["s"]="SuiteJ"; t["n"]=7; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000907000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T9) {
  json t; t["s"]="SuiteJ"; t["n"]=8; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000908000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T10) {
  json t; t["s"]="SuiteJ"; t["n"]=9; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000909000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T11) {
  json t; t["s"]="SuiteJ"; t["n"]=10; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000910000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T12) {
  json t; t["s"]="SuiteJ"; t["n"]=11; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000911000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T13) {
  json t; t["s"]="SuiteJ"; t["n"]=12; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000912000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T14) {
  json t; t["s"]="SuiteJ"; t["n"]=13; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000913000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T15) {
  json t; t["s"]="SuiteJ"; t["n"]=14; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000914000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T16) {
  json t; t["s"]="SuiteJ"; t["n"]=15; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000915000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T17) {
  json t; t["s"]="SuiteJ"; t["n"]=16; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000916000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T18) {
  json t; t["s"]="SuiteJ"; t["n"]=17; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000917000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T19) {
  json t; t["s"]="SuiteJ"; t["n"]=18; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000918000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T20) {
  json t; t["s"]="SuiteJ"; t["n"]=19; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000919000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T21) {
  json t; t["s"]="SuiteJ"; t["n"]=20; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000920000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T22) {
  json t; t["s"]="SuiteJ"; t["n"]=21; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000921000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T23) {
  json t; t["s"]="SuiteJ"; t["n"]=22; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000922000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T24) {
  json t; t["s"]="SuiteJ"; t["n"]=23; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000923000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T25) {
  json t; t["s"]="SuiteJ"; t["n"]=24; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000924000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T26) {
  json t; t["s"]="SuiteJ"; t["n"]=25; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000925000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T27) {
  json t; t["s"]="SuiteJ"; t["n"]=26; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000926000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T28) {
  json t; t["s"]="SuiteJ"; t["n"]=27; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000927000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T29) {
  json t; t["s"]="SuiteJ"; t["n"]=28; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000928000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T30) {
  json t; t["s"]="SuiteJ"; t["n"]=29; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000929000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T31) {
  json t; t["s"]="SuiteJ"; t["n"]=30; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000930000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T32) {
  json t; t["s"]="SuiteJ"; t["n"]=31; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000931000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T33) {
  json t; t["s"]="SuiteJ"; t["n"]=32; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000932000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T34) {
  json t; t["s"]="SuiteJ"; t["n"]=33; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000933000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T35) {
  json t; t["s"]="SuiteJ"; t["n"]=34; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000934000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T36) {
  json t; t["s"]="SuiteJ"; t["n"]=35; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000935000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T37) {
  json t; t["s"]="SuiteJ"; t["n"]=36; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000936000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T38) {
  json t; t["s"]="SuiteJ"; t["n"]=37; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000937000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T39) {
  json t; t["s"]="SuiteJ"; t["n"]=38; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000938000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T40) {
  json t; t["s"]="SuiteJ"; t["n"]=39; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000939000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T41) {
  json t; t["s"]="SuiteJ"; t["n"]=40; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000940000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T42) {
  json t; t["s"]="SuiteJ"; t["n"]=41; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000941000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T43) {
  json t; t["s"]="SuiteJ"; t["n"]=42; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000942000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T44) {
  json t; t["s"]="SuiteJ"; t["n"]=43; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000943000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T45) {
  json t; t["s"]="SuiteJ"; t["n"]=44; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000944000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T46) {
  json t; t["s"]="SuiteJ"; t["n"]=45; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000945000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T47) {
  json t; t["s"]="SuiteJ"; t["n"]=46; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000946000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T48) {
  json t; t["s"]="SuiteJ"; t["n"]=47; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000947000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T49) {
  json t; t["s"]="SuiteJ"; t["n"]=48; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000948000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T50) {
  json t; t["s"]="SuiteJ"; t["n"]=49; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000949000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T51) {
  json t; t["s"]="SuiteJ"; t["n"]=50; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000950000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T52) {
  json t; t["s"]="SuiteJ"; t["n"]=51; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000951000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T53) {
  json t; t["s"]="SuiteJ"; t["n"]=52; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000952000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T54) {
  json t; t["s"]="SuiteJ"; t["n"]=53; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000953000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T55) {
  json t; t["s"]="SuiteJ"; t["n"]=54; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000954000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T56) {
  json t; t["s"]="SuiteJ"; t["n"]=55; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000955000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T57) {
  json t; t["s"]="SuiteJ"; t["n"]=56; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000956000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T58) {
  json t; t["s"]="SuiteJ"; t["n"]=57; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000957000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T59) {
  json t; t["s"]="SuiteJ"; t["n"]=58; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000958000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T60) {
  json t; t["s"]="SuiteJ"; t["n"]=59; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000959000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T61) {
  json t; t["s"]="SuiteJ"; t["n"]=60; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000960000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T62) {
  json t; t["s"]="SuiteJ"; t["n"]=61; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000961000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T63) {
  json t; t["s"]="SuiteJ"; t["n"]=62; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000962000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T64) {
  json t; t["s"]="SuiteJ"; t["n"]=63; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000963000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T65) {
  json t; t["s"]="SuiteJ"; t["n"]=64; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000964000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T66) {
  json t; t["s"]="SuiteJ"; t["n"]=65; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000965000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T67) {
  json t; t["s"]="SuiteJ"; t["n"]=66; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000966000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T68) {
  json t; t["s"]="SuiteJ"; t["n"]=67; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000967000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T69) {
  json t; t["s"]="SuiteJ"; t["n"]=68; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000968000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteJ, T70) {
  json t; t["s"]="SuiteJ"; t["n"]=69; t["b"]=9;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(9) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_9_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000969000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
