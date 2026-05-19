#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ==== Room Display Name Resolver ====
//
// Computes the display name of a room based on Matrix spec rules.
// Original Kotlin: RoomDisplayNameResolver.kt
//
// Priority:
//   1. Room name (m.room.name state event) — if set, use it
//   2. Canonical alias (m.room.canonical_alias) — if set, use it
//   3. Heroes list — build name from member display names
//   4. Fallback: "Empty room" (empty string for us)

struct RoomNameMember {
    std::string userId;
    std::string displayName;
    bool isCurrentUser = false;
};

// ==== Room Display Name Fallback ====
// Original Kotlin: RoomDisplayNameFallbackProvider.kt
// Tracks which source provided the room display name

enum class RoomDisplayNameFallback {
    EMPTY_ROOM,       // No members, no name
    ALIAS,            // From canonical alias
    MEMBER_NAMES,     // Computed from member display names
    CANONICAL_ALIAS,  // From canonical alias (redundant with ALIAS, kept for tracing)
    HEROES,           // From heroes list subset
    ROOM_ID           // Raw room ID as last resort
};

// ==== Room Name Styling ====
// Original Kotlin: rendering variant for different UI contexts

enum class RoomNameStyling {
    PLAIN,             // Raw name, no extras
    WITH_MEMBER_COUNT, // "Room Name (42 members)"
    WITH_AVATAR,       // With avatar prefix hint
    FULL,              // Full name with member count and alias
    COMPACT            // Truncated for tight layouts
};

// ==== Room Display Name Result ====
// Original Kotlin: full result from room name resolution with fallback chain

struct RoomDisplayNameResult {
    std::string name;                    // Final computed room display name
    RoomDisplayNameFallback source;      // Which source provided the name
    std::vector<RoomDisplayNameFallback> fallbackChain;  // Chain of attempts
    bool isGenerated = false;            // Auto-generated from members (vs. explicitly set)
    std::vector<std::string> heroNames;  // Hero names used in computation
    int memberCount = 0;                 // Number of members used for generating name
    int totalMemberCount = 0;            // Total members in the room
};

// Compute the room display name following Matrix spec rules.
//
// Parameters:
//   roomName: from m.room.name state event (empty if not set)
//   canonicalAlias: from m.room.canonical_alias state event
//   members: list of active room members with display names
//   maxHeroes: max number of heroes to show (default 3)
//
// Returns: computed display name

inline std::string resolveRoomDisplayName(
    const std::string& roomName,
    const std::string& canonicalAlias,
    const std::vector<RoomNameMember>& members,
    int maxHeroes = 3)
{
    // Rule 1: Room name takes highest priority
    if (!roomName.empty()) return roomName;

    // Rule 2: Canonical alias
    if (!canonicalAlias.empty()) return canonicalAlias;

    // Rule 3: Build from member display names (heroes)
    // Filter: active members only (exclude left/banned)
    // Exclude current user from the hero list
    std::vector<std::string> heroNames;
    for (const auto& m : members) {
        if (m.isCurrentUser) continue;
        std::string name = m.displayName.empty() ? m.userId : m.displayName;
        heroNames.push_back(name);
        if ((int)heroNames.size() >= maxHeroes) break;
    }

    if (heroNames.empty()) {
        // Only the current user is in the room (Direct chat with self)
        for (const auto& m : members) {
            if (m.isCurrentUser) {
                return m.displayName.empty() ? m.userId : m.displayName;
            }
        }
        return ""; // "Empty room"
    }

    // Join hero names
    if (heroNames.size() == 1) {
        return heroNames[0];
    }

    // Build: "Alice, Bob and Charlie"
    std::string result;
    for (size_t i = 0; i < heroNames.size(); i++) {
        if (i > 0) {
            result += (i == heroNames.size() - 1) ? " and " : ", ";
        }
        result += heroNames[i];
    }

    // Count total members beyond heroes
    int totalOthers = 0;
    for (const auto& m : members) {
        if (!m.isCurrentUser) totalOthers++;
    }

    int remaining = totalOthers - (int)heroNames.size();
    if (remaining > 0) {
        result += " and " + std::to_string(remaining) + " other";
        if (remaining > 1) result += "s";
    }

    return result;
}

// ==== Compute Room Display Name with Full Fallback Chain ====
// Original Kotlin: full chain as in RoomDisplayNameResolver.kt
//
// Fallback order:
//   1. m.room.name state event
//   2. Canonical alias
//   3. For DMs: other member name
//   4. Hero names (max 3)
//   5. Member count ("N members")
//   6. "Empty Room"

inline RoomDisplayNameResult computeRoomDisplayNameWithFallback(
    const std::string& roomName,
    const std::string& canonicalAlias,
    const std::string& directUserId,
    const std::vector<RoomNameMember>& members,
    const std::string& currentUserId)
{
    // Original Kotlin: full fallback chain resolution
    RoomDisplayNameResult result;
    result.totalMemberCount = static_cast<int>(members.size());

    // 1. Room name from state
    if (!roomName.empty()) {
        result.name = roomName;
        result.source = RoomDisplayNameFallback::ALIAS;  // ALIAS here tracks state name
        result.fallbackChain = {RoomDisplayNameFallback::CANONICAL_ALIAS};
        return result;
    }

    // 2. Canonical alias
    if (!canonicalAlias.empty()) {
        result.name = canonicalAlias;
        result.source = RoomDisplayNameFallback::CANONICAL_ALIAS;
        result.fallbackChain = {RoomDisplayNameFallback::CANONICAL_ALIAS};
        return result;
    }

    // 3. For DMs: other member name
    if (!directUserId.empty()) {
        for (const auto& m : members) {
            if (m.userId != currentUserId && !m.isCurrentUser) {
                result.name = m.displayName.empty() ? m.userId : m.displayName;
                result.source = RoomDisplayNameFallback::MEMBER_NAMES;
                result.fallbackChain = {RoomDisplayNameFallback::MEMBER_NAMES};
                result.isGenerated = true;
                result.heroNames.push_back(result.name);
                result.memberCount = 1;
                return result;
            }
        }
        // Fallback: use direct user ID when member list is empty
        result.name = directUserId;
        result.source = RoomDisplayNameFallback::MEMBER_NAMES;
        result.fallbackChain = {RoomDisplayNameFallback::MEMBER_NAMES};
        result.isGenerated = true;
        result.heroNames.push_back(directUserId);
        result.memberCount = 1;
        return result;
    }

    // 4. Hero names (max 3)
    std::vector<std::string> heroNames;
    for (const auto& m : members) {
        if (m.isCurrentUser) continue;
        if (m.userId == currentUserId) continue;
        std::string name = m.displayName.empty() ? m.userId : m.displayName;
        heroNames.push_back(name);
        if ((int)heroNames.size() >= 3) break;
    }

    if (!heroNames.empty()) {
        result.source = RoomDisplayNameFallback::HEROES;
        result.fallbackChain = {RoomDisplayNameFallback::HEROES};
        result.isGenerated = true;
        result.heroNames = heroNames;
        result.memberCount = static_cast<int>(heroNames.size());

        if (heroNames.size() == 1) {
            result.name = heroNames[0];
        } else if (heroNames.size() == 2) {
            result.name = heroNames[0] + " and " + heroNames[1];
        } else {
            result.name = heroNames[0] + ", " + heroNames[1] + " and " + heroNames[2];
        }

        // Count remaining members
        int remaining = result.totalMemberCount - (int)heroNames.size();
        if (remaining > 0) {
            result.name += " and " + std::to_string(remaining) + " other";
            if (remaining > 1) result.name += "s";
        }
        return result;
    }

    // 5. Member count
    if (result.totalMemberCount > 0) {
        result.name = std::to_string(result.totalMemberCount) + " members";
        result.source = RoomDisplayNameFallback::MEMBER_NAMES;
        result.fallbackChain = {RoomDisplayNameFallback::MEMBER_NAMES};
        result.isGenerated = true;
        result.memberCount = result.totalMemberCount;
        return result;
    }

    // 6. Empty room
    result.name = "Empty Room";
    result.source = RoomDisplayNameFallback::EMPTY_ROOM;
    result.fallbackChain = {RoomDisplayNameFallback::EMPTY_ROOM};
    result.isGenerated = true;
    return result;
}

// ==== Format Room Name for Notification ====
// Original Kotlin: short format for push notifications
// Truncated to fit push notification constraints

inline std::string formatRoomNameForNotification(const std::string& roomName, int maxLen = 30) {
    // Original Kotlin: short format for Android notification tray
    if ((int)roomName.size() <= maxLen) return roomName;
    return roomName.substr(0, maxLen - 3) + "...";
}

// ==== Format Room Name for Room List ====
// Original Kotlin: medium format for room list items
// Includes member count for rooms with > 2 members

inline std::string formatRoomNameForRoomList(
    const RoomDisplayNameResult& result,
    RoomNameStyling style = RoomNameStyling::PLAIN)
{
    // Original Kotlin: room list formatting
    switch (style) {
        case RoomNameStyling::PLAIN:
            return result.name;
        case RoomNameStyling::WITH_MEMBER_COUNT:
            if (result.totalMemberCount > 1) {
                return result.name + " (" + std::to_string(result.totalMemberCount) + ")";
            }
            return result.name;
        case RoomNameStyling::COMPACT:
            if ((int)result.name.size() > 25) {
                return result.name.substr(0, 22) + "...";
            }
            return result.name;
        default:
            return result.name;
    }
}

// ==== Format Room Name for Header ====
// Original Kotlin: full format for room header/title bar
// May include canonical alias or member count

inline std::string formatRoomNameForHeader(
    const std::string& roomName,
    int memberCount = 0,
    RoomNameStyling style = RoomNameStyling::FULL)
{
    // Original Kotlin: room header formatting
    std::string result = roomName;
    if (style == RoomNameStyling::FULL && memberCount > 0) {
        result += "  ·  " + std::to_string(memberCount) + " members";
    }
    return result;
}

// ==== Format Room Name for Share ====
// Original Kotlin: format for sharing room via link/text
// "Room Name (via Matrix)" or room name + alias

inline std::string formatRoomNameForShare(const std::string& roomName, const std::string& alias) {
    // Original Kotlin: sharing format
    if (!alias.empty()) {
        return roomName + " (" + alias + ")";
    }
    return roomName;
}

// ==== Truncate Room Name ====
// Original Kotlin: truncate for display constraints (e.g. small screens)

inline std::string truncateRoomName(const std::string& roomName, int maxLen, const std::string& suffix = "...") {
    // Original Kotlin: truncate for UI constraints
    if ((int)roomName.size() <= maxLen) return roomName;
    if (maxLen <= (int)suffix.size()) return suffix.substr(0, maxLen);
    return roomName.substr(0, maxLen - (int)suffix.size()) + suffix;
}

} // namespace progressive
