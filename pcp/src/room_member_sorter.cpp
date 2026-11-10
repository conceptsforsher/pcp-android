#include "pcp/room_member_sorter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_member_sorter_validate(const std::string& i) { return !i.empty(); }
std::string room_member_sorter_process(const std::string& i) { return i; }
json room_member_sorter_toJson(const std::string& i) { return json::object(); }
}
