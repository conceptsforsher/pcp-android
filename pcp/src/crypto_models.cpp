#include "pcp/crypto_models.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool crypto_models_validate(const std::string& i) { return !i.empty(); }
std::string crypto_models_process(const std::string& i) { return i; }
json crypto_models_toJson(const std::string& i) { return json::object(); }
}
