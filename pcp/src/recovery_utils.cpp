#include "pcp/recovery_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool recovery_utils_validate(const std::string& i) { return !i.empty(); }
std::string recovery_utils_process(const std::string& i) { return i; }
json recovery_utils_toJson(const std::string& i) { return json::object(); }
}
