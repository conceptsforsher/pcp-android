#include "pcp/session_restore.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_restore_validate(const std::string& i) { return !i.empty(); }
std::string session_restore_process(const std::string& i) { return i; }
json session_restore_toJson(const std::string& i) { return json::object(); }
}
