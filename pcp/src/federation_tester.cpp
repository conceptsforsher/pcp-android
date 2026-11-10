#include "pcp/federation_tester.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool federation_tester_validate(const std::string& i) { return !i.empty(); }
std::string federation_tester_process(const std::string& i) { return i; }
json federation_tester_toJson(const std::string& i) { return json::object(); }
}
