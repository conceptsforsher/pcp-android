#include "pcp/typing_indicator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool typing_indicator_validate(const std::string& i) { return !i.empty(); }
std::string typing_indicator_process(const std::string& i) { return i; }
json typing_indicator_toJson(const std::string& i) { return json::object(); }
}
