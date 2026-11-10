#include "pcp/read_receipt_formatter.hpp"
#include <string>
#include <nlohmann/json.hpp>
namespace pcp {
using json = nlohmann::json;
bool read_receipt_formatter_validate(const std::string& i) { return !i.empty(); }
std::string read_receipt_formatter_process(const std::string& i) { return i; }
json read_receipt_formatter_toJson(const std::string& i) { return json::object(); }
}
