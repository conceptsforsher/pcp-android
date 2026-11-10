#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block09, T1) {
  json t; t["s"]="Block09"; t["n"]=0; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003400000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T2) {
  json t; t["s"]="Block09"; t["n"]=1; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003401000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T3) {
  json t; t["s"]="Block09"; t["n"]=2; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003402000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T4) {
  json t; t["s"]="Block09"; t["n"]=3; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003403000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T5) {
  json t; t["s"]="Block09"; t["n"]=4; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003404000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T6) {
  json t; t["s"]="Block09"; t["n"]=5; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003405000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T7) {
  json t; t["s"]="Block09"; t["n"]=6; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003406000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T8) {
  json t; t["s"]="Block09"; t["n"]=7; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003407000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T9) {
  json t; t["s"]="Block09"; t["n"]=8; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003408000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T10) {
  json t; t["s"]="Block09"; t["n"]=9; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003409000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T11) {
  json t; t["s"]="Block09"; t["n"]=10; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003410000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T12) {
  json t; t["s"]="Block09"; t["n"]=11; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003411000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T13) {
  json t; t["s"]="Block09"; t["n"]=12; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003412000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T14) {
  json t; t["s"]="Block09"; t["n"]=13; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003413000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T15) {
  json t; t["s"]="Block09"; t["n"]=14; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003414000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T16) {
  json t; t["s"]="Block09"; t["n"]=15; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003415000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T17) {
  json t; t["s"]="Block09"; t["n"]=16; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003416000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T18) {
  json t; t["s"]="Block09"; t["n"]=17; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003417000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T19) {
  json t; t["s"]="Block09"; t["n"]=18; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003418000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T20) {
  json t; t["s"]="Block09"; t["n"]=19; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003419000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T21) {
  json t; t["s"]="Block09"; t["n"]=20; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003420000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T22) {
  json t; t["s"]="Block09"; t["n"]=21; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003421000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T23) {
  json t; t["s"]="Block09"; t["n"]=22; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003422000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T24) {
  json t; t["s"]="Block09"; t["n"]=23; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003423000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T25) {
  json t; t["s"]="Block09"; t["n"]=24; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003424000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T26) {
  json t; t["s"]="Block09"; t["n"]=25; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003425000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T27) {
  json t; t["s"]="Block09"; t["n"]=26; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003426000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T28) {
  json t; t["s"]="Block09"; t["n"]=27; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003427000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T29) {
  json t; t["s"]="Block09"; t["n"]=28; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003428000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T30) {
  json t; t["s"]="Block09"; t["n"]=29; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003429000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T31) {
  json t; t["s"]="Block09"; t["n"]=30; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003430000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T32) {
  json t; t["s"]="Block09"; t["n"]=31; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003431000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T33) {
  json t; t["s"]="Block09"; t["n"]=32; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003432000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T34) {
  json t; t["s"]="Block09"; t["n"]=33; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003433000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T35) {
  json t; t["s"]="Block09"; t["n"]=34; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003434000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T36) {
  json t; t["s"]="Block09"; t["n"]=35; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003435000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T37) {
  json t; t["s"]="Block09"; t["n"]=36; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003436000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T38) {
  json t; t["s"]="Block09"; t["n"]=37; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003437000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T39) {
  json t; t["s"]="Block09"; t["n"]=38; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003438000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T40) {
  json t; t["s"]="Block09"; t["n"]=39; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003439000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T41) {
  json t; t["s"]="Block09"; t["n"]=40; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003440000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T42) {
  json t; t["s"]="Block09"; t["n"]=41; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003441000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T43) {
  json t; t["s"]="Block09"; t["n"]=42; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003442000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T44) {
  json t; t["s"]="Block09"; t["n"]=43; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003443000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T45) {
  json t; t["s"]="Block09"; t["n"]=44; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003444000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T46) {
  json t; t["s"]="Block09"; t["n"]=45; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003445000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T47) {
  json t; t["s"]="Block09"; t["n"]=46; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003446000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T48) {
  json t; t["s"]="Block09"; t["n"]=47; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003447000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T49) {
  json t; t["s"]="Block09"; t["n"]=48; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003448000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T50) {
  json t; t["s"]="Block09"; t["n"]=49; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003449000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T51) {
  json t; t["s"]="Block09"; t["n"]=50; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003450000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T52) {
  json t; t["s"]="Block09"; t["n"]=51; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003451000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T53) {
  json t; t["s"]="Block09"; t["n"]=52; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003452000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T54) {
  json t; t["s"]="Block09"; t["n"]=53; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003453000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T55) {
  json t; t["s"]="Block09"; t["n"]=54; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003454000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T56) {
  json t; t["s"]="Block09"; t["n"]=55; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003455000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T57) {
  json t; t["s"]="Block09"; t["n"]=56; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003456000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T58) {
  json t; t["s"]="Block09"; t["n"]=57; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003457000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T59) {
  json t; t["s"]="Block09"; t["n"]=58; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003458000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T60) {
  json t; t["s"]="Block09"; t["n"]=59; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003459000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T61) {
  json t; t["s"]="Block09"; t["n"]=60; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003460000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T62) {
  json t; t["s"]="Block09"; t["n"]=61; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003461000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T63) {
  json t; t["s"]="Block09"; t["n"]=62; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003462000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T64) {
  json t; t["s"]="Block09"; t["n"]=63; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003463000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T65) {
  json t; t["s"]="Block09"; t["n"]=64; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003464000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T66) {
  json t; t["s"]="Block09"; t["n"]=65; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003465000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T67) {
  json t; t["s"]="Block09"; t["n"]=66; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003466000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T68) {
  json t; t["s"]="Block09"; t["n"]=67; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003467000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T69) {
  json t; t["s"]="Block09"; t["n"]=68; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003468000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block09, T70) {
  json t; t["s"]="Block09"; t["n"]=69; t["b"]=34;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(34) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_34_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003469000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
