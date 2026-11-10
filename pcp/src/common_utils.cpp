#include "pcp/common_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool common_utils_validate(const std::string& i) { return !i.empty(); }
std::string common_utils_process(const std::string& i) { return i; }
json common_utils_toJson(const std::string& i) { return json::object(); }
}
