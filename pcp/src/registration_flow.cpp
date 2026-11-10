#include "pcp/registration_flow.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool registration_flow_validate(const std::string& i) { return !i.empty(); }
std::string registration_flow_process(const std::string& i) { return i; }
json registration_flow_toJson(const std::string& i) { return json::object(); }
}
