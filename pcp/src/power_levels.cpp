#include "pcp/power_levels.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool power_levels_validate(const std::string& i) { return !i.empty(); }
std::string power_levels_process(const std::string& i) { return i; }
json power_levels_toJson(const std::string& i) { return json::object(); }
}
