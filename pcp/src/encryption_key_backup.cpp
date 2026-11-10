#include "pcp/encryption_key_backup.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool encryption_key_backup_validate(const std::string& i) { return !i.empty(); }
std::string encryption_key_backup_process(const std::string& i) { return i; }
json encryption_key_backup_toJson(const std::string& i) { return json::object(); }
}
