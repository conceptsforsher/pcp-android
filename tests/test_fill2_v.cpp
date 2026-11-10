#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FV,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FV,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$v29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
