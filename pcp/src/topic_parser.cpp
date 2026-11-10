#include "pcp/topic_parser.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool topic_parser_validate(const std::string& i) { return !i.empty(); }
std::string topic_parser_process(const std::string& i) { return i; }
json topic_parser_toJson(const std::string& i) { return json::object(); }
}
