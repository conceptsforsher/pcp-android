#include "pcp/contact_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool contact_utils_validate(const std::string& i) { return !i.empty(); }
std::string contact_utils_process(const std::string& i) { return i; }
json contact_utils_toJson(const std::string& i) { return json::object(); }
}
