#include "pcp/sas_verification_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sas_verification_utils_validate(const std::string& i) { return !i.empty(); }
std::string sas_verification_utils_process(const std::string& i) { return i; }
json sas_verification_utils_toJson(const std::string& i) { return json::object(); }
}
