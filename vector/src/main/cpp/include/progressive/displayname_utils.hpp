#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

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

// ==== Disambiguation Strategy ====
// Original Kotlin: DisplayNameResolver.kt disambiguation logic

enum class DisambiguationStrategy {
    NONE,                // No disambiguation needed
    APPEND_USERID,       // "Alice (@alice:matrix.org)"
    USE_FULL_USERID,     // Use full MXID instead of display name
    APPEND_MEMBER_NUMBER // "Alice (2)" based on join order
};

// ==== Name Collision Map ====
// Original Kotlin: tracks name collisions within a room

class NameCollisionMap {
public:
    // Register a user's display name. Returns true if a collision was created.
    bool registerName(const std::string& userId, const std::string& displayName);

    // Remove a user from the map.
    void removeUser(const std::string& userId);

    // Check if a display name has collisions (used by >1 user).
    bool hasCollision(const std::string& displayName) const;

    // Get the number of users sharing a given display name.
    int collisionCount(const std::string& displayName) const;

    // Get all users that share a given display name.
    std::vector<std::string> getCollidingUsers(const std::string& displayName) const;

    // Clear all entries.
    void clear();

    // Get total user count.
    int size() const;

private:
    // Map: displayName (lowercase) → count of users
    std::map<std::string, int> nameCounts_;
    // Map: userId → displayName (lowercase)
    std::map<std::string, std::string> userNames_;
    // Map: displayName (lowercase) → list of userIds
    std::map<std::string, std::vector<std::string>> nameToUsers_;
};

// ==== Display Name Change ====
// Original Kotlin: tracks display name change events

struct DisplayNameChange {
    std::string userId;           // who changed
    std::string oldName;          // previous display name (empty if first set)
    std::string newName;          // new display name
    bool isAmbiguous = false;     // new name collides with another user
    std::string disambiguation;   // disambiguated form if needed
    int64_t timestampMs = 0;      // when the change occurred
};

// Format a display name change as human-readable description.
// "Alice changed their name to Alice Cooper"
// "Alice (was Alice Johnson)"
std::string formatDisplayNameChange(const DisplayNameChange& change);

// Get disambiguated display name based on strategy.
// Original Kotlin: apply disambiguation suffix
std::string getDisambiguatedDisplayName(
    const std::string& displayName,
    const std::string& userId,
    DisambiguationStrategy strategy = DisambiguationStrategy::APPEND_USERID,
    int memberNumber = 0);

// Check for name collision in a list of names.
// Returns true if the given displayName matches any existing name (case-insensitive).
bool checkNameCollision(
    const std::string& displayName,
    const std::vector<std::string>& existingNames);

// Resolve a name collision by applying disambiguation.
// Returns the disambiguated name, or original if no collision.
std::string resolveNameCollision(
    const std::string& displayName,
    const std::string& userId,
    const NameCollisionMap& collisionMap,
    DisambiguationStrategy strategy = DisambiguationStrategy::APPEND_USERID);

// ==== Display Name Resolver ====
// Original Kotlin: DisplayNameResolver.kt + member event resolution
// Full display name resolution pipeline:
//   1. Prefer display name from profile
//   2. Fallback to member event display name
//   3. Fallback to userId localpart extraction

class DisplayNameResolver {
public:
    // Main entry: resolve the best display name for a user.
    // Priority: profile displayName > member event displayName > userId fallback
    std::string resolveDisplayName(
        const std::string& userId,
        const std::string& profileDisplayName,
        const std::string& memberEventDisplayName);

    // Resolve from user profile (server /profile API).
    // Original Kotlin: ProfileService.getProfile -> displayname
    std::string resolveFromProfile(
        const std::string& profileDisplayName,
        const std::string& userId);

    // Resolve from m.room.member state event.
    // Original Kotlin: member content.displayname
    std::string resolveFromMemberEvent(
        const std::string& memberEventDisplayName,
        const std::string& userId);

    // Resolve from userId — extract localpart from @localpart:domain.
    // @alice_johnson:matrix.org → "Alice Johnson"
    std::string resolveFromUserId(const std::string& userId);

    // Check if a display name is ambiguous (same name as another user in room).
    // Original Kotlin: isUniqueDisplayName check
    bool isDisplayNameAmbiguous(
        const std::string& displayName,
        const std::vector<std::string>& otherMemberNames) const;
};

} // namespace progressive
