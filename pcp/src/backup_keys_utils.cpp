#include "pcp/backup_keys_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool backup_keys_utils_validate(const std::string& i) { return !i.empty(); }
std::string backup_keys_utils_process(const std::string& i) { return i; }
json backup_keys_utils_toJson(const std::string& i) { return json::object(); }
}
