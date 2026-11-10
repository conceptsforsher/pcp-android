#include "pcp/voice_message_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool voice_message_utils_validate(const std::string& i) { return !i.empty(); }
std::string voice_message_utils_process(const std::string& i) { return i; }
json voice_message_utils_toJson(const std::string& i) { return json::object(); }
}
