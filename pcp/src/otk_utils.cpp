#include "pcp/otk_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool otk_utils_validate(const std::string& i) { return !i.empty(); }
std::string otk_utils_process(const std::string& i) { return i; }
json otk_utils_toJson(const std::string& i) { return json::object(); }
}
