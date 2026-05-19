#ifndef PROGRESSIVE_IDENTITY_UTILS_HPP
#define PROGRESSIVE_IDENTITY_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Matrix ID Resolution ----

struct ResolvedId {
    std::string input;       // original input
    std::string resolved;    // resolved MXID or room ID
    std::string type;        // "user", "room", "alias", "event", "unknown"
    bool valid = false;
};

// Resolve a Matrix identifier from various input formats.
// Handles: @user:server, #alias:server, !room:server, $event, matrix.to URLs.
ResolvedId resolveMatrixId(const std::string& input);

// ---- 3PID (Third-Party ID) Utilities ----

struct IdentityThreePid {
    std::string medium;     // "email", "msisdn"
    std::string address;    // "user@example.com", "+1234567890"
    bool valid = false;
};

// Parse a 3PID from input.
IdentityThreePid parseThreePid(const std::string& input);

// Check if a string is an email address.
bool isEmail(const std::string& input);

// Check if a string is a phone number (MSISDN).
bool isMsisdn(const std::string& input);

// Format a 3PID for display.
std::string formatThreePid(const IdentityThreePid& pid);

// ---- Display Name Utilities ----

// Check if two display names are ambiguous (same or very similar).
bool isAmbiguousName(const std::string& name1, const std::string& name2);

// Generate a unique display name if duplicates exist.
// "Alice" + duplicates → "Alice (@alice:matrix.org)"
std::string disambiguateName(const std::string& displayName, const std::string& mxid);

// Check if a display name violates Matrix rules.
bool isValidDisplayName(const std::string& name);

// Get initials from a display name: "Alice Johnson" → "AJ"
std::string getIdentityInitials(const std::string& displayName, int maxChars = 2);

// ---- Room Alias Utilities ----

// Check if an alias is canonical (matches expected format).
bool isCanonicalAlias(const std::string& alias, const std::string& expectedRoomId);

// Extract the local part of an alias: "#room:server" → "room"
std::string extractAliasLocalpart(const std::string& alias);

// Suggest room aliases from a room name.
std::vector<std::string> suggestAliases(const std::string& roomName, int maxResults = 3);

} // namespace progressive

#endif // PROGRESSIVE_IDENTITY_UTILS_HPP
