#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <string>

namespace pcp { namespace test {
using json = nlohmann::json;

TEST(Expand5, T1) {
  json t; t["s"]="Expand5"; t["n"]=0;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(0)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T2) {
  json t; t["s"]="Expand5"; t["n"]=1;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(1)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T3) {
  json t; t["s"]="Expand5"; t["n"]=2;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(2)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T4) {
  json t; t["s"]="Expand5"; t["n"]=3;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(3)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T5) {
  json t; t["s"]="Expand5"; t["n"]=4;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(4)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T6) {
  json t; t["s"]="Expand5"; t["n"]=5;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(5)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T7) {
  json t; t["s"]="Expand5"; t["n"]=6;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(6)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T8) {
  json t; t["s"]="Expand5"; t["n"]=7;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(7)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T9) {
  json t; t["s"]="Expand5"; t["n"]=8;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(8)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T10) {
  json t; t["s"]="Expand5"; t["n"]=9;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(9)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T11) {
  json t; t["s"]="Expand5"; t["n"]=10;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(10)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T12) {
  json t; t["s"]="Expand5"; t["n"]=11;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(11)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T13) {
  json t; t["s"]="Expand5"; t["n"]=12;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(12)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T14) {
  json t; t["s"]="Expand5"; t["n"]=13;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(13)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T15) {
  json t; t["s"]="Expand5"; t["n"]=14;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(14)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T16) {
  json t; t["s"]="Expand5"; t["n"]=15;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(15)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T17) {
  json t; t["s"]="Expand5"; t["n"]=16;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(16)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T18) {
  json t; t["s"]="Expand5"; t["n"]=17;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(17)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T19) {
  json t; t["s"]="Expand5"; t["n"]=18;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(18)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T20) {
  json t; t["s"]="Expand5"; t["n"]=19;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(19)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T21) {
  json t; t["s"]="Expand5"; t["n"]=20;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(20)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T22) {
  json t; t["s"]="Expand5"; t["n"]=21;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(21)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T23) {
  json t; t["s"]="Expand5"; t["n"]=22;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(22)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T24) {
  json t; t["s"]="Expand5"; t["n"]=23;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(23)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T25) {
  json t; t["s"]="Expand5"; t["n"]=24;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(24)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T26) {
  json t; t["s"]="Expand5"; t["n"]=25;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(25)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T27) {
  json t; t["s"]="Expand5"; t["n"]=26;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(26)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T28) {
  json t; t["s"]="Expand5"; t["n"]=27;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(27)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T29) {
  json t; t["s"]="Expand5"; t["n"]=28;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(28)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T30) {
  json t; t["s"]="Expand5"; t["n"]=29;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(29)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T31) {
  json t; t["s"]="Expand5"; t["n"]=30;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(30)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T32) {
  json t; t["s"]="Expand5"; t["n"]=31;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(31)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T33) {
  json t; t["s"]="Expand5"; t["n"]=32;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(32)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T34) {
  json t; t["s"]="Expand5"; t["n"]=33;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(33)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T35) {
  json t; t["s"]="Expand5"; t["n"]=34;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(34)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T36) {
  json t; t["s"]="Expand5"; t["n"]=35;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(35)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T37) {
  json t; t["s"]="Expand5"; t["n"]=36;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(36)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T38) {
  json t; t["s"]="Expand5"; t["n"]=37;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(37)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T39) {
  json t; t["s"]="Expand5"; t["n"]=38;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(38)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T40) {
  json t; t["s"]="Expand5"; t["n"]=39;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(39)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T41) {
  json t; t["s"]="Expand5"; t["n"]=40;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(40)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T42) {
  json t; t["s"]="Expand5"; t["n"]=41;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(41)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T43) {
  json t; t["s"]="Expand5"; t["n"]=42;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(42)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T44) {
  json t; t["s"]="Expand5"; t["n"]=43;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(43)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T45) {
  json t; t["s"]="Expand5"; t["n"]=44;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(44)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T46) {
  json t; t["s"]="Expand5"; t["n"]=45;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(45)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T47) {
  json t; t["s"]="Expand5"; t["n"]=46;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(46)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T48) {
  json t; t["s"]="Expand5"; t["n"]=47;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(47)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T49) {
  json t; t["s"]="Expand5"; t["n"]=48;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(48)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T50) {
  json t; t["s"]="Expand5"; t["n"]=49;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(49)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T51) {
  json t; t["s"]="Expand5"; t["n"]=50;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(50)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T52) {
  json t; t["s"]="Expand5"; t["n"]=51;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(51)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T53) {
  json t; t["s"]="Expand5"; t["n"]=52;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(52)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T54) {
  json t; t["s"]="Expand5"; t["n"]=53;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(53)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T55) {
  json t; t["s"]="Expand5"; t["n"]=54;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(54)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T56) {
  json t; t["s"]="Expand5"; t["n"]=55;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(55)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T57) {
  json t; t["s"]="Expand5"; t["n"]=56;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(56)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T58) {
  json t; t["s"]="Expand5"; t["n"]=57;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(57)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T59) {
  json t; t["s"]="Expand5"; t["n"]=58;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(58)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

TEST(Expand5, T60) {
  json t; t["s"]="Expand5"; t["n"]=59;
  for(int e=0;e<5;++e){json v;v["eid"]="$ex5_"+std::to_string(59)+"_"+std::to_string(e)+":m";v["c"]["b"]="Body "+std::to_string(e);t["evs"].push_back(v);}
  ASSERT_EQ(t["evs"].size(),5);auto d=t.dump();ASSERT_FALSE(d.empty());
}

}}
