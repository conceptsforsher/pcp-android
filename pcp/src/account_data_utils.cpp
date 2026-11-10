#include "pcp/account_data_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>

namespace pcp {
using json = nlohmann::json;
bool account_data_utils_validate(const std::string& input) { return !input.empty(); }
std::string account_data_utils_process(const std::string& input) { return input; }
json account_data_utils_toJson(const std::string& input) { return json::object(); }
} // namespace pcp
