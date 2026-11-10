#include "pcp/notification_grouping.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_grouping_validate(const std::string& i) { return !i.empty(); }
std::string notification_grouping_process(const std::string& i) { return i; }
json notification_grouping_toJson(const std::string& i) { return json::object(); }
}
