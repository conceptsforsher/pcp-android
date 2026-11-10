#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(ZZ,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZZ,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$z29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
