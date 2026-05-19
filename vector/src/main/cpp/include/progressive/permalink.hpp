#pragma once

#include <string>
#include <vector>

namespace progressive {

// ================================================================
// Matrix Permalink Utilities
//
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.permalinks.PermalinkParser.kt (144 lines)
//   Also: MatrixToConverter.kt, PermalinkData.kt
// ================================================================

// ---- Entity Type (for permalink parsing) ----
// Original Kotlin: implicit from identifier type (!, @, #, +)

enum class EntityType {
    ROOM,    // !roomId:server or #roomAlias:server
    EVENT,   // !roomId:server/$eventId
    USER,    // @userId:server
    GROUP,   // +groupId:server (legacy)
    UNKNOWN,
};

const char* entityTypeToString(EntityType type);
EntityType entityTypeFromId(const std::string& identifier);

// ---- Permalink Result ----

struct PermalinkResult {
    std::string fullUrl;          // https://matrix.to/#/!room:server/$event
    std::string roomId;
    std::string userId;
    std::string eventId;
    std::string roomAlias;
    std::string type;             // "room", "user", "event"
    EntityType entityType = EntityType::UNKNOWN;
    bool valid = false;
    bool isRoomAlias = false;     // #alias:server (vs !room:server)
    std::vector<std::string> viaParameters; // via server names

    // Room email invite fields (from PermalinkParser.kt:101-112)
    std::string email;            // invitee email
    std::string signUrl;          // identity server sign URL
    std::string identityServer;   // identity server host
    std::string token;            // invitation token
    std::string privateKey;       // invitation private key
    std::string roomName;         // room name from params
    std::string inviterName;      // inviter name from params
    std::string roomAvatarUrl;    // room avatar from params
    std::string roomType;         // room type from params
    bool isEmailInvite = false;   // this is an email invite link

    // Thread support
    std::string threadId;         // thread root event ID
    bool isThreadLink = false;    // link includes thread context
};

// ================================================================
// PermalinkBuilder — build permalink URLs
// ================================================================
// Original Kotlin: various builders across the SDK

struct PermalinkBuilder {
    // Build a matrix.to permalink for a room.
    // Original Kotlin: "https://matrix.to/#/" + roomIdOrAlias
    static std::string buildRoomPermalink(const std::string& roomIdOrAlias,
        const std::vector<std::string>& viaServers = {});

    // Build a matrix.to permalink for an event in a room.
    // Original Kotlin: "https://matrix.to/#/" + roomId + "/" + eventId
    static std::string buildEventPermalink(const std::string& roomId,
        const std::string& eventId,
        const std::vector<std::string>& viaServers = {});

    // Build a matrix.to permalink for a user.
    // Original Kotlin: "https://matrix.to/#/" + userId
    static std::string buildUserPermalink(const std::string& userId);

    // Build a matrix.to permalink for a group (legacy).
    static std::string buildGroupPermalink(const std::string& groupId);

    // Build a matrix.to permalink for a thread event.
    // Original Kotlin: room/event with thread context
    static std::string buildThreadPermalink(const std::string& roomId,
        const std::string& eventId,
        const std::string& threadRootId,
        const std::vector<std::string>& viaServers = {});

    // Build a matrix: scheme link (matrix:u/user, matrix:r/room, matrix:e/event).
    // Original Kotlin: MatrixToConverter
    static std::string buildMatrixSchemeLink(EntityType type, const std::string& id);

    // Build a general matrix.to link from a PermalinkResult.
    static std::string buildFromResult(const PermalinkResult& result);

    // Format via parameters as URL query string.
    // ["matrix.org", "elsewhere.com"] → "?via=matrix.org&via=elsewhere.com"
    static std::string formatViaParamsUrl(const std::vector<std::string>& viaServers);
};

// ================================================================
// PermalinkParser — parse matrix.to and matrix: URIs
// ================================================================
// Original Kotlin: PermalinkParser.kt:parse(uri), MatrixToConverter.kt

struct PermalinkParser {
    // Parse a matrix.to or matrix: permalink.
    // Handles: user links, room links, room+event links, room alias links,
    //   room email invite links, group links, thread links.
    static PermalinkResult parse(const std::string& url);

    // Parse a matrix.to permalink with full parameter extraction.
    // Original Kotlin: PermalinkParser.kt:parse(uri)
    static PermalinkResult parsePermalink(const std::string& url);

    // Parse a matrix: scheme URI (matrix:r/room, matrix:u/user, matrix:e/event).
    static PermalinkResult parseMatrixSchemeUri(const std::string& uri);

    // Extract via parameters from a permalink fragment.
    static std::vector<std::string> extractViaParameters(const std::string& fragment);

    // Check if a URL is a valid Matrix permalink (matrix.to or matrix: scheme).
    static bool isMatrixPermalink(const std::string& url);

    // Check if a URL is a matrix.to permalink.
    static bool isMatrixToLink(const std::string& url);

    // Check if a URL is a matrix: scheme link.
    static bool isMatrixSchemeLink(const std::string& url);

    // Check if a permalink is a room email invite link.
    static bool isEmailInviteLink(const std::string& url);
};

// ================================================================
// Convenience functions (backward compatible with existing API)
// ================================================================

// Build a matrix.to permalink for a room.
std::string buildRoomPermalink(const std::string& roomIdOrAlias);

// Build a matrix.to permalink for a user.
std::string buildUserPermalink(const std::string& userId);

// Build a matrix.to permalink for an event.
std::string buildEventPermalink(const std::string& roomId, const std::string& eventId);

// Build a matrix: scheme link (matrix:u/user, matrix:r/room, matrix:e/event).
std::string buildMatrixSchemeLink(const std::string& type, const std::string& id);

// Parse a matrix.to permalink.
PermalinkResult parsePermalink(const std::string& url);

// Check if a URL is a valid Matrix permalink.
bool isPermalink(const std::string& url);
bool isMatrixPermalink(const std::string& url);

// Extract room ID from permalink.
std::string extractRoomIdFromPermalink(const std::string& url);

// Extract event ID from permalink.
std::string extractEventIdFromPermalink(const std::string& url);

// Extract user ID from permalink.
std::string extractUserIdFromPermalink(const std::string& url);

// Format a permalink for sharing (short form).
std::string formatPermalinkForShare(const PermalinkResult& info);

// Check if two permalinks point to the same room.
bool isSameRoomPermalink(const std::string& url1, const std::string& url2);

// Parse a matrix.to permalink with full parameter extraction.
PermalinkResult parsePermalinkFull(const std::string& url);

// Parse a matrix.to URI extracting entity type, IDs, and query params.
// Original Kotlin: full URI parser (supports matrix.to and matrix: schemes)
PermalinkResult parseMatrixToUri(const std::string& uri);

// Extract via parameters from a permalink fragment.
std::vector<std::string> extractViaParameters(const std::string& fragment);

// Check if a permalink is a room email invite link.
bool isEmailInviteLink(const std::string& url);

// Compute via parameters for a Matrix permalink.
std::vector<std::string> computeViaParams(
    const std::string& myUserId,
    const std::vector<std::string>& memberUserIds,
    const std::vector<std::string>& historicalUserIds = {},
    int maxServers = 3,
    bool includeHistorical = false
);

// Format via parameters as URL query string.
std::string formatViaParamsUrl(const std::vector<std::string>& viaServers);

} // namespace progressive
