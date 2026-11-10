#include "pcp/device_verifier.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool device_verifier_validate(const std::string& i) { return !i.empty(); }
std::string device_verifier_process(const std::string& i) { return i; }
json device_verifier_toJson(const std::string& i) { return json::object(); }
}
