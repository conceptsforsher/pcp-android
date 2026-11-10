#include "pcp/encryption_verify_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool encryption_verify_utils_validate(const std::string& i) { return !i.empty(); }
std::string encryption_verify_utils_process(const std::string& i) { return i; }
json encryption_verify_utils_toJson(const std::string& i) { return json::object(); }
}
