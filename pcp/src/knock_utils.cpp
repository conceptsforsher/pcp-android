#include "pcp/knock_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool knock_utils_validate(const std::string& i) { return !i.empty(); }
std::string knock_utils_process(const std::string& i) { return i; }
json knock_utils_toJson(const std::string& i) { return json::object(); }
}
