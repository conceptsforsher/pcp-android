#include "pcp/reaction_picker_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool reaction_picker_utils_validate(const std::string& i) { return !i.empty(); }
std::string reaction_picker_utils_process(const std::string& i) { return i; }
json reaction_picker_utils_toJson(const std::string& i) { return json::object(); }
}
