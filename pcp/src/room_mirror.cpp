#include "pcp/room_mirror.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_mirror_validate(const std::string& i) { return !i.empty(); }
std::string room_mirror_process(const std::string& i) { return i; }
json room_mirror_toJson(const std::string& i) { return json::object(); }
}
