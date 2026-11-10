#include "pcp/account_export.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool account_export_validate(const std::string& i) { return !i.empty(); }
std::string account_export_process(const std::string& i) { return i; }
json account_export_toJson(const std::string& i) { return json::object(); }
}
