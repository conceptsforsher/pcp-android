#include "pcp/thirdparty_invite.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool thirdparty_invite_validate(const std::string& i) { return !i.empty(); }
std::string thirdparty_invite_process(const std::string& i) { return i; }
json thirdparty_invite_toJson(const std::string& i) { return json::object(); }
}
