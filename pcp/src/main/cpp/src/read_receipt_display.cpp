#include "pcp/read_receipt_display.hpp"
#include <sstream>

std::string formatReadReceiptCount(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReadReceiptCount"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string formatReadReceiptNames(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatReadReceiptNames"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
