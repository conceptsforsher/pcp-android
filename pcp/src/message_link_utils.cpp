#include "pcp/message_link_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_link_utils_validate(const std::string& i) { return !i.empty(); }
std::string message_link_utils_process(const std::string& i) { return i; }
json message_link_utils_toJson(const std::string& i) { return json::object(); }
}
