#include "pcp/user_directory.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool user_directory_validate(const std::string& i) { return !i.empty(); }
std::string user_directory_process(const std::string& i) { return i; }
json user_directory_toJson(const std::string& i) { return json::object(); }
}
