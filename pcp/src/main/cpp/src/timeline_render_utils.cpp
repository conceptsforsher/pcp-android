#include "pcp/timeline_render_utils.hpp"
#include <sstream>

std::string formatDateSeparator(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDateSeparator"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatNewMessagesDivider(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatNewMessagesDivider"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
