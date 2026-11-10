#include "pcp/push_rule_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool push_rule_utils_validate(const std::string& i) { return !i.empty(); }
std::string push_rule_utils_process(const std::string& i) { return i; }
json push_rule_utils_toJson(const std::string& i) { return json::object(); }
}
