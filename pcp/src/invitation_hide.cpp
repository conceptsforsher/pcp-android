#include "pcp/invitation_hide.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool invitation_hide_validate(const std::string& i) { return !i.empty(); }
std::string invitation_hide_process(const std::string& i) { return i; }
json invitation_hide_toJson(const std::string& i) { return json::object(); }
}
