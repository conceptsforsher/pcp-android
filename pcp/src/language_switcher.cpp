#include "pcp/language_switcher.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool language_switcher_validate(const std::string& i) { return !i.empty(); }
std::string language_switcher_process(const std::string& i) { return i; }
json language_switcher_toJson(const std::string& i) { return json::object(); }
}
