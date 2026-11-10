#include "pcp/event_context_builder.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool event_context_builder_validate(const std::string& i) { return !i.empty(); }
std::string event_context_builder_process(const std::string& i) { return i; }
json event_context_builder_toJson(const std::string& i) { return json::object(); }
}
