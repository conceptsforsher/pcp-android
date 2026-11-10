#include "pcp/identity_lookup.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool identity_lookup_validate(const std::string& i) { return !i.empty(); }
std::string identity_lookup_process(const std::string& i) { return i; }
json identity_lookup_toJson(const std::string& i) { return json::object(); }
}
