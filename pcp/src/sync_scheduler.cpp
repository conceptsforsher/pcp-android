#include "pcp/sync_scheduler.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sync_scheduler_validate(const std::string& i) { return !i.empty(); }
std::string sync_scheduler_process(const std::string& i) { return i; }
json sync_scheduler_toJson(const std::string& i) { return json::object(); }
}
