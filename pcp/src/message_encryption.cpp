#include "pcp/message_encryption.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_encryption_validate(const std::string& i) { return !i.empty(); }
std::string message_encryption_process(const std::string& i) { return i; }
json message_encryption_toJson(const std::string& i) { return json::object(); }
}
