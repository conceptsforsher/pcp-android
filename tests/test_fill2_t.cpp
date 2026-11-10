#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FT,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FT,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$t29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
