#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillL,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$l_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$l_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$l_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$l_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$l_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$l_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$l_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$l_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$l_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$l_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$l_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$l_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$l_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$l_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$l_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$l_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$l_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$l_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$l_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$l_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$l_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$l_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$l_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$l_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillL,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$l_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
