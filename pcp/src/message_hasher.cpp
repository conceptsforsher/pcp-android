#include "pcp/message_hasher.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_hasher_validate(const std::string& i) { return !i.empty(); }
std::string message_hasher_process(const std::string& i) { return i; }
json message_hasher_toJson(const std::string& i) { return json::object(); }
}
