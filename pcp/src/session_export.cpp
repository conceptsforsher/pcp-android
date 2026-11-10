#include "pcp/session_export.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool session_export_validate(const std::string& i) { return !i.empty(); }
std::string session_export_process(const std::string& i) { return i; }
json session_export_toJson(const std::string& i) { return json::object(); }
}
