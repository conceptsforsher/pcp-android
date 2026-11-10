#include "pcp/public_room_lister.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool public_room_lister_validate(const std::string& i) { return !i.empty(); }
std::string public_room_lister_process(const std::string& i) { return i; }
json public_room_lister_toJson(const std::string& i) { return json::object(); }
}
