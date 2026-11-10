#include "pcp/spoiler_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool spoiler_utils_validate(const std::string& i) { return !i.empty(); }
std::string spoiler_utils_process(const std::string& i) { return i; }
json spoiler_utils_toJson(const std::string& i) { return json::object(); }
}
