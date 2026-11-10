#include "pcp/media_download.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_download_validate(const std::string& i) { return !i.empty(); }
std::string media_download_process(const std::string& i) { return i; }
json media_download_toJson(const std::string& i) { return json::object(); }
}
