#include "pcp/module_loader.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool module_loader_validate(const std::string& i) { return !i.empty(); }
std::string module_loader_process(const std::string& i) { return i; }
json module_loader_toJson(const std::string& i) { return json::object(); }
}
