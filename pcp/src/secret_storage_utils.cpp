#include "pcp/secret_storage_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool secret_storage_utils_validate(const std::string& i) { return !i.empty(); }
std::string secret_storage_utils_process(const std::string& i) { return i; }
json secret_storage_utils_toJson(const std::string& i) { return json::object(); }
}
