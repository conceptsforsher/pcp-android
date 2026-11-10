#include "pcp/event_delivery_utils.hpp"
#include <sstream>

std::string formatDeliveryStatus(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatDeliveryStatus"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
