#include "pcp/room_upload_progress.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool room_upload_progress_validate(const std::string& i) { return !i.empty(); }
std::string room_upload_progress_process(const std::string& i) { return i; }
json room_upload_progress_toJson(const std::string& i) { return json::object(); }
}
