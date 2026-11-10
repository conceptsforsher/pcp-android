#include "pcp/event_receipt_utils.hpp"
#include <sstream>

std::string std(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"std"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReadReceiptContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReadReceiptContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildFullyReadContent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildFullyReadContent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
