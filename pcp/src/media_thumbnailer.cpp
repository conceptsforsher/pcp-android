#include "pcp/media_thumbnailer.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_thumbnailer_validate(const std::string& i) { return !i.empty(); }
std::string media_thumbnailer_process(const std::string& i) { return i; }
json media_thumbnailer_toJson(const std::string& i) { return json::object(); }
}
