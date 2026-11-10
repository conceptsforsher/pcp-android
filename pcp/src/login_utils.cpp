#include "pcp/login_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool login_utils_validate(const std::string& i) { return !i.empty(); }
std::string login_utils_process(const std::string& i) { return i; }
json login_utils_toJson(const std::string& i) { return json::object(); }
}
