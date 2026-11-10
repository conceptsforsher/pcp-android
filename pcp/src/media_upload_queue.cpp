#include "pcp/media_upload_queue.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_upload_queue_validate(const std::string& i) { return !i.empty(); }
std::string media_upload_queue_process(const std::string& i) { return i; }
json media_upload_queue_toJson(const std::string& i) { return json::object(); }
}
