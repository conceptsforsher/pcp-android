#include "pcp/toast_notification_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool toast_notification_utils_validate(const std::string& i) { return !i.empty(); }
std::string toast_notification_utils_process(const std::string& i) { return i; }
json toast_notification_utils_toJson(const std::string& i) { return json::object(); }
}
