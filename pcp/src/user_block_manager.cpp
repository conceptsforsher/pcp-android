#include "pcp/user_block_manager.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool user_block_manager_validate(const std::string& i) { return !i.empty(); }
std::string user_block_manager_process(const std::string& i) { return i; }
json user_block_manager_toJson(const std::string& i) { return json::object(); }
}
