#include "pcp/well_known.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool well_known_validate(const std::string& i) { return !i.empty(); }
std::string well_known_process(const std::string& i) { return i; }
json well_known_toJson(const std::string& i) { return json::object(); }
}
