#include "pcp/room_permission_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_permission_utils_validate(const std::string& i) { return !i.empty(); }
std::string room_permission_utils_process(const std::string& i) { return i; }
json room_permission_utils_toJson(const std::string& i) { return json::object(); }
}
