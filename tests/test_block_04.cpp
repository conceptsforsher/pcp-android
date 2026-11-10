#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Block04, T1) {
  json t; t["s"]="Block04"; t["n"]=0; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002900000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T2) {
  json t; t["s"]="Block04"; t["n"]=1; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002901000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T3) {
  json t; t["s"]="Block04"; t["n"]=2; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002902000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T4) {
  json t; t["s"]="Block04"; t["n"]=3; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002903000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T5) {
  json t; t["s"]="Block04"; t["n"]=4; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002904000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T6) {
  json t; t["s"]="Block04"; t["n"]=5; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002905000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T7) {
  json t; t["s"]="Block04"; t["n"]=6; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002906000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T8) {
  json t; t["s"]="Block04"; t["n"]=7; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002907000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T9) {
  json t; t["s"]="Block04"; t["n"]=8; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002908000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T10) {
  json t; t["s"]="Block04"; t["n"]=9; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002909000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T11) {
  json t; t["s"]="Block04"; t["n"]=10; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002910000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T12) {
  json t; t["s"]="Block04"; t["n"]=11; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002911000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T13) {
  json t; t["s"]="Block04"; t["n"]=12; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002912000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T14) {
  json t; t["s"]="Block04"; t["n"]=13; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002913000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T15) {
  json t; t["s"]="Block04"; t["n"]=14; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002914000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T16) {
  json t; t["s"]="Block04"; t["n"]=15; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002915000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T17) {
  json t; t["s"]="Block04"; t["n"]=16; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002916000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T18) {
  json t; t["s"]="Block04"; t["n"]=17; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002917000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T19) {
  json t; t["s"]="Block04"; t["n"]=18; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002918000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T20) {
  json t; t["s"]="Block04"; t["n"]=19; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002919000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T21) {
  json t; t["s"]="Block04"; t["n"]=20; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002920000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T22) {
  json t; t["s"]="Block04"; t["n"]=21; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002921000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T23) {
  json t; t["s"]="Block04"; t["n"]=22; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002922000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T24) {
  json t; t["s"]="Block04"; t["n"]=23; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002923000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T25) {
  json t; t["s"]="Block04"; t["n"]=24; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002924000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T26) {
  json t; t["s"]="Block04"; t["n"]=25; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002925000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T27) {
  json t; t["s"]="Block04"; t["n"]=26; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002926000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T28) {
  json t; t["s"]="Block04"; t["n"]=27; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002927000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T29) {
  json t; t["s"]="Block04"; t["n"]=28; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002928000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T30) {
  json t; t["s"]="Block04"; t["n"]=29; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002929000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T31) {
  json t; t["s"]="Block04"; t["n"]=30; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002930000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T32) {
  json t; t["s"]="Block04"; t["n"]=31; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002931000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T33) {
  json t; t["s"]="Block04"; t["n"]=32; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002932000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T34) {
  json t; t["s"]="Block04"; t["n"]=33; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002933000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T35) {
  json t; t["s"]="Block04"; t["n"]=34; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002934000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T36) {
  json t; t["s"]="Block04"; t["n"]=35; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002935000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T37) {
  json t; t["s"]="Block04"; t["n"]=36; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002936000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T38) {
  json t; t["s"]="Block04"; t["n"]=37; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002937000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T39) {
  json t; t["s"]="Block04"; t["n"]=38; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002938000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T40) {
  json t; t["s"]="Block04"; t["n"]=39; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002939000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T41) {
  json t; t["s"]="Block04"; t["n"]=40; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002940000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T42) {
  json t; t["s"]="Block04"; t["n"]=41; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002941000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T43) {
  json t; t["s"]="Block04"; t["n"]=42; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002942000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T44) {
  json t; t["s"]="Block04"; t["n"]=43; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002943000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T45) {
  json t; t["s"]="Block04"; t["n"]=44; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002944000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T46) {
  json t; t["s"]="Block04"; t["n"]=45; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002945000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T47) {
  json t; t["s"]="Block04"; t["n"]=46; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002946000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T48) {
  json t; t["s"]="Block04"; t["n"]=47; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002947000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T49) {
  json t; t["s"]="Block04"; t["n"]=48; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002948000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T50) {
  json t; t["s"]="Block04"; t["n"]=49; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002949000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T51) {
  json t; t["s"]="Block04"; t["n"]=50; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002950000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T52) {
  json t; t["s"]="Block04"; t["n"]=51; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002951000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T53) {
  json t; t["s"]="Block04"; t["n"]=52; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002952000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T54) {
  json t; t["s"]="Block04"; t["n"]=53; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002953000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T55) {
  json t; t["s"]="Block04"; t["n"]=54; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002954000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T56) {
  json t; t["s"]="Block04"; t["n"]=55; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002955000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T57) {
  json t; t["s"]="Block04"; t["n"]=56; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002956000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T58) {
  json t; t["s"]="Block04"; t["n"]=57; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002957000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T59) {
  json t; t["s"]="Block04"; t["n"]=58; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002958000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T60) {
  json t; t["s"]="Block04"; t["n"]=59; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002959000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T61) {
  json t; t["s"]="Block04"; t["n"]=60; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002960000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T62) {
  json t; t["s"]="Block04"; t["n"]=61; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002961000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T63) {
  json t; t["s"]="Block04"; t["n"]=62; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002962000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T64) {
  json t; t["s"]="Block04"; t["n"]=63; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002963000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T65) {
  json t; t["s"]="Block04"; t["n"]=64; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002964000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T66) {
  json t; t["s"]="Block04"; t["n"]=65; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002965000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T67) {
  json t; t["s"]="Block04"; t["n"]=66; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002966000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T68) {
  json t; t["s"]="Block04"; t["n"]=67; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002967000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T69) {
  json t; t["s"]="Block04"; t["n"]=68; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002968000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(Block04, T70) {
  json t; t["s"]="Block04"; t["n"]=69; t["b"]=29;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$blo_" + std::to_string(29) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_29_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002969000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
