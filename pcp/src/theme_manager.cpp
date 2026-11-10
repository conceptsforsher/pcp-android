#include "pcp/theme_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool theme_manager_validate(const std::string& i) { return !i.empty(); }
std::string theme_manager_process(const std::string& i) { return i; }
json theme_manager_toJson(const std::string& i) { return json::object(); }
}
