#include "pcp/push_condition.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool push_condition_validate(const std::string& i) { return !i.empty(); }
std::string push_condition_process(const std::string& i) { return i; }
json push_condition_toJson(const std::string& i) { return json::object(); }
}
