#include "pcp/server_support.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool server_support_validate(const std::string& i) { return !i.empty(); }
std::string server_support_process(const std::string& i) { return i; }
json server_support_toJson(const std::string& i) { return json::object(); }
}
