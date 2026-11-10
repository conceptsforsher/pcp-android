#include "pcp/session_backup.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_backup_validate(const std::string& i) { return !i.empty(); }
std::string session_backup_process(const std::string& i) { return i; }
json session_backup_toJson(const std::string& i) { return json::object(); }
}
