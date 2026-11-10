#include "pcp/session_signout.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_signout_validate(const std::string& i) { return !i.empty(); }
std::string session_signout_process(const std::string& i) { return i; }
json session_signout_toJson(const std::string& i) { return json::object(); }
}
