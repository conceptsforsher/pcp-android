#include "pcp/server_capabilities.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool server_capabilities_validate(const std::string& i) { return !i.empty(); }
std::string server_capabilities_process(const std::string& i) { return i; }
json server_capabilities_toJson(const std::string& i) { return json::object(); }
}
