#include "pcp/power_level_visualizer.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool power_level_visualizer_validate(const std::string& i) { return !i.empty(); }
std::string power_level_visualizer_process(const std::string& i) { return i; }
json power_level_visualizer_toJson(const std::string& i) { return json::object(); }
}
