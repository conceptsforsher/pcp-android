#include "pcp/reaction_aggregator.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool reaction_aggregator_validate(const std::string& i) { return !i.empty(); }
std::string reaction_aggregator_process(const std::string& i) { return i; }
json reaction_aggregator_toJson(const std::string& i) { return json::object(); }
}
