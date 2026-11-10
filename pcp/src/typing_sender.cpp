#include "pcp/typing_sender.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool typing_sender_validate(const std::string& i) { return !i.empty(); }
std::string typing_sender_process(const std::string& i) { return i; }
json typing_sender_toJson(const std::string& i) { return json::object(); }
}
