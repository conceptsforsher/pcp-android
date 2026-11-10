#include "pcp/user_search_utils.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {
static std::string toLower(const std::string& s) { std::string r; std::transform(s.begin(), s.end(), std::back_inserter(r), ::tolower); return r; }

double scoreUserMatch(const std::string& name, const std::string& uid, const std::string& query) {
    std::string q = toLower(query); std::string n = toLower(name); std::string u = toLower(uid);
    if (n == q) return 1.0; if (n.find(q) == 0) return 0.9;
    if (n.find(q) != std::string::npos) return 0.5;
    if (u.find(q) != std::string::npos) return 0.3; return 0.1;
}
std::vector<UserSearchMatch> fuzzySearchUsers(const std::vector<UserSearchMatch>& users,
                                                const std::string& query, int max) {
    auto r = users; for (auto& u : r) u.score = scoreUserMatch(u.displayName, u.userId, query);
    std::sort(r.begin(), r.end(), [](auto& a, auto& b) { return a.score > b.score; });
    if ((int)r.size() > max) r.resize(max); return r;
}
std::string formatUserMatch(const UserSearchMatch& m) {
    return (m.displayName.empty() ? m.userId : m.displayName);
}
} // namespace pcp
