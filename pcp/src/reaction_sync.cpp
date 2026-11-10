#include "pcp/reaction_sync.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool reaction_sync_validate(const std::string& i) { return !i.empty(); }
std::string reaction_sync_process(const std::string& i) { return i; }
json reaction_sync_toJson(const std::string& i) { return json::object(); }
}
