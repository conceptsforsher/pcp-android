#include "pcp/call_duration.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool call_duration_validate(const std::string& i) { return !i.empty(); }
std::string call_duration_process(const std::string& i) { return i; }
json call_duration_toJson(const std::string& i) { return json::object(); }
}
