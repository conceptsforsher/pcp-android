#include "pcp/message_format_helper.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_format_helper_validate(const std::string& i) { return !i.empty(); }
std::string message_format_helper_process(const std::string& i) { return i; }
json message_format_helper_toJson(const std::string& i) { return json::object(); }
}
