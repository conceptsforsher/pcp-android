#include "pcp/favorite_messages.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool favorite_messages_validate(const std::string& i) { return !i.empty(); }
std::string favorite_messages_process(const std::string& i) { return i; }
json favorite_messages_toJson(const std::string& i) { return json::object(); }
}
