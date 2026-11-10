#include "pcp/sqlite_wrapper.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sqlite_wrapper_validate(const std::string& i) { return !i.empty(); }
std::string sqlite_wrapper_process(const std::string& i) { return i; }
json sqlite_wrapper_toJson(const std::string& i) { return json::object(); }
}
