#include "pcp/event_aggregation_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_aggregation_utils_validate(const std::string& i) { return !i.empty(); }
std::string event_aggregation_utils_process(const std::string& i) { return i; }
json event_aggregation_utils_toJson(const std::string& i) { return json::object(); }
}
