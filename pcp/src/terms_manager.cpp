#include "pcp/terms_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool terms_manager_validate(const std::string& i) { return !i.empty(); }
std::string terms_manager_process(const std::string& i) { return i; }
json terms_manager_toJson(const std::string& i) { return json::object(); }
}
