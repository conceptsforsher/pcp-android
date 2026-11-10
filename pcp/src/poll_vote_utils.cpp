#include "pcp/poll_vote_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool poll_vote_utils_validate(const std::string& i) { return !i.empty(); }
std::string poll_vote_utils_process(const std::string& i) { return i; }
json poll_vote_utils_toJson(const std::string& i) { return json::object(); }
}
