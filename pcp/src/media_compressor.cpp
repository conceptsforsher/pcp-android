#include "pcp/media_compressor.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool media_compressor_validate(const std::string& i) { return !i.empty(); }
std::string media_compressor_process(const std::string& i) { return i; }
json media_compressor_toJson(const std::string& i) { return json::object(); }
}
