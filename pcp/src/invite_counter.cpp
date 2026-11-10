#include "pcp/invite_counter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool invite_counter_validate(const std::string& i) { return !i.empty(); }
std::string invite_counter_process(const std::string& i) { return i; }
json invite_counter_toJson(const std::string& i) { return json::object(); }
}
