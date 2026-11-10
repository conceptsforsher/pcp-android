#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Huge0,T1){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(0)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T2){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(1)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T3){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(2)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T4){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(3)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T5){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(4)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T6){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(5)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T7){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(6)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T8){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(7)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T9){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(8)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T10){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(9)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T11){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(10)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T12){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(11)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T13){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(12)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T14){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(13)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T15){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(14)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T16){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(15)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T17){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(16)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T18){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(17)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T19){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(18)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T20){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(19)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T21){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(20)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T22){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(21)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T23){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(22)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T24){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(23)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T25){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(24)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T26){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(25)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T27){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(26)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T28){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(27)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T29){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(28)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T30){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(29)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T31){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(30)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T32){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(31)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T33){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(32)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T34){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(33)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T35){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(34)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T36){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(35)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T37){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(36)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T38){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(37)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T39){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(38)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T40){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(39)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T41){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(40)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T42){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(41)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T43){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(42)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T44){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(43)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T45){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(44)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T46){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(45)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T47){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(46)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T48){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(47)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T49){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(48)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T50){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(49)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T51){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(50)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T52){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(51)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T53){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(52)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T54){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(53)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T55){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(54)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T56){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(55)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T57){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(56)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T58){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(57)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T59){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(58)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T60){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(59)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T61){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(60)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T62){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(61)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T63){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(62)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T64){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(63)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T65){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(64)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T66){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(65)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T67){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(66)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T68){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(67)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T69){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(68)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T70){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(69)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T71){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(70)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T72){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(71)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T73){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(72)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T74){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(73)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T75){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(74)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T76){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(75)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T77){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(76)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T78){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(77)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T79){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(78)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T80){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(79)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T81){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(80)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T82){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(81)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T83){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(82)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T84){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(83)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T85){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(84)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T86){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(85)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T87){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(86)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T88){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(87)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T89){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(88)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T90){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(89)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T91){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(90)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T92){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(91)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T93){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(92)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T94){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(93)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T95){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(94)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T96){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(95)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T97){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(96)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T98){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(97)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T99){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(98)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

TEST(Huge0,T100){json t;for(int e=0;e<5;++e){json v;v["eid"]="$h0_"+std::to_string(99)+"_"+std::to_string(e)+":m";v["c"]["b"]="B"+std::to_string(e);t["evs"].push_back(v);ASSERT_FALSE(v["eid"].empty());}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());auto p=json::parse(d);ASSERT_EQ(p["evs"].size(),5);}

}}
