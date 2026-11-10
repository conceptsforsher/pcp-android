#include "pcp/read_receipts_utils.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool read_receipts_utils_validate(const std::string& i) { return !i.empty(); }
std::string read_receipts_utils_process(const std::string& i) { return i; }
json read_receipts_utils_toJson(const std::string& i) { return json::object(); }
}
