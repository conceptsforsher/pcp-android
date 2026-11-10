#include "pcp/notification.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_validate(const std::string& i) { return !i.empty(); }
std::string notification_process(const std::string& i) { return i; }
json notification_toJson(const std::string& i) { return json::object(); }
}
