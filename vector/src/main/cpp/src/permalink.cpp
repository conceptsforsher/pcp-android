#include "progressive/permalink.hpp"
#include "progressive/url_tools.hpp"
#include <sstream>
#include <unordered_map>

namespace progressive {

std::string buildRoomPermalink(const std::string& roomIdOrAlias) {
    return "https://matrix.to/#/" + roomIdOrAlias;
}

std::string buildUserPermalink(const std::string& userId) {
    return "https://matrix.to/#/" + userId;
}

std::string buildEventPermalink(const std::string& roomId, const std::string& eventId) {
    return "https://matrix.to/#/" + roomId + "/" + eventId;
}

std::string buildMatrixSchemeLink(const std::string& type, const std::string& id) {
    return "matrix:" + type + "/" + id;
}

PermalinkResult parsePermalink(const std::string& url) {
    PermalinkResult info;
    info.fullUrl = url;

    // Strip https://matrix.to/#/
    std::string prefix = "https://matrix.to/#/";
    if (url.rfind(prefix, 0) != 0) return info;

    auto rest = url.substr(prefix.size());

    if (rest.empty()) return info;

    if (rest[0] == '@') {
        // User permalink
        info.type = "user";
        info.userId = rest;
        info.valid = true;
    } else if (rest[0] == '#') {
        // Room alias permalink
        info.type = "room";
        info.roomAlias = rest;
        info.valid = true;
    } else if (rest[0] == '!') {
        // Room ID permalink, optionally with /$event
        auto slash = rest.find('/');
        if (slash != std::string::npos) {
            info.type = "event";
            info.roomId = rest.substr(0, slash);
            info.eventId = rest.substr(slash + 1);
        } else {
            info.type = "room";
            info.roomId = rest;
        }
        info.valid = true;
    }

    return info;
}

bool isPermalink(const std::string& url) {
    return url.rfind("https://matrix.to/#/", 0) == 0;
}

std::string extractRoomIdFromPermalink(const std::string& url) {
    auto info = parsePermalink(url);
    return info.roomId;
}

std::string extractEventIdFromPermalink(const std::string& url) {
    auto info = parsePermalink(url);
    return info.eventId;
}

std::string extractUserIdFromPermalink(const std::string& url) {
    auto info = parsePermalink(url);
    return info.userId;
}

std::string formatPermalinkForShare(const PermalinkResult& info) {
    std::ostringstream out;
    if (info.type == "room") {
        out << "Join room: " << info.fullUrl;
    } else if (info.type == "user") {
        out << "Contact: " << info.fullUrl;
    } else if (info.type == "event") {
        out << "Message: " << info.fullUrl;
    }
    return out.str();
}

bool isSameRoomPermalink(const std::string& url1, const std::string& url2) {
    return extractRoomIdFromPermalink(url1) == extractRoomIdFromPermalink(url2);
}

// ---- Enhanced Permalink Parser (from PermalinkParser.kt:45-88) ----
// Original Kotlin:
//   fun parse(uri: Uri): PermalinkData {
//       val matrixToUri = MatrixToConverter.convert(uri) ?: return PermalinkData.FallbackLink(uri)
//       val fragment = matrixToUri.toString().substringAfter("#")
//       val safeFragment = fragment.substringBefore('?')
//       val params = safeFragment.split("/").filter { it.isNotEmpty() }.take(2)
//       val decodedIdentifier = decodedParams.getOrNull(0)
//       return when {
//           isUserId(decodedIdentifier) -> UserLink(userId = decodedIdentifier)
//           isRoomId(decodedIdentifier) -> handleRoomIdCase(...)
//           isRoomAlias(decodedIdentifier) -> RoomLink(roomIdOrAlias = ..., isRoomAlias = true)
//           else -> FallbackLink(uri)
//       }
//   }

PermalinkResult parsePermalinkFull(const std::string& url) {
    PermalinkResult result;
    result.fullUrl = url;

    // First try the existing parser
    result = parsePermalink(url);
    if (!result.valid) {
        result.fullUrl = url;
        return result;
    }

    // Extract fragment (everything after #)
    auto hashPos = url.find('#');
    if (hashPos == std::string::npos) return result;
    std::string fragment = url.substr(hashPos + 1);

    // Get safe fragment (before ?)
    auto queryPos = fragment.find('?');
    std::string safeFragment = (queryPos != std::string::npos) ? fragment.substr(0, queryPos) : fragment;

    // Extract via parameters
    result.viaParameters = extractViaParameters(fragment);

    // Check for email invite parameters
    auto emailPos = fragment.find("email=");
    auto signurlPos = fragment.find("signurl=");
    if (emailPos != std::string::npos && signurlPos != std::string::npos) {
        result.isEmailInvite = true;
        // Extract email
        emailPos += 6;
        auto emailEnd = fragment.find('&', emailPos);
        result.email = urlDecode(fragment.substr(emailPos, emailEnd - emailPos));
        // Extract signurl
        signurlPos += 8;
        auto signurlEnd = fragment.find('&', signurlPos);
        result.signUrl = urlDecode(fragment.substr(signurlPos, signurlEnd - signurlPos));
        // Extract other params
        auto extractParam = [&](const std::string& key) -> std::string {
            auto kp = fragment.find(key + "=");
            if (kp == std::string::npos) return "";
            kp += key.size() + 1;
            auto ke = fragment.find('&', kp);
            return urlDecode(fragment.substr(kp, ke - kp));
        };
        result.roomName = extractParam("room_name");
        result.inviterName = extractParam("inviter_name");
        result.roomAvatarUrl = extractParam("room_avatar_url");
        result.roomType = extractParam("room_type");
        result.token = extractParam("token");
        result.privateKey = extractParam("private_key");
    }

    result.isRoomAlias = !result.roomAlias.empty();
    return result;
}

std::vector<std::string> extractViaParameters(const std::string& fragment) {
    std::vector<std::string> vias;
    // Original Kotlin: UrlQuerySanitizer(this).parameterList.filter { it.mParameter == "via" }
    size_t pos = 0;
    while (true) {
        pos = fragment.find("via=", pos);
        if (pos == std::string::npos) break;
        pos += 4;
        auto end = fragment.find('&', pos);
        std::string value = (end != std::string::npos) ? fragment.substr(pos, end - pos) : fragment.substr(pos);
        vias.push_back(urlDecode(value));
        if (end == std::string::npos) break;
        pos = end;
    }
    return vias;
}

bool isEmailInviteLink(const std::string& url) {
    auto hashPos = url.find('#');
    if (hashPos == std::string::npos) return false;
    std::string fragment = url.substr(hashPos + 1);
    return fragment.find("email=") != std::string::npos && fragment.find("signurl=") != std::string::npos;
}

// urlDecode is defined in progressive/url_tools.cpp

// ==== Via Parameter Computation (from ViaParameterFinder.kt:36-64) ====
// Original Kotlin:
//   fun computeViaParams(userId: String, roomId: String, max: Int): List<String> {
//       val userHomeserver = userId.getServerName()
//       return getUserIdsOfJoinedMembers(roomId)
//           .map { it.getServerName() }
//           .groupBy { it }.mapValues { it.value.size }.toMutableMap()
//           .apply { this[userHomeserver] = Int.MAX_VALUE }
//           .let { map -> map.keys.sortedByDescending { map[it] } }
//           .take(max)
//   }

std::vector<std::string> computeViaParams(
    const std::string& myUserId,
    const std::vector<std::string>& memberUserIds,
    const std::vector<std::string>& historicalUserIds,
    int maxServers,
    bool includeHistorical)
{
    // Extract the current user's server name
    std::string myServer;
    {
        auto colon = myUserId.rfind(':');
        if (colon != std::string::npos) myServer = myUserId.substr(colon + 1);
    }

    // Extract server names from current member MXIDs
    std::unordered_map<std::string, int> serverCounts;
    for (const auto& uid : memberUserIds) {
        auto colon = uid.rfind(':');
        if (colon != std::string::npos) {
            serverCounts[uid.substr(colon + 1)]++;
        }
    }

    // Original: optionally include historical (left) members
    if (includeHistorical) {
        for (const auto& uid : historicalUserIds) {
            auto colon = uid.rfind(':');
            if (colon != std::string::npos) {
                // Lower weight for historical servers
                serverCounts[uid.substr(colon + 1)] += 1;
            }
        }
    }

    // Original: .apply { this[userHomeserver] = Int.MAX_VALUE }
    if (!myServer.empty()) serverCounts[myServer] = INT32_MAX;

    // Sort servers by count (descending)
    std::vector<std::pair<std::string, int>> sorted;
    for (const auto& [srv, count] : serverCounts) {
        sorted.push_back({srv, count});
    }
    std::sort(sorted.begin(), sorted.end(),
        [](const auto& a, const auto& b) { return a.second > b.second; });

    // Take top N (0 = all)
    std::vector<std::string> result;
    int limit = (maxServers > 0) ? maxServers : static_cast<int>(sorted.size());
    for (int i = 0; i < limit && i < static_cast<int>(sorted.size()); ++i) {
        result.push_back(sorted[i].first);
    }

    return result;
}

std::string formatViaParamsUrl(const std::vector<std::string>& viaServers) {
    if (viaServers.empty()) return "";

    std::ostringstream out;
    out << "?via=";
    for (size_t i = 0; i < viaServers.size(); ++i) {
        if (i > 0) out << "&via=";
        out << viaServers[i];  // URL-encoding could be added
    }
    return out.str();
}

} // namespace progressive
