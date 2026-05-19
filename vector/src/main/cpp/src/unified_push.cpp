#include "progressive/unified_push.hpp"
#include "progressive/http_client.hpp"
#include <sstream>

namespace progressive {

// ==== Discover Distributors ====
// On Android, this uses PackageManager to find installed distributor apps.
// The C++ layer provides the algorithm; the actual Android discovery
// is done via JNI callback.
//
// For now, returns known UnifiedPush distributors from a hardcoded list.

std::vector<UnifiedPushDistributor> discoverUnifiedPushDistributors() {
    std::vector<UnifiedPushDistributor> result;

    // Known UnifiedPush distributors (updated as ecosystem grows)
    struct KnownDistributor {
        const char* pkg; const char* name;
    };
    static const KnownDistributor known[] = {
        {"io.heckel.ntfy", "ntfy"},
        {"org.unifiedpush.distributor.noprovider2push", "NoProvider2Push"},
        {"org.unifiedpush.distributor.nextpush", "NextPush"},
        {"com.example.upfcmdistributor", "UP-FCM Distributor"},
        {"io.element.android.fcm", "Element FCM"},         // Element X own distributor
        {"org.matrix.distributor", "Matrix Push Distributor"},
    };

    for (const auto& k : known) {
        UnifiedPushDistributor d;
        d.packageName = k.pkg;
        d.displayName = k.name;
        d.isAvailable = false; // Requires Android PackageManager check
        result.push_back(d);
    }

    return result;
}

// ==== Registration ====
// Actual registration uses Android Intents, called from Kotlin layer.
// The C++ side handles the logic of constructing the registration request.

std::string registerUnifiedPushEndpoint(
    const std::string& /*distributorPackage*/,
    const std::string& connectorToken)
{
    // The actual registration involves Android Intents and is done in Kotlin.
    // C++ provides the token generation and endpoint validation logic.

    // In production, the endpoint is returned by the distributor.
    // For now, return a formatted placeholder demonstrating the format.
    return "up://" + connectorToken.substr(0, 8);
}

bool unregisterUnifiedPushEndpoint(const std::string& /*endpointUrl*/) {
    return true; // Stub — Android Intent in Kotlin layer
}

// ==== Pusher JSON ====

std::string buildPusherJson(const UnifiedPushPusherConfig& config) {
    std::ostringstream os;
    os << "{";
    os << R"("pushkey":")" << config.pushKey << R"(",)";
    os << R"("kind":"http",)";
    os << R"("app_id":")" << config.appId << R"(",)";
    os << R"("app_display_name":"Progressive Chat",)";
    os << R"("device_display_name":"Android Device",)";
    os << R"("lang":")" << config.lang << R"(",)";
    if (!config.profileTag.empty()) os << R"("profile_tag":")" << config.profileTag << R"(",)";
    os << R"("data":{"url":")" << config.pushKey << R"(","format":"event_id_only"},)";
    os << R"("append":false,)";
    os << R"("enabled":)" << (config.enabled ? "true" : "false");
    os << "}";
    return os.str();
}

// ==== Set Pusher ====

bool setUnifiedPushPusher(const UnifiedPushPusherConfig& config,
                          const std::string& homeserverUrl,
                          const std::string& accessToken) {
    std::string url = homeserverUrl;
    if (!url.empty() && url.back() == '/') url.pop_back();
    url += "/_matrix/client/r0/pushers/set";

    std::string body = buildPusherJson(config);

    std::unordered_map<std::string, std::string> headers;
    if (!accessToken.empty()) headers["Authorization"] = "Bearer " + accessToken;

    auto resp = httpPost(url, body, headers);
    return resp.isOk();
}

// ==== Parse UnifiedPush Message ====

UnifiedPushMessage parseUnifiedPushMessage(const std::string& json) {
    // Helper: extract JSON string value
    auto extractFrom = [](const std::string& src, const std::string& key) -> std::string {
        auto pos = src.find("\"" + key + "\"");
        if (pos == std::string::npos) return "";
        pos = src.find(':', pos);
        if (pos == std::string::npos) return "";
        pos++;
        while (pos < src.size() && (src[pos] == ' ' || src[pos] == '\t' || src[pos] == '"')) pos++;
        size_t end = pos;
        while (end < src.size() && src[end] != '"') { if (src[end] == '\\') end++; end++; }
        return src.substr(pos, end - pos);
    };

    UnifiedPushMessage msg;

    // Find the notification object inside message
    auto notifPos = json.find("\"notification\"");
    if (notifPos == std::string::npos) return msg;

    // Brace-count to extract the notification object
    notifPos = json.find('{', notifPos);
    if (notifPos == std::string::npos) return msg;
    int depth = 1;
    size_t start = notifPos;
    notifPos++;
    while (notifPos < json.size() && depth > 0) {
        if (json[notifPos] == '{') depth++;
        else if (json[notifPos] == '}') depth--;
        notifPos++;
    }
    std::string notifJson = json.substr(start, notifPos - start);

    msg.eventId = extractFrom(notifJson, "event_id");
    msg.roomId = extractFrom(notifJson, "room_id");
    msg.sender = extractFrom(notifJson, "sender");

    // Extract counts
    auto countsPos = notifJson.find("\"counts\"");
    if (countsPos != std::string::npos) {
        countsPos = notifJson.find('{', countsPos);
        if (countsPos != std::string::npos) {
            int d = 1;
            size_t cs = countsPos;
            countsPos++;
            while (countsPos < notifJson.size() && d > 0) {
                if (notifJson[countsPos] == '{') d++;
                else if (notifJson[countsPos] == '}') d--;
                countsPos++;
            }
            std::string countsJson = notifJson.substr(cs, countsPos - cs);
            // Parse unread count
            auto unreadPos = countsJson.find("\"unread\"");
            if (unreadPos != std::string::npos) {
                unreadPos = countsJson.find(':', unreadPos);
                if (unreadPos != std::string::npos) {
                    unreadPos++;
                    while (unreadPos < countsJson.size() && countsJson[unreadPos] == ' ') unreadPos++;
                    while (unreadPos < countsJson.size() && countsJson[unreadPos] >= '0' && countsJson[unreadPos] <= '9') {
                        msg.unreadCount = msg.unreadCount * 10 + (countsJson[unreadPos] - '0');
                        unreadPos++;
                    }
                }
            }
        }
    }

    msg.contentJson = extractFrom(notifJson, "content");
    msg.valid = !msg.eventId.empty() && !msg.roomId.empty();

    return msg;
}

bool isMatrixNotification(const UnifiedPushMessage& msg) {
    return msg.valid && !msg.eventId.empty();
}

// ==== State Serialization ====

std::string serializePushState(const std::string& endpointUrl,
                               const std::string& connectorToken) {
    return "{\"endpoint\":\"" + endpointUrl + "\",\"token\":\"" + connectorToken + "\"}";
}

bool parsePushState(const std::string& state,
                    std::string& endpointUrl,
                    std::string& connectorToken) {
    auto epPos = state.find("\"endpoint\"");
    if (epPos == std::string::npos) return false;
    epPos = state.find(':', epPos);
    if (epPos == std::string::npos) return false;
    epPos++;
    while (epPos < state.size() && (state[epPos] == ' ' || state[epPos] == '\t' || state[epPos] == '"')) epPos++;
    size_t end = epPos;
    while (end < state.size() && state[end] != '"') end++;
    endpointUrl = state.substr(epPos, end - epPos);

    auto tokPos = state.find("\"token\"");
    if (tokPos != std::string::npos) {
        tokPos = state.find(':', tokPos);
        if (tokPos != std::string::npos) {
            tokPos++;
            while (tokPos < state.size() && (state[tokPos] == ' ' || state[tokPos] == '\t' || state[tokPos] == '"')) tokPos++;
            end = tokPos;
            while (end < state.size() && state[end] != '"') end++;
            connectorToken = state.substr(tokPos, end - tokPos);
        }
    }

    return !endpointUrl.empty();
}

} // namespace progressive
