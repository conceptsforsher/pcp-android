#include "pcp/event_signature_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_signature_utils_validate(const std::string& i) { return !i.empty(); }
std::string event_signature_utils_process(const std::string& i) { return i; }
json event_signature_utils_toJson(const std::string& i) { return json::object(); }
}
