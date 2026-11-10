#include "pcp/state_resolver.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool state_resolver_validate(const std::string& i) { return !i.empty(); }
std::string state_resolver_process(const std::string& i) { return i; }
json state_resolver_toJson(const std::string& i) { return json::object(); }
}
