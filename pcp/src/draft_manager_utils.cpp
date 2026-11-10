#include "pcp/draft_manager_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool draft_manager_utils_validate(const std::string& i) { return !i.empty(); }
std::string draft_manager_utils_process(const std::string& i) { return i; }
json draft_manager_utils_toJson(const std::string& i) { return json::object(); }
}
