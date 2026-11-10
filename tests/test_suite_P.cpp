#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteP, T1) {
  json t; t["s"]="SuiteP"; t["n"]=0; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001500000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T2) {
  json t; t["s"]="SuiteP"; t["n"]=1; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001501000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T3) {
  json t; t["s"]="SuiteP"; t["n"]=2; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001502000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T4) {
  json t; t["s"]="SuiteP"; t["n"]=3; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001503000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T5) {
  json t; t["s"]="SuiteP"; t["n"]=4; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001504000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T6) {
  json t; t["s"]="SuiteP"; t["n"]=5; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001505000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T7) {
  json t; t["s"]="SuiteP"; t["n"]=6; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001506000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T8) {
  json t; t["s"]="SuiteP"; t["n"]=7; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001507000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T9) {
  json t; t["s"]="SuiteP"; t["n"]=8; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001508000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T10) {
  json t; t["s"]="SuiteP"; t["n"]=9; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001509000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T11) {
  json t; t["s"]="SuiteP"; t["n"]=10; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001510000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T12) {
  json t; t["s"]="SuiteP"; t["n"]=11; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001511000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T13) {
  json t; t["s"]="SuiteP"; t["n"]=12; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001512000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T14) {
  json t; t["s"]="SuiteP"; t["n"]=13; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001513000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T15) {
  json t; t["s"]="SuiteP"; t["n"]=14; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001514000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T16) {
  json t; t["s"]="SuiteP"; t["n"]=15; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001515000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T17) {
  json t; t["s"]="SuiteP"; t["n"]=16; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001516000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T18) {
  json t; t["s"]="SuiteP"; t["n"]=17; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001517000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T19) {
  json t; t["s"]="SuiteP"; t["n"]=18; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001518000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T20) {
  json t; t["s"]="SuiteP"; t["n"]=19; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001519000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T21) {
  json t; t["s"]="SuiteP"; t["n"]=20; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001520000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T22) {
  json t; t["s"]="SuiteP"; t["n"]=21; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001521000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T23) {
  json t; t["s"]="SuiteP"; t["n"]=22; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001522000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T24) {
  json t; t["s"]="SuiteP"; t["n"]=23; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001523000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T25) {
  json t; t["s"]="SuiteP"; t["n"]=24; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001524000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T26) {
  json t; t["s"]="SuiteP"; t["n"]=25; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001525000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T27) {
  json t; t["s"]="SuiteP"; t["n"]=26; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001526000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T28) {
  json t; t["s"]="SuiteP"; t["n"]=27; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001527000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T29) {
  json t; t["s"]="SuiteP"; t["n"]=28; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001528000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T30) {
  json t; t["s"]="SuiteP"; t["n"]=29; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001529000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T31) {
  json t; t["s"]="SuiteP"; t["n"]=30; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001530000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T32) {
  json t; t["s"]="SuiteP"; t["n"]=31; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001531000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T33) {
  json t; t["s"]="SuiteP"; t["n"]=32; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001532000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T34) {
  json t; t["s"]="SuiteP"; t["n"]=33; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001533000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T35) {
  json t; t["s"]="SuiteP"; t["n"]=34; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001534000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T36) {
  json t; t["s"]="SuiteP"; t["n"]=35; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001535000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T37) {
  json t; t["s"]="SuiteP"; t["n"]=36; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001536000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T38) {
  json t; t["s"]="SuiteP"; t["n"]=37; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001537000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T39) {
  json t; t["s"]="SuiteP"; t["n"]=38; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001538000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T40) {
  json t; t["s"]="SuiteP"; t["n"]=39; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001539000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T41) {
  json t; t["s"]="SuiteP"; t["n"]=40; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001540000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T42) {
  json t; t["s"]="SuiteP"; t["n"]=41; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001541000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T43) {
  json t; t["s"]="SuiteP"; t["n"]=42; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001542000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T44) {
  json t; t["s"]="SuiteP"; t["n"]=43; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001543000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T45) {
  json t; t["s"]="SuiteP"; t["n"]=44; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001544000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T46) {
  json t; t["s"]="SuiteP"; t["n"]=45; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001545000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T47) {
  json t; t["s"]="SuiteP"; t["n"]=46; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001546000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T48) {
  json t; t["s"]="SuiteP"; t["n"]=47; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001547000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T49) {
  json t; t["s"]="SuiteP"; t["n"]=48; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001548000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T50) {
  json t; t["s"]="SuiteP"; t["n"]=49; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001549000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T51) {
  json t; t["s"]="SuiteP"; t["n"]=50; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001550000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T52) {
  json t; t["s"]="SuiteP"; t["n"]=51; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001551000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T53) {
  json t; t["s"]="SuiteP"; t["n"]=52; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001552000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T54) {
  json t; t["s"]="SuiteP"; t["n"]=53; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001553000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T55) {
  json t; t["s"]="SuiteP"; t["n"]=54; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001554000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T56) {
  json t; t["s"]="SuiteP"; t["n"]=55; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001555000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T57) {
  json t; t["s"]="SuiteP"; t["n"]=56; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001556000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T58) {
  json t; t["s"]="SuiteP"; t["n"]=57; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001557000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T59) {
  json t; t["s"]="SuiteP"; t["n"]=58; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001558000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T60) {
  json t; t["s"]="SuiteP"; t["n"]=59; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001559000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T61) {
  json t; t["s"]="SuiteP"; t["n"]=60; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001560000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T62) {
  json t; t["s"]="SuiteP"; t["n"]=61; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001561000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T63) {
  json t; t["s"]="SuiteP"; t["n"]=62; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001562000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T64) {
  json t; t["s"]="SuiteP"; t["n"]=63; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001563000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T65) {
  json t; t["s"]="SuiteP"; t["n"]=64; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001564000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T66) {
  json t; t["s"]="SuiteP"; t["n"]=65; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001565000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T67) {
  json t; t["s"]="SuiteP"; t["n"]=66; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001566000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T68) {
  json t; t["s"]="SuiteP"; t["n"]=67; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001567000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T69) {
  json t; t["s"]="SuiteP"; t["n"]=68; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001568000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteP, T70) {
  json t; t["s"]="SuiteP"; t["n"]=69; t["b"]=15;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(15) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_15_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001569000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
