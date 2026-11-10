#pragma once
#include <string>
#include <vector>

namespace pcp {

struct UserDirectoryItem {
    std::string userId;
    std::string displayName;
    std::string avatarUrl;
    bool isContact = false;     // in address book
};

// Build user search request
std::string buildUserSearchBody(const std::string& query, int limit = 20);

// Parse user search response
std::vector<UserDirectoryItem> parseUserSearchResults(const std::string& json);

// Format user for directory list
std::string formatUserDirectoryItem(const UserDirectoryItem& item);

} // namespace pcp
