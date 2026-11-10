#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;

TEST(FillH,T1){json t;t["i"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$h_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T2){json t;t["i"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$h_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T3){json t;t["i"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$h_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T4){json t;t["i"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$h_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T5){json t;t["i"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$h_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T6){json t;t["i"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$h_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T7){json t;t["i"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$h_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T8){json t;t["i"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$h_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T9){json t;t["i"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$h_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T10){json t;t["i"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$h_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T11){json t;t["i"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$h_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T12){json t;t["i"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$h_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T13){json t;t["i"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$h_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T14){json t;t["i"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$h_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T15){json t;t["i"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$h_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T16){json t;t["i"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$h_15_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T17){json t;t["i"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$h_16_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T18){json t;t["i"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$h_17_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T19){json t;t["i"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$h_18_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T20){json t;t["i"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$h_19_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T21){json t;t["i"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$h_20_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T22){json t;t["i"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$h_21_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T23){json t;t["i"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$h_22_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T24){json t;t["i"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$h_23_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}

TEST(FillH,T25){json t;t["i"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$h_24_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),5);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
