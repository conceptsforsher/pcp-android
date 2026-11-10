#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(LastOne,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
TEST(LastOne,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$last_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();ASSERT_FALSE(s.empty());}
}}
