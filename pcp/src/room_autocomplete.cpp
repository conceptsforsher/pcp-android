#include "pcp/room_autocomplete.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_autocomplete_validate(const std::string& i) { return !i.empty(); }
std::string room_autocomplete_process(const std::string& i) { return i; }
json room_autocomplete_toJson(const std::string& i) { return json::object(); }
}
