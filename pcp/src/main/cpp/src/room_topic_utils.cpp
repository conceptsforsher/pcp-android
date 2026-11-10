#include "pcp/room_topic_utils.hpp"
#include <sstream>

std::string buildTopicEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildTopicEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseTopicFromEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseTopicFromEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string truncateTopic(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"truncateTopic"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatTopicPreview(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatTopicPreview"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
