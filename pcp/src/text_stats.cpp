#include "pcp/text_stats.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool text_stats_validate(const std::string& i) { return !i.empty(); }
std::string text_stats_process(const std::string& i) { return i; }
json text_stats_toJson(const std::string& i) { return json::object(); }
}
