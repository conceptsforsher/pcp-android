#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FM,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FM,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$m29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
