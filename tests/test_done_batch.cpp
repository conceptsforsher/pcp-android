#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(DoneBatch,T1){
  json t;t["i"]=0;t["r"]="!room_0:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_0_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T2){
  json t;t["i"]=1;t["r"]="!room_1:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_1_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T3){
  json t;t["i"]=2;t["r"]="!room_2:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_2_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T4){
  json t;t["i"]=3;t["r"]="!room_3:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_3_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T5){
  json t;t["i"]=4;t["r"]="!room_4:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_4_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T6){
  json t;t["i"]=5;t["r"]="!room_5:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_5_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T7){
  json t;t["i"]=6;t["r"]="!room_6:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_6_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T8){
  json t;t["i"]=7;t["r"]="!room_7:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_7_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T9){
  json t;t["i"]=8;t["r"]="!room_8:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_8_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T10){
  json t;t["i"]=9;t["r"]="!room_9:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_9_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T11){
  json t;t["i"]=10;t["r"]="!room_10:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_10_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T12){
  json t;t["i"]=11;t["r"]="!room_11:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_11_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T13){
  json t;t["i"]=12;t["r"]="!room_12:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_12_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T14){
  json t;t["i"]=13;t["r"]="!room_13:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_13_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T15){
  json t;t["i"]=14;t["r"]="!room_14:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_14_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T16){
  json t;t["i"]=15;t["r"]="!room_15:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_15_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T17){
  json t;t["i"]=16;t["r"]="!room_16:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_16_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T18){
  json t;t["i"]=17;t["r"]="!room_17:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_17_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T19){
  json t;t["i"]=18;t["r"]="!room_18:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_18_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T20){
  json t;t["i"]=19;t["r"]="!room_19:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_19_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T21){
  json t;t["i"]=20;t["r"]="!room_20:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_20_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T22){
  json t;t["i"]=21;t["r"]="!room_21:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_21_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T23){
  json t;t["i"]=22;t["r"]="!room_22:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_22_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T24){
  json t;t["i"]=23;t["r"]="!room_23:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_23_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T25){
  json t;t["i"]=24;t["r"]="!room_24:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_24_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T26){
  json t;t["i"]=25;t["r"]="!room_25:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_25_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T27){
  json t;t["i"]=26;t["r"]="!room_26:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_26_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T28){
  json t;t["i"]=27;t["r"]="!room_27:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_27_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T29){
  json t;t["i"]=28;t["r"]="!room_28:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_28_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T30){
  json t;t["i"]=29;t["r"]="!room_29:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_29_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T31){
  json t;t["i"]=30;t["r"]="!room_30:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_30_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T32){
  json t;t["i"]=31;t["r"]="!room_31:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_31_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T33){
  json t;t["i"]=32;t["r"]="!room_32:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_32_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T34){
  json t;t["i"]=33;t["r"]="!room_33:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_33_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T35){
  json t;t["i"]=34;t["r"]="!room_34:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_34_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T36){
  json t;t["i"]=35;t["r"]="!room_35:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_35_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T37){
  json t;t["i"]=36;t["r"]="!room_36:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_36_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T38){
  json t;t["i"]=37;t["r"]="!room_37:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_37_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T39){
  json t;t["i"]=38;t["r"]="!room_38:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_38_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}

TEST(DoneBatch,T40){
  json t;t["i"]=39;t["r"]="!room_39:m.org";
  for(int e=0;e<6;++e){json v;v["eid"]="$done_39_"+std::to_string(e)+":m";v["r"]=t["r"];t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),6);ASSERT_FALSE(t["r"].empty());
  auto s=t.dump();ASSERT_FALSE(s.empty());
}
}}
