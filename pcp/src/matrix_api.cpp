#include "pcp/matrix_api.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool matrix_api_validate(const std::string& i) { return !i.empty(); }
std::string matrix_api_process(const std::string& i) { return i; }
json matrix_api_toJson(const std::string& i) { return json::object(); }
}
