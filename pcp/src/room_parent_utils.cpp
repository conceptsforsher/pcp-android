#include "pcp/room_parent_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_parent_utils_validate(const std::string& i) { return !i.empty(); }
std::string room_parent_utils_process(const std::string& i) { return i; }
json room_parent_utils_toJson(const std::string& i) { return json::object(); }
}
