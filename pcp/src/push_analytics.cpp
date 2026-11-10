#include "pcp/push_analytics.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool push_analytics_validate(const std::string& i) { return !i.empty(); }
std::string push_analytics_process(const std::string& i) { return i; }
json push_analytics_toJson(const std::string& i) { return json::object(); }
}
