#include "pcp/member_list.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool member_list_validate(const std::string& i) { return !i.empty(); }
std::string member_list_process(const std::string& i) { return i; }
json member_list_toJson(const std::string& i) { return json::object(); }
}
