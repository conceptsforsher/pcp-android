#include "pcp/thirdparty_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool thirdparty_utils_validate(const std::string& i) { return !i.empty(); }
std::string thirdparty_utils_process(const std::string& i) { return i; }
json thirdparty_utils_toJson(const std::string& i) { return json::object(); }
}
