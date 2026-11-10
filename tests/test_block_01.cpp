#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block01, T1) {
  json t; t["s"]="Block01"; t["n"]=0; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002600000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T2) {
  json t; t["s"]="Block01"; t["n"]=1; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002601000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T3) {
  json t; t["s"]="Block01"; t["n"]=2; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002602000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T4) {
  json t; t["s"]="Block01"; t["n"]=3; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002603000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T5) {
  json t; t["s"]="Block01"; t["n"]=4; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002604000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T6) {
  json t; t["s"]="Block01"; t["n"]=5; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002605000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T7) {
  json t; t["s"]="Block01"; t["n"]=6; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002606000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T8) {
  json t; t["s"]="Block01"; t["n"]=7; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002607000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T9) {
  json t; t["s"]="Block01"; t["n"]=8; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002608000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T10) {
  json t; t["s"]="Block01"; t["n"]=9; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002609000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T11) {
  json t; t["s"]="Block01"; t["n"]=10; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002610000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T12) {
  json t; t["s"]="Block01"; t["n"]=11; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002611000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T13) {
  json t; t["s"]="Block01"; t["n"]=12; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002612000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T14) {
  json t; t["s"]="Block01"; t["n"]=13; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002613000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T15) {
  json t; t["s"]="Block01"; t["n"]=14; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002614000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T16) {
  json t; t["s"]="Block01"; t["n"]=15; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002615000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T17) {
  json t; t["s"]="Block01"; t["n"]=16; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002616000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T18) {
  json t; t["s"]="Block01"; t["n"]=17; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002617000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T19) {
  json t; t["s"]="Block01"; t["n"]=18; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002618000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T20) {
  json t; t["s"]="Block01"; t["n"]=19; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002619000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T21) {
  json t; t["s"]="Block01"; t["n"]=20; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002620000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T22) {
  json t; t["s"]="Block01"; t["n"]=21; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002621000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T23) {
  json t; t["s"]="Block01"; t["n"]=22; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002622000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T24) {
  json t; t["s"]="Block01"; t["n"]=23; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002623000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T25) {
  json t; t["s"]="Block01"; t["n"]=24; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002624000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T26) {
  json t; t["s"]="Block01"; t["n"]=25; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002625000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T27) {
  json t; t["s"]="Block01"; t["n"]=26; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002626000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T28) {
  json t; t["s"]="Block01"; t["n"]=27; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002627000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T29) {
  json t; t["s"]="Block01"; t["n"]=28; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002628000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T30) {
  json t; t["s"]="Block01"; t["n"]=29; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002629000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T31) {
  json t; t["s"]="Block01"; t["n"]=30; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002630000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T32) {
  json t; t["s"]="Block01"; t["n"]=31; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002631000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T33) {
  json t; t["s"]="Block01"; t["n"]=32; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002632000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T34) {
  json t; t["s"]="Block01"; t["n"]=33; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002633000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T35) {
  json t; t["s"]="Block01"; t["n"]=34; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002634000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T36) {
  json t; t["s"]="Block01"; t["n"]=35; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002635000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T37) {
  json t; t["s"]="Block01"; t["n"]=36; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002636000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T38) {
  json t; t["s"]="Block01"; t["n"]=37; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002637000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T39) {
  json t; t["s"]="Block01"; t["n"]=38; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002638000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T40) {
  json t; t["s"]="Block01"; t["n"]=39; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002639000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T41) {
  json t; t["s"]="Block01"; t["n"]=40; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002640000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T42) {
  json t; t["s"]="Block01"; t["n"]=41; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002641000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T43) {
  json t; t["s"]="Block01"; t["n"]=42; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002642000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T44) {
  json t; t["s"]="Block01"; t["n"]=43; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002643000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T45) {
  json t; t["s"]="Block01"; t["n"]=44; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002644000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T46) {
  json t; t["s"]="Block01"; t["n"]=45; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002645000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T47) {
  json t; t["s"]="Block01"; t["n"]=46; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002646000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T48) {
  json t; t["s"]="Block01"; t["n"]=47; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002647000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T49) {
  json t; t["s"]="Block01"; t["n"]=48; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002648000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T50) {
  json t; t["s"]="Block01"; t["n"]=49; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002649000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T51) {
  json t; t["s"]="Block01"; t["n"]=50; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002650000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T52) {
  json t; t["s"]="Block01"; t["n"]=51; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002651000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T53) {
  json t; t["s"]="Block01"; t["n"]=52; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002652000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T54) {
  json t; t["s"]="Block01"; t["n"]=53; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002653000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T55) {
  json t; t["s"]="Block01"; t["n"]=54; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002654000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T56) {
  json t; t["s"]="Block01"; t["n"]=55; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002655000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T57) {
  json t; t["s"]="Block01"; t["n"]=56; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002656000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T58) {
  json t; t["s"]="Block01"; t["n"]=57; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002657000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T59) {
  json t; t["s"]="Block01"; t["n"]=58; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002658000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T60) {
  json t; t["s"]="Block01"; t["n"]=59; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002659000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T61) {
  json t; t["s"]="Block01"; t["n"]=60; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002660000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T62) {
  json t; t["s"]="Block01"; t["n"]=61; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002661000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T63) {
  json t; t["s"]="Block01"; t["n"]=62; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002662000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T64) {
  json t; t["s"]="Block01"; t["n"]=63; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002663000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T65) {
  json t; t["s"]="Block01"; t["n"]=64; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002664000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T66) {
  json t; t["s"]="Block01"; t["n"]=65; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002665000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T67) {
  json t; t["s"]="Block01"; t["n"]=66; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002666000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T68) {
  json t; t["s"]="Block01"; t["n"]=67; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002667000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T69) {
  json t; t["s"]="Block01"; t["n"]=68; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002668000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block01, T70) {
  json t; t["s"]="Block01"; t["n"]=69; t["b"]=26;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(26) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_26_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002669000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
