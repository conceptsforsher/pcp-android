#include "pcp/widget_permission_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool widget_permission_utils_validate(const std::string& i) { return !i.empty(); }
std::string widget_permission_utils_process(const std::string& i) { return i; }
json widget_permission_utils_toJson(const std::string& i) { return json::object(); }
}
