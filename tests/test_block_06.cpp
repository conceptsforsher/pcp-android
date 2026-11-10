#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block06, T1) {
  json t; t["s"]="Block06"; t["n"]=0; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003100000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T2) {
  json t; t["s"]="Block06"; t["n"]=1; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003101000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T3) {
  json t; t["s"]="Block06"; t["n"]=2; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003102000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T4) {
  json t; t["s"]="Block06"; t["n"]=3; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003103000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T5) {
  json t; t["s"]="Block06"; t["n"]=4; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003104000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T6) {
  json t; t["s"]="Block06"; t["n"]=5; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003105000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T7) {
  json t; t["s"]="Block06"; t["n"]=6; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003106000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T8) {
  json t; t["s"]="Block06"; t["n"]=7; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003107000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T9) {
  json t; t["s"]="Block06"; t["n"]=8; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003108000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T10) {
  json t; t["s"]="Block06"; t["n"]=9; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003109000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T11) {
  json t; t["s"]="Block06"; t["n"]=10; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003110000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T12) {
  json t; t["s"]="Block06"; t["n"]=11; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003111000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T13) {
  json t; t["s"]="Block06"; t["n"]=12; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003112000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T14) {
  json t; t["s"]="Block06"; t["n"]=13; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003113000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T15) {
  json t; t["s"]="Block06"; t["n"]=14; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003114000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T16) {
  json t; t["s"]="Block06"; t["n"]=15; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003115000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T17) {
  json t; t["s"]="Block06"; t["n"]=16; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003116000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T18) {
  json t; t["s"]="Block06"; t["n"]=17; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003117000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T19) {
  json t; t["s"]="Block06"; t["n"]=18; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003118000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T20) {
  json t; t["s"]="Block06"; t["n"]=19; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003119000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T21) {
  json t; t["s"]="Block06"; t["n"]=20; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003120000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T22) {
  json t; t["s"]="Block06"; t["n"]=21; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003121000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T23) {
  json t; t["s"]="Block06"; t["n"]=22; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003122000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T24) {
  json t; t["s"]="Block06"; t["n"]=23; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003123000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T25) {
  json t; t["s"]="Block06"; t["n"]=24; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003124000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T26) {
  json t; t["s"]="Block06"; t["n"]=25; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003125000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T27) {
  json t; t["s"]="Block06"; t["n"]=26; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003126000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T28) {
  json t; t["s"]="Block06"; t["n"]=27; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003127000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T29) {
  json t; t["s"]="Block06"; t["n"]=28; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003128000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T30) {
  json t; t["s"]="Block06"; t["n"]=29; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003129000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T31) {
  json t; t["s"]="Block06"; t["n"]=30; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003130000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T32) {
  json t; t["s"]="Block06"; t["n"]=31; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003131000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T33) {
  json t; t["s"]="Block06"; t["n"]=32; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003132000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T34) {
  json t; t["s"]="Block06"; t["n"]=33; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003133000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T35) {
  json t; t["s"]="Block06"; t["n"]=34; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003134000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T36) {
  json t; t["s"]="Block06"; t["n"]=35; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003135000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T37) {
  json t; t["s"]="Block06"; t["n"]=36; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003136000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T38) {
  json t; t["s"]="Block06"; t["n"]=37; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003137000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T39) {
  json t; t["s"]="Block06"; t["n"]=38; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003138000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T40) {
  json t; t["s"]="Block06"; t["n"]=39; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003139000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T41) {
  json t; t["s"]="Block06"; t["n"]=40; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003140000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T42) {
  json t; t["s"]="Block06"; t["n"]=41; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003141000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T43) {
  json t; t["s"]="Block06"; t["n"]=42; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003142000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T44) {
  json t; t["s"]="Block06"; t["n"]=43; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003143000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T45) {
  json t; t["s"]="Block06"; t["n"]=44; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003144000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T46) {
  json t; t["s"]="Block06"; t["n"]=45; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003145000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T47) {
  json t; t["s"]="Block06"; t["n"]=46; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003146000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T48) {
  json t; t["s"]="Block06"; t["n"]=47; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003147000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T49) {
  json t; t["s"]="Block06"; t["n"]=48; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003148000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T50) {
  json t; t["s"]="Block06"; t["n"]=49; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003149000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T51) {
  json t; t["s"]="Block06"; t["n"]=50; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003150000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T52) {
  json t; t["s"]="Block06"; t["n"]=51; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003151000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T53) {
  json t; t["s"]="Block06"; t["n"]=52; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003152000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T54) {
  json t; t["s"]="Block06"; t["n"]=53; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003153000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T55) {
  json t; t["s"]="Block06"; t["n"]=54; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003154000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T56) {
  json t; t["s"]="Block06"; t["n"]=55; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003155000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T57) {
  json t; t["s"]="Block06"; t["n"]=56; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003156000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T58) {
  json t; t["s"]="Block06"; t["n"]=57; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003157000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T59) {
  json t; t["s"]="Block06"; t["n"]=58; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003158000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T60) {
  json t; t["s"]="Block06"; t["n"]=59; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003159000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T61) {
  json t; t["s"]="Block06"; t["n"]=60; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003160000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T62) {
  json t; t["s"]="Block06"; t["n"]=61; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003161000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T63) {
  json t; t["s"]="Block06"; t["n"]=62; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003162000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T64) {
  json t; t["s"]="Block06"; t["n"]=63; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003163000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T65) {
  json t; t["s"]="Block06"; t["n"]=64; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003164000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T66) {
  json t; t["s"]="Block06"; t["n"]=65; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003165000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T67) {
  json t; t["s"]="Block06"; t["n"]=66; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003166000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T68) {
  json t; t["s"]="Block06"; t["n"]=67; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003167000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T69) {
  json t; t["s"]="Block06"; t["n"]=68; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003168000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block06, T70) {
  json t; t["s"]="Block06"; t["n"]=69; t["b"]=31;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(31) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_31_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003169000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
