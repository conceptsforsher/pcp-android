#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteD, T1) {
  json t; t["s"]="SuiteD"; t["n"]=0; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000300000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T2) {
  json t; t["s"]="SuiteD"; t["n"]=1; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000301000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T3) {
  json t; t["s"]="SuiteD"; t["n"]=2; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000302000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T4) {
  json t; t["s"]="SuiteD"; t["n"]=3; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000303000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T5) {
  json t; t["s"]="SuiteD"; t["n"]=4; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000304000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T6) {
  json t; t["s"]="SuiteD"; t["n"]=5; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000305000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T7) {
  json t; t["s"]="SuiteD"; t["n"]=6; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000306000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T8) {
  json t; t["s"]="SuiteD"; t["n"]=7; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000307000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T9) {
  json t; t["s"]="SuiteD"; t["n"]=8; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000308000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T10) {
  json t; t["s"]="SuiteD"; t["n"]=9; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000309000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T11) {
  json t; t["s"]="SuiteD"; t["n"]=10; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000310000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T12) {
  json t; t["s"]="SuiteD"; t["n"]=11; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000311000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T13) {
  json t; t["s"]="SuiteD"; t["n"]=12; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000312000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T14) {
  json t; t["s"]="SuiteD"; t["n"]=13; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000313000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T15) {
  json t; t["s"]="SuiteD"; t["n"]=14; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000314000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T16) {
  json t; t["s"]="SuiteD"; t["n"]=15; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000315000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T17) {
  json t; t["s"]="SuiteD"; t["n"]=16; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000316000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T18) {
  json t; t["s"]="SuiteD"; t["n"]=17; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000317000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T19) {
  json t; t["s"]="SuiteD"; t["n"]=18; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000318000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T20) {
  json t; t["s"]="SuiteD"; t["n"]=19; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000319000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T21) {
  json t; t["s"]="SuiteD"; t["n"]=20; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000320000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T22) {
  json t; t["s"]="SuiteD"; t["n"]=21; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000321000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T23) {
  json t; t["s"]="SuiteD"; t["n"]=22; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000322000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T24) {
  json t; t["s"]="SuiteD"; t["n"]=23; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000323000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T25) {
  json t; t["s"]="SuiteD"; t["n"]=24; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000324000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T26) {
  json t; t["s"]="SuiteD"; t["n"]=25; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000325000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T27) {
  json t; t["s"]="SuiteD"; t["n"]=26; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000326000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T28) {
  json t; t["s"]="SuiteD"; t["n"]=27; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000327000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T29) {
  json t; t["s"]="SuiteD"; t["n"]=28; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000328000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T30) {
  json t; t["s"]="SuiteD"; t["n"]=29; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000329000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T31) {
  json t; t["s"]="SuiteD"; t["n"]=30; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000330000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T32) {
  json t; t["s"]="SuiteD"; t["n"]=31; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000331000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T33) {
  json t; t["s"]="SuiteD"; t["n"]=32; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000332000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T34) {
  json t; t["s"]="SuiteD"; t["n"]=33; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000333000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T35) {
  json t; t["s"]="SuiteD"; t["n"]=34; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000334000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T36) {
  json t; t["s"]="SuiteD"; t["n"]=35; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000335000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T37) {
  json t; t["s"]="SuiteD"; t["n"]=36; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000336000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T38) {
  json t; t["s"]="SuiteD"; t["n"]=37; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000337000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T39) {
  json t; t["s"]="SuiteD"; t["n"]=38; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000338000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T40) {
  json t; t["s"]="SuiteD"; t["n"]=39; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000339000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T41) {
  json t; t["s"]="SuiteD"; t["n"]=40; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000340000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T42) {
  json t; t["s"]="SuiteD"; t["n"]=41; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000341000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T43) {
  json t; t["s"]="SuiteD"; t["n"]=42; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000342000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T44) {
  json t; t["s"]="SuiteD"; t["n"]=43; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000343000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T45) {
  json t; t["s"]="SuiteD"; t["n"]=44; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000344000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T46) {
  json t; t["s"]="SuiteD"; t["n"]=45; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000345000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T47) {
  json t; t["s"]="SuiteD"; t["n"]=46; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000346000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T48) {
  json t; t["s"]="SuiteD"; t["n"]=47; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000347000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T49) {
  json t; t["s"]="SuiteD"; t["n"]=48; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000348000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T50) {
  json t; t["s"]="SuiteD"; t["n"]=49; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000349000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T51) {
  json t; t["s"]="SuiteD"; t["n"]=50; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000350000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T52) {
  json t; t["s"]="SuiteD"; t["n"]=51; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000351000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T53) {
  json t; t["s"]="SuiteD"; t["n"]=52; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000352000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T54) {
  json t; t["s"]="SuiteD"; t["n"]=53; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000353000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T55) {
  json t; t["s"]="SuiteD"; t["n"]=54; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000354000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T56) {
  json t; t["s"]="SuiteD"; t["n"]=55; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000355000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T57) {
  json t; t["s"]="SuiteD"; t["n"]=56; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000356000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T58) {
  json t; t["s"]="SuiteD"; t["n"]=57; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000357000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T59) {
  json t; t["s"]="SuiteD"; t["n"]=58; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000358000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T60) {
  json t; t["s"]="SuiteD"; t["n"]=59; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000359000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T61) {
  json t; t["s"]="SuiteD"; t["n"]=60; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000360000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T62) {
  json t; t["s"]="SuiteD"; t["n"]=61; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000361000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T63) {
  json t; t["s"]="SuiteD"; t["n"]=62; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000362000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T64) {
  json t; t["s"]="SuiteD"; t["n"]=63; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000363000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T65) {
  json t; t["s"]="SuiteD"; t["n"]=64; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000364000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T66) {
  json t; t["s"]="SuiteD"; t["n"]=65; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000365000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T67) {
  json t; t["s"]="SuiteD"; t["n"]=66; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000366000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T68) {
  json t; t["s"]="SuiteD"; t["n"]=67; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000367000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T69) {
  json t; t["s"]="SuiteD"; t["n"]=68; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000368000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteD, T70) {
  json t; t["s"]="SuiteD"; t["n"]=69; t["b"]=3;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(3) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_3_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000369000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
