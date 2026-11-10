#include "pcp/message_retry_utils.hpp"
#include <sstream>
namespace pcp {
bool shouldRetryMessage(const std::string& code, int count, int max) {
    return count < max && code != "M_FORBIDDEN" && code != "M_UNKNOWN_TOKEN" && code != "M_BAD_JSON";
}
int64_t getRetryDelayMs(int count) { return (1LL << (count * 2)) * 1000; }
std::string formatRetryStatus(int count, int max) {
    std::ostringstream os; os << "Retry " << count << "/" << max; return os.str();
}
}
