#include "pcp/gossip_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool gossip_manager_validate(const std::string& i) { return !i.empty(); }
std::string gossip_manager_process(const std::string& i) { return i; }
json gossip_manager_toJson(const std::string& i) { return json::object(); }
}
