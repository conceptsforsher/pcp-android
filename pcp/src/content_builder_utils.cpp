#include "pcp/content_builder_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool content_builder_utils_validate(const std::string& i) { return !i.empty(); }
std::string content_builder_utils_process(const std::string& i) { return i; }
json content_builder_utils_toJson(const std::string& i) { return json::object(); }
}
