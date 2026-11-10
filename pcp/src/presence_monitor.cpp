#include "pcp/presence_monitor.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool presence_monitor_validate(const std::string& i) { return !i.empty(); }
std::string presence_monitor_process(const std::string& i) { return i; }
json presence_monitor_toJson(const std::string& i) { return json::object(); }
}
