#include "pcp/user_mask.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool user_mask_validate(const std::string& i) { return !i.empty(); }
std::string user_mask_process(const std::string& i) { return i; }
json user_mask_toJson(const std::string& i) { return json::object(); }
}
