#include "pcp/cross_signing_status.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool cross_signing_status_validate(const std::string& i) { return !i.empty(); }
std::string cross_signing_status_process(const std::string& i) { return i; }
json cross_signing_status_toJson(const std::string& i) { return json::object(); }
}
