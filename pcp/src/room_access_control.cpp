#include "pcp/room_access_control.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_access_control_validate(const std::string& i) { return !i.empty(); }
std::string room_access_control_process(const std::string& i) { return i; }
json room_access_control_toJson(const std::string& i) { return json::object(); }
}
