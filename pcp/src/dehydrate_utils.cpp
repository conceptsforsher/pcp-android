#include "pcp/dehydrate_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool dehydrate_utils_validate(const std::string& i) { return !i.empty(); }
std::string dehydrate_utils_process(const std::string& i) { return i; }
json dehydrate_utils_toJson(const std::string& i) { return json::object(); }
}
