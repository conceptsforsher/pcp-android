#include "pcp/crypto_device_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool crypto_device_utils_validate(const std::string& i) { return !i.empty(); }
std::string crypto_device_utils_process(const std::string& i) { return i; }
json crypto_device_utils_toJson(const std::string& i) { return json::object(); }
}
