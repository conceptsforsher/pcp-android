#include "pcp/member_count_cache.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool member_count_cache_validate(const std::string& i) { return !i.empty(); }
std::string member_count_cache_process(const std::string& i) { return i; }
json member_count_cache_toJson(const std::string& i) { return json::object(); }
}
