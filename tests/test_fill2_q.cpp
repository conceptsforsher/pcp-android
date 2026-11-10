#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FQ,T1){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q0e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T2){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q1e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T3){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q2e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T4){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q3e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T5){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q4e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T6){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q5e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T7){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q6e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T8){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q7e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T9){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q8e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T10){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q9e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T11){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q10e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T12){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q11e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T13){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q12e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T14){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q13e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T15){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q14e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T16){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q15e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T17){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q16e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T18){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q17e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T19){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q18e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T20){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q19e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T21){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q20e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T22){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q21e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T23){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q22e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T24){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q23e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T25){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q24e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T26){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q25e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T27){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q26e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T28){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q27e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T29){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q28e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
TEST(FQ,T30){json t;for(int e=0;e<3;++e){json v;v["eid"]="$q29e"+std::to_string(e)+":m";t["evs"].push_back(v);}ASSERT_EQ(t["evs"].size(),3);auto s=t.dump();(void)s;}
}}
