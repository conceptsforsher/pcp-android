#include "pcp/call_event_formatter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool call_event_formatter_validate(const std::string& i) { return !i.empty(); }
std::string call_event_formatter_process(const std::string& i) { return i; }
json call_event_formatter_toJson(const std::string& i) { return json::object(); }
}
