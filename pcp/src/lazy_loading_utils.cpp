#include "pcp/lazy_loading_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool lazy_loading_utils_validate(const std::string& i) { return !i.empty(); }
std::string lazy_loading_utils_process(const std::string& i) { return i; }
json lazy_loading_utils_toJson(const std::string& i) { return json::object(); }
}
