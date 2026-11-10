#include "pcp/call_models.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool call_models_validate(const std::string& i) { return !i.empty(); }
std::string call_models_process(const std::string& i) { return i; }
json call_models_toJson(const std::string& i) { return json::object(); }
}
