#include "pcp/space_navigation.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool space_navigation_validate(const std::string& i) { return !i.empty(); }
std::string space_navigation_process(const std::string& i) { return i; }
json space_navigation_toJson(const std::string& i) { return json::object(); }
}
