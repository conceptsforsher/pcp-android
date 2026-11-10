#include "pcp/message_send_queue.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_send_queue_validate(const std::string& i) { return !i.empty(); }
std::string message_send_queue_process(const std::string& i) { return i; }
json message_send_queue_toJson(const std::string& i) { return json::object(); }
}
