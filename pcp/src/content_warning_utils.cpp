#include "pcp/content_warning_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool content_warning_utils_validate(const std::string& i) { return !i.empty(); }
std::string content_warning_utils_process(const std::string& i) { return i; }
json content_warning_utils_toJson(const std::string& i) { return json::object(); }
}
