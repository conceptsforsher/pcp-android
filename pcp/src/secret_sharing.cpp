#include "pcp/secret_sharing.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool secret_sharing_validate(const std::string& i) { return !i.empty(); }
std::string secret_sharing_process(const std::string& i) { return i; }
json secret_sharing_toJson(const std::string& i) { return json::object(); }
}
