#include "pcp/permission_validator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool permission_validator_validate(const std::string& i) { return !i.empty(); }
std::string permission_validator_process(const std::string& i) { return i; }
json permission_validator_toJson(const std::string& i) { return json::object(); }
}
