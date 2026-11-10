#include "pcp/megolm_decryptor.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool megolm_decryptor_validate(const std::string& i) { return !i.empty(); }
std::string megolm_decryptor_process(const std::string& i) { return i; }
json megolm_decryptor_toJson(const std::string& i) { return json::object(); }
}
