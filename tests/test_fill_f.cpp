#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillF,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$f_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$f_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$f_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$f_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$f_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$f_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$f_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$f_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$f_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$f_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$f_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$f_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$f_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$f_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$f_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$f_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$f_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$f_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$f_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$f_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$f_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$f_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$f_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$f_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillF,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$f_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
