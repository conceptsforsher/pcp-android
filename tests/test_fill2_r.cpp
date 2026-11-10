#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FR,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FR,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$r29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
