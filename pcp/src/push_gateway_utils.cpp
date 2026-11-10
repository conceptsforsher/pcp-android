#include "pcp/push_gateway_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool push_gateway_utils_validate(const std::string& i) { return !i.empty(); }
std::string push_gateway_utils_process(const std::string& i) { return i; }
json push_gateway_utils_toJson(const std::string& i) { return json::object(); }
}
