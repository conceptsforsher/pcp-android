#include "pcp/room_retention_utils.hpp"
#include <sstream>
namespace pcp {
int64_t parseRetentionPeriod(const std::string& json){auto p=json.find("\"max_lifetime\":");if(p==std::string::npos)return 0;p+=15;try{return std::stoll(json.substr(p));}catch(...){return 0;}}
bool hasMessageRetention(const std::string& json){return json.find("\"max_lifetime\"")!=std::string::npos;}
std::string buildRetentionEvent(int64_t ms){std::ostringstream os;os<<R"({"max_lifetime":)"<<ms<<"}";return os.str();}
}
