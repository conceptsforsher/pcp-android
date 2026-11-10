#include "pcp/thread_summary.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool thread_summary_validate(const std::string& i) { return !i.empty(); }
std::string thread_summary_process(const std::string& i) { return i; }
json thread_summary_toJson(const std::string& i) { return json::object(); }
}
