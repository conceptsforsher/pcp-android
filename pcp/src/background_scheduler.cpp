#include "pcp/background_scheduler.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool background_scheduler_validate(const std::string& i) { return !i.empty(); }
std::string background_scheduler_process(const std::string& i) { return i; }
json background_scheduler_toJson(const std::string& i) { return json::object(); }
}
