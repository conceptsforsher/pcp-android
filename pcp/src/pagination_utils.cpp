#include "pcp/pagination_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool pagination_utils_validate(const std::string& i) { return !i.empty(); }
std::string pagination_utils_process(const std::string& i) { return i; }
json pagination_utils_toJson(const std::string& i) { return json::object(); }
}
