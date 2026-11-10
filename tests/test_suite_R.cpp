#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteR, T1) {
  json t; t["s"]="SuiteR"; t["n"]=0; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001700000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T2) {
  json t; t["s"]="SuiteR"; t["n"]=1; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001701000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T3) {
  json t; t["s"]="SuiteR"; t["n"]=2; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001702000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T4) {
  json t; t["s"]="SuiteR"; t["n"]=3; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001703000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T5) {
  json t; t["s"]="SuiteR"; t["n"]=4; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001704000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T6) {
  json t; t["s"]="SuiteR"; t["n"]=5; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001705000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T7) {
  json t; t["s"]="SuiteR"; t["n"]=6; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001706000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T8) {
  json t; t["s"]="SuiteR"; t["n"]=7; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001707000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T9) {
  json t; t["s"]="SuiteR"; t["n"]=8; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001708000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T10) {
  json t; t["s"]="SuiteR"; t["n"]=9; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001709000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T11) {
  json t; t["s"]="SuiteR"; t["n"]=10; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001710000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T12) {
  json t; t["s"]="SuiteR"; t["n"]=11; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001711000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T13) {
  json t; t["s"]="SuiteR"; t["n"]=12; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001712000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T14) {
  json t; t["s"]="SuiteR"; t["n"]=13; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001713000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T15) {
  json t; t["s"]="SuiteR"; t["n"]=14; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001714000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T16) {
  json t; t["s"]="SuiteR"; t["n"]=15; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001715000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T17) {
  json t; t["s"]="SuiteR"; t["n"]=16; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001716000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T18) {
  json t; t["s"]="SuiteR"; t["n"]=17; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001717000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T19) {
  json t; t["s"]="SuiteR"; t["n"]=18; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001718000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T20) {
  json t; t["s"]="SuiteR"; t["n"]=19; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001719000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T21) {
  json t; t["s"]="SuiteR"; t["n"]=20; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001720000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T22) {
  json t; t["s"]="SuiteR"; t["n"]=21; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001721000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T23) {
  json t; t["s"]="SuiteR"; t["n"]=22; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001722000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T24) {
  json t; t["s"]="SuiteR"; t["n"]=23; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001723000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T25) {
  json t; t["s"]="SuiteR"; t["n"]=24; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001724000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T26) {
  json t; t["s"]="SuiteR"; t["n"]=25; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001725000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T27) {
  json t; t["s"]="SuiteR"; t["n"]=26; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001726000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T28) {
  json t; t["s"]="SuiteR"; t["n"]=27; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001727000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T29) {
  json t; t["s"]="SuiteR"; t["n"]=28; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001728000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T30) {
  json t; t["s"]="SuiteR"; t["n"]=29; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001729000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T31) {
  json t; t["s"]="SuiteR"; t["n"]=30; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001730000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T32) {
  json t; t["s"]="SuiteR"; t["n"]=31; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001731000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T33) {
  json t; t["s"]="SuiteR"; t["n"]=32; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001732000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T34) {
  json t; t["s"]="SuiteR"; t["n"]=33; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001733000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T35) {
  json t; t["s"]="SuiteR"; t["n"]=34; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001734000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T36) {
  json t; t["s"]="SuiteR"; t["n"]=35; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001735000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T37) {
  json t; t["s"]="SuiteR"; t["n"]=36; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001736000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T38) {
  json t; t["s"]="SuiteR"; t["n"]=37; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001737000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T39) {
  json t; t["s"]="SuiteR"; t["n"]=38; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001738000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T40) {
  json t; t["s"]="SuiteR"; t["n"]=39; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001739000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T41) {
  json t; t["s"]="SuiteR"; t["n"]=40; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001740000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T42) {
  json t; t["s"]="SuiteR"; t["n"]=41; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001741000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T43) {
  json t; t["s"]="SuiteR"; t["n"]=42; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001742000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T44) {
  json t; t["s"]="SuiteR"; t["n"]=43; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001743000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T45) {
  json t; t["s"]="SuiteR"; t["n"]=44; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001744000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T46) {
  json t; t["s"]="SuiteR"; t["n"]=45; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001745000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T47) {
  json t; t["s"]="SuiteR"; t["n"]=46; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001746000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T48) {
  json t; t["s"]="SuiteR"; t["n"]=47; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001747000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T49) {
  json t; t["s"]="SuiteR"; t["n"]=48; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001748000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T50) {
  json t; t["s"]="SuiteR"; t["n"]=49; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001749000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T51) {
  json t; t["s"]="SuiteR"; t["n"]=50; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001750000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T52) {
  json t; t["s"]="SuiteR"; t["n"]=51; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001751000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T53) {
  json t; t["s"]="SuiteR"; t["n"]=52; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001752000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T54) {
  json t; t["s"]="SuiteR"; t["n"]=53; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001753000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T55) {
  json t; t["s"]="SuiteR"; t["n"]=54; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001754000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T56) {
  json t; t["s"]="SuiteR"; t["n"]=55; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001755000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T57) {
  json t; t["s"]="SuiteR"; t["n"]=56; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001756000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T58) {
  json t; t["s"]="SuiteR"; t["n"]=57; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001757000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T59) {
  json t; t["s"]="SuiteR"; t["n"]=58; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001758000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T60) {
  json t; t["s"]="SuiteR"; t["n"]=59; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001759000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T61) {
  json t; t["s"]="SuiteR"; t["n"]=60; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001760000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T62) {
  json t; t["s"]="SuiteR"; t["n"]=61; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001761000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T63) {
  json t; t["s"]="SuiteR"; t["n"]=62; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001762000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T64) {
  json t; t["s"]="SuiteR"; t["n"]=63; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001763000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T65) {
  json t; t["s"]="SuiteR"; t["n"]=64; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001764000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T66) {
  json t; t["s"]="SuiteR"; t["n"]=65; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001765000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T67) {
  json t; t["s"]="SuiteR"; t["n"]=66; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001766000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T68) {
  json t; t["s"]="SuiteR"; t["n"]=67; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001767000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T69) {
  json t; t["s"]="SuiteR"; t["n"]=68; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001768000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteR, T70) {
  json t; t["s"]="SuiteR"; t["n"]=69; t["b"]=17;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(17) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_17_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001769000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
