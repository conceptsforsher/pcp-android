#include "pcp/rainbow_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool rainbow_utils_validate(const std::string& i) { return !i.empty(); }
std::string rainbow_utils_process(const std::string& i) { return i; }
json rainbow_utils_toJson(const std::string& i) { return json::object(); }
}
