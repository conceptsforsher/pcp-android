#ifndef PROGRESSIVE_MATRIX_PATTERNS_HPP
#define PROGRESSIVE_MATRIX_PATTERNS_HPP

#include <string>
#include <vector>
#include "progressive/permalink.hpp"

namespace progressive {

// Matrix Identifier grammar (per spec appendices).
// https://matrix.org/docs/spec/appendices#identifier-grammar

// Check if a string is a valid Matrix user ID: @user:server
bool isUserId(const std::string& input);

// Check if a string is a valid Matrix room ID: !xxx:server
bool isRoomId(const std::string& input);

// Check if a string is a valid Matrix room alias: #alias:server
bool isRoomAlias(const std::string& input);

// Check if a string is a valid Matrix event ID: $xxx
bool isEventId(const std::string& input);

// Check if a string is a valid Matrix group ID: +group:server
bool isGroupId(const std::string& input);

// Check if a URL is a Matrix.to permalink.
bool isMatrixToPermalink(const std::string& url);

// Check if a URL is an app permalink (https://domain/#/room/user/...).
bool isAppPermalink(const std::string& url);

// Parse a Matrix.to permalink and extract the type and identifier.
// e.g. "https://matrix.to/#/@user:server" → {"user", "@user:server"}
// e.g. "https://matrix.to/#/!room:server/$event" → {"room", "!room:server", "$event"}
struct PermalinkInfo {
    std::string type;        // "user", "room", "event"
    std::string roomId;
    std::string userId;
    std::string eventId;
    bool valid = false;
};
PermalinkInfo parseMatrixToPermalink(const std::string& url);

// Extract all Matrix IDs (user, room, alias, event) from text.
struct ExtractedIds {
    std::vector<std::string> userIds;
    std::vector<std::string> roomIds;
    std::vector<std::string> roomAliases;
    std::vector<std::string> eventIds;
};
ExtractedIds extractMatrixIds(const std::string& text);

// Check if a string is a valid matrix.to URL for a room.
bool isMxcUrl(const std::string& url);

// Check if a string is a valid phone number (E.164-ish).
bool isPhoneNumber(const std::string& input);

// Check if a string is a valid email address.
bool isValidEmail(const std::string& input);

// Extract server name (domain) from a Matrix ID.
// Original Kotlin (MatrixPatterns.kt:extractServerNameFromId):
//   matrixId?.substringAfter(":", missingDelimiterValue = "")?.takeIf { it.isNotEmpty() }
// "@alice:matrix.org" → "matrix.org"
// "!room:matrix.org:8448" → "matrix.org:8448"
std::string extractServerNameFromId(const std::string& matrixId);

// Extract user name (localpart) from a Matrix user ID.
// Original Kotlin: matrixId.removePrefix("@").substringBefore(":")
// "@alice:matrix.org" → "alice"
std::string extractUserNameFromId(const std::string& matrixId);

// Check if an order string is valid (ASCII printable, < 50 chars).
// Original Kotlin: order != null && order.length < 50 && order matches ORDER_STRING_REGEX
// ORDER_STRING_REGEX = "[ -~]+" (ASCII printable chars: space through ~)
bool isValidOrderString(const std::string& order);

// Generate a candidate room alias from a room name.
// Original Kotlin (MatrixPatterns.kt:candidateAliasFromRoomName):
//   roomName.lowercase().replaceSpaceChars("_").removeInvalidRoomNameChars()
//       .take(maxAliasLocalPartLength(domain))
// "My Room Name" → "my_room_name"
std::string candidateAliasFromRoomName(const std::string& roomName, const std::string& domain, int maxAliasLength = 100);

} // namespace progressive

#endif // PROGRESSIVE_MATRIX_PATTERNS_HPP
