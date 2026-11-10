#include "pcp/account_deactivation_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool account_deactivation_utils_validate(const std::string& i) { return !i.empty(); }
std::string account_deactivation_utils_process(const std::string& i) { return i; }
json account_deactivation_utils_toJson(const std::string& i) { return json::object(); }
}
