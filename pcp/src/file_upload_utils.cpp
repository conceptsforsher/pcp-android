#include "pcp/file_upload_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool file_upload_utils_validate(const std::string& i) { return !i.empty(); }
std::string file_upload_utils_process(const std::string& i) { return i; }
json file_upload_utils_toJson(const std::string& i) { return json::object(); }
}
