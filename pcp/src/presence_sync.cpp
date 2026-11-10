#include "pcp/presence_sync.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool presence_sync_validate(const std::string& i) { return !i.empty(); }
std::string presence_sync_process(const std::string& i) { return i; }
json presence_sync_toJson(const std::string& i) { return json::object(); }
}
