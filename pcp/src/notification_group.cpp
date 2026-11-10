#include "pcp/notification_group.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_group_validate(const std::string& i) { return !i.empty(); }
std::string notification_group_process(const std::string& i) { return i; }
json notification_group_toJson(const std::string& i) { return json::object(); }
}
