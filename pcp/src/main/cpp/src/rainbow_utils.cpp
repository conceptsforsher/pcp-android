#include "pcp/rainbow_utils.hpp"
#include <sstream>

std::string textToRainbow(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"textToRainbow"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string textToWave(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"textToWave"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getRainbowColor(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getRainbowColor"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string stripFormatCommands(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"stripFormatCommands"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
