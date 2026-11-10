#include "pcp/whois_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool whois_utils_validate(const std::string& i) { return !i.empty(); }
std::string whois_utils_process(const std::string& i) { return i; }
json whois_utils_toJson(const std::string& i) { return json::object(); }
}
