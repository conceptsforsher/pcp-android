#include "pcp/url_preview_parser.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool url_preview_parser_validate(const std::string& i) { return !i.empty(); }
std::string url_preview_parser_process(const std::string& i) { return i; }
json url_preview_parser_toJson(const std::string& i) { return json::object(); }
}
