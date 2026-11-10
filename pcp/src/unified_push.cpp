#include "pcp/unified_push.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool unified_push_validate(const std::string& i) { return !i.empty(); }
std::string unified_push_process(const std::string& i) { return i; }
json unified_push_toJson(const std::string& i) { return json::object(); }
}
