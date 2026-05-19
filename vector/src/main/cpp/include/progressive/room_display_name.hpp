#pragma once

#include <string>
#include <vector>

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

} // namespace progressive
