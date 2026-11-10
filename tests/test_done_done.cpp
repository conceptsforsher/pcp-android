#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;
TEST(DoneDone,T1){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T2){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T3){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T4){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T5){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T6){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T7){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T8){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T9){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T10){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T11){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T12){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T13){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T14){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}
TEST(DoneDone,T15){json t;for(int e=0;e<2;++e){json v;v["eid"]="$dd_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),2);auto d=t.dump();ASSERT_FALSE(d.empty());}

}}
