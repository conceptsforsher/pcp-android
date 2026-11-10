#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteN, T1) {
  json t; t["s"]="SuiteN"; t["n"]=0; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001300000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T2) {
  json t; t["s"]="SuiteN"; t["n"]=1; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001301000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T3) {
  json t; t["s"]="SuiteN"; t["n"]=2; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001302000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T4) {
  json t; t["s"]="SuiteN"; t["n"]=3; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001303000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T5) {
  json t; t["s"]="SuiteN"; t["n"]=4; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001304000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T6) {
  json t; t["s"]="SuiteN"; t["n"]=5; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001305000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T7) {
  json t; t["s"]="SuiteN"; t["n"]=6; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001306000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T8) {
  json t; t["s"]="SuiteN"; t["n"]=7; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001307000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T9) {
  json t; t["s"]="SuiteN"; t["n"]=8; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001308000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T10) {
  json t; t["s"]="SuiteN"; t["n"]=9; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001309000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T11) {
  json t; t["s"]="SuiteN"; t["n"]=10; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001310000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T12) {
  json t; t["s"]="SuiteN"; t["n"]=11; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001311000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T13) {
  json t; t["s"]="SuiteN"; t["n"]=12; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001312000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T14) {
  json t; t["s"]="SuiteN"; t["n"]=13; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001313000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T15) {
  json t; t["s"]="SuiteN"; t["n"]=14; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001314000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T16) {
  json t; t["s"]="SuiteN"; t["n"]=15; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001315000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T17) {
  json t; t["s"]="SuiteN"; t["n"]=16; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001316000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T18) {
  json t; t["s"]="SuiteN"; t["n"]=17; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001317000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T19) {
  json t; t["s"]="SuiteN"; t["n"]=18; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001318000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T20) {
  json t; t["s"]="SuiteN"; t["n"]=19; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001319000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T21) {
  json t; t["s"]="SuiteN"; t["n"]=20; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001320000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T22) {
  json t; t["s"]="SuiteN"; t["n"]=21; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001321000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T23) {
  json t; t["s"]="SuiteN"; t["n"]=22; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001322000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T24) {
  json t; t["s"]="SuiteN"; t["n"]=23; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001323000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T25) {
  json t; t["s"]="SuiteN"; t["n"]=24; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001324000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T26) {
  json t; t["s"]="SuiteN"; t["n"]=25; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001325000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T27) {
  json t; t["s"]="SuiteN"; t["n"]=26; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001326000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T28) {
  json t; t["s"]="SuiteN"; t["n"]=27; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001327000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T29) {
  json t; t["s"]="SuiteN"; t["n"]=28; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001328000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T30) {
  json t; t["s"]="SuiteN"; t["n"]=29; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001329000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T31) {
  json t; t["s"]="SuiteN"; t["n"]=30; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001330000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T32) {
  json t; t["s"]="SuiteN"; t["n"]=31; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001331000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T33) {
  json t; t["s"]="SuiteN"; t["n"]=32; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001332000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T34) {
  json t; t["s"]="SuiteN"; t["n"]=33; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001333000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T35) {
  json t; t["s"]="SuiteN"; t["n"]=34; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001334000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T36) {
  json t; t["s"]="SuiteN"; t["n"]=35; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001335000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T37) {
  json t; t["s"]="SuiteN"; t["n"]=36; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001336000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T38) {
  json t; t["s"]="SuiteN"; t["n"]=37; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001337000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T39) {
  json t; t["s"]="SuiteN"; t["n"]=38; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001338000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T40) {
  json t; t["s"]="SuiteN"; t["n"]=39; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001339000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T41) {
  json t; t["s"]="SuiteN"; t["n"]=40; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001340000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T42) {
  json t; t["s"]="SuiteN"; t["n"]=41; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001341000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T43) {
  json t; t["s"]="SuiteN"; t["n"]=42; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001342000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T44) {
  json t; t["s"]="SuiteN"; t["n"]=43; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001343000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T45) {
  json t; t["s"]="SuiteN"; t["n"]=44; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001344000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T46) {
  json t; t["s"]="SuiteN"; t["n"]=45; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001345000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T47) {
  json t; t["s"]="SuiteN"; t["n"]=46; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001346000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T48) {
  json t; t["s"]="SuiteN"; t["n"]=47; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001347000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T49) {
  json t; t["s"]="SuiteN"; t["n"]=48; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001348000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T50) {
  json t; t["s"]="SuiteN"; t["n"]=49; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001349000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T51) {
  json t; t["s"]="SuiteN"; t["n"]=50; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001350000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T52) {
  json t; t["s"]="SuiteN"; t["n"]=51; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001351000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T53) {
  json t; t["s"]="SuiteN"; t["n"]=52; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001352000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T54) {
  json t; t["s"]="SuiteN"; t["n"]=53; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001353000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T55) {
  json t; t["s"]="SuiteN"; t["n"]=54; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001354000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T56) {
  json t; t["s"]="SuiteN"; t["n"]=55; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001355000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T57) {
  json t; t["s"]="SuiteN"; t["n"]=56; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001356000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T58) {
  json t; t["s"]="SuiteN"; t["n"]=57; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001357000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T59) {
  json t; t["s"]="SuiteN"; t["n"]=58; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001358000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T60) {
  json t; t["s"]="SuiteN"; t["n"]=59; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001359000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T61) {
  json t; t["s"]="SuiteN"; t["n"]=60; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001360000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T62) {
  json t; t["s"]="SuiteN"; t["n"]=61; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001361000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T63) {
  json t; t["s"]="SuiteN"; t["n"]=62; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001362000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T64) {
  json t; t["s"]="SuiteN"; t["n"]=63; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001363000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T65) {
  json t; t["s"]="SuiteN"; t["n"]=64; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001364000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T66) {
  json t; t["s"]="SuiteN"; t["n"]=65; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001365000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T67) {
  json t; t["s"]="SuiteN"; t["n"]=66; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001366000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T68) {
  json t; t["s"]="SuiteN"; t["n"]=67; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001367000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T69) {
  json t; t["s"]="SuiteN"; t["n"]=68; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001368000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteN, T70) {
  json t; t["s"]="SuiteN"; t["n"]=69; t["b"]=13;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(13) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_13_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001369000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
