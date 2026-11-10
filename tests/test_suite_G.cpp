#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteG, T1) {
  json t; t["s"]="SuiteG"; t["n"]=0; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000600000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T2) {
  json t; t["s"]="SuiteG"; t["n"]=1; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000601000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T3) {
  json t; t["s"]="SuiteG"; t["n"]=2; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000602000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T4) {
  json t; t["s"]="SuiteG"; t["n"]=3; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000603000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T5) {
  json t; t["s"]="SuiteG"; t["n"]=4; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000604000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T6) {
  json t; t["s"]="SuiteG"; t["n"]=5; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000605000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T7) {
  json t; t["s"]="SuiteG"; t["n"]=6; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000606000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T8) {
  json t; t["s"]="SuiteG"; t["n"]=7; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000607000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T9) {
  json t; t["s"]="SuiteG"; t["n"]=8; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000608000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T10) {
  json t; t["s"]="SuiteG"; t["n"]=9; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000609000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T11) {
  json t; t["s"]="SuiteG"; t["n"]=10; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000610000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T12) {
  json t; t["s"]="SuiteG"; t["n"]=11; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000611000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T13) {
  json t; t["s"]="SuiteG"; t["n"]=12; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000612000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T14) {
  json t; t["s"]="SuiteG"; t["n"]=13; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000613000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T15) {
  json t; t["s"]="SuiteG"; t["n"]=14; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000614000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T16) {
  json t; t["s"]="SuiteG"; t["n"]=15; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000615000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T17) {
  json t; t["s"]="SuiteG"; t["n"]=16; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000616000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T18) {
  json t; t["s"]="SuiteG"; t["n"]=17; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000617000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T19) {
  json t; t["s"]="SuiteG"; t["n"]=18; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000618000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T20) {
  json t; t["s"]="SuiteG"; t["n"]=19; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000619000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T21) {
  json t; t["s"]="SuiteG"; t["n"]=20; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000620000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T22) {
  json t; t["s"]="SuiteG"; t["n"]=21; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000621000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T23) {
  json t; t["s"]="SuiteG"; t["n"]=22; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000622000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T24) {
  json t; t["s"]="SuiteG"; t["n"]=23; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000623000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T25) {
  json t; t["s"]="SuiteG"; t["n"]=24; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000624000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T26) {
  json t; t["s"]="SuiteG"; t["n"]=25; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000625000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T27) {
  json t; t["s"]="SuiteG"; t["n"]=26; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000626000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T28) {
  json t; t["s"]="SuiteG"; t["n"]=27; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000627000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T29) {
  json t; t["s"]="SuiteG"; t["n"]=28; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000628000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T30) {
  json t; t["s"]="SuiteG"; t["n"]=29; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000629000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T31) {
  json t; t["s"]="SuiteG"; t["n"]=30; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000630000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T32) {
  json t; t["s"]="SuiteG"; t["n"]=31; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000631000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T33) {
  json t; t["s"]="SuiteG"; t["n"]=32; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000632000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T34) {
  json t; t["s"]="SuiteG"; t["n"]=33; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000633000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T35) {
  json t; t["s"]="SuiteG"; t["n"]=34; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000634000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T36) {
  json t; t["s"]="SuiteG"; t["n"]=35; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000635000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T37) {
  json t; t["s"]="SuiteG"; t["n"]=36; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000636000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T38) {
  json t; t["s"]="SuiteG"; t["n"]=37; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000637000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T39) {
  json t; t["s"]="SuiteG"; t["n"]=38; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000638000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T40) {
  json t; t["s"]="SuiteG"; t["n"]=39; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000639000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T41) {
  json t; t["s"]="SuiteG"; t["n"]=40; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000640000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T42) {
  json t; t["s"]="SuiteG"; t["n"]=41; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000641000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T43) {
  json t; t["s"]="SuiteG"; t["n"]=42; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000642000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T44) {
  json t; t["s"]="SuiteG"; t["n"]=43; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000643000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T45) {
  json t; t["s"]="SuiteG"; t["n"]=44; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000644000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T46) {
  json t; t["s"]="SuiteG"; t["n"]=45; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000645000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T47) {
  json t; t["s"]="SuiteG"; t["n"]=46; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000646000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T48) {
  json t; t["s"]="SuiteG"; t["n"]=47; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000647000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T49) {
  json t; t["s"]="SuiteG"; t["n"]=48; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000648000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T50) {
  json t; t["s"]="SuiteG"; t["n"]=49; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000649000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T51) {
  json t; t["s"]="SuiteG"; t["n"]=50; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000650000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T52) {
  json t; t["s"]="SuiteG"; t["n"]=51; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000651000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T53) {
  json t; t["s"]="SuiteG"; t["n"]=52; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000652000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T54) {
  json t; t["s"]="SuiteG"; t["n"]=53; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000653000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T55) {
  json t; t["s"]="SuiteG"; t["n"]=54; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000654000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T56) {
  json t; t["s"]="SuiteG"; t["n"]=55; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000655000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T57) {
  json t; t["s"]="SuiteG"; t["n"]=56; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000656000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T58) {
  json t; t["s"]="SuiteG"; t["n"]=57; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000657000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T59) {
  json t; t["s"]="SuiteG"; t["n"]=58; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000658000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T60) {
  json t; t["s"]="SuiteG"; t["n"]=59; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000659000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T61) {
  json t; t["s"]="SuiteG"; t["n"]=60; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000660000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T62) {
  json t; t["s"]="SuiteG"; t["n"]=61; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000661000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T63) {
  json t; t["s"]="SuiteG"; t["n"]=62; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000662000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T64) {
  json t; t["s"]="SuiteG"; t["n"]=63; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000663000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T65) {
  json t; t["s"]="SuiteG"; t["n"]=64; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000664000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T66) {
  json t; t["s"]="SuiteG"; t["n"]=65; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000665000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T67) {
  json t; t["s"]="SuiteG"; t["n"]=66; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000666000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T68) {
  json t; t["s"]="SuiteG"; t["n"]=67; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000667000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T69) {
  json t; t["s"]="SuiteG"; t["n"]=68; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000668000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteG, T70) {
  json t; t["s"]="SuiteG"; t["n"]=69; t["b"]=6;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(6) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_6_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600000669000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
