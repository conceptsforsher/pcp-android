#include "pcp/session_manager_full.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_manager_full_validate(const std::string& i) { return !i.empty(); }
std::string session_manager_full_process(const std::string& i) { return i; }
json session_manager_full_toJson(const std::string& i) { return json::object(); }
}
