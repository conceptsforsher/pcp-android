#include "pcp/room_knock_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_knock_manager_validate(const std::string& i) { return !i.empty(); }
std::string room_knock_manager_process(const std::string& i) { return i; }
json room_knock_manager_toJson(const std::string& i) { return json::object(); }
}
