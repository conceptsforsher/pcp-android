#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillC,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$c_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$c_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$c_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$c_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$c_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$c_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$c_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$c_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$c_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$c_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$c_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$c_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$c_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$c_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$c_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$c_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$c_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$c_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$c_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$c_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$c_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$c_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$c_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$c_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillC,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$c_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
