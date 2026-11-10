#include "pcp/display_name_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool display_name_utils_validate(const std::string& i) { return !i.empty(); }
std::string display_name_utils_process(const std::string& i) { return i; }
json display_name_utils_toJson(const std::string& i) { return json::object(); }
}
