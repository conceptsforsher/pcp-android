#include "pcp/olm_session_utils.hpp"
#include <sstream>

std::string formatOlmSession(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatOlmSession"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildOlmSessionId(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildOlmSessionId"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatKeyFingerprint(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatKeyFingerprint"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
