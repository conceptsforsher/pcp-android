#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(ZW,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZW,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$w29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
