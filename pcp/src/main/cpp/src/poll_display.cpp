#include "pcp/poll_display.hpp"
#include <sstream>

std::string formatPollResults(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatPollResults"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatPollBar(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatPollBar"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getPollWinner(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getPollWinner"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
