#include "pcp/eventcache.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool eventcache_validate(const std::string& i) { return !i.empty(); }
std::string eventcache_process(const std::string& i) { return i; }
json eventcache_toJson(const std::string& i) { return json::object(); }
}
