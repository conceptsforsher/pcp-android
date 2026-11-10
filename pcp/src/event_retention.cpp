#include "pcp/event_retention.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_retention_validate(const std::string& i) { return !i.empty(); }
std::string event_retention_process(const std::string& i) { return i; }
json event_retention_toJson(const std::string& i) { return json::object(); }
}
