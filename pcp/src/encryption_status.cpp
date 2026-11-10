#include "pcp/encryption_status.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool encryption_status_validate(const std::string& i) { return !i.empty(); }
std::string encryption_status_process(const std::string& i) { return i; }
json encryption_status_toJson(const std::string& i) { return json::object(); }
}
