#include "pcp/sso_handler.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sso_handler_validate(const std::string& i) { return !i.empty(); }
std::string sso_handler_process(const std::string& i) { return i; }
json sso_handler_toJson(const std::string& i) { return json::object(); }
}
