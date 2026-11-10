#include "pcp/sso_provider_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sso_provider_utils_validate(const std::string& i) { return !i.empty(); }
std::string sso_provider_utils_process(const std::string& i) { return i; }
json sso_provider_utils_toJson(const std::string& i) { return json::object(); }
}
