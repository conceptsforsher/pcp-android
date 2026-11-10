#include "pcp/read_marker_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool read_marker_utils_validate(const std::string& i) { return !i.empty(); }
std::string read_marker_utils_process(const std::string& i) { return i; }
json read_marker_utils_toJson(const std::string& i) { return json::object(); }
}
