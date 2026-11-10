#include "pcp/room_previewer.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_previewer_validate(const std::string& i) { return !i.empty(); }
std::string room_previewer_process(const std::string& i) { return i; }
json room_previewer_toJson(const std::string& i) { return json::object(); }
}
