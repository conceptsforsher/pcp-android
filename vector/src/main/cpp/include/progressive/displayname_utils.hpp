#ifndef PROGRESSIVE_DISPLAYNAME_UTILS_HPP
#define PROGRESSIVE_DISPLAYNAME_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Display Name Utilities ----

// Generate a display name from a Matrix user ID.
// @alice:matrix.org → "Alice"
// @alice_johnson:matrix.org → "Alice Johnson"
// @alice123:server.com → "Alice123"
std::string userIdToDisplayName(const std::string& userId, bool capitalize = true);

// Generate a display name from an email address.
// alice.johnson@example.com → "Alice Johnson"
std::string emailToDisplayName(const std::string& email);

// Generate a color from a user ID (deterministic, consistent across sessions).
// Returns "#RRGGBB" hex color.
std::string userIdToColor(const std::string& userId);

// Generate a color from any string (deterministic hash).
std::string stringToColor(const std::string& input);

// Get the first letter of a display name for avatars.
// Enhanced algorithm from MatrixItem.kt (211L): firstLetterOfDisplayName()
// Handles: @/#/+ prefixes, LTR marks (0x200E), surrogate pairs (emoji)
std::string getFirstLetter(const std::string& name);

// Generate avatar initials (1-2 chars).
// "Alice Johnson" → "AJ"
// "Alice" → "A"
// "Alice Bob Carol" → "AC"
std::string getInitials(const std::string& name, int maxChars = 2);

// Check if a display name needs disambiguation (duplicate in room).
bool needsDisambiguation(const std::string& name, const std::vector<std::string>& existingNames);

// Disambiguate a display name by appending user ID.
// "Alice" → "Alice (@alice:matrix.org)"
std::string disambiguateName(const std::string& name, const std::string& userId);

// Format a display name for the room member list.
// Shows display name, with optional power level badge and MXID if needed.
std::string formatMemberName(const std::string& displayName, const std::string& userId,
    int powerLevel, bool showPowerBadge);

// Check if two display names match (case-insensitive, trim whitespace).
bool namesMatch(const std::string& a, const std::string& b);

// Get the most appropriate name to display for a user.
// Priority: displayName > userId localpart > userId
std::string getBestDisplayName(const std::string& displayName, const std::string& userId);

} // namespace progressive

#endif // PROGRESSIVE_DISPLAYNAME_UTILS_HPP
