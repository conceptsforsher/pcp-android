#include "pcp/olm_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool olm_utils_validate(const std::string& i) { return !i.empty(); }
std::string olm_utils_process(const std::string& i) { return i; }
json olm_utils_toJson(const std::string& i) { return json::object(); }
}
