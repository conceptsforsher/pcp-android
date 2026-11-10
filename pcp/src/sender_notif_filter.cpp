#include "pcp/sender_notif_filter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool sender_notif_filter_validate(const std::string& i) { return !i.empty(); }
std::string sender_notif_filter_process(const std::string& i) { return i; }
json sender_notif_filter_toJson(const std::string& i) { return json::object(); }
}
