#include "pcp/signout_service.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool signout_service_validate(const std::string& i) { return !i.empty(); }
std::string signout_service_process(const std::string& i) { return i; }
json signout_service_toJson(const std::string& i) { return json::object(); }
}
