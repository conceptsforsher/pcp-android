#include "pcp/account_validator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool account_validator_validate(const std::string& i) { return !i.empty(); }
std::string account_validator_process(const std::string& i) { return i; }
json account_validator_toJson(const std::string& i) { return json::object(); }
}
