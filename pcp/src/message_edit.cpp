#include "pcp/message_edit.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_edit_validate(const std::string& i) { return !i.empty(); }
std::string message_edit_process(const std::string& i) { return i; }
json message_edit_toJson(const std::string& i) { return json::object(); }
}
