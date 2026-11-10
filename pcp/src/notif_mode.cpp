#include "pcp/notif_mode.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notif_mode_validate(const std::string& i) { return !i.empty(); }
std::string notif_mode_process(const std::string& i) { return i; }
json notif_mode_toJson(const std::string& i) { return json::object(); }
}
