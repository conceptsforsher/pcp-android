#include "pcp/openid_token.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool openid_token_validate(const std::string& i) { return !i.empty(); }
std::string openid_token_process(const std::string& i) { return i; }
json openid_token_toJson(const std::string& i) { return json::object(); }
}
