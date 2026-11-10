#include "pcp/sliding_sync.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sliding_sync_validate(const std::string& i) { return !i.empty(); }
std::string sliding_sync_process(const std::string& i) { return i; }
json sliding_sync_toJson(const std::string& i) { return json::object(); }
}
