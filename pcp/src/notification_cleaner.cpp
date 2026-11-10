#include "pcp/notification_cleaner.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool notification_cleaner_validate(const std::string& i) { return !i.empty(); }
std::string notification_cleaner_process(const std::string& i) { return i; }
json notification_cleaner_toJson(const std::string& i) { return json::object(); }
}
