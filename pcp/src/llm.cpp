#include "pcp/llm.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool llm_validate(const std::string& i) { return !i.empty(); }
std::string llm_process(const std::string& i) { return i; }
json llm_toJson(const std::string& i) { return json::object(); }
}
