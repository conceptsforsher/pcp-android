#include "pcp/translation_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool translation_utils_validate(const std::string& i) { return !i.empty(); }
std::string translation_utils_process(const std::string& i) { return i; }
json translation_utils_toJson(const std::string& i) { return json::object(); }
}
