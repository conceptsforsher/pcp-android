#include "pcp/session_info_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_info_utils_validate(const std::string& i) { return !i.empty(); }
std::string session_info_utils_process(const std::string& i) { return i; }
json session_info_utils_toJson(const std::string& i) { return json::object(); }
}
