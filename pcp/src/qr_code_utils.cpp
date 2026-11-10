#include "pcp/qr_code_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool qr_code_utils_validate(const std::string& i) { return !i.empty(); }
std::string qr_code_utils_process(const std::string& i) { return i; }
json qr_code_utils_toJson(const std::string& i) { return json::object(); }
}
