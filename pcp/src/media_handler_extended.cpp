#include "pcp/media_handler_extended.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_handler_extended_validate(const std::string& i) { return !i.empty(); }
std::string media_handler_extended_process(const std::string& i) { return i; }
json media_handler_extended_toJson(const std::string& i) { return json::object(); }
}
