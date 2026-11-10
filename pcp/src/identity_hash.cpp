#include "pcp/identity_hash.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool identity_hash_validate(const std::string& i) { return !i.empty(); }
std::string identity_hash_process(const std::string& i) { return i; }
json identity_hash_toJson(const std::string& i) { return json::object(); }
}
