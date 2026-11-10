#include "pcp/widget_event_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool widget_event_utils_validate(const std::string& i) { return !i.empty(); }
std::string widget_event_utils_process(const std::string& i) { return i; }
json widget_event_utils_toJson(const std::string& i) { return json::object(); }
}
