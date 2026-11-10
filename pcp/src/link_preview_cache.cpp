#include "pcp/link_preview_cache.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool link_preview_cache_validate(const std::string& i) { return !i.empty(); }
std::string link_preview_cache_process(const std::string& i) { return i; }
json link_preview_cache_toJson(const std::string& i) { return json::object(); }
}
