#include "pcp/notification_summary.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_summary_validate(const std::string& i) { return !i.empty(); }
std::string notification_summary_process(const std::string& i) { return i; }
json notification_summary_toJson(const std::string& i) { return json::object(); }
}
