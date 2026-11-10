#include "pcp/search_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool search_utils_validate(const std::string& i) { return !i.empty(); }
std::string search_utils_process(const std::string& i) { return i; }
json search_utils_toJson(const std::string& i) { return json::object(); }
}
