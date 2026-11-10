#include "pcp/notification_action_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_action_utils_validate(const std::string& i) { return !i.empty(); }
std::string notification_action_utils_process(const std::string& i) { return i; }
json notification_action_utils_toJson(const std::string& i) { return json::object(); }
}
