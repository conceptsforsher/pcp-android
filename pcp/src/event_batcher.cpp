#include "pcp/event_batcher.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_batcher_validate(const std::string& i) { return !i.empty(); }
std::string event_batcher_process(const std::string& i) { return i; }
json event_batcher_toJson(const std::string& i) { return json::object(); }
}
