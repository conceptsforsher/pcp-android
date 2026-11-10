#include "pcp/media_utils_ext.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_utils_ext_validate(const std::string& i) { return !i.empty(); }
std::string media_utils_ext_process(const std::string& i) { return i; }
json media_utils_ext_toJson(const std::string& i) { return json::object(); }
}
