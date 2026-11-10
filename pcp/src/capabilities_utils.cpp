#include "pcp/capabilities_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool capabilities_utils_validate(const std::string& i) { return !i.empty(); }
std::string capabilities_utils_process(const std::string& i) { return i; }
json capabilities_utils_toJson(const std::string& i) { return json::object(); }
}
