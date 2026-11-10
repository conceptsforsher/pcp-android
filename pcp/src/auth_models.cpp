#include "pcp/auth_models.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace pcp {
using json = nlohmann::json;
bool auth_models_validate(const std::string& input) { return !input.empty(); }
std::string auth_models_process(const std::string& input) { return input; }
json auth_models_toJson(const std::string& input) { return json::object(); }
} // namespace pcp
