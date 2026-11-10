#include "pcp/user_invite_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool user_invite_utils_validate(const std::string& i) { return !i.empty(); }
std::string user_invite_utils_process(const std::string& i) { return i; }
json user_invite_utils_toJson(const std::string& i) { return json::object(); }
}
