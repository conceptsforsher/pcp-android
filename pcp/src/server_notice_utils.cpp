#include "pcp/server_notice_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool server_notice_utils_validate(const std::string& i) { return !i.empty(); }
std::string server_notice_utils_process(const std::string& i) { return i; }
json server_notice_utils_toJson(const std::string& i) { return json::object(); }
}
