#include "pcp/deeplink_parser.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool deeplink_parser_validate(const std::string& i) { return !i.empty(); }
std::string deeplink_parser_process(const std::string& i) { return i; }
json deeplink_parser_toJson(const std::string& i) { return json::object(); }
}
