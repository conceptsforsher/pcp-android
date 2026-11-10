#include "pcp/matrix_error.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool matrix_error_validate(const std::string& i) { return !i.empty(); }
std::string matrix_error_process(const std::string& i) { return i; }
json matrix_error_toJson(const std::string& i) { return json::object(); }
}
