#include "pcp/message_searcher.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool message_searcher_validate(const std::string& i) { return !i.empty(); }
std::string message_searcher_process(const std::string& i) { return i; }
json message_searcher_toJson(const std::string& i) { return json::object(); }
}
