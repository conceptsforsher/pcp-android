#include "pcp/sync_handler.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sync_handler_validate(const std::string& i) { return !i.empty(); }
std::string sync_handler_process(const std::string& i) { return i; }
json sync_handler_toJson(const std::string& i) { return json::object(); }
}
