#include "pcp/user_invite_utils.hpp"
#include <sstream>
namespace pcp {
InviteTarget parseInviteTarget(const std::string& input) {
    InviteTarget t; t.value = input;
    if (input.find('@') != std::string::npos && input.find(':') == std::string::npos) t.isEmail = true;
    t.isValid = !input.empty(); return t;
}
std::string buildMultiInviteRequest(const std::vector<std::string>& ids, const std::string& reason) {
    std::ostringstream os; os << R"({"user_ids":[)";
    for (size_t i=0;i<ids.size();i++) { if (i>0) os<<","; os << R"(")" << ids[i] << R"(")"; }
    os << "]";
    if (!reason.empty()) os << R"(,"reason":")" << reason << R"(")";
    os << "}"; return os.str();
}
std::string formatInviteList(const std::vector<std::string>& invitees) {
    std::ostringstream os;
    for (size_t i=0;i<invitees.size();i++) { if (i>0) os<<", "; os<<invitees[i]; }
    return os.str();
}
}
