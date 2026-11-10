#include "pcp/timeline_render_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool timeline_render_utils_validate(const std::string& i) { return !i.empty(); }
std::string timeline_render_utils_process(const std::string& i) { return i; }
json timeline_render_utils_toJson(const std::string& i) { return json::object(); }
}
