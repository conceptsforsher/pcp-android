#include "pcp/reaction_aggregator.hpp"
#include <sstream>

std::string aggregateReactions(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"aggregateReactions"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getReactionCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getReactionCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string whoReactedWith(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"whoReactedWith"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string isSelfReaction(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"isSelfReaction"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
