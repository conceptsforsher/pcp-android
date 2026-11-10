#include "pcp/alarm_engine.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool alarm_engine_validate(const std::string& i) { return !i.empty(); }
std::string alarm_engine_process(const std::string& i) { return i; }
json alarm_engine_toJson(const std::string& i) { return json::object(); }
}
