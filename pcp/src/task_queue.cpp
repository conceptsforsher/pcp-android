#include "pcp/task_queue.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool task_queue_validate(const std::string& i) { return !i.empty(); }
std::string task_queue_process(const std::string& i) { return i; }
json task_queue_toJson(const std::string& i) { return json::object(); }
}
