#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillJ,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$j_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$j_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$j_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$j_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$j_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$j_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$j_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$j_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$j_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$j_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$j_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$j_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$j_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$j_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$j_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$j_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$j_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$j_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$j_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$j_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$j_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$j_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$j_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$j_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillJ,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$j_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
