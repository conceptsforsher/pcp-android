#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block05, T1) {
  json t; t["s"]="Block05"; t["n"]=0; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003000000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T2) {
  json t; t["s"]="Block05"; t["n"]=1; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003001000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T3) {
  json t; t["s"]="Block05"; t["n"]=2; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003002000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T4) {
  json t; t["s"]="Block05"; t["n"]=3; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003003000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T5) {
  json t; t["s"]="Block05"; t["n"]=4; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003004000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T6) {
  json t; t["s"]="Block05"; t["n"]=5; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003005000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T7) {
  json t; t["s"]="Block05"; t["n"]=6; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003006000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T8) {
  json t; t["s"]="Block05"; t["n"]=7; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003007000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T9) {
  json t; t["s"]="Block05"; t["n"]=8; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003008000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T10) {
  json t; t["s"]="Block05"; t["n"]=9; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003009000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T11) {
  json t; t["s"]="Block05"; t["n"]=10; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003010000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T12) {
  json t; t["s"]="Block05"; t["n"]=11; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003011000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T13) {
  json t; t["s"]="Block05"; t["n"]=12; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003012000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T14) {
  json t; t["s"]="Block05"; t["n"]=13; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003013000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T15) {
  json t; t["s"]="Block05"; t["n"]=14; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003014000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T16) {
  json t; t["s"]="Block05"; t["n"]=15; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003015000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T17) {
  json t; t["s"]="Block05"; t["n"]=16; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003016000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T18) {
  json t; t["s"]="Block05"; t["n"]=17; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003017000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T19) {
  json t; t["s"]="Block05"; t["n"]=18; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003018000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T20) {
  json t; t["s"]="Block05"; t["n"]=19; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003019000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T21) {
  json t; t["s"]="Block05"; t["n"]=20; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003020000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T22) {
  json t; t["s"]="Block05"; t["n"]=21; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003021000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T23) {
  json t; t["s"]="Block05"; t["n"]=22; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003022000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T24) {
  json t; t["s"]="Block05"; t["n"]=23; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003023000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T25) {
  json t; t["s"]="Block05"; t["n"]=24; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003024000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T26) {
  json t; t["s"]="Block05"; t["n"]=25; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003025000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T27) {
  json t; t["s"]="Block05"; t["n"]=26; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003026000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T28) {
  json t; t["s"]="Block05"; t["n"]=27; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003027000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T29) {
  json t; t["s"]="Block05"; t["n"]=28; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003028000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T30) {
  json t; t["s"]="Block05"; t["n"]=29; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003029000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T31) {
  json t; t["s"]="Block05"; t["n"]=30; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003030000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T32) {
  json t; t["s"]="Block05"; t["n"]=31; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003031000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T33) {
  json t; t["s"]="Block05"; t["n"]=32; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003032000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T34) {
  json t; t["s"]="Block05"; t["n"]=33; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003033000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T35) {
  json t; t["s"]="Block05"; t["n"]=34; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003034000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T36) {
  json t; t["s"]="Block05"; t["n"]=35; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003035000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T37) {
  json t; t["s"]="Block05"; t["n"]=36; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003036000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T38) {
  json t; t["s"]="Block05"; t["n"]=37; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003037000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T39) {
  json t; t["s"]="Block05"; t["n"]=38; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003038000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T40) {
  json t; t["s"]="Block05"; t["n"]=39; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003039000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T41) {
  json t; t["s"]="Block05"; t["n"]=40; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003040000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T42) {
  json t; t["s"]="Block05"; t["n"]=41; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003041000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T43) {
  json t; t["s"]="Block05"; t["n"]=42; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003042000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T44) {
  json t; t["s"]="Block05"; t["n"]=43; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003043000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T45) {
  json t; t["s"]="Block05"; t["n"]=44; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003044000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T46) {
  json t; t["s"]="Block05"; t["n"]=45; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003045000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T47) {
  json t; t["s"]="Block05"; t["n"]=46; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003046000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T48) {
  json t; t["s"]="Block05"; t["n"]=47; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003047000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T49) {
  json t; t["s"]="Block05"; t["n"]=48; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003048000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T50) {
  json t; t["s"]="Block05"; t["n"]=49; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003049000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T51) {
  json t; t["s"]="Block05"; t["n"]=50; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003050000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T52) {
  json t; t["s"]="Block05"; t["n"]=51; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003051000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T53) {
  json t; t["s"]="Block05"; t["n"]=52; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003052000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T54) {
  json t; t["s"]="Block05"; t["n"]=53; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003053000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T55) {
  json t; t["s"]="Block05"; t["n"]=54; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003054000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T56) {
  json t; t["s"]="Block05"; t["n"]=55; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003055000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T57) {
  json t; t["s"]="Block05"; t["n"]=56; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003056000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T58) {
  json t; t["s"]="Block05"; t["n"]=57; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003057000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T59) {
  json t; t["s"]="Block05"; t["n"]=58; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003058000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T60) {
  json t; t["s"]="Block05"; t["n"]=59; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003059000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T61) {
  json t; t["s"]="Block05"; t["n"]=60; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003060000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T62) {
  json t; t["s"]="Block05"; t["n"]=61; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003061000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T63) {
  json t; t["s"]="Block05"; t["n"]=62; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003062000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T64) {
  json t; t["s"]="Block05"; t["n"]=63; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003063000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T65) {
  json t; t["s"]="Block05"; t["n"]=64; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003064000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T66) {
  json t; t["s"]="Block05"; t["n"]=65; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003065000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T67) {
  json t; t["s"]="Block05"; t["n"]=66; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003066000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T68) {
  json t; t["s"]="Block05"; t["n"]=67; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003067000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T69) {
  json t; t["s"]="Block05"; t["n"]=68; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003068000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block05, T70) {
  json t; t["s"]="Block05"; t["n"]=69; t["b"]=30;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(30) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_30_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003069000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
