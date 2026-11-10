#include "pcp/url_preview_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool url_preview_utils_validate(const std::string& i) { return !i.empty(); }
std::string url_preview_utils_process(const std::string& i) { return i; }
json url_preview_utils_toJson(const std::string& i) { return json::object(); }
}
