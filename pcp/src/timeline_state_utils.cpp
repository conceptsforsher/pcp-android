#include "pcp/timeline_state_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool timeline_state_utils_validate(const std::string& i) { return !i.empty(); }
std::string timeline_state_utils_process(const std::string& i) { return i; }
json timeline_state_utils_toJson(const std::string& i) { return json::object(); }
}
