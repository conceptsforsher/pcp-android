#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Mass0,T1){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(0)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T2){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(1)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T3){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(2)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T4){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(3)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T5){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(4)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T6){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(5)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T7){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(6)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T8){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(7)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T9){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(8)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T10){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(9)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T11){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(10)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T12){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(11)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T13){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(12)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T14){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(13)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T15){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(14)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T16){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(15)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T17){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(16)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T18){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(17)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T19){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(18)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T20){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(19)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T21){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(20)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T22){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(21)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T23){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(22)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T24){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(23)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T25){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(24)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T26){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(25)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T27){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(26)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T28){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(27)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T29){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(28)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T30){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(29)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T31){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(30)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T32){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(31)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T33){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(32)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T34){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(33)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T35){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(34)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T36){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(35)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T37){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(36)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T38){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(37)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T39){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(38)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T40){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(39)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T41){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(40)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T42){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(41)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T43){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(42)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T44){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(43)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T45){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(44)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T46){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(45)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T47){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(46)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T48){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(47)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T49){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(48)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T50){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(49)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T51){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(50)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T52){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(51)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T53){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(52)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T54){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(53)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T55){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(54)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T56){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(55)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T57){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(56)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T58){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(57)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T59){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(58)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T60){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(59)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T61){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(60)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T62){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(61)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T63){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(62)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T64){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(63)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T65){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(64)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T66){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(65)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T67){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(66)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T68){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(67)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T69){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(68)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T70){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(69)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T71){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(70)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T72){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(71)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T73){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(72)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T74){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(73)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T75){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(74)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T76){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(75)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T77){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(76)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T78){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(77)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T79){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(78)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T80){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(79)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T81){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(80)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T82){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(81)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T83){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(82)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T84){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(83)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T85){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(84)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T86){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(85)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T87){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(86)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T88){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(87)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T89){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(88)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T90){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(89)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T91){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(90)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T92){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(91)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T93){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(92)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T94){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(93)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T95){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(94)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T96){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(95)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T97){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(96)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T98){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(97)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T99){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(98)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T100){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(99)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T101){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(100)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T102){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(101)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T103){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(102)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T104){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(103)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T105){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(104)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T106){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(105)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T107){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(106)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T108){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(107)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T109){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(108)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T110){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(109)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T111){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(110)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T112){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(111)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T113){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(112)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T114){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(113)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T115){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(114)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T116){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(115)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T117){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(116)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T118){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(117)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T119){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(118)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T120){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(119)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T121){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(120)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T122){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(121)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T123){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(122)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T124){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(123)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T125){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(124)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T126){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(125)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T127){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(126)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T128){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(127)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T129){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(128)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T130){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(129)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T131){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(130)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T132){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(131)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T133){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(132)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T134){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(133)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T135){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(134)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T136){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(135)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T137){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(136)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T138){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(137)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T139){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(138)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T140){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(139)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T141){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(140)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T142){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(141)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T143){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(142)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T144){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(143)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T145){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(144)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T146){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(145)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T147){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(146)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T148){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(147)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T149){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(148)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Mass0,T150){json t;for(int e=0;e<5;++e){json v;v["eid"]="$m0_"+std::to_string(149)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

}}
