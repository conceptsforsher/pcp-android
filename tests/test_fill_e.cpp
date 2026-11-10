#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillE,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$e_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$e_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$e_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$e_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$e_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$e_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$e_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$e_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$e_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$e_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$e_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$e_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$e_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$e_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$e_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$e_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$e_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$e_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$e_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$e_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$e_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$e_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$e_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$e_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillE,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$e_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
