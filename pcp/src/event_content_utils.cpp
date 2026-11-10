#include "pcp/event_content_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_content_utils_validate(const std::string& i) { return !i.empty(); }
std::string event_content_utils_process(const std::string& i) { return i; }
json event_content_utils_toJson(const std::string& i) { return json::object(); }
}
