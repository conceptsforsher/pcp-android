#include "pcp/key_backup_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool key_backup_manager_validate(const std::string& i) { return !i.empty(); }
std::string key_backup_manager_process(const std::string& i) { return i; }
json key_backup_manager_toJson(const std::string& i) { return json::object(); }
}
