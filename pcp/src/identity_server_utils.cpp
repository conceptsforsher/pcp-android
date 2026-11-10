#include "pcp/identity_server_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool identity_server_utils_validate(const std::string& i) { return !i.empty(); }
std::string identity_server_utils_process(const std::string& i) { return i; }
json identity_server_utils_toJson(const std::string& i) { return json::object(); }
}
