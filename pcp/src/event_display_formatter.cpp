#include "pcp/event_display_formatter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_display_formatter_validate(const std::string& i) { return !i.empty(); }
std::string event_display_formatter_process(const std::string& i) { return i; }
json event_display_formatter_toJson(const std::string& i) { return json::object(); }
}
