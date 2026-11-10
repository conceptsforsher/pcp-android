#include "pcp/push_translator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool push_translator_validate(const std::string& i) { return !i.empty(); }
std::string push_translator_process(const std::string& i) { return i; }
json push_translator_toJson(const std::string& i) { return json::object(); }
}
