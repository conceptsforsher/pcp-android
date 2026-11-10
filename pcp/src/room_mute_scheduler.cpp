#include "pcp/room_mute_scheduler.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_mute_scheduler_validate(const std::string& i) { return !i.empty(); }
std::string room_mute_scheduler_process(const std::string& i) { return i; }
json room_mute_scheduler_toJson(const std::string& i) { return json::object(); }
}
