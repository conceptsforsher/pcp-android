#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillD,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$d_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$d_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$d_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$d_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$d_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$d_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$d_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$d_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$d_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$d_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$d_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$d_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$d_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$d_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$d_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$d_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$d_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$d_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$d_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$d_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$d_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$d_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$d_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$d_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillD,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$d_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
