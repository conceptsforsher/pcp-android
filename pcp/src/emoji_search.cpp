#include "pcp/emoji_search.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool emoji_search_validate(const std::string& i) { return !i.empty(); }
std::string emoji_search_process(const std::string& i) { return i; }
json emoji_search_toJson(const std::string& i) { return json::object(); }
}
