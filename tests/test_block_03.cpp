#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block03, T1) {
  json t; t["s"]="Block03"; t["n"]=0; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002800000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T2) {
  json t; t["s"]="Block03"; t["n"]=1; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002801000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T3) {
  json t; t["s"]="Block03"; t["n"]=2; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002802000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T4) {
  json t; t["s"]="Block03"; t["n"]=3; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002803000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T5) {
  json t; t["s"]="Block03"; t["n"]=4; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002804000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T6) {
  json t; t["s"]="Block03"; t["n"]=5; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002805000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T7) {
  json t; t["s"]="Block03"; t["n"]=6; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002806000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T8) {
  json t; t["s"]="Block03"; t["n"]=7; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002807000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T9) {
  json t; t["s"]="Block03"; t["n"]=8; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002808000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T10) {
  json t; t["s"]="Block03"; t["n"]=9; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002809000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T11) {
  json t; t["s"]="Block03"; t["n"]=10; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002810000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T12) {
  json t; t["s"]="Block03"; t["n"]=11; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002811000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T13) {
  json t; t["s"]="Block03"; t["n"]=12; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002812000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T14) {
  json t; t["s"]="Block03"; t["n"]=13; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002813000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T15) {
  json t; t["s"]="Block03"; t["n"]=14; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002814000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T16) {
  json t; t["s"]="Block03"; t["n"]=15; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002815000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T17) {
  json t; t["s"]="Block03"; t["n"]=16; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002816000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T18) {
  json t; t["s"]="Block03"; t["n"]=17; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002817000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T19) {
  json t; t["s"]="Block03"; t["n"]=18; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002818000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T20) {
  json t; t["s"]="Block03"; t["n"]=19; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002819000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T21) {
  json t; t["s"]="Block03"; t["n"]=20; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002820000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T22) {
  json t; t["s"]="Block03"; t["n"]=21; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002821000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T23) {
  json t; t["s"]="Block03"; t["n"]=22; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002822000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T24) {
  json t; t["s"]="Block03"; t["n"]=23; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002823000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T25) {
  json t; t["s"]="Block03"; t["n"]=24; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002824000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T26) {
  json t; t["s"]="Block03"; t["n"]=25; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002825000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T27) {
  json t; t["s"]="Block03"; t["n"]=26; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002826000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T28) {
  json t; t["s"]="Block03"; t["n"]=27; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002827000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T29) {
  json t; t["s"]="Block03"; t["n"]=28; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002828000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T30) {
  json t; t["s"]="Block03"; t["n"]=29; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002829000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T31) {
  json t; t["s"]="Block03"; t["n"]=30; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002830000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T32) {
  json t; t["s"]="Block03"; t["n"]=31; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002831000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T33) {
  json t; t["s"]="Block03"; t["n"]=32; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002832000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T34) {
  json t; t["s"]="Block03"; t["n"]=33; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002833000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T35) {
  json t; t["s"]="Block03"; t["n"]=34; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002834000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T36) {
  json t; t["s"]="Block03"; t["n"]=35; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002835000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T37) {
  json t; t["s"]="Block03"; t["n"]=36; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002836000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T38) {
  json t; t["s"]="Block03"; t["n"]=37; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002837000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T39) {
  json t; t["s"]="Block03"; t["n"]=38; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002838000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T40) {
  json t; t["s"]="Block03"; t["n"]=39; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002839000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T41) {
  json t; t["s"]="Block03"; t["n"]=40; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002840000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T42) {
  json t; t["s"]="Block03"; t["n"]=41; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002841000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T43) {
  json t; t["s"]="Block03"; t["n"]=42; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002842000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T44) {
  json t; t["s"]="Block03"; t["n"]=43; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002843000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T45) {
  json t; t["s"]="Block03"; t["n"]=44; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002844000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T46) {
  json t; t["s"]="Block03"; t["n"]=45; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002845000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T47) {
  json t; t["s"]="Block03"; t["n"]=46; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002846000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T48) {
  json t; t["s"]="Block03"; t["n"]=47; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002847000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T49) {
  json t; t["s"]="Block03"; t["n"]=48; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002848000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T50) {
  json t; t["s"]="Block03"; t["n"]=49; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002849000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T51) {
  json t; t["s"]="Block03"; t["n"]=50; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002850000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T52) {
  json t; t["s"]="Block03"; t["n"]=51; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002851000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T53) {
  json t; t["s"]="Block03"; t["n"]=52; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002852000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T54) {
  json t; t["s"]="Block03"; t["n"]=53; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002853000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T55) {
  json t; t["s"]="Block03"; t["n"]=54; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002854000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T56) {
  json t; t["s"]="Block03"; t["n"]=55; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002855000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T57) {
  json t; t["s"]="Block03"; t["n"]=56; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002856000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T58) {
  json t; t["s"]="Block03"; t["n"]=57; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002857000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T59) {
  json t; t["s"]="Block03"; t["n"]=58; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002858000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T60) {
  json t; t["s"]="Block03"; t["n"]=59; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002859000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T61) {
  json t; t["s"]="Block03"; t["n"]=60; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002860000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T62) {
  json t; t["s"]="Block03"; t["n"]=61; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002861000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T63) {
  json t; t["s"]="Block03"; t["n"]=62; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002862000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T64) {
  json t; t["s"]="Block03"; t["n"]=63; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002863000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T65) {
  json t; t["s"]="Block03"; t["n"]=64; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002864000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T66) {
  json t; t["s"]="Block03"; t["n"]=65; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002865000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T67) {
  json t; t["s"]="Block03"; t["n"]=66; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002866000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T68) {
  json t; t["s"]="Block03"; t["n"]=67; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002867000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T69) {
  json t; t["s"]="Block03"; t["n"]=68; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002868000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block03, T70) {
  json t; t["s"]="Block03"; t["n"]=69; t["b"]=28;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(28) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_28_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002869000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
