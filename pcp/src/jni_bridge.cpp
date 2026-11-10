#include "pcp/jni_bridge.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool jni_bridge_validate(const std::string& i) { return !i.empty(); }
std::string jni_bridge_process(const std::string& i) { return i; }
json jni_bridge_toJson(const std::string& i) { return json::object(); }
}
