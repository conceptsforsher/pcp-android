#include "pcp/room_upgrade.hpp"
#include <sstream>

std::string parseUpgradeEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseUpgradeEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRecommendedVersion(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRecommendedVersion"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildUpgradeTombstone(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildUpgradeTombstone"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string canUpgrade(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"canUpgrade"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
