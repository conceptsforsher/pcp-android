#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

// Test scenario 1 for pcp chat client
TEST(FillGap,T1){
  json t;t["i"]=0;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_0_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 2 for pcp chat client
TEST(FillGap,T2){
  json t;t["i"]=1;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_1_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 3 for pcp chat client
TEST(FillGap,T3){
  json t;t["i"]=2;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_2_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 4 for pcp chat client
TEST(FillGap,T4){
  json t;t["i"]=3;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_3_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 5 for pcp chat client
TEST(FillGap,T5){
  json t;t["i"]=4;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_4_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 6 for pcp chat client
TEST(FillGap,T6){
  json t;t["i"]=5;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_5_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 7 for pcp chat client
TEST(FillGap,T7){
  json t;t["i"]=6;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_6_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 8 for pcp chat client
TEST(FillGap,T8){
  json t;t["i"]=7;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_7_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 9 for pcp chat client
TEST(FillGap,T9){
  json t;t["i"]=8;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_8_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 10 for pcp chat client
TEST(FillGap,T10){
  json t;t["i"]=9;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_9_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 11 for pcp chat client
TEST(FillGap,T11){
  json t;t["i"]=10;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_10_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}

// Test scenario 12 for pcp chat client
TEST(FillGap,T12){
  json t;t["i"]=11;
  for(int e=0;e<3;++e){
    json v;v["eid"]="$gap_11_"+std::to_string(e)+":m";
    v["c"]["b"]="Gap body "+std::to_string(e);
    t["evs"].push_back(v);
  }
  ASSERT_EQ(t["evs"].size(),3);
  t["st"]["init"]=true;ASSERT_TRUE(t["st"]["init"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),3);
}
}}
