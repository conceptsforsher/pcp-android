#include "pcp/avatar_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool avatar_utils_validate(const std::string& i) { return !i.empty(); }
std::string avatar_utils_process(const std::string& i) { return i; }
json avatar_utils_toJson(const std::string& i) { return json::object(); }
}
