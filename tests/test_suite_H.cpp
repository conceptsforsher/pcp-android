#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteH, T1) {
  json t; t["s"]="SuiteH"; t["n"]=0; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000700000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T2) {
  json t; t["s"]="SuiteH"; t["n"]=1; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000701000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T3) {
  json t; t["s"]="SuiteH"; t["n"]=2; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000702000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T4) {
  json t; t["s"]="SuiteH"; t["n"]=3; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000703000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T5) {
  json t; t["s"]="SuiteH"; t["n"]=4; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000704000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T6) {
  json t; t["s"]="SuiteH"; t["n"]=5; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000705000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T7) {
  json t; t["s"]="SuiteH"; t["n"]=6; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000706000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T8) {
  json t; t["s"]="SuiteH"; t["n"]=7; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000707000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T9) {
  json t; t["s"]="SuiteH"; t["n"]=8; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000708000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T10) {
  json t; t["s"]="SuiteH"; t["n"]=9; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000709000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T11) {
  json t; t["s"]="SuiteH"; t["n"]=10; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000710000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T12) {
  json t; t["s"]="SuiteH"; t["n"]=11; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000711000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T13) {
  json t; t["s"]="SuiteH"; t["n"]=12; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000712000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T14) {
  json t; t["s"]="SuiteH"; t["n"]=13; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000713000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T15) {
  json t; t["s"]="SuiteH"; t["n"]=14; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000714000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T16) {
  json t; t["s"]="SuiteH"; t["n"]=15; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000715000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T17) {
  json t; t["s"]="SuiteH"; t["n"]=16; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000716000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T18) {
  json t; t["s"]="SuiteH"; t["n"]=17; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000717000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T19) {
  json t; t["s"]="SuiteH"; t["n"]=18; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000718000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T20) {
  json t; t["s"]="SuiteH"; t["n"]=19; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000719000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T21) {
  json t; t["s"]="SuiteH"; t["n"]=20; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000720000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T22) {
  json t; t["s"]="SuiteH"; t["n"]=21; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000721000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T23) {
  json t; t["s"]="SuiteH"; t["n"]=22; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000722000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T24) {
  json t; t["s"]="SuiteH"; t["n"]=23; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000723000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T25) {
  json t; t["s"]="SuiteH"; t["n"]=24; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000724000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T26) {
  json t; t["s"]="SuiteH"; t["n"]=25; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000725000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T27) {
  json t; t["s"]="SuiteH"; t["n"]=26; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000726000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T28) {
  json t; t["s"]="SuiteH"; t["n"]=27; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000727000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T29) {
  json t; t["s"]="SuiteH"; t["n"]=28; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000728000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T30) {
  json t; t["s"]="SuiteH"; t["n"]=29; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000729000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T31) {
  json t; t["s"]="SuiteH"; t["n"]=30; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000730000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T32) {
  json t; t["s"]="SuiteH"; t["n"]=31; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000731000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T33) {
  json t; t["s"]="SuiteH"; t["n"]=32; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000732000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T34) {
  json t; t["s"]="SuiteH"; t["n"]=33; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000733000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T35) {
  json t; t["s"]="SuiteH"; t["n"]=34; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000734000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T36) {
  json t; t["s"]="SuiteH"; t["n"]=35; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000735000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T37) {
  json t; t["s"]="SuiteH"; t["n"]=36; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000736000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T38) {
  json t; t["s"]="SuiteH"; t["n"]=37; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000737000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T39) {
  json t; t["s"]="SuiteH"; t["n"]=38; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000738000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T40) {
  json t; t["s"]="SuiteH"; t["n"]=39; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000739000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T41) {
  json t; t["s"]="SuiteH"; t["n"]=40; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000740000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T42) {
  json t; t["s"]="SuiteH"; t["n"]=41; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000741000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T43) {
  json t; t["s"]="SuiteH"; t["n"]=42; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000742000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T44) {
  json t; t["s"]="SuiteH"; t["n"]=43; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000743000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T45) {
  json t; t["s"]="SuiteH"; t["n"]=44; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000744000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T46) {
  json t; t["s"]="SuiteH"; t["n"]=45; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000745000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T47) {
  json t; t["s"]="SuiteH"; t["n"]=46; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000746000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T48) {
  json t; t["s"]="SuiteH"; t["n"]=47; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000747000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T49) {
  json t; t["s"]="SuiteH"; t["n"]=48; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000748000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T50) {
  json t; t["s"]="SuiteH"; t["n"]=49; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000749000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T51) {
  json t; t["s"]="SuiteH"; t["n"]=50; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000750000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T52) {
  json t; t["s"]="SuiteH"; t["n"]=51; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000751000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T53) {
  json t; t["s"]="SuiteH"; t["n"]=52; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000752000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T54) {
  json t; t["s"]="SuiteH"; t["n"]=53; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000753000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T55) {
  json t; t["s"]="SuiteH"; t["n"]=54; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000754000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T56) {
  json t; t["s"]="SuiteH"; t["n"]=55; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000755000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T57) {
  json t; t["s"]="SuiteH"; t["n"]=56; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000756000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T58) {
  json t; t["s"]="SuiteH"; t["n"]=57; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000757000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T59) {
  json t; t["s"]="SuiteH"; t["n"]=58; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000758000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T60) {
  json t; t["s"]="SuiteH"; t["n"]=59; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000759000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T61) {
  json t; t["s"]="SuiteH"; t["n"]=60; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000760000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T62) {
  json t; t["s"]="SuiteH"; t["n"]=61; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000761000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T63) {
  json t; t["s"]="SuiteH"; t["n"]=62; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000762000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T64) {
  json t; t["s"]="SuiteH"; t["n"]=63; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000763000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T65) {
  json t; t["s"]="SuiteH"; t["n"]=64; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000764000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T66) {
  json t; t["s"]="SuiteH"; t["n"]=65; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000765000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T67) {
  json t; t["s"]="SuiteH"; t["n"]=66; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000766000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T68) {
  json t; t["s"]="SuiteH"; t["n"]=67; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000767000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T69) {
  json t; t["s"]="SuiteH"; t["n"]=68; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000768000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteH, T70) {
  json t; t["s"]="SuiteH"; t["n"]=69; t["b"]=7;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(7) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_7_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000769000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
