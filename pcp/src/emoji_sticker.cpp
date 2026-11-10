#include "pcp/emoji_sticker.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool emoji_sticker_validate(const std::string& i) { return !i.empty(); }
std::string emoji_sticker_process(const std::string& i) { return i; }
json emoji_sticker_toJson(const std::string& i) { return json::object(); }
}
