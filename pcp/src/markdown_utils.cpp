#include "pcp/markdown_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool markdown_utils_validate(const std::string& i) { return !i.empty(); }
std::string markdown_utils_process(const std::string& i) { return i; }
json markdown_utils_toJson(const std::string& i) { return json::object(); }
}
