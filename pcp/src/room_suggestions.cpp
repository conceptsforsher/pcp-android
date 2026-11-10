#include "pcp/room_suggestions.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_suggestions_validate(const std::string& i) { return !i.empty(); }
std::string room_suggestions_process(const std::string& i) { return i; }
json room_suggestions_toJson(const std::string& i) { return json::object(); }
}
