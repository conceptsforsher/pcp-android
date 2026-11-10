#include "pcp/room_sidebar.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_sidebar_validate(const std::string& i) { return !i.empty(); }
std::string room_sidebar_process(const std::string& i) { return i; }
json room_sidebar_toJson(const std::string& i) { return json::object(); }
}
