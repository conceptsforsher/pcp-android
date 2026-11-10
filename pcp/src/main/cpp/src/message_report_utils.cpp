#include "pcp/message_report_utils.hpp"
#include <sstream>

std::string parseReportEvent(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"parseReportEvent"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string buildReportBody(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"buildReportBody"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getReportReason(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getReportReason"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string validateServerReport(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"validateServerReport"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
