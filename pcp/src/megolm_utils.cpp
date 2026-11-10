#include "pcp/megolm_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool megolm_utils_validate(const std::string& i) { return !i.empty(); }
std::string megolm_utils_process(const std::string& i) { return i; }
json megolm_utils_toJson(const std::string& i) { return json::object(); }
}
