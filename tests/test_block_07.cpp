#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block07, T1) {
  json t; t["s"]="Block07"; t["n"]=0; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003200000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T2) {
  json t; t["s"]="Block07"; t["n"]=1; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003201000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T3) {
  json t; t["s"]="Block07"; t["n"]=2; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003202000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T4) {
  json t; t["s"]="Block07"; t["n"]=3; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003203000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T5) {
  json t; t["s"]="Block07"; t["n"]=4; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003204000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T6) {
  json t; t["s"]="Block07"; t["n"]=5; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003205000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T7) {
  json t; t["s"]="Block07"; t["n"]=6; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003206000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T8) {
  json t; t["s"]="Block07"; t["n"]=7; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003207000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T9) {
  json t; t["s"]="Block07"; t["n"]=8; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003208000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T10) {
  json t; t["s"]="Block07"; t["n"]=9; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003209000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T11) {
  json t; t["s"]="Block07"; t["n"]=10; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003210000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T12) {
  json t; t["s"]="Block07"; t["n"]=11; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003211000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T13) {
  json t; t["s"]="Block07"; t["n"]=12; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003212000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T14) {
  json t; t["s"]="Block07"; t["n"]=13; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003213000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T15) {
  json t; t["s"]="Block07"; t["n"]=14; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003214000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T16) {
  json t; t["s"]="Block07"; t["n"]=15; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003215000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T17) {
  json t; t["s"]="Block07"; t["n"]=16; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003216000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T18) {
  json t; t["s"]="Block07"; t["n"]=17; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003217000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T19) {
  json t; t["s"]="Block07"; t["n"]=18; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003218000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T20) {
  json t; t["s"]="Block07"; t["n"]=19; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003219000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T21) {
  json t; t["s"]="Block07"; t["n"]=20; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003220000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T22) {
  json t; t["s"]="Block07"; t["n"]=21; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003221000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T23) {
  json t; t["s"]="Block07"; t["n"]=22; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003222000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T24) {
  json t; t["s"]="Block07"; t["n"]=23; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003223000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T25) {
  json t; t["s"]="Block07"; t["n"]=24; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003224000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T26) {
  json t; t["s"]="Block07"; t["n"]=25; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003225000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T27) {
  json t; t["s"]="Block07"; t["n"]=26; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003226000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T28) {
  json t; t["s"]="Block07"; t["n"]=27; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003227000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T29) {
  json t; t["s"]="Block07"; t["n"]=28; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003228000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T30) {
  json t; t["s"]="Block07"; t["n"]=29; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003229000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T31) {
  json t; t["s"]="Block07"; t["n"]=30; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003230000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T32) {
  json t; t["s"]="Block07"; t["n"]=31; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003231000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T33) {
  json t; t["s"]="Block07"; t["n"]=32; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003232000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T34) {
  json t; t["s"]="Block07"; t["n"]=33; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003233000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T35) {
  json t; t["s"]="Block07"; t["n"]=34; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003234000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T36) {
  json t; t["s"]="Block07"; t["n"]=35; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003235000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T37) {
  json t; t["s"]="Block07"; t["n"]=36; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003236000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T38) {
  json t; t["s"]="Block07"; t["n"]=37; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003237000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T39) {
  json t; t["s"]="Block07"; t["n"]=38; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003238000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T40) {
  json t; t["s"]="Block07"; t["n"]=39; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003239000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T41) {
  json t; t["s"]="Block07"; t["n"]=40; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003240000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T42) {
  json t; t["s"]="Block07"; t["n"]=41; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003241000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T43) {
  json t; t["s"]="Block07"; t["n"]=42; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003242000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T44) {
  json t; t["s"]="Block07"; t["n"]=43; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003243000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T45) {
  json t; t["s"]="Block07"; t["n"]=44; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003244000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T46) {
  json t; t["s"]="Block07"; t["n"]=45; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003245000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T47) {
  json t; t["s"]="Block07"; t["n"]=46; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003246000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T48) {
  json t; t["s"]="Block07"; t["n"]=47; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003247000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T49) {
  json t; t["s"]="Block07"; t["n"]=48; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003248000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T50) {
  json t; t["s"]="Block07"; t["n"]=49; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003249000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T51) {
  json t; t["s"]="Block07"; t["n"]=50; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003250000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T52) {
  json t; t["s"]="Block07"; t["n"]=51; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003251000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T53) {
  json t; t["s"]="Block07"; t["n"]=52; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003252000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T54) {
  json t; t["s"]="Block07"; t["n"]=53; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003253000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T55) {
  json t; t["s"]="Block07"; t["n"]=54; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003254000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T56) {
  json t; t["s"]="Block07"; t["n"]=55; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003255000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T57) {
  json t; t["s"]="Block07"; t["n"]=56; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003256000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T58) {
  json t; t["s"]="Block07"; t["n"]=57; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003257000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T59) {
  json t; t["s"]="Block07"; t["n"]=58; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003258000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T60) {
  json t; t["s"]="Block07"; t["n"]=59; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003259000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T61) {
  json t; t["s"]="Block07"; t["n"]=60; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003260000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T62) {
  json t; t["s"]="Block07"; t["n"]=61; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003261000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T63) {
  json t; t["s"]="Block07"; t["n"]=62; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003262000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T64) {
  json t; t["s"]="Block07"; t["n"]=63; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003263000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T65) {
  json t; t["s"]="Block07"; t["n"]=64; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003264000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T66) {
  json t; t["s"]="Block07"; t["n"]=65; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003265000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T67) {
  json t; t["s"]="Block07"; t["n"]=66; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003266000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T68) {
  json t; t["s"]="Block07"; t["n"]=67; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003267000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T69) {
  json t; t["s"]="Block07"; t["n"]=68; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003268000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block07, T70) {
  json t; t["s"]="Block07"; t["n"]=69; t["b"]=32;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(32) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_32_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003269000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
