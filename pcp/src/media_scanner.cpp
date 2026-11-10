#include "pcp/media_scanner.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_scanner_validate(const std::string& i) { return !i.empty(); }
std::string media_scanner_process(const std::string& i) { return i; }
json media_scanner_toJson(const std::string& i) { return json::object(); }
}
