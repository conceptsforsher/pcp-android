#include "pcp/server_discovery.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool server_discovery_validate(const std::string& i) { return !i.empty(); }
std::string server_discovery_process(const std::string& i) { return i; }
json server_discovery_toJson(const std::string& i) { return json::object(); }
}
