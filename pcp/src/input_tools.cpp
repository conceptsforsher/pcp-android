#include "pcp/input_tools.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool input_tools_validate(const std::string& i) { return !i.empty(); }
std::string input_tools_process(const std::string& i) { return i; }
json input_tools_toJson(const std::string& i) { return json::object(); }
}
