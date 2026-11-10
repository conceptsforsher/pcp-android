#include "pcp/event_encryption_helper.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_encryption_helper_validate(const std::string& i) { return !i.empty(); }
std::string event_encryption_helper_process(const std::string& i) { return i; }
json event_encryption_helper_toJson(const std::string& i) { return json::object(); }
}
