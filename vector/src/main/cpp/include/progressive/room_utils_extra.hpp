#pragma once

#include <string>
#include <vector>

namespace progressive {

// ==== Room Utilities ====
//
// Room alias building, room ID validation, room membership helpers.

// Build a canonical room alias from a localpart and server.
// "myroom" + "matrix.org" → "#myroom:matrix.org"
inline std::string buildRoomAlias(const std::string& localpart, const std::string& server) {
    return "#" + localpart + ":" + server;
}

// Parse a room alias into localpart and server.
inline bool parseRoomAlias(const std::string& alias, std::string& localpart, std::string& server) {
    if (alias.empty() || alias[0] != '#') return false;
    auto colon = alias.find(':');
    if (colon == std::string::npos || colon <= 1 || colon >= alias.size() - 1) return false;
    localpart = alias.substr(1, colon - 1);
    server = alias.substr(colon + 1);
    return true;
}

// Build a candidate room alias from a room name.
// "My Awesome Room" + "matrix.org" → "#my_awesome_room:matrix.org"
inline std::string candidateAliasFromRoomName(const std::string& name, const std::string& server) {
    std::string localpart;
    for (char c : name) {
        if (c >= 'A' && c <= 'Z') localpart += c + 32;
        else if (c >= 'a' && c <= 'z' || c >= '0' && c <= '9') localpart += c;
        else if (c == ' ' || c == '-' || c == '_') localpart += (localpart.empty() || localpart.back() == '_') ? ' ' : '_';
    }
    // Collapse underscores and remove leading/trailing
    std::string result;
    for (char c : localpart) {
        if (c == ' ') {
            if (!result.empty() && result.back() != '_') result += '_';
        } else {
            result += c;
        }
    }
    return "#" + result + ":" + server;
}

// ==== Room Version Comparison ====

// Known Matrix room versions (in increasing order).
enum class RoomVersion {
    V1 = 0, V2, V3, V4, V5, V6, V7, V8, V9, V10, V11, V12,
    UNKNOWN = 99
};

inline RoomVersion parseRoomVersion(const std::string& version) {
    if (version == "1") return RoomVersion::V1;
    if (version == "2") return RoomVersion::V2;
    if (version == "3") return RoomVersion::V3;
    if (version == "4") return RoomVersion::V4;
    if (version == "5") return RoomVersion::V5;
    if (version == "6") return RoomVersion::V6;
    if (version == "7") return RoomVersion::V7;
    if (version == "8") return RoomVersion::V8;
    if (version == "9") return RoomVersion::V9;
    if (version == "10") return RoomVersion::V10;
    if (version == "11") return RoomVersion::V11;
    if (version == "12") return RoomVersion::V12;
    return RoomVersion::UNKNOWN;
}

inline const char* roomVersionToString(RoomVersion v) {
    switch (v) {
        case RoomVersion::V1: return "1";
        case RoomVersion::V2: return "2";
        case RoomVersion::V3: return "3";
        case RoomVersion::V4: return "4";
        case RoomVersion::V5: return "5";
        case RoomVersion::V6: return "6";
        case RoomVersion::V7: return "7";
        case RoomVersion::V8: return "8";
        case RoomVersion::V9: return "9";
        case RoomVersion::V10: return "10";
        case RoomVersion::V11: return "11";
        case RoomVersion::V12: return "12";
        default: return "unknown";
    }
}

// Check if a room version supports a feature (e.g. restricted joins need v8+).
inline bool roomVersionSupportsRestrictedJoins(RoomVersion v) { return v >= RoomVersion::V8; }
inline bool roomVersionSupportsKnock(RoomVersion v) { return v >= RoomVersion::V7; }
inline bool roomVersionSupportsSpace(RoomVersion v) { return v >= RoomVersion::V6; }

// ==== Room Member Count Formatter ====

// Format room member count for display.
// 1 → "1 member", 42 → "42 members", 1234 → "1.2k members"
inline std::string formatMemberCount(int count) {
    if (count < 1000) {
        return std::to_string(count) + " member" + (count != 1 ? "s" : "");
    }
    double k = count / 1000.0;
    char buf[16];
    if (k < 10.0) snprintf(buf, sizeof(buf), "%.1fk members", k);
    else snprintf(buf, sizeof(buf), "%.0fk members", k);
    return buf;
}

// ==== Invite Count Badge ====

// Format an unread/invite count badge.
// 0 → "", 5 → "5", 99 → "99", 100 → "99+", 1234 → "99+"
inline std::string formatBadgeCount(int count, int max = 99) {
    if (count <= 0) return "";
    if (count <= max) return std::to_string(count);
    return std::to_string(max) + "+";
}

} // namespace progressive
