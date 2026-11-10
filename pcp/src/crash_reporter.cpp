#include "pcp/crash_reporter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool crash_reporter_validate(const std::string& i) { return !i.empty(); }
std::string crash_reporter_process(const std::string& i) { return i; }
json crash_reporter_toJson(const std::string& i) { return json::object(); }
}
