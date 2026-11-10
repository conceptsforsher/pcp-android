#include "pcp/custom_emoji_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool custom_emoji_utils_validate(const std::string& i) { return !i.empty(); }
std::string custom_emoji_utils_process(const std::string& i) { return i; }
json custom_emoji_utils_toJson(const std::string& i) { return json::object(); }
}
