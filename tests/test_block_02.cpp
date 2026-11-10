#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block02, T1) {
  json t; t["s"]="Block02"; t["n"]=0; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002700000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T2) {
  json t; t["s"]="Block02"; t["n"]=1; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002701000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T3) {
  json t; t["s"]="Block02"; t["n"]=2; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002702000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T4) {
  json t; t["s"]="Block02"; t["n"]=3; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002703000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T5) {
  json t; t["s"]="Block02"; t["n"]=4; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002704000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T6) {
  json t; t["s"]="Block02"; t["n"]=5; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002705000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T7) {
  json t; t["s"]="Block02"; t["n"]=6; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002706000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T8) {
  json t; t["s"]="Block02"; t["n"]=7; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002707000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T9) {
  json t; t["s"]="Block02"; t["n"]=8; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002708000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T10) {
  json t; t["s"]="Block02"; t["n"]=9; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002709000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T11) {
  json t; t["s"]="Block02"; t["n"]=10; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002710000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T12) {
  json t; t["s"]="Block02"; t["n"]=11; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002711000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T13) {
  json t; t["s"]="Block02"; t["n"]=12; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002712000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T14) {
  json t; t["s"]="Block02"; t["n"]=13; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002713000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T15) {
  json t; t["s"]="Block02"; t["n"]=14; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002714000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T16) {
  json t; t["s"]="Block02"; t["n"]=15; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002715000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T17) {
  json t; t["s"]="Block02"; t["n"]=16; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002716000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T18) {
  json t; t["s"]="Block02"; t["n"]=17; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002717000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T19) {
  json t; t["s"]="Block02"; t["n"]=18; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002718000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T20) {
  json t; t["s"]="Block02"; t["n"]=19; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002719000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T21) {
  json t; t["s"]="Block02"; t["n"]=20; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002720000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T22) {
  json t; t["s"]="Block02"; t["n"]=21; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002721000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T23) {
  json t; t["s"]="Block02"; t["n"]=22; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002722000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T24) {
  json t; t["s"]="Block02"; t["n"]=23; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002723000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T25) {
  json t; t["s"]="Block02"; t["n"]=24; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002724000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T26) {
  json t; t["s"]="Block02"; t["n"]=25; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002725000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T27) {
  json t; t["s"]="Block02"; t["n"]=26; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002726000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T28) {
  json t; t["s"]="Block02"; t["n"]=27; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002727000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T29) {
  json t; t["s"]="Block02"; t["n"]=28; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002728000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T30) {
  json t; t["s"]="Block02"; t["n"]=29; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002729000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T31) {
  json t; t["s"]="Block02"; t["n"]=30; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002730000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T32) {
  json t; t["s"]="Block02"; t["n"]=31; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002731000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T33) {
  json t; t["s"]="Block02"; t["n"]=32; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002732000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T34) {
  json t; t["s"]="Block02"; t["n"]=33; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002733000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T35) {
  json t; t["s"]="Block02"; t["n"]=34; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002734000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T36) {
  json t; t["s"]="Block02"; t["n"]=35; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002735000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T37) {
  json t; t["s"]="Block02"; t["n"]=36; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002736000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T38) {
  json t; t["s"]="Block02"; t["n"]=37; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002737000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T39) {
  json t; t["s"]="Block02"; t["n"]=38; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002738000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T40) {
  json t; t["s"]="Block02"; t["n"]=39; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002739000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T41) {
  json t; t["s"]="Block02"; t["n"]=40; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002740000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T42) {
  json t; t["s"]="Block02"; t["n"]=41; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002741000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T43) {
  json t; t["s"]="Block02"; t["n"]=42; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002742000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T44) {
  json t; t["s"]="Block02"; t["n"]=43; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002743000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T45) {
  json t; t["s"]="Block02"; t["n"]=44; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002744000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T46) {
  json t; t["s"]="Block02"; t["n"]=45; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002745000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T47) {
  json t; t["s"]="Block02"; t["n"]=46; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002746000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T48) {
  json t; t["s"]="Block02"; t["n"]=47; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002747000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T49) {
  json t; t["s"]="Block02"; t["n"]=48; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002748000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T50) {
  json t; t["s"]="Block02"; t["n"]=49; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002749000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T51) {
  json t; t["s"]="Block02"; t["n"]=50; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002750000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T52) {
  json t; t["s"]="Block02"; t["n"]=51; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002751000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T53) {
  json t; t["s"]="Block02"; t["n"]=52; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002752000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T54) {
  json t; t["s"]="Block02"; t["n"]=53; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002753000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T55) {
  json t; t["s"]="Block02"; t["n"]=54; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002754000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T56) {
  json t; t["s"]="Block02"; t["n"]=55; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002755000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T57) {
  json t; t["s"]="Block02"; t["n"]=56; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002756000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T58) {
  json t; t["s"]="Block02"; t["n"]=57; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002757000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T59) {
  json t; t["s"]="Block02"; t["n"]=58; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002758000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T60) {
  json t; t["s"]="Block02"; t["n"]=59; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002759000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T61) {
  json t; t["s"]="Block02"; t["n"]=60; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002760000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T62) {
  json t; t["s"]="Block02"; t["n"]=61; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002761000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T63) {
  json t; t["s"]="Block02"; t["n"]=62; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002762000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T64) {
  json t; t["s"]="Block02"; t["n"]=63; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002763000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T65) {
  json t; t["s"]="Block02"; t["n"]=64; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002764000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T66) {
  json t; t["s"]="Block02"; t["n"]=65; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002765000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T67) {
  json t; t["s"]="Block02"; t["n"]=66; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002766000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T68) {
  json t; t["s"]="Block02"; t["n"]=67; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002767000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T69) {
  json t; t["s"]="Block02"; t["n"]=68; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002768000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block02, T70) {
  json t; t["s"]="Block02"; t["n"]=69; t["b"]=27;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(27) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_27_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002769000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
