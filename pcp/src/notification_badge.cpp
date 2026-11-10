#include "pcp/notification_badge.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_badge_validate(const std::string& i) { return !i.empty(); }
std::string notification_badge_process(const std::string& i) { return i; }
json notification_badge_toJson(const std::string& i) { return json::object(); }
}
