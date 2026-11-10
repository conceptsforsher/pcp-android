#include "pcp/matrix_error_utils.hpp"
#include <sstream>

std::string formatMatrixError(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"formatMatrixError"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
std::string getFriendlyErrorMessage(const std::string& json){if(json.empty())return R"({"ok":false})";std::ostringstream o;o<<R"({"ok":true,"fn":")"<<"getFriendlyErrorMessage"<<R"(","sz":)"<<json.size()<<"}";return o.str();}
