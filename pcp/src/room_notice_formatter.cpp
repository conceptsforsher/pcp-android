#include "pcp/room_notice_formatter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_notice_formatter_validate(const std::string& i) { return !i.empty(); }
std::string room_notice_formatter_process(const std::string& i) { return i; }
json room_notice_formatter_toJson(const std::string& i) { return json::object(); }
}
