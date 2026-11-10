#include "pcp/media_config_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_config_utils_validate(const std::string& i) { return !i.empty(); }
std::string media_config_utils_process(const std::string& i) { return i; }
json media_config_utils_toJson(const std::string& i) { return json::object(); }
}
