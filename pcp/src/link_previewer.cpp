#include "pcp/link_previewer.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool link_previewer_validate(const std::string& i) { return !i.empty(); }
std::string link_previewer_process(const std::string& i) { return i; }
json link_previewer_toJson(const std::string& i) { return json::object(); }
}
