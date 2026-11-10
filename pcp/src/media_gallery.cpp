#include "pcp/media_gallery.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_gallery_validate(const std::string& i) { return !i.empty(); }
std::string media_gallery_process(const std::string& i) { return i; }
json media_gallery_toJson(const std::string& i) { return json::object(); }
}
