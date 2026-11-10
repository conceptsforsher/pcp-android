#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteE, T1) {
  json t; t["s"]="SuiteE"; t["n"]=0; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000400000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T2) {
  json t; t["s"]="SuiteE"; t["n"]=1; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000401000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T3) {
  json t; t["s"]="SuiteE"; t["n"]=2; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000402000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T4) {
  json t; t["s"]="SuiteE"; t["n"]=3; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000403000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T5) {
  json t; t["s"]="SuiteE"; t["n"]=4; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000404000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T6) {
  json t; t["s"]="SuiteE"; t["n"]=5; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000405000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T7) {
  json t; t["s"]="SuiteE"; t["n"]=6; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000406000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T8) {
  json t; t["s"]="SuiteE"; t["n"]=7; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000407000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T9) {
  json t; t["s"]="SuiteE"; t["n"]=8; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000408000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T10) {
  json t; t["s"]="SuiteE"; t["n"]=9; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000409000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T11) {
  json t; t["s"]="SuiteE"; t["n"]=10; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000410000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T12) {
  json t; t["s"]="SuiteE"; t["n"]=11; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000411000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T13) {
  json t; t["s"]="SuiteE"; t["n"]=12; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000412000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T14) {
  json t; t["s"]="SuiteE"; t["n"]=13; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000413000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T15) {
  json t; t["s"]="SuiteE"; t["n"]=14; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000414000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T16) {
  json t; t["s"]="SuiteE"; t["n"]=15; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000415000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T17) {
  json t; t["s"]="SuiteE"; t["n"]=16; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000416000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T18) {
  json t; t["s"]="SuiteE"; t["n"]=17; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000417000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T19) {
  json t; t["s"]="SuiteE"; t["n"]=18; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000418000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T20) {
  json t; t["s"]="SuiteE"; t["n"]=19; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000419000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T21) {
  json t; t["s"]="SuiteE"; t["n"]=20; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000420000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T22) {
  json t; t["s"]="SuiteE"; t["n"]=21; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000421000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T23) {
  json t; t["s"]="SuiteE"; t["n"]=22; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000422000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T24) {
  json t; t["s"]="SuiteE"; t["n"]=23; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000423000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T25) {
  json t; t["s"]="SuiteE"; t["n"]=24; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000424000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T26) {
  json t; t["s"]="SuiteE"; t["n"]=25; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000425000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T27) {
  json t; t["s"]="SuiteE"; t["n"]=26; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000426000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T28) {
  json t; t["s"]="SuiteE"; t["n"]=27; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000427000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T29) {
  json t; t["s"]="SuiteE"; t["n"]=28; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000428000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T30) {
  json t; t["s"]="SuiteE"; t["n"]=29; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000429000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T31) {
  json t; t["s"]="SuiteE"; t["n"]=30; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000430000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T32) {
  json t; t["s"]="SuiteE"; t["n"]=31; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000431000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T33) {
  json t; t["s"]="SuiteE"; t["n"]=32; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000432000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T34) {
  json t; t["s"]="SuiteE"; t["n"]=33; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000433000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T35) {
  json t; t["s"]="SuiteE"; t["n"]=34; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000434000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T36) {
  json t; t["s"]="SuiteE"; t["n"]=35; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000435000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T37) {
  json t; t["s"]="SuiteE"; t["n"]=36; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000436000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T38) {
  json t; t["s"]="SuiteE"; t["n"]=37; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000437000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T39) {
  json t; t["s"]="SuiteE"; t["n"]=38; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000438000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T40) {
  json t; t["s"]="SuiteE"; t["n"]=39; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000439000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T41) {
  json t; t["s"]="SuiteE"; t["n"]=40; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000440000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T42) {
  json t; t["s"]="SuiteE"; t["n"]=41; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000441000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T43) {
  json t; t["s"]="SuiteE"; t["n"]=42; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000442000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T44) {
  json t; t["s"]="SuiteE"; t["n"]=43; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000443000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T45) {
  json t; t["s"]="SuiteE"; t["n"]=44; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000444000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T46) {
  json t; t["s"]="SuiteE"; t["n"]=45; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000445000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T47) {
  json t; t["s"]="SuiteE"; t["n"]=46; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000446000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T48) {
  json t; t["s"]="SuiteE"; t["n"]=47; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000447000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T49) {
  json t; t["s"]="SuiteE"; t["n"]=48; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000448000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T50) {
  json t; t["s"]="SuiteE"; t["n"]=49; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000449000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T51) {
  json t; t["s"]="SuiteE"; t["n"]=50; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000450000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T52) {
  json t; t["s"]="SuiteE"; t["n"]=51; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000451000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T53) {
  json t; t["s"]="SuiteE"; t["n"]=52; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000452000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T54) {
  json t; t["s"]="SuiteE"; t["n"]=53; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000453000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T55) {
  json t; t["s"]="SuiteE"; t["n"]=54; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000454000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T56) {
  json t; t["s"]="SuiteE"; t["n"]=55; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000455000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T57) {
  json t; t["s"]="SuiteE"; t["n"]=56; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000456000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T58) {
  json t; t["s"]="SuiteE"; t["n"]=57; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000457000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T59) {
  json t; t["s"]="SuiteE"; t["n"]=58; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000458000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T60) {
  json t; t["s"]="SuiteE"; t["n"]=59; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000459000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T61) {
  json t; t["s"]="SuiteE"; t["n"]=60; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000460000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T62) {
  json t; t["s"]="SuiteE"; t["n"]=61; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000461000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T63) {
  json t; t["s"]="SuiteE"; t["n"]=62; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000462000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T64) {
  json t; t["s"]="SuiteE"; t["n"]=63; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000463000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T65) {
  json t; t["s"]="SuiteE"; t["n"]=64; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000464000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T66) {
  json t; t["s"]="SuiteE"; t["n"]=65; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000465000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T67) {
  json t; t["s"]="SuiteE"; t["n"]=66; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000466000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T68) {
  json t; t["s"]="SuiteE"; t["n"]=67; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000467000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T69) {
  json t; t["s"]="SuiteE"; t["n"]=68; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000468000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteE, T70) {
  json t; t["s"]="SuiteE"; t["n"]=69; t["b"]=4;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(4) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_4_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000469000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
