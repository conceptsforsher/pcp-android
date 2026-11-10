#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(ZX,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(ZX,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$x29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
