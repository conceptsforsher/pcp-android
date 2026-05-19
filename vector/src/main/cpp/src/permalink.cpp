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

std::string entityTypeToString(EntityType type) {
    switch (type) {
        case EntityType::ROOM:  return "room";
        case EntityType::EVENT: return "event";
        case EntityType::USER:  return "user";
        case EntityType::GROUP: return "group";
        default:               return "unknown";
    }
}

EntityType entityTypeFromId(const std::string& identifier) {
    if (identifier.empty()) return EntityType::UNKNOWN;
    if (identifier[0] == '!' || identifier[0] == '#') return EntityType::ROOM;
    if (identifier[0] == '@') return EntityType::USER;
    if (identifier[0] == '+') return EntityType::GROUP;
    return EntityType::UNKNOWN;
}

// ==== PermalinkBuilder static methods ====

// Original Kotlin: "https://matrix.to/#/" + roomIdOrAlias + via params
std::string PermalinkBuilder::buildRoomPermalink(const std::string& roomIdOrAlias,
    const std::vector<std::string>& viaServers) {
    std::string url = "https://matrix.to/#/" + roomIdOrAlias;
    if (!viaServers.empty()) {
        url += formatViaParamsUrl(viaServers);
    }
    return url;
}

// Original Kotlin: "https://matrix.to/#/" + roomId + "/" + eventId + via params
std::string PermalinkBuilder::buildEventPermalink(const std::string& roomId,
    const std::string& eventId, const std::vector<std::string>& viaServers) {
    std::string url = "https://matrix.to/#/" + roomId + "/" + eventId;
    if (!viaServers.empty()) {
        url += formatViaParamsUrl(viaServers);
    }
    return url;
}

std::string PermalinkBuilder::buildUserPermalink(const std::string& userId) {
    return "https://matrix.to/#/" + userId;
}

std::string PermalinkBuilder::buildGroupPermalink(const std::string& groupId) {
    return "https://matrix.to/#/" + groupId;
}

// Original Kotlin: room/event with thread context
std::string PermalinkBuilder::buildThreadPermalink(const std::string& roomId,
    const std::string& eventId, const std::string& threadRootId,
    const std::vector<std::string>& viaServers) {
    std::string url = "https://matrix.to/#/" + roomId + "/" + eventId;
    bool hasVia = !viaServers.empty();
    if (!threadRootId.empty()) {
        url += (hasVia ? "&" : "?") + std::string("thread_id=") + threadRootId;
    }
    if (hasVia) {
        if (threadRootId.empty()) {
            url += formatViaParamsUrl(viaServers);
        } else {
            for (const auto& via : viaServers) {
                url += "&via=" + via;
            }
        }
    }
    return url;
}

// Original Kotlin: MatrixToConverter — matrix:u/user, matrix:r/room, matrix:e/event
std::string PermalinkBuilder::buildMatrixSchemeLink(EntityType type, const std::string& id) {
    switch (type) {
        case EntityType::ROOM:  return "matrix:r/" + id;
        case EntityType::EVENT: return "matrix:e/" + id;
        case EntityType::USER:  return "matrix:u/" + id;
        case EntityType::GROUP: return "matrix:g/" + id;
        default:                return "matrix:r/" + id;
    }
}

std::string PermalinkBuilder::buildFromResult(const PermalinkResult& result) {
    if (!result.valid) return result.fullUrl;

    if (result.type == "user") {
        return buildUserPermalink(result.userId);
    }
    if (result.type == "room") {
        std::string id = result.isRoomAlias ? result.roomAlias : result.roomId;
        return buildRoomPermalink(id, result.viaParameters);
    }
    if (result.type == "event") {
        std::string url = buildEventPermalink(result.roomId, result.eventId, result.viaParameters);
        if (result.isThreadLink && !result.threadId.empty()) {
            url += "&thread_id=" + result.threadId;
        }
        return url;
    }
    return result.fullUrl;
}

std::string PermalinkBuilder::formatViaParamsUrl(const std::vector<std::string>& viaServers) {
    return progressive::formatViaParamsUrl(viaServers);
}

// ==== PermalinkParser static methods ====

// Original Kotlin (PermalinkParser.kt:45-88)
PermalinkResult PermalinkParser::parse(const std::string& url) {
    // Try matrix.to first, then matrix: scheme
    std::string prefix = "https://matrix.to/#/";
    if (url.rfind(prefix, 0) == 0) return parsePermalinkFull(url);
    if (url.rfind("matrix:", 0) == 0) return parseMatrixSchemeUri(url);
    return {};
}

PermalinkResult PermalinkParser::parsePermalink(const std::string& url) {
    return parse(url);
}

// Original Kotlin: parse matrix: scheme URIs
// matrix:u/@user:server        → user link
// matrix:r/!room:server        → room link
// matrix:r/#alias:server       → room alias link
// matrix:e/!room:server/$event → event link
PermalinkResult PermalinkParser::parseMatrixSchemeUri(const std::string& uri) {
    PermalinkResult result;
    result.fullUrl = uri;

    // Must start with matrix:
    if (uri.rfind("matrix:", 0) != 0) return result;

    // Extract scheme type: u, r, e, g
    auto colonPos = uri.find(':');
    if (colonPos == std::string::npos) return result;
    std::string scheme = uri.substr(colonPos + 1);

    if (scheme.size() < 2) return result;
    char typeChar = scheme[0];
    std::string id;

    // If next char is / then skip it
    if (scheme.size() > 1 && scheme[1] == '/') {
        id = scheme.substr(2);
    } else {
        id = scheme.substr(1);
    }

    // Parse query params
    auto queryPos = id.find('?');
    std::string cleanId = (queryPos != std::string::npos) ? id.substr(0, queryPos) : id;

    switch (typeChar) {
        case 'u':
            result.type = "user";
            result.userId = cleanId;
            result.entityType = EntityType::USER;
            result.valid = !cleanId.empty();
            break;
        case 'r': {
            result.entityType = EntityType::ROOM;
            if (!cleanId.empty() && cleanId[0] == '!') {
                auto slash = cleanId.find('/');
                if (slash != std::string::npos && slash + 1 < cleanId.size() && cleanId[slash + 1] == '$') {
                    result.type = "event";
                    result.roomId = cleanId.substr(0, slash);
                    result.eventId = cleanId.substr(slash + 1);
                } else {
                    result.type = "room";
                    result.roomId = cleanId;
                }
            } else if (!cleanId.empty() && cleanId[0] == '#') {
                result.type = "room";
                result.roomAlias = cleanId;
                result.isRoomAlias = true;
            }
            result.valid = !result.roomId.empty() || !result.roomAlias.empty();
            break;
        }
        case 'e':
            result.entityType = EntityType::EVENT;
            if (!cleanId.empty()) {
                auto slash = cleanId.find('/');
                if (slash != std::string::npos) {
                    result.type = "event";
                    result.roomId = cleanId.substr(0, slash);
                    result.eventId = cleanId.substr(slash + 1);
                } else {
                    result.type = "event";
                    result.eventId = cleanId;
                }
            }
            result.valid = !result.eventId.empty();
            break;
        case 'g':
            result.type = "group";
            result.entityType = EntityType::GROUP;
            result.valid = !cleanId.empty();
            break;
        default:
            break;
    }

    // Extract via and query params
    if (queryPos != std::string::npos) {
        result.viaParameters = extractViaParameters(id.substr(queryPos));
    }

    return result;
}

std::vector<std::string> PermalinkParser::extractViaParameters(const std::string& fragment) {
    return progressive::extractViaParameters(fragment);
}

// Original Kotlin: detect matrix.to or matrix: scheme URLs
bool PermalinkParser::isMatrixPermalink(const std::string& url) {
    return isMatrixToLink(url) || isMatrixSchemeLink(url);
}

bool PermalinkParser::isMatrixToLink(const std::string& url) {
    return url.rfind("https://matrix.to/#/", 0) == 0
        || url.rfind("http://matrix.to/#/", 0) == 0;
}

bool PermalinkParser::isMatrixSchemeLink(const std::string& url) {
    return url.rfind("matrix:", 0) == 0;
}

bool PermalinkParser::isEmailInviteLink(const std::string& url) {
    return progressive::isEmailInviteLink(url);
}

// ==== Free functions declared in hpp ====

bool isMatrixPermalink(const std::string& url) {
    return PermalinkParser::isMatrixPermalink(url);
}

// Original Kotlin: parse a matrix.to URI extracting entity type, IDs, and query params.
// This is the full parser that extracts all components including query params.
PermalinkResult parseMatrixToUri(const std::string& uri) {
    return PermalinkParser::parse(uri);
}

} // namespace progressive
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 