#include "pcp/message_search.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_search_validate(const std::string& i) { return !i.empty(); }
std::string message_search_process(const std::string& i) { return i; }
json message_search_toJson(const std::string& i) { return json::object(); }
}
