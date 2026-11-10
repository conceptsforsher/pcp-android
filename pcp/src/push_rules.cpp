#include "pcp/push_rules.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool push_rules_validate(const std::string& i) { return !i.empty(); }
std::string push_rules_process(const std::string& i) { return i; }
json push_rules_toJson(const std::string& i) { return json::object(); }
}
