#include "pcp/scheduled_edit.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool scheduled_edit_validate(const std::string& i) { return !i.empty(); }
std::string scheduled_edit_process(const std::string& i) { return i; }
json scheduled_edit_toJson(const std::string& i) { return json::object(); }
}
