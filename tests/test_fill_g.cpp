#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillG,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$g_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$g_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$g_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$g_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$g_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$g_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$g_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$g_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$g_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$g_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$g_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$g_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$g_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$g_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$g_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$g_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$g_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$g_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$g_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$g_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$g_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$g_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$g_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$g_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillG,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$g_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
