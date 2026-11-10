#include "pcp/room_content.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_content_validate(const std::string& i) { return !i.empty(); }
std::string room_content_process(const std::string& i) { return i; }
json room_content_toJson(const std::string& i) { return json::object(); }
}
