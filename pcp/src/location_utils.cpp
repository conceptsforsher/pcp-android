#include "pcp/location_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool location_utils_validate(const std::string& i) { return !i.empty(); }
std::string location_utils_process(const std::string& i) { return i; }
json location_utils_toJson(const std::string& i) { return json::object(); }
}
