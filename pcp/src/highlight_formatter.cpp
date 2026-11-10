#include "pcp/highlight_formatter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool highlight_formatter_validate(const std::string& i) { return !i.empty(); }
std::string highlight_formatter_process(const std::string& i) { return i; }
json highlight_formatter_toJson(const std::string& i) { return json::object(); }
}
