#include "pcp/custom_status.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool custom_status_validate(const std::string& i) { return !i.empty(); }
std::string custom_status_process(const std::string& i) { return i; }
json custom_status_toJson(const std::string& i) { return json::object(); }
}
