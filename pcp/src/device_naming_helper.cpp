#include "pcp/device_naming_helper.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool device_naming_helper_validate(const std::string& i) { return !i.empty(); }
std::string device_naming_helper_process(const std::string& i) { return i; }
json device_naming_helper_toJson(const std::string& i) { return json::object(); }
}
