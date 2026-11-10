#include "pcp/admin_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool admin_utils_validate(const std::string& i) { return !i.empty(); }
std::string admin_utils_process(const std::string& i) { return i; }
json admin_utils_toJson(const std::string& i) { return json::object(); }
}
