#include "pcp/invite_validator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool invite_validator_validate(const std::string& i) { return !i.empty(); }
std::string invite_validator_process(const std::string& i) { return i; }
json invite_validator_toJson(const std::string& i) { return json::object(); }
}
