#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteV, T1) {
  json t; t["s"]="SuiteV"; t["n"]=0; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002100000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T2) {
  json t; t["s"]="SuiteV"; t["n"]=1; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002101000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T3) {
  json t; t["s"]="SuiteV"; t["n"]=2; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002102000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T4) {
  json t; t["s"]="SuiteV"; t["n"]=3; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002103000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T5) {
  json t; t["s"]="SuiteV"; t["n"]=4; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002104000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T6) {
  json t; t["s"]="SuiteV"; t["n"]=5; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002105000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T7) {
  json t; t["s"]="SuiteV"; t["n"]=6; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002106000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T8) {
  json t; t["s"]="SuiteV"; t["n"]=7; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002107000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T9) {
  json t; t["s"]="SuiteV"; t["n"]=8; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002108000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T10) {
  json t; t["s"]="SuiteV"; t["n"]=9; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002109000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T11) {
  json t; t["s"]="SuiteV"; t["n"]=10; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002110000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T12) {
  json t; t["s"]="SuiteV"; t["n"]=11; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002111000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T13) {
  json t; t["s"]="SuiteV"; t["n"]=12; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002112000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T14) {
  json t; t["s"]="SuiteV"; t["n"]=13; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002113000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T15) {
  json t; t["s"]="SuiteV"; t["n"]=14; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002114000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T16) {
  json t; t["s"]="SuiteV"; t["n"]=15; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002115000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T17) {
  json t; t["s"]="SuiteV"; t["n"]=16; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002116000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T18) {
  json t; t["s"]="SuiteV"; t["n"]=17; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002117000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T19) {
  json t; t["s"]="SuiteV"; t["n"]=18; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002118000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T20) {
  json t; t["s"]="SuiteV"; t["n"]=19; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002119000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T21) {
  json t; t["s"]="SuiteV"; t["n"]=20; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002120000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T22) {
  json t; t["s"]="SuiteV"; t["n"]=21; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002121000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T23) {
  json t; t["s"]="SuiteV"; t["n"]=22; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002122000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T24) {
  json t; t["s"]="SuiteV"; t["n"]=23; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002123000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T25) {
  json t; t["s"]="SuiteV"; t["n"]=24; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002124000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T26) {
  json t; t["s"]="SuiteV"; t["n"]=25; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002125000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T27) {
  json t; t["s"]="SuiteV"; t["n"]=26; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002126000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T28) {
  json t; t["s"]="SuiteV"; t["n"]=27; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002127000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T29) {
  json t; t["s"]="SuiteV"; t["n"]=28; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002128000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T30) {
  json t; t["s"]="SuiteV"; t["n"]=29; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002129000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T31) {
  json t; t["s"]="SuiteV"; t["n"]=30; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002130000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T32) {
  json t; t["s"]="SuiteV"; t["n"]=31; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002131000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T33) {
  json t; t["s"]="SuiteV"; t["n"]=32; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002132000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T34) {
  json t; t["s"]="SuiteV"; t["n"]=33; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002133000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T35) {
  json t; t["s"]="SuiteV"; t["n"]=34; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002134000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T36) {
  json t; t["s"]="SuiteV"; t["n"]=35; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002135000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T37) {
  json t; t["s"]="SuiteV"; t["n"]=36; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002136000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T38) {
  json t; t["s"]="SuiteV"; t["n"]=37; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002137000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T39) {
  json t; t["s"]="SuiteV"; t["n"]=38; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002138000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T40) {
  json t; t["s"]="SuiteV"; t["n"]=39; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002139000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T41) {
  json t; t["s"]="SuiteV"; t["n"]=40; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002140000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T42) {
  json t; t["s"]="SuiteV"; t["n"]=41; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002141000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T43) {
  json t; t["s"]="SuiteV"; t["n"]=42; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002142000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T44) {
  json t; t["s"]="SuiteV"; t["n"]=43; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002143000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T45) {
  json t; t["s"]="SuiteV"; t["n"]=44; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002144000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T46) {
  json t; t["s"]="SuiteV"; t["n"]=45; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002145000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T47) {
  json t; t["s"]="SuiteV"; t["n"]=46; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002146000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T48) {
  json t; t["s"]="SuiteV"; t["n"]=47; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002147000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T49) {
  json t; t["s"]="SuiteV"; t["n"]=48; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002148000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T50) {
  json t; t["s"]="SuiteV"; t["n"]=49; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002149000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T51) {
  json t; t["s"]="SuiteV"; t["n"]=50; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002150000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T52) {
  json t; t["s"]="SuiteV"; t["n"]=51; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002151000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T53) {
  json t; t["s"]="SuiteV"; t["n"]=52; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002152000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T54) {
  json t; t["s"]="SuiteV"; t["n"]=53; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002153000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T55) {
  json t; t["s"]="SuiteV"; t["n"]=54; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002154000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T56) {
  json t; t["s"]="SuiteV"; t["n"]=55; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002155000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T57) {
  json t; t["s"]="SuiteV"; t["n"]=56; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002156000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T58) {
  json t; t["s"]="SuiteV"; t["n"]=57; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002157000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T59) {
  json t; t["s"]="SuiteV"; t["n"]=58; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002158000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T60) {
  json t; t["s"]="SuiteV"; t["n"]=59; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002159000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T61) {
  json t; t["s"]="SuiteV"; t["n"]=60; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002160000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T62) {
  json t; t["s"]="SuiteV"; t["n"]=61; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002161000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T63) {
  json t; t["s"]="SuiteV"; t["n"]=62; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002162000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T64) {
  json t; t["s"]="SuiteV"; t["n"]=63; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002163000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T65) {
  json t; t["s"]="SuiteV"; t["n"]=64; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002164000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T66) {
  json t; t["s"]="SuiteV"; t["n"]=65; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002165000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T67) {
  json t; t["s"]="SuiteV"; t["n"]=66; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002166000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T68) {
  json t; t["s"]="SuiteV"; t["n"]=67; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002167000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T69) {
  json t; t["s"]="SuiteV"; t["n"]=68; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002168000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteV, T70) {
  json t; t["s"]="SuiteV"; t["n"]=69; t["b"]=21;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(21) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_21_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002169000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
