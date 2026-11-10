#include "pcp/voice_broadcast_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool voice_broadcast_manager_validate(const std::string& i) { return !i.empty(); }
std::string voice_broadcast_manager_process(const std::string& i) { return i; }
json voice_broadcast_manager_toJson(const std::string& i) { return json::object(); }
}
