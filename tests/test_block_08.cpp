#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block08, T1) {
  json t; t["s"]="Block08"; t["n"]=0; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003300000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T2) {
  json t; t["s"]="Block08"; t["n"]=1; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003301000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T3) {
  json t; t["s"]="Block08"; t["n"]=2; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003302000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T4) {
  json t; t["s"]="Block08"; t["n"]=3; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003303000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T5) {
  json t; t["s"]="Block08"; t["n"]=4; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003304000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T6) {
  json t; t["s"]="Block08"; t["n"]=5; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003305000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T7) {
  json t; t["s"]="Block08"; t["n"]=6; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003306000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T8) {
  json t; t["s"]="Block08"; t["n"]=7; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003307000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T9) {
  json t; t["s"]="Block08"; t["n"]=8; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003308000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T10) {
  json t; t["s"]="Block08"; t["n"]=9; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003309000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T11) {
  json t; t["s"]="Block08"; t["n"]=10; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003310000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T12) {
  json t; t["s"]="Block08"; t["n"]=11; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003311000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T13) {
  json t; t["s"]="Block08"; t["n"]=12; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003312000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T14) {
  json t; t["s"]="Block08"; t["n"]=13; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003313000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T15) {
  json t; t["s"]="Block08"; t["n"]=14; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003314000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T16) {
  json t; t["s"]="Block08"; t["n"]=15; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003315000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T17) {
  json t; t["s"]="Block08"; t["n"]=16; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003316000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T18) {
  json t; t["s"]="Block08"; t["n"]=17; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003317000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T19) {
  json t; t["s"]="Block08"; t["n"]=18; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003318000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T20) {
  json t; t["s"]="Block08"; t["n"]=19; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003319000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T21) {
  json t; t["s"]="Block08"; t["n"]=20; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003320000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T22) {
  json t; t["s"]="Block08"; t["n"]=21; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003321000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T23) {
  json t; t["s"]="Block08"; t["n"]=22; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003322000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T24) {
  json t; t["s"]="Block08"; t["n"]=23; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003323000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T25) {
  json t; t["s"]="Block08"; t["n"]=24; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003324000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T26) {
  json t; t["s"]="Block08"; t["n"]=25; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003325000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T27) {
  json t; t["s"]="Block08"; t["n"]=26; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003326000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T28) {
  json t; t["s"]="Block08"; t["n"]=27; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003327000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T29) {
  json t; t["s"]="Block08"; t["n"]=28; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003328000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T30) {
  json t; t["s"]="Block08"; t["n"]=29; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003329000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T31) {
  json t; t["s"]="Block08"; t["n"]=30; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003330000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T32) {
  json t; t["s"]="Block08"; t["n"]=31; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003331000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T33) {
  json t; t["s"]="Block08"; t["n"]=32; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003332000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T34) {
  json t; t["s"]="Block08"; t["n"]=33; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003333000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T35) {
  json t; t["s"]="Block08"; t["n"]=34; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003334000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T36) {
  json t; t["s"]="Block08"; t["n"]=35; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003335000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T37) {
  json t; t["s"]="Block08"; t["n"]=36; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003336000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T38) {
  json t; t["s"]="Block08"; t["n"]=37; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003337000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T39) {
  json t; t["s"]="Block08"; t["n"]=38; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003338000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T40) {
  json t; t["s"]="Block08"; t["n"]=39; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003339000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T41) {
  json t; t["s"]="Block08"; t["n"]=40; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003340000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T42) {
  json t; t["s"]="Block08"; t["n"]=41; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003341000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T43) {
  json t; t["s"]="Block08"; t["n"]=42; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003342000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T44) {
  json t; t["s"]="Block08"; t["n"]=43; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003343000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T45) {
  json t; t["s"]="Block08"; t["n"]=44; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003344000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T46) {
  json t; t["s"]="Block08"; t["n"]=45; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003345000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T47) {
  json t; t["s"]="Block08"; t["n"]=46; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003346000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T48) {
  json t; t["s"]="Block08"; t["n"]=47; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003347000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T49) {
  json t; t["s"]="Block08"; t["n"]=48; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003348000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T50) {
  json t; t["s"]="Block08"; t["n"]=49; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003349000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T51) {
  json t; t["s"]="Block08"; t["n"]=50; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003350000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T52) {
  json t; t["s"]="Block08"; t["n"]=51; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003351000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T53) {
  json t; t["s"]="Block08"; t["n"]=52; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003352000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T54) {
  json t; t["s"]="Block08"; t["n"]=53; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003353000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T55) {
  json t; t["s"]="Block08"; t["n"]=54; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003354000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T56) {
  json t; t["s"]="Block08"; t["n"]=55; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003355000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T57) {
  json t; t["s"]="Block08"; t["n"]=56; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003356000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T58) {
  json t; t["s"]="Block08"; t["n"]=57; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003357000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T59) {
  json t; t["s"]="Block08"; t["n"]=58; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003358000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T60) {
  json t; t["s"]="Block08"; t["n"]=59; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003359000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T61) {
  json t; t["s"]="Block08"; t["n"]=60; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003360000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T62) {
  json t; t["s"]="Block08"; t["n"]=61; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003361000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T63) {
  json t; t["s"]="Block08"; t["n"]=62; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003362000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T64) {
  json t; t["s"]="Block08"; t["n"]=63; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003363000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T65) {
  json t; t["s"]="Block08"; t["n"]=64; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003364000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T66) {
  json t; t["s"]="Block08"; t["n"]=65; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003365000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T67) {
  json t; t["s"]="Block08"; t["n"]=66; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003366000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T68) {
  json t; t["s"]="Block08"; t["n"]=67; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003367000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T69) {
  json t; t["s"]="Block08"; t["n"]=68; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003368000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block08, T70) {
  json t; t["s"]="Block08"; t["n"]=69; t["b"]=33;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(33) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_33_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600003369000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
