#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FinishBatch,T1){
  json t;t["i"]=0;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_0_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T2){
  json t;t["i"]=1;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_1_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T3){
  json t;t["i"]=2;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_2_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T4){
  json t;t["i"]=3;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_3_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T5){
  json t;t["i"]=4;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_4_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T6){
  json t;t["i"]=5;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_5_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T7){
  json t;t["i"]=6;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_6_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T8){
  json t;t["i"]=7;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_7_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T9){
  json t;t["i"]=8;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_8_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T10){
  json t;t["i"]=9;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_9_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T11){
  json t;t["i"]=10;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_10_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T12){
  json t;t["i"]=11;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_11_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T13){
  json t;t["i"]=12;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_12_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T14){
  json t;t["i"]=13;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_13_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T15){
  json t;t["i"]=14;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_14_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T16){
  json t;t["i"]=15;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_15_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T17){
  json t;t["i"]=16;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_16_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T18){
  json t;t["i"]=17;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_17_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T19){
  json t;t["i"]=18;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_18_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T20){
  json t;t["i"]=19;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_19_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T21){
  json t;t["i"]=20;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_20_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T22){
  json t;t["i"]=21;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_21_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T23){
  json t;t["i"]=22;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_22_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T24){
  json t;t["i"]=23;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_23_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T25){
  json t;t["i"]=24;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_24_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T26){
  json t;t["i"]=25;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_25_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T27){
  json t;t["i"]=26;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_26_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T28){
  json t;t["i"]=27;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_27_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T29){
  json t;t["i"]=28;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_28_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T30){
  json t;t["i"]=29;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_29_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T31){
  json t;t["i"]=30;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_30_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T32){
  json t;t["i"]=31;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_31_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T33){
  json t;t["i"]=32;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_32_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T34){
  json t;t["i"]=33;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_33_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T35){
  json t;t["i"]=34;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_34_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T36){
  json t;t["i"]=35;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_35_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T37){
  json t;t["i"]=36;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_36_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T38){
  json t;t["i"]=37;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_37_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T39){
  json t;t["i"]=38;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_38_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T40){
  json t;t["i"]=39;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_39_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T41){
  json t;t["i"]=40;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_40_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T42){
  json t;t["i"]=41;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_41_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T43){
  json t;t["i"]=42;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_42_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=true;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T44){
  json t;t["i"]=43;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_43_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}

TEST(FinishBatch,T45){
  json t;t["i"]=44;
  for(int e=0;e<7;++e){json v;v["eid"]="$fin_44_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),7);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),7);
  t["st"]["init"]=true;t["st"]["enc"]=false;ASSERT_TRUE(t["st"]["init"]);
}
}}
