#include "pcp/eventdb.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool eventdb_validate(const std::string& i) { return !i.empty(); }
std::string eventdb_process(const std::string& i) { return i; }
json eventdb_toJson(const std::string& i) { return json::object(); }
}
