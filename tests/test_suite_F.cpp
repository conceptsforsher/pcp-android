#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteF, T1) {
  json t; t["s"]="SuiteF"; t["n"]=0; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000500000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T2) {
  json t; t["s"]="SuiteF"; t["n"]=1; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000501000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T3) {
  json t; t["s"]="SuiteF"; t["n"]=2; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000502000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T4) {
  json t; t["s"]="SuiteF"; t["n"]=3; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000503000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T5) {
  json t; t["s"]="SuiteF"; t["n"]=4; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000504000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T6) {
  json t; t["s"]="SuiteF"; t["n"]=5; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000505000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T7) {
  json t; t["s"]="SuiteF"; t["n"]=6; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000506000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T8) {
  json t; t["s"]="SuiteF"; t["n"]=7; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000507000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T9) {
  json t; t["s"]="SuiteF"; t["n"]=8; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000508000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T10) {
  json t; t["s"]="SuiteF"; t["n"]=9; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000509000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T11) {
  json t; t["s"]="SuiteF"; t["n"]=10; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000510000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T12) {
  json t; t["s"]="SuiteF"; t["n"]=11; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000511000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T13) {
  json t; t["s"]="SuiteF"; t["n"]=12; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000512000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T14) {
  json t; t["s"]="SuiteF"; t["n"]=13; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000513000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T15) {
  json t; t["s"]="SuiteF"; t["n"]=14; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000514000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T16) {
  json t; t["s"]="SuiteF"; t["n"]=15; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000515000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T17) {
  json t; t["s"]="SuiteF"; t["n"]=16; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000516000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T18) {
  json t; t["s"]="SuiteF"; t["n"]=17; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000517000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T19) {
  json t; t["s"]="SuiteF"; t["n"]=18; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000518000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T20) {
  json t; t["s"]="SuiteF"; t["n"]=19; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000519000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T21) {
  json t; t["s"]="SuiteF"; t["n"]=20; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000520000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T22) {
  json t; t["s"]="SuiteF"; t["n"]=21; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000521000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T23) {
  json t; t["s"]="SuiteF"; t["n"]=22; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000522000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T24) {
  json t; t["s"]="SuiteF"; t["n"]=23; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000523000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T25) {
  json t; t["s"]="SuiteF"; t["n"]=24; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000524000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T26) {
  json t; t["s"]="SuiteF"; t["n"]=25; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000525000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T27) {
  json t; t["s"]="SuiteF"; t["n"]=26; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000526000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T28) {
  json t; t["s"]="SuiteF"; t["n"]=27; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000527000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T29) {
  json t; t["s"]="SuiteF"; t["n"]=28; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000528000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T30) {
  json t; t["s"]="SuiteF"; t["n"]=29; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000529000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T31) {
  json t; t["s"]="SuiteF"; t["n"]=30; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000530000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T32) {
  json t; t["s"]="SuiteF"; t["n"]=31; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000531000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T33) {
  json t; t["s"]="SuiteF"; t["n"]=32; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000532000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T34) {
  json t; t["s"]="SuiteF"; t["n"]=33; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000533000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T35) {
  json t; t["s"]="SuiteF"; t["n"]=34; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000534000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T36) {
  json t; t["s"]="SuiteF"; t["n"]=35; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000535000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T37) {
  json t; t["s"]="SuiteF"; t["n"]=36; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000536000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T38) {
  json t; t["s"]="SuiteF"; t["n"]=37; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000537000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T39) {
  json t; t["s"]="SuiteF"; t["n"]=38; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000538000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T40) {
  json t; t["s"]="SuiteF"; t["n"]=39; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000539000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T41) {
  json t; t["s"]="SuiteF"; t["n"]=40; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000540000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T42) {
  json t; t["s"]="SuiteF"; t["n"]=41; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000541000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T43) {
  json t; t["s"]="SuiteF"; t["n"]=42; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000542000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T44) {
  json t; t["s"]="SuiteF"; t["n"]=43; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000543000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T45) {
  json t; t["s"]="SuiteF"; t["n"]=44; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000544000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T46) {
  json t; t["s"]="SuiteF"; t["n"]=45; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000545000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T47) {
  json t; t["s"]="SuiteF"; t["n"]=46; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000546000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T48) {
  json t; t["s"]="SuiteF"; t["n"]=47; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000547000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T49) {
  json t; t["s"]="SuiteF"; t["n"]=48; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000548000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T50) {
  json t; t["s"]="SuiteF"; t["n"]=49; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000549000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T51) {
  json t; t["s"]="SuiteF"; t["n"]=50; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000550000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T52) {
  json t; t["s"]="SuiteF"; t["n"]=51; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000551000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T53) {
  json t; t["s"]="SuiteF"; t["n"]=52; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000552000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T54) {
  json t; t["s"]="SuiteF"; t["n"]=53; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000553000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T55) {
  json t; t["s"]="SuiteF"; t["n"]=54; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000554000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T56) {
  json t; t["s"]="SuiteF"; t["n"]=55; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000555000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T57) {
  json t; t["s"]="SuiteF"; t["n"]=56; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000556000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T58) {
  json t; t["s"]="SuiteF"; t["n"]=57; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000557000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T59) {
  json t; t["s"]="SuiteF"; t["n"]=58; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000558000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T60) {
  json t; t["s"]="SuiteF"; t["n"]=59; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000559000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T61) {
  json t; t["s"]="SuiteF"; t["n"]=60; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000560000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T62) {
  json t; t["s"]="SuiteF"; t["n"]=61; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000561000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T63) {
  json t; t["s"]="SuiteF"; t["n"]=62; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000562000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T64) {
  json t; t["s"]="SuiteF"; t["n"]=63; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000563000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T65) {
  json t; t["s"]="SuiteF"; t["n"]=64; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000564000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T66) {
  json t; t["s"]="SuiteF"; t["n"]=65; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000565000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T67) {
  json t; t["s"]="SuiteF"; t["n"]=66; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000566000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T68) {
  json t; t["s"]="SuiteF"; t["n"]=67; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000567000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T69) {
  json t; t["s"]="SuiteF"; t["n"]=68; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000568000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteF, T70) {
  json t; t["s"]="SuiteF"; t["n"]=69; t["b"]=5;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(5) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_5_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000569000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
