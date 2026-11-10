#include "pcp/notification_filter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_filter_validate(const std::string& i) { return !i.empty(); }
std::string notification_filter_process(const std::string& i) { return i; }
json notification_filter_toJson(const std::string& i) { return json::object(); }
}
