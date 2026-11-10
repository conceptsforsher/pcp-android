#include "pcp/event_search.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_search_validate(const std::string& i) { return !i.empty(); }
std::string event_search_process(const std::string& i) { return i; }
json event_search_toJson(const std::string& i) { return json::object(); }
}
