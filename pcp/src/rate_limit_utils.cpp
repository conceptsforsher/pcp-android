#include "pcp/rate_limit_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool rate_limit_utils_validate(const std::string& i) { return !i.empty(); }
std::string rate_limit_utils_process(const std::string& i) { return i; }
json rate_limit_utils_toJson(const std::string& i) { return json::object(); }
}
