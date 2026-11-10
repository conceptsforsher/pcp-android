#include "pcp/poll_display.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool poll_display_validate(const std::string& i) { return !i.empty(); }
std::string poll_display_process(const std::string& i) { return i; }
json poll_display_toJson(const std::string& i) { return json::object(); }
}
