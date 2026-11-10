#include "pcp/sliding_sync_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sliding_sync_utils_validate(const std::string& i) { return !i.empty(); }
std::string sliding_sync_utils_process(const std::string& i) { return i; }
json sliding_sync_utils_toJson(const std::string& i) { return json::object(); }
}
