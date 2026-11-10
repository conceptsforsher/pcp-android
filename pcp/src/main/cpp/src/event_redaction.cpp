#include "pcp/event_redaction.hpp"
#include <sstream>

std::string buildRedactionContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildRedactionContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string applyRedaction(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"applyRedaction"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatRedactionNotice(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatRedactionNotice"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
