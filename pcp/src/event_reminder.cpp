#include "pcp/event_reminder.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_reminder_validate(const std::string& i) { return !i.empty(); }
std::string event_reminder_process(const std::string& i) { return i; }
json event_reminder_toJson(const std::string& i) { return json::object(); }
}
