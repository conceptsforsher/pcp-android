#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(SuiteY, T1) {
  json t; t["s"]="SuiteY"; t["n"]=0; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(0) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_0:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002400000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T2) {
  json t; t["s"]="SuiteY"; t["n"]=1; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(1) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_1:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002401000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T3) {
  json t; t["s"]="SuiteY"; t["n"]=2; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(2) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_2:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002402000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T4) {
  json t; t["s"]="SuiteY"; t["n"]=3; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(3) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_3:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002403000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T5) {
  json t; t["s"]="SuiteY"; t["n"]=4; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(4) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_4:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002404000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T6) {
  json t; t["s"]="SuiteY"; t["n"]=5; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(5) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_5:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002405000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T7) {
  json t; t["s"]="SuiteY"; t["n"]=6; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(6) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_6:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002406000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T8) {
  json t; t["s"]="SuiteY"; t["n"]=7; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(7) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_7:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002407000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T9) {
  json t; t["s"]="SuiteY"; t["n"]=8; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(8) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_8:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002408000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T10) {
  json t; t["s"]="SuiteY"; t["n"]=9; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(9) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_9:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002409000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T11) {
  json t; t["s"]="SuiteY"; t["n"]=10; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(10) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_10:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002410000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T12) {
  json t; t["s"]="SuiteY"; t["n"]=11; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(11) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_11:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002411000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T13) {
  json t; t["s"]="SuiteY"; t["n"]=12; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(12) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_12:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002412000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T14) {
  json t; t["s"]="SuiteY"; t["n"]=13; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(13) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_13:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002413000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T15) {
  json t; t["s"]="SuiteY"; t["n"]=14; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(14) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_14:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002414000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T16) {
  json t; t["s"]="SuiteY"; t["n"]=15; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(15) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_15:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002415000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T17) {
  json t; t["s"]="SuiteY"; t["n"]=16; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(16) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_16:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002416000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T18) {
  json t; t["s"]="SuiteY"; t["n"]=17; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(17) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_17:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002417000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T19) {
  json t; t["s"]="SuiteY"; t["n"]=18; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(18) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_18:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002418000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T20) {
  json t; t["s"]="SuiteY"; t["n"]=19; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(19) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_19:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002419000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T21) {
  json t; t["s"]="SuiteY"; t["n"]=20; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(20) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_20:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002420000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T22) {
  json t; t["s"]="SuiteY"; t["n"]=21; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(21) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_21:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002421000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T23) {
  json t; t["s"]="SuiteY"; t["n"]=22; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(22) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_22:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002422000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T24) {
  json t; t["s"]="SuiteY"; t["n"]=23; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(23) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_23:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002423000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T25) {
  json t; t["s"]="SuiteY"; t["n"]=24; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(24) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_24:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002424000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T26) {
  json t; t["s"]="SuiteY"; t["n"]=25; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(25) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_25:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002425000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T27) {
  json t; t["s"]="SuiteY"; t["n"]=26; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(26) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_26:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002426000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T28) {
  json t; t["s"]="SuiteY"; t["n"]=27; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(27) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_27:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002427000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T29) {
  json t; t["s"]="SuiteY"; t["n"]=28; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(28) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_28:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002428000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T30) {
  json t; t["s"]="SuiteY"; t["n"]=29; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(29) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_29:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002429000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T31) {
  json t; t["s"]="SuiteY"; t["n"]=30; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(30) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_30:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002430000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T32) {
  json t; t["s"]="SuiteY"; t["n"]=31; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(31) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_31:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002431000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T33) {
  json t; t["s"]="SuiteY"; t["n"]=32; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(32) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_32:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002432000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T34) {
  json t; t["s"]="SuiteY"; t["n"]=33; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(33) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_33:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002433000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T35) {
  json t; t["s"]="SuiteY"; t["n"]=34; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(34) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_34:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002434000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T36) {
  json t; t["s"]="SuiteY"; t["n"]=35; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(35) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_35:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002435000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T37) {
  json t; t["s"]="SuiteY"; t["n"]=36; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(36) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_36:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002436000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T38) {
  json t; t["s"]="SuiteY"; t["n"]=37; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(37) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_37:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002437000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T39) {
  json t; t["s"]="SuiteY"; t["n"]=38; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(38) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_38:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002438000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T40) {
  json t; t["s"]="SuiteY"; t["n"]=39; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(39) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_39:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002439000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T41) {
  json t; t["s"]="SuiteY"; t["n"]=40; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(40) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_40:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002440000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T42) {
  json t; t["s"]="SuiteY"; t["n"]=41; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(41) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_41:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002441000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T43) {
  json t; t["s"]="SuiteY"; t["n"]=42; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(42) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_42:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002442000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T44) {
  json t; t["s"]="SuiteY"; t["n"]=43; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(43) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_43:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002443000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T45) {
  json t; t["s"]="SuiteY"; t["n"]=44; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(44) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_44:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002444000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T46) {
  json t; t["s"]="SuiteY"; t["n"]=45; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(45) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_45:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002445000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T47) {
  json t; t["s"]="SuiteY"; t["n"]=46; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(46) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_46:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002446000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T48) {
  json t; t["s"]="SuiteY"; t["n"]=47; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(47) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_47:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002447000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T49) {
  json t; t["s"]="SuiteY"; t["n"]=48; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(48) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_48:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002448000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T50) {
  json t; t["s"]="SuiteY"; t["n"]=49; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(49) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_49:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002449000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T51) {
  json t; t["s"]="SuiteY"; t["n"]=50; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(50) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_50:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002450000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T52) {
  json t; t["s"]="SuiteY"; t["n"]=51; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(51) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_51:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002451000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T53) {
  json t; t["s"]="SuiteY"; t["n"]=52; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(52) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_52:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002452000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T54) {
  json t; t["s"]="SuiteY"; t["n"]=53; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(53) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_53:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002453000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T55) {
  json t; t["s"]="SuiteY"; t["n"]=54; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(54) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_54:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002454000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T56) {
  json t; t["s"]="SuiteY"; t["n"]=55; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(55) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_55:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002455000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T57) {
  json t; t["s"]="SuiteY"; t["n"]=56; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(56) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_56:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002456000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T58) {
  json t; t["s"]="SuiteY"; t["n"]=57; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(57) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_57:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002457000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T59) {
  json t; t["s"]="SuiteY"; t["n"]=58; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(58) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_58:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002458000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T60) {
  json t; t["s"]="SuiteY"; t["n"]=59; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(59) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_59:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002459000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T61) {
  json t; t["s"]="SuiteY"; t["n"]=60; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(60) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_60:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002460000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T62) {
  json t; t["s"]="SuiteY"; t["n"]=61; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(61) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_61:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002461000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T63) {
  json t; t["s"]="SuiteY"; t["n"]=62; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(62) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_62:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002462000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T64) {
  json t; t["s"]="SuiteY"; t["n"]=63; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(63) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_63:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002463000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T65) {
  json t; t["s"]="SuiteY"; t["n"]=64; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(64) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_64:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002464000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T66) {
  json t; t["s"]="SuiteY"; t["n"]=65; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(65) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_65:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002465000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T67) {
  json t; t["s"]="SuiteY"; t["n"]=66; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(66) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_66:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002466000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T68) {
  json t; t["s"]="SuiteY"; t["n"]=67; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(67) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_67:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002467000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T69) {
  json t; t["s"]="SuiteY"; t["n"]=68; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(68) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_68:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002468000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=false; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

TEST(SuiteY, T70) {
  json t; t["s"]="SuiteY"; t["n"]=69; t["b"]=24;
  for (int e=0;e<5;++e) {
    json v; v["eid"]="$sui_" + std::to_string(24) + "_" + std::to_string(69) + "_" + std::to_string(e) + ":m";
    v["t"]="m.room.message"; v["s"]="@u_24_69:m";
    v["c"]["b"]="Body " + std::to_string(e); v["ts"]=1600002469000+e;
    t["evs"].push_back(v); ASSERT_FALSE(v["eid"].empty());
  }
  ASSERT_EQ(t["evs"].size(),5);
  t["st"]["init"]=true; t["st"]["enc"]=true; ASSERT_TRUE(t["st"]["init"]);
  auto d=t.dump(); ASSERT_FALSE(d.empty());
  auto p=json::parse(d); ASSERT_EQ(p["evs"].size(),5);
}

}}
