#include "pcp/location_share.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool location_share_validate(const std::string& i) { return !i.empty(); }
std::string location_share_process(const std::string& i) { return i; }
json location_share_toJson(const std::string& i) { return json::object(); }
}
