#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
namespace pcp { namespace test { using json = nlohmann::json;
TEST(FinalBatch,T1){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_0_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T2){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_1_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T3){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_2_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T4){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_3_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T5){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_4_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T6){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_5_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T7){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_6_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T8){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_7_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T9){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_8_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T10){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_9_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T11){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_10_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T12){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_11_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T13){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_12_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T14){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_13_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
TEST(FinalBatch,T15){
 json t; for(int e=0;e<7;++e){json v;v["eid"]="$fin_14_"+std::to_string(e)+":m";t["evs"].push_back(v);}
 ASSERT_EQ(t["evs"].size(),7); ASSERT_NO_THROW({auto d=t.dump();});
}
}}
