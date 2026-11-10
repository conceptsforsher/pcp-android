#include "pcp/sync_state_monitor.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sync_state_monitor_validate(const std::string& i) { return !i.empty(); }
std::string sync_state_monitor_process(const std::string& i) { return i; }
json sync_state_monitor_toJson(const std::string& i) { return json::object(); }
}
