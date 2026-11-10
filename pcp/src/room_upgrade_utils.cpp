#include "pcp/room_upgrade_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_upgrade_utils_validate(const std::string& i) { return !i.empty(); }
std::string room_upgrade_utils_process(const std::string& i) { return i; }
json room_upgrade_utils_toJson(const std::string& i) { return json::object(); }
}
