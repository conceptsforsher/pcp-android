#include "pcp/avatar_picker.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool avatar_picker_validate(const std::string& i) { return !i.empty(); }
std::string avatar_picker_process(const std::string& i) { return i; }
json avatar_picker_toJson(const std::string& i) { return json::object(); }
}
