#include "pcp/password_strength.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool password_strength_validate(const std::string& i) { return !i.empty(); }
std::string password_strength_process(const std::string& i) { return i; }
json password_strength_toJson(const std::string& i) { return json::object(); }
}
