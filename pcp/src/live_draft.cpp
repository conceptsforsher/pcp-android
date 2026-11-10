#include "pcp/live_draft.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool live_draft_validate(const std::string& i) { return !i.empty(); }
std::string live_draft_process(const std::string& i) { return i; }
json live_draft_toJson(const std::string& i) { return json::object(); }
}
