#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(F2,T1){json t;t["s"]="F2";t["n"]=0;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(0)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T2){json t;t["s"]="F2";t["n"]=1;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(1)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T3){json t;t["s"]="F2";t["n"]=2;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(2)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T4){json t;t["s"]="F2";t["n"]=3;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(3)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T5){json t;t["s"]="F2";t["n"]=4;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(4)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T6){json t;t["s"]="F2";t["n"]=5;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(5)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T7){json t;t["s"]="F2";t["n"]=6;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(6)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T8){json t;t["s"]="F2";t["n"]=7;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(7)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T9){json t;t["s"]="F2";t["n"]=8;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(8)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T10){json t;t["s"]="F2";t["n"]=9;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(9)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T11){json t;t["s"]="F2";t["n"]=10;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(10)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T12){json t;t["s"]="F2";t["n"]=11;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(11)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T13){json t;t["s"]="F2";t["n"]=12;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(12)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T14){json t;t["s"]="F2";t["n"]=13;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(13)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T15){json t;t["s"]="F2";t["n"]=14;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(14)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T16){json t;t["s"]="F2";t["n"]=15;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(15)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T17){json t;t["s"]="F2";t["n"]=16;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(16)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T18){json t;t["s"]="F2";t["n"]=17;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(17)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T19){json t;t["s"]="F2";t["n"]=18;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(18)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T20){json t;t["s"]="F2";t["n"]=19;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(19)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T21){json t;t["s"]="F2";t["n"]=20;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(20)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T22){json t;t["s"]="F2";t["n"]=21;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(21)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T23){json t;t["s"]="F2";t["n"]=22;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(22)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T24){json t;t["s"]="F2";t["n"]=23;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(23)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T25){json t;t["s"]="F2";t["n"]=24;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(24)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T26){json t;t["s"]="F2";t["n"]=25;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(25)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T27){json t;t["s"]="F2";t["n"]=26;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(26)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T28){json t;t["s"]="F2";t["n"]=27;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(27)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T29){json t;t["s"]="F2";t["n"]=28;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(28)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T30){json t;t["s"]="F2";t["n"]=29;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(29)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T31){json t;t["s"]="F2";t["n"]=30;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(30)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T32){json t;t["s"]="F2";t["n"]=31;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(31)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T33){json t;t["s"]="F2";t["n"]=32;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(32)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T34){json t;t["s"]="F2";t["n"]=33;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(33)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T35){json t;t["s"]="F2";t["n"]=34;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(34)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T36){json t;t["s"]="F2";t["n"]=35;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(35)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T37){json t;t["s"]="F2";t["n"]=36;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(36)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T38){json t;t["s"]="F2";t["n"]=37;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(37)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T39){json t;t["s"]="F2";t["n"]=38;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(38)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

TEST(F2,T40){json t;t["s"]="F2";t["n"]=39;for(int e=0;e<5;++e){json v;v["eid"]="$f2_"+std::to_string(39)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
ASSERT_EQ(t["evs"].size(),5);t["st"]["i"]=true;ASSERT_TRUE(t["st"]["i"]);auto d=t.dump();ASSERT_FALSE(d.empty());}

}}
