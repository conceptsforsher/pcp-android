#include "pcp/thumbnail_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool thumbnail_utils_validate(const std::string& i) { return !i.empty(); }
std::string thumbnail_utils_process(const std::string& i) { return i; }
json thumbnail_utils_toJson(const std::string& i) { return json::object(); }
}
