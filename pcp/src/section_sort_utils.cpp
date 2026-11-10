#include "pcp/section_sort_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool section_sort_utils_validate(const std::string& i) { return !i.empty(); }
std::string section_sort_utils_process(const std::string& i) { return i; }
json section_sort_utils_toJson(const std::string& i) { return json::object(); }
}
