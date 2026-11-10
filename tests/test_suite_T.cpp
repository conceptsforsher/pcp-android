#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteT, T1) {
  json t; t["s"]="SuiteT"; t["n"]=0; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001900000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T2) {
  json t; t["s"]="SuiteT"; t["n"]=1; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001901000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T3) {
  json t; t["s"]="SuiteT"; t["n"]=2; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001902000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T4) {
  json t; t["s"]="SuiteT"; t["n"]=3; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001903000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T5) {
  json t; t["s"]="SuiteT"; t["n"]=4; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001904000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T6) {
  json t; t["s"]="SuiteT"; t["n"]=5; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001905000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T7) {
  json t; t["s"]="SuiteT"; t["n"]=6; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001906000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T8) {
  json t; t["s"]="SuiteT"; t["n"]=7; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001907000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T9) {
  json t; t["s"]="SuiteT"; t["n"]=8; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001908000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T10) {
  json t; t["s"]="SuiteT"; t["n"]=9; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001909000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T11) {
  json t; t["s"]="SuiteT"; t["n"]=10; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001910000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T12) {
  json t; t["s"]="SuiteT"; t["n"]=11; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001911000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T13) {
  json t; t["s"]="SuiteT"; t["n"]=12; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001912000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T14) {
  json t; t["s"]="SuiteT"; t["n"]=13; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001913000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T15) {
  json t; t["s"]="SuiteT"; t["n"]=14; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001914000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T16) {
  json t; t["s"]="SuiteT"; t["n"]=15; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001915000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T17) {
  json t; t["s"]="SuiteT"; t["n"]=16; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001916000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T18) {
  json t; t["s"]="SuiteT"; t["n"]=17; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001917000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T19) {
  json t; t["s"]="SuiteT"; t["n"]=18; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001918000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T20) {
  json t; t["s"]="SuiteT"; t["n"]=19; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001919000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T21) {
  json t; t["s"]="SuiteT"; t["n"]=20; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001920000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T22) {
  json t; t["s"]="SuiteT"; t["n"]=21; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001921000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T23) {
  json t; t["s"]="SuiteT"; t["n"]=22; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001922000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T24) {
  json t; t["s"]="SuiteT"; t["n"]=23; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001923000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T25) {
  json t; t["s"]="SuiteT"; t["n"]=24; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001924000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T26) {
  json t; t["s"]="SuiteT"; t["n"]=25; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001925000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T27) {
  json t; t["s"]="SuiteT"; t["n"]=26; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001926000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T28) {
  json t; t["s"]="SuiteT"; t["n"]=27; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001927000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T29) {
  json t; t["s"]="SuiteT"; t["n"]=28; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001928000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T30) {
  json t; t["s"]="SuiteT"; t["n"]=29; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001929000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T31) {
  json t; t["s"]="SuiteT"; t["n"]=30; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001930000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T32) {
  json t; t["s"]="SuiteT"; t["n"]=31; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001931000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T33) {
  json t; t["s"]="SuiteT"; t["n"]=32; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001932000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T34) {
  json t; t["s"]="SuiteT"; t["n"]=33; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001933000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T35) {
  json t; t["s"]="SuiteT"; t["n"]=34; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001934000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T36) {
  json t; t["s"]="SuiteT"; t["n"]=35; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001935000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T37) {
  json t; t["s"]="SuiteT"; t["n"]=36; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001936000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T38) {
  json t; t["s"]="SuiteT"; t["n"]=37; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001937000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T39) {
  json t; t["s"]="SuiteT"; t["n"]=38; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001938000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T40) {
  json t; t["s"]="SuiteT"; t["n"]=39; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001939000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T41) {
  json t; t["s"]="SuiteT"; t["n"]=40; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001940000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T42) {
  json t; t["s"]="SuiteT"; t["n"]=41; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001941000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T43) {
  json t; t["s"]="SuiteT"; t["n"]=42; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001942000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T44) {
  json t; t["s"]="SuiteT"; t["n"]=43; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001943000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T45) {
  json t; t["s"]="SuiteT"; t["n"]=44; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001944000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T46) {
  json t; t["s"]="SuiteT"; t["n"]=45; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001945000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T47) {
  json t; t["s"]="SuiteT"; t["n"]=46; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001946000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T48) {
  json t; t["s"]="SuiteT"; t["n"]=47; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001947000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T49) {
  json t; t["s"]="SuiteT"; t["n"]=48; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001948000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T50) {
  json t; t["s"]="SuiteT"; t["n"]=49; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001949000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T51) {
  json t; t["s"]="SuiteT"; t["n"]=50; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001950000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T52) {
  json t; t["s"]="SuiteT"; t["n"]=51; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001951000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T53) {
  json t; t["s"]="SuiteT"; t["n"]=52; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001952000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T54) {
  json t; t["s"]="SuiteT"; t["n"]=53; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001953000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T55) {
  json t; t["s"]="SuiteT"; t["n"]=54; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001954000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T56) {
  json t; t["s"]="SuiteT"; t["n"]=55; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001955000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T57) {
  json t; t["s"]="SuiteT"; t["n"]=56; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001956000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T58) {
  json t; t["s"]="SuiteT"; t["n"]=57; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001957000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T59) {
  json t; t["s"]="SuiteT"; t["n"]=58; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001958000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T60) {
  json t; t["s"]="SuiteT"; t["n"]=59; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001959000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T61) {
  json t; t["s"]="SuiteT"; t["n"]=60; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001960000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T62) {
  json t; t["s"]="SuiteT"; t["n"]=61; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001961000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T63) {
  json t; t["s"]="SuiteT"; t["n"]=62; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001962000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T64) {
  json t; t["s"]="SuiteT"; t["n"]=63; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001963000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T65) {
  json t; t["s"]="SuiteT"; t["n"]=64; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001964000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T66) {
  json t; t["s"]="SuiteT"; t["n"]=65; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001965000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T67) {
  json t; t["s"]="SuiteT"; t["n"]=66; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001966000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T68) {
  json t; t["s"]="SuiteT"; t["n"]=67; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001967000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T69) {
  json t; t["s"]="SuiteT"; t["n"]=68; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001968000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteT, T70) {
  json t; t["s"]="SuiteT"; t["n"]=69; t["b"]=19;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(19) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_19_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001969000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
