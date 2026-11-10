#include "pcp/login_token_util.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool login_token_util_validate(const std::string& i) { return !i.empty(); }
std::string login_token_util_process(const std::string& i) { return i; }
json login_token_util_toJson(const std::string& i) { return json::object(); }
}
