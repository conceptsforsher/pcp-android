#include "pcp/canonical_json.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool canonical_json_validate(const std::string& i) { return !i.empty(); }
std::string canonical_json_process(const std::string& i) { return i; }
json canonical_json_toJson(const std::string& i) { return json::object(); }
}
