#include "pcp/room_activity.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_activity_validate(const std::string& i) { return !i.empty(); }
std::string room_activity_process(const std::string& i) { return i; }
json room_activity_toJson(const std::string& i) { return json::object(); }
}
