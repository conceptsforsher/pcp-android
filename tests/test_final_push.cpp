#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
#include <vector>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FinalPush,T1){
  json t;t["i"]=0;t["r"]="!fr_0:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_0_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T2){
  json t;t["i"]=1;t["r"]="!fr_1:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_1_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T3){
  json t;t["i"]=2;t["r"]="!fr_2:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_2_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T4){
  json t;t["i"]=3;t["r"]="!fr_3:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_3_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T5){
  json t;t["i"]=4;t["r"]="!fr_4:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_4_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T6){
  json t;t["i"]=5;t["r"]="!fr_5:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_5_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T7){
  json t;t["i"]=6;t["r"]="!fr_6:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_6_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T8){
  json t;t["i"]=7;t["r"]="!fr_7:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_7_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T9){
  json t;t["i"]=8;t["r"]="!fr_8:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_8_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T10){
  json t;t["i"]=9;t["r"]="!fr_9:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_9_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T11){
  json t;t["i"]=10;t["r"]="!fr_10:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_10_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T12){
  json t;t["i"]=11;t["r"]="!fr_11:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_11_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T13){
  json t;t["i"]=12;t["r"]="!fr_12:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_12_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T14){
  json t;t["i"]=13;t["r"]="!fr_13:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_13_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T15){
  json t;t["i"]=14;t["r"]="!fr_14:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_14_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T16){
  json t;t["i"]=15;t["r"]="!fr_15:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_15_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T17){
  json t;t["i"]=16;t["r"]="!fr_16:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_16_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T18){
  json t;t["i"]=17;t["r"]="!fr_17:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_17_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T19){
  json t;t["i"]=18;t["r"]="!fr_18:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_18_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T20){
  json t;t["i"]=19;t["r"]="!fr_19:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_19_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T21){
  json t;t["i"]=20;t["r"]="!fr_20:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_20_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T22){
  json t;t["i"]=21;t["r"]="!fr_21:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_21_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T23){
  json t;t["i"]=22;t["r"]="!fr_22:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_22_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T24){
  json t;t["i"]=23;t["r"]="!fr_23:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_23_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T25){
  json t;t["i"]=24;t["r"]="!fr_24:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_24_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T26){
  json t;t["i"]=25;t["r"]="!fr_25:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_25_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T27){
  json t;t["i"]=26;t["r"]="!fr_26:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_26_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T28){
  json t;t["i"]=27;t["r"]="!fr_27:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_27_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T29){
  json t;t["i"]=28;t["r"]="!fr_28:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_28_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T30){
  json t;t["i"]=29;t["r"]="!fr_29:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_29_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T31){
  json t;t["i"]=30;t["r"]="!fr_30:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_30_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T32){
  json t;t["i"]=31;t["r"]="!fr_31:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_31_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T33){
  json t;t["i"]=32;t["r"]="!fr_32:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_32_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T34){
  json t;t["i"]=33;t["r"]="!fr_33:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_33_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T35){
  json t;t["i"]=34;t["r"]="!fr_34:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_34_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T36){
  json t;t["i"]=35;t["r"]="!fr_35:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_35_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T37){
  json t;t["i"]=36;t["r"]="!fr_36:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_36_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T38){
  json t;t["i"]=37;t["r"]="!fr_37:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_37_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T39){
  json t;t["i"]=38;t["r"]="!fr_38:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_38_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T40){
  json t;t["i"]=39;t["r"]="!fr_39:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_39_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T41){
  json t;t["i"]=40;t["r"]="!fr_40:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_40_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T42){
  json t;t["i"]=41;t["r"]="!fr_41:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_41_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T43){
  json t;t["i"]=42;t["r"]="!fr_42:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_42_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T44){
  json t;t["i"]=43;t["r"]="!fr_43:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_43_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T45){
  json t;t["i"]=44;t["r"]="!fr_44:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_44_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T46){
  json t;t["i"]=45;t["r"]="!fr_45:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_45_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T47){
  json t;t["i"]=46;t["r"]="!fr_46:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_46_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T48){
  json t;t["i"]=47;t["r"]="!fr_47:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_47_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T49){
  json t;t["i"]=48;t["r"]="!fr_48:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_48_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=true;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}

TEST(FinalPush,T50){
  json t;t["i"]=49;t["r"]="!fr_49:m";
  for(int e=0;e<4;++e){json v;v["eid"]="$fpush_49_"+std::to_string(e)+":m";v["r"]=t["r"];v["c"]["b"]="Push "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),4);
  t["st"]["i"]=true;t["st"]["e"]=false;
  ASSERT_TRUE(t["st"]["i"]);
  auto s=t.dump();ASSERT_FALSE(s.empty());
  auto p=json::parse(s);ASSERT_EQ(p["evs"].size(),4);
}
}}
