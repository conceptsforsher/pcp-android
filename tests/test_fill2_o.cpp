#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FO,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FO,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$o29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
