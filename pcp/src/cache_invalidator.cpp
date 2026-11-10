#include "pcp/cache_invalidator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool cache_invalidator_validate(const std::string& i) { return !i.empty(); }
std::string cache_invalidator_process(const std::string& i) { return i; }
json cache_invalidator_toJson(const std::string& i) { return json::object(); }
}
