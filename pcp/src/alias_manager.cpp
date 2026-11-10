#include "pcp/alias_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool alias_manager_validate(const std::string& i) { return !i.empty(); }
std::string alias_manager_process(const std::string& i) { return i; }
json alias_manager_toJson(const std::string& i) { return json::object(); }
}
