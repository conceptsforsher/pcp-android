#include "pcp/room_favorite_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_favorite_utils_validate(const std::string& i) { return !i.empty(); }
std::string room_favorite_utils_process(const std::string& i) { return i; }
json room_favorite_utils_toJson(const std::string& i) { return json::object(); }
}
