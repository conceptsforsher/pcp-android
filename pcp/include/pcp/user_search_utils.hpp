#pragma once
#include <string>
#include <vector>

namespace pcp {

struct UserSearchMatch {
    std::string userId;
    std::string displayName;
    double score = 0.0;
};

// Search users by query
std::vector<UserSearchMatch> fuzzySearchUsers(const std::vector<UserSearchMatch>& users,
                                                const std::string& query, int maxResults = 10);

// Score user match
double scoreUserMatch(const std::string& name, const std::string& userId, const std::string& query);

// Format user search result
std::string formatUserMatch(const UserSearchMatch& match);

} // namespace pcp
