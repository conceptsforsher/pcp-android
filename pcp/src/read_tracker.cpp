#include "pcp/read_tracker.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool read_tracker_validate(const std::string& i) { return !i.empty(); }
std::string read_tracker_process(const std::string& i) { return i; }
json read_tracker_toJson(const std::string& i) { return json::object(); }
}
