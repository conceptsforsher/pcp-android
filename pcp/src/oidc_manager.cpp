#include "pcp/oidc_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool oidc_manager_validate(const std::string& i) { return !i.empty(); }
std::string oidc_manager_process(const std::string& i) { return i; }
json oidc_manager_toJson(const std::string& i) { return json::object(); }
}
