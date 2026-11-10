#include "pcp/event_delivery_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_delivery_utils_validate(const std::string& i) { return !i.empty(); }
std::string event_delivery_utils_process(const std::string& i) { return i; }
json event_delivery_utils_toJson(const std::string& i) { return json::object(); }
}
