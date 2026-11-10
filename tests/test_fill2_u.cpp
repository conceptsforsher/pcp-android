#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FU,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FU,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$u29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
