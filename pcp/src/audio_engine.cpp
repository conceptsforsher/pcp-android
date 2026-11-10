#include "pcp/audio_engine.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool audio_engine_validate(const std::string& i) { return !i.empty(); }
std::string audio_engine_process(const std::string& i) { return i; }
json audio_engine_toJson(const std::string& i) { return json::object(); }
}
