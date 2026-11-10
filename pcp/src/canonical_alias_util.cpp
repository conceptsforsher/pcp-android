#include "pcp/canonical_alias_util.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool canonical_alias_util_validate(const std::string& i) { return !i.empty(); }
std::string canonical_alias_util_process(const std::string& i) { return i; }
json canonical_alias_util_toJson(const std::string& i) { return json::object(); }
}
