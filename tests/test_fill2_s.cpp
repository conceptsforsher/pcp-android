#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FS,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FS,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$s29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
