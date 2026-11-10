#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FP,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FP,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$p29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
