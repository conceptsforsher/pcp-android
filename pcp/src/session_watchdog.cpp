#include "pcp/session_watchdog.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_watchdog_validate(const std::string& i) { return !i.empty(); }
std::string session_watchdog_process(const std::string& i) { return i; }
json session_watchdog_toJson(const std::string& i) { return json::object(); }
}
