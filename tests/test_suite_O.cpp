#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteO, T1) {
  json t; t["s"]="SuiteO"; t["n"]=0; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001400000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T2) {
  json t; t["s"]="SuiteO"; t["n"]=1; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001401000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T3) {
  json t; t["s"]="SuiteO"; t["n"]=2; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001402000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T4) {
  json t; t["s"]="SuiteO"; t["n"]=3; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001403000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T5) {
  json t; t["s"]="SuiteO"; t["n"]=4; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001404000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T6) {
  json t; t["s"]="SuiteO"; t["n"]=5; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001405000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T7) {
  json t; t["s"]="SuiteO"; t["n"]=6; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001406000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T8) {
  json t; t["s"]="SuiteO"; t["n"]=7; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001407000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T9) {
  json t; t["s"]="SuiteO"; t["n"]=8; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001408000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T10) {
  json t; t["s"]="SuiteO"; t["n"]=9; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001409000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T11) {
  json t; t["s"]="SuiteO"; t["n"]=10; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001410000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T12) {
  json t; t["s"]="SuiteO"; t["n"]=11; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001411000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T13) {
  json t; t["s"]="SuiteO"; t["n"]=12; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001412000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T14) {
  json t; t["s"]="SuiteO"; t["n"]=13; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001413000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T15) {
  json t; t["s"]="SuiteO"; t["n"]=14; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001414000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T16) {
  json t; t["s"]="SuiteO"; t["n"]=15; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001415000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T17) {
  json t; t["s"]="SuiteO"; t["n"]=16; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001416000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T18) {
  json t; t["s"]="SuiteO"; t["n"]=17; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001417000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T19) {
  json t; t["s"]="SuiteO"; t["n"]=18; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001418000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T20) {
  json t; t["s"]="SuiteO"; t["n"]=19; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001419000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T21) {
  json t; t["s"]="SuiteO"; t["n"]=20; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001420000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T22) {
  json t; t["s"]="SuiteO"; t["n"]=21; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001421000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T23) {
  json t; t["s"]="SuiteO"; t["n"]=22; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001422000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T24) {
  json t; t["s"]="SuiteO"; t["n"]=23; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001423000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T25) {
  json t; t["s"]="SuiteO"; t["n"]=24; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001424000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T26) {
  json t; t["s"]="SuiteO"; t["n"]=25; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001425000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T27) {
  json t; t["s"]="SuiteO"; t["n"]=26; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001426000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T28) {
  json t; t["s"]="SuiteO"; t["n"]=27; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001427000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T29) {
  json t; t["s"]="SuiteO"; t["n"]=28; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001428000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T30) {
  json t; t["s"]="SuiteO"; t["n"]=29; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001429000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T31) {
  json t; t["s"]="SuiteO"; t["n"]=30; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001430000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T32) {
  json t; t["s"]="SuiteO"; t["n"]=31; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001431000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T33) {
  json t; t["s"]="SuiteO"; t["n"]=32; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001432000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T34) {
  json t; t["s"]="SuiteO"; t["n"]=33; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001433000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T35) {
  json t; t["s"]="SuiteO"; t["n"]=34; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001434000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T36) {
  json t; t["s"]="SuiteO"; t["n"]=35; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001435000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T37) {
  json t; t["s"]="SuiteO"; t["n"]=36; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001436000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T38) {
  json t; t["s"]="SuiteO"; t["n"]=37; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001437000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T39) {
  json t; t["s"]="SuiteO"; t["n"]=38; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001438000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T40) {
  json t; t["s"]="SuiteO"; t["n"]=39; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001439000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T41) {
  json t; t["s"]="SuiteO"; t["n"]=40; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001440000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T42) {
  json t; t["s"]="SuiteO"; t["n"]=41; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001441000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T43) {
  json t; t["s"]="SuiteO"; t["n"]=42; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001442000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T44) {
  json t; t["s"]="SuiteO"; t["n"]=43; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001443000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T45) {
  json t; t["s"]="SuiteO"; t["n"]=44; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001444000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T46) {
  json t; t["s"]="SuiteO"; t["n"]=45; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001445000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T47) {
  json t; t["s"]="SuiteO"; t["n"]=46; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001446000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T48) {
  json t; t["s"]="SuiteO"; t["n"]=47; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001447000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T49) {
  json t; t["s"]="SuiteO"; t["n"]=48; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001448000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T50) {
  json t; t["s"]="SuiteO"; t["n"]=49; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001449000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T51) {
  json t; t["s"]="SuiteO"; t["n"]=50; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001450000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T52) {
  json t; t["s"]="SuiteO"; t["n"]=51; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001451000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T53) {
  json t; t["s"]="SuiteO"; t["n"]=52; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001452000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T54) {
  json t; t["s"]="SuiteO"; t["n"]=53; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001453000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T55) {
  json t; t["s"]="SuiteO"; t["n"]=54; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001454000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T56) {
  json t; t["s"]="SuiteO"; t["n"]=55; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001455000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T57) {
  json t; t["s"]="SuiteO"; t["n"]=56; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001456000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T58) {
  json t; t["s"]="SuiteO"; t["n"]=57; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001457000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T59) {
  json t; t["s"]="SuiteO"; t["n"]=58; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001458000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T60) {
  json t; t["s"]="SuiteO"; t["n"]=59; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001459000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T61) {
  json t; t["s"]="SuiteO"; t["n"]=60; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001460000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T62) {
  json t; t["s"]="SuiteO"; t["n"]=61; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001461000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T63) {
  json t; t["s"]="SuiteO"; t["n"]=62; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001462000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T64) {
  json t; t["s"]="SuiteO"; t["n"]=63; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001463000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T65) {
  json t; t["s"]="SuiteO"; t["n"]=64; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001464000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T66) {
  json t; t["s"]="SuiteO"; t["n"]=65; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001465000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T67) {
  json t; t["s"]="SuiteO"; t["n"]=66; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001466000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T68) {
  json t; t["s"]="SuiteO"; t["n"]=67; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001467000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T69) {
  json t; t["s"]="SuiteO"; t["n"]=68; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001468000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteO, T70) {
  json t; t["s"]="SuiteO"; t["n"]=69; t["b"]=14;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(14) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_14_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001469000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
