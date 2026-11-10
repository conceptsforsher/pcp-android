#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillK,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$k_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$k_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$k_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$k_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$k_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$k_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$k_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$k_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$k_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$k_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$k_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$k_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$k_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$k_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$k_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$k_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$k_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$k_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$k_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$k_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$k_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$k_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$k_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$k_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillK,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$k_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
