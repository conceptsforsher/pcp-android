#include "pcp/wellknown_helper.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool wellknown_helper_validate(const std::string& i) { return !i.empty(); }
std::string wellknown_helper_process(const std::string& i) { return i; }
json wellknown_helper_toJson(const std::string& i) { return json::object(); }
}
