#include "pcp/tag_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool tag_manager_validate(const std::string& i) { return !i.empty(); }
std::string tag_manager_process(const std::string& i) { return i; }
json tag_manager_toJson(const std::string& i) { return json::object(); }
}
