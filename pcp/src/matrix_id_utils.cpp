#include "pcp/matrix_id_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool matrix_id_utils_validate(const std::string& i) { return !i.empty(); }
std::string matrix_id_utils_process(const std::string& i) { return i; }
json matrix_id_utils_toJson(const std::string& i) { return json::object(); }
}
