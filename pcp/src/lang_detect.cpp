#include "pcp/lang_detect.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool lang_detect_validate(const std::string& i) { return !i.empty(); }
std::string lang_detect_process(const std::string& i) { return i; }
json lang_detect_toJson(const std::string& i) { return json::object(); }
}
