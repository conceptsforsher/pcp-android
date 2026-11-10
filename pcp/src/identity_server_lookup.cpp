#include "pcp/identity_server_lookup.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool identity_server_lookup_validate(const std::string& i) { return !i.empty(); }
std::string identity_server_lookup_process(const std::string& i) { return i; }
json identity_server_lookup_toJson(const std::string& i) { return json::object(); }
}
