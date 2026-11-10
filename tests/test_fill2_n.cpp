#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FN,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FN,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$n29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
