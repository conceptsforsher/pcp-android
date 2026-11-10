#include "pcp/weather_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool weather_utils_validate(const std::string& i) { return !i.empty(); }
std::string weather_utils_process(const std::string& i) { return i; }
json weather_utils_toJson(const std::string& i) { return json::object(); }
}
