#include "pcp/cross_sign_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool cross_sign_utils_validate(const std::string& i) { return !i.empty(); }
std::string cross_sign_utils_process(const std::string& i) { return i; }
json cross_sign_utils_toJson(const std::string& i) { return json::object(); }
}
