#include "pcp/emoji_reaction_picker.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool emoji_reaction_picker_validate(const std::string& i) { return !i.empty(); }
std::string emoji_reaction_picker_process(const std::string& i) { return i; }
json emoji_reaction_picker_toJson(const std::string& i) { return json::object(); }
}
