#include "pcp/widget_manager_ext.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool widget_manager_ext_validate(const std::string& i) { return !i.empty(); }
std::string widget_manager_ext_process(const std::string& i) { return i; }
json widget_manager_ext_toJson(const std::string& i) { return json::object(); }
}
