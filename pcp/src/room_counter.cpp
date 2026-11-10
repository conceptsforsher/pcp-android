#include "pcp/room_counter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_counter_validate(const std::string& i) { return !i.empty(); }
std::string room_counter_process(const std::string& i) { return i; }
json room_counter_toJson(const std::string& i) { return json::object(); }
}
