#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteQ, T1) {
  json t; t["s"]="SuiteQ"; t["n"]=0; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001600000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T2) {
  json t; t["s"]="SuiteQ"; t["n"]=1; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001601000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T3) {
  json t; t["s"]="SuiteQ"; t["n"]=2; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001602000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T4) {
  json t; t["s"]="SuiteQ"; t["n"]=3; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001603000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T5) {
  json t; t["s"]="SuiteQ"; t["n"]=4; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001604000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T6) {
  json t; t["s"]="SuiteQ"; t["n"]=5; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001605000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T7) {
  json t; t["s"]="SuiteQ"; t["n"]=6; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001606000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T8) {
  json t; t["s"]="SuiteQ"; t["n"]=7; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001607000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T9) {
  json t; t["s"]="SuiteQ"; t["n"]=8; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001608000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T10) {
  json t; t["s"]="SuiteQ"; t["n"]=9; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001609000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T11) {
  json t; t["s"]="SuiteQ"; t["n"]=10; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001610000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T12) {
  json t; t["s"]="SuiteQ"; t["n"]=11; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001611000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T13) {
  json t; t["s"]="SuiteQ"; t["n"]=12; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001612000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T14) {
  json t; t["s"]="SuiteQ"; t["n"]=13; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001613000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T15) {
  json t; t["s"]="SuiteQ"; t["n"]=14; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001614000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T16) {
  json t; t["s"]="SuiteQ"; t["n"]=15; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001615000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T17) {
  json t; t["s"]="SuiteQ"; t["n"]=16; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001616000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T18) {
  json t; t["s"]="SuiteQ"; t["n"]=17; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001617000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T19) {
  json t; t["s"]="SuiteQ"; t["n"]=18; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001618000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T20) {
  json t; t["s"]="SuiteQ"; t["n"]=19; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001619000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T21) {
  json t; t["s"]="SuiteQ"; t["n"]=20; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001620000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T22) {
  json t; t["s"]="SuiteQ"; t["n"]=21; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001621000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T23) {
  json t; t["s"]="SuiteQ"; t["n"]=22; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001622000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T24) {
  json t; t["s"]="SuiteQ"; t["n"]=23; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001623000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T25) {
  json t; t["s"]="SuiteQ"; t["n"]=24; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001624000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T26) {
  json t; t["s"]="SuiteQ"; t["n"]=25; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001625000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T27) {
  json t; t["s"]="SuiteQ"; t["n"]=26; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001626000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T28) {
  json t; t["s"]="SuiteQ"; t["n"]=27; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001627000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T29) {
  json t; t["s"]="SuiteQ"; t["n"]=28; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001628000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T30) {
  json t; t["s"]="SuiteQ"; t["n"]=29; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001629000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T31) {
  json t; t["s"]="SuiteQ"; t["n"]=30; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001630000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T32) {
  json t; t["s"]="SuiteQ"; t["n"]=31; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001631000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T33) {
  json t; t["s"]="SuiteQ"; t["n"]=32; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001632000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T34) {
  json t; t["s"]="SuiteQ"; t["n"]=33; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001633000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T35) {
  json t; t["s"]="SuiteQ"; t["n"]=34; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001634000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T36) {
  json t; t["s"]="SuiteQ"; t["n"]=35; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001635000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T37) {
  json t; t["s"]="SuiteQ"; t["n"]=36; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001636000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T38) {
  json t; t["s"]="SuiteQ"; t["n"]=37; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001637000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T39) {
  json t; t["s"]="SuiteQ"; t["n"]=38; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001638000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T40) {
  json t; t["s"]="SuiteQ"; t["n"]=39; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001639000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T41) {
  json t; t["s"]="SuiteQ"; t["n"]=40; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001640000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T42) {
  json t; t["s"]="SuiteQ"; t["n"]=41; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001641000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T43) {
  json t; t["s"]="SuiteQ"; t["n"]=42; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001642000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T44) {
  json t; t["s"]="SuiteQ"; t["n"]=43; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001643000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T45) {
  json t; t["s"]="SuiteQ"; t["n"]=44; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001644000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T46) {
  json t; t["s"]="SuiteQ"; t["n"]=45; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001645000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T47) {
  json t; t["s"]="SuiteQ"; t["n"]=46; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001646000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T48) {
  json t; t["s"]="SuiteQ"; t["n"]=47; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001647000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T49) {
  json t; t["s"]="SuiteQ"; t["n"]=48; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001648000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T50) {
  json t; t["s"]="SuiteQ"; t["n"]=49; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001649000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T51) {
  json t; t["s"]="SuiteQ"; t["n"]=50; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001650000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T52) {
  json t; t["s"]="SuiteQ"; t["n"]=51; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001651000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T53) {
  json t; t["s"]="SuiteQ"; t["n"]=52; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001652000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T54) {
  json t; t["s"]="SuiteQ"; t["n"]=53; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001653000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T55) {
  json t; t["s"]="SuiteQ"; t["n"]=54; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001654000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T56) {
  json t; t["s"]="SuiteQ"; t["n"]=55; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001655000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T57) {
  json t; t["s"]="SuiteQ"; t["n"]=56; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001656000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T58) {
  json t; t["s"]="SuiteQ"; t["n"]=57; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001657000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T59) {
  json t; t["s"]="SuiteQ"; t["n"]=58; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001658000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T60) {
  json t; t["s"]="SuiteQ"; t["n"]=59; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001659000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T61) {
  json t; t["s"]="SuiteQ"; t["n"]=60; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001660000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T62) {
  json t; t["s"]="SuiteQ"; t["n"]=61; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001661000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T63) {
  json t; t["s"]="SuiteQ"; t["n"]=62; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001662000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T64) {
  json t; t["s"]="SuiteQ"; t["n"]=63; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001663000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T65) {
  json t; t["s"]="SuiteQ"; t["n"]=64; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001664000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T66) {
  json t; t["s"]="SuiteQ"; t["n"]=65; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001665000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T67) {
  json t; t["s"]="SuiteQ"; t["n"]=66; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001666000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T68) {
  json t; t["s"]="SuiteQ"; t["n"]=67; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001667000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T69) {
  json t; t["s"]="SuiteQ"; t["n"]=68; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001668000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteQ, T70) {
  json t; t["s"]="SuiteQ"; t["n"]=69; t["b"]=16;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(16) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_16_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600001669000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
