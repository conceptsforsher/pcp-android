#include "pcp/message_scheduler.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_scheduler_validate(const std::string& i) { return !i.empty(); }
std::string message_scheduler_process(const std::string& i) { return i; }
json message_scheduler_toJson(const std::string& i) { return json::object(); }
}
