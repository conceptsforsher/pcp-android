#ifndef PROGRESSIVE_PERMALINK_HPP
#define PROGRESSIVE_PERMALINK_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Matrix Permalink Utilities ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.permalinks.PermalinkParser.kt (144 lines)
//   Also: MatrixToConverter.kt, PermalinkData.kt

struct PermalinkResult {
    std::string fullUrl;          // https://matrix.to/#/!room:server/$event
    std::string roomId;
    std::string userId;
    std::string eventId;
    std::string roomAlias;
    std::string type;             // "room", "user", "event"
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
};

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
// Faithful to PermalinkParser.kt:parse(uri)
// Handles: user links, room links, room+event links, room alias links,
//   room email invite links (signurl, email, token), group links
PermalinkResult parsePermalinkFull(const std::string& url);

// Extract via parameters from a permalink fragment.
// Original Kotlin: String.getViaParameters()
std::vector<std::string> extractViaParameters(const std::string& fragment);

// Check if a permalink is a room email invite link.
bool isEmailInviteLink(const std::string& url);

// urlDecode is declared in progressive/url_tools.hpp

// Compute via parameters for a Matrix permalink.
// Faithful port from org.matrix.android.sdk.internal.session.permalinks.ViaParameterFinder.kt (103L)
//
// Selects up to `max` server names by:
//   1. Getting joined members of the room
//   2. Extracting server names (domain from MXID)
//   3. Grouping by server, counting members
//   4. Sorting by count (most representative first)
//   5. Ensuring the requesting user's server is included
//   6. Taking top `max` servers
//
// @param myUserId  The requesting user's MXID
// @param memberUserIds  List of joined member MXIDs
// @param historicalUserIds  List of members who left (for extended via params)
// @param maxServers  Max via servers (0 = all)
// @param includeHistorical  Whether to include left members' servers
// @return List of server names, sorted by representativeness
std::vector<std::string> computeViaParams(
    const std::string& myUserId,
    const std::vector<std::string>& memberUserIds,
    const std::vector<std::string>& historicalUserIds = {},
    int maxServers = 3,
    bool includeHistorical = false
);

// Format via parameters as URL query string.
// ["matrix.org", "elsewhere.com"] → "?via=matrix.org&via=elsewhere.com"
std::string formatViaParamsUrl(const std::vector<std::string>& viaServers);

} // namespace progressive

#endif // PROGRESSIVE_PERMALINK_HPP
