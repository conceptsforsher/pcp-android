#include "pcp/event_signature_utils.hpp"
#include <sstream>

std::string verifyEventSignature(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"verifyEventSignature"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string signEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"signEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string parseSignatures(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseSignatures"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
