#include "pcp/permission_checker.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool permission_checker_validate(const std::string& i) { return !i.empty(); }
std::string permission_checker_process(const std::string& i) { return i; }
json permission_checker_toJson(const std::string& i) { return json::object(); }
}
