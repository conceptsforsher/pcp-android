#include "pcp/widget_url_check.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool widget_url_check_validate(const std::string& i) { return !i.empty(); }
std::string widget_url_check_process(const std::string& i) { return i; }
json widget_url_check_toJson(const std::string& i) { return json::object(); }
}
