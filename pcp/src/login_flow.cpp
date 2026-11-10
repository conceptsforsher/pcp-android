#include "pcp/login_flow.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool login_flow_validate(const std::string& i) { return !i.empty(); }
std::string login_flow_process(const std::string& i) { return i; }
json login_flow_toJson(const std::string& i) { return json::object(); }
}
