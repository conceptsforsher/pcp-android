#include "pcp/emoji_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool emoji_utils_validate(const std::string& i) { return !i.empty(); }
std::string emoji_utils_process(const std::string& i) { return i; }
json emoji_utils_toJson(const std::string& i) { return json::object(); }
}
