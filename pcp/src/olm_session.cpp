#include "pcp/olm_session.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool olm_session_validate(const std::string& i) { return !i.empty(); }
std::string olm_session_process(const std::string& i) { return i; }
json olm_session_toJson(const std::string& i) { return json::object(); }
}
