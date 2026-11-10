#include "pcp/event_state_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_state_utils_validate(const std::string& i) { return !i.empty(); }
std::string event_state_utils_process(const std::string& i) { return i; }
json event_state_utils_toJson(const std::string& i) { return json::object(); }
}
