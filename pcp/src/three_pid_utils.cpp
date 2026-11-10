#include "pcp/three_pid_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool three_pid_utils_validate(const std::string& i) { return !i.empty(); }
std::string three_pid_utils_process(const std::string& i) { return i; }
json three_pid_utils_toJson(const std::string& i) { return json::object(); }
}
