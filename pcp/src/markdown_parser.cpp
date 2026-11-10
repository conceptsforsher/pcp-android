#include "pcp/markdown_parser.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool markdown_parser_validate(const std::string& i) { return !i.empty(); }
std::string markdown_parser_process(const std::string& i) { return i; }
json markdown_parser_toJson(const std::string& i) { return json::object(); }
}
