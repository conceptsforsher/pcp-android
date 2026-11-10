#include "pcp/mention_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool mention_utils_validate(const std::string& i) { return !i.empty(); }
std::string mention_utils_process(const std::string& i) { return i; }
json mention_utils_toJson(const std::string& i) { return json::object(); }
}
