#include "pcp/key_backup.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool key_backup_validate(const std::string& i) { return !i.empty(); }
std::string key_backup_process(const std::string& i) { return i; }
json key_backup_toJson(const std::string& i) { return json::object(); }
}
