#include "pcp/key_share.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool key_share_validate(const std::string& i) { return !i.empty(); }
std::string key_share_process(const std::string& i) { return i; }
json key_share_toJson(const std::string& i) { return json::object(); }
}
