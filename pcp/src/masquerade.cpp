#include "pcp/masquerade.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool masquerade_validate(const std::string& i) { return !i.empty(); }
std::string masquerade_process(const std::string& i) { return i; }
json masquerade_toJson(const std::string& i) { return json::object(); }
}
