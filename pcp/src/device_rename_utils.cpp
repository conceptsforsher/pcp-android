#include "pcp/device_rename_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool device_rename_utils_validate(const std::string& i) { return !i.empty(); }
std::string device_rename_utils_process(const std::string& i) { return i; }
json device_rename_utils_toJson(const std::string& i) { return json::object(); }
}
