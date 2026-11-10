#include "pcp/user_badge_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool user_badge_utils_validate(const std::string& i) { return !i.empty(); }
std::string user_badge_utils_process(const std::string& i) { return i; }
json user_badge_utils_toJson(const std::string& i) { return json::object(); }
}
