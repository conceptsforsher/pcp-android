#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillI,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$i_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$i_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$i_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$i_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$i_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$i_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$i_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$i_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$i_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$i_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$i_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$i_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$i_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$i_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$i_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$i_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$i_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$i_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$i_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$i_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$i_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$i_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$i_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$i_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillI,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$i_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
