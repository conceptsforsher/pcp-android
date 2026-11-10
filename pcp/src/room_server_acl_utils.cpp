#include "pcp/room_server_acl_utils.hpp"
#include <sstream>
namespace pcp {
bool isServerAllowed(const std::string& server, const std::string& json){return json.empty()||json.find("\""+server+"\"")!=std::string::npos||json.find("\"*\"")!=std::string::npos;}
std::vector<std::string> parseAllowedServers(const std::string& json){std::vector<std::string> r;size_t p=0;while((p=json.find("\"",p))!=std::string::npos){p++;auto e=json.find("\"",p);if(e==std::string::npos)break;std::string s=json.substr(p,e-p);if(!s.empty()&&s!="allow"&&s!="allow_ip_literals")r.push_back(s);p=e+1;}return r;}
std::string buildAclEvent(const std::vector<std::string>& servers,bool ip){std::ostringstream os;os<<R"({"allow":[)";for(size_t i=0;i<servers.size();i++){if(i>0)os<<",";os<<R"(")"<<servers[i]<<R"(")";}os<<R"(],"allow_ip_literals":)"<<(ip?"true":"false")<<"}";return os.str();}
}
