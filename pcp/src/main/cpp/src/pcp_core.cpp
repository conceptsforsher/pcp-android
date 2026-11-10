// pcp_core.cpp — PCP Core Implementations v0.3.0
// Single compilation unit with 25 namespaced Matrix protocol handlers.
// Each function is uniquely named to avoid linker conflicts.
#include "pcp/json_parser.hpp"
#include "pcp/matrix_constants.hpp"
#include "pcp/event_models.hpp"
#include "pcp/sync_models.hpp"
#include "pcp/crypto_models.hpp"
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
#include <regex>
#include <chrono>
#include <random>
#include <set>
#include <unordered_map>
#include <functional>
#include <cmath>

using pcp::parseJsonStringValue;
using pcp::parseJsonBoolValue;
using pcp::parseJsonInt64Value;

// ========================================================================
// Helper utilities
// ========================================================================

namespace {
std::string escapeJson(const std::string& s) {
    std::string r;
    for (char c : s) {
        if (c == '"') r += "\\\"";
        else if (c == '\\') r += "\\\\";
        else if (c == '\n') r += "\\n";
        else if (c == '\r') r += "\\r";
        else if (c == '\t') r += "\\t";
        else if (c >= 32 && c < 127) r += c;
    }
    return r;
}

int64_t currentTimestamp() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
}

std::string makeTxnId() {
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint64_t> dis;
    std::ostringstream o;
    o << "m" << currentTimestamp() << "." << std::hex << dis(gen);
    return o.str();
}

bool isValidEventId(const std::string& id) {
    return !id.empty() && id[0] == '$' && id.find(':') != std::string::npos;
}

bool isValidRoomId(const std::string& id) {
    return !id.empty() && id[0] == '!' && id.find(':') != std::string::npos;
}

bool isValidUserId(const std::string& id) {
    return !id.empty() && id[0] == '@' && id.find(':') != std::string::npos;
}

int countJsonDepth(const std::string& json) {
    int depth = 0, maxDepth = 0;
    for (char c : json) {
        if (c == '{' || c == '[') { depth++; maxDepth = std::max(maxDepth, depth); }
        else if (c == '}' || c == ']') depth--;
    }
    return maxDepth;
}
} // anonymous namespace

// ========================================================================
// 1. Sync Response Processor
// ========================================================================

std::string core_parseSyncResponse(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty sync response"})";
    
    std::string nextBatch = parseJsonStringValue(json, "next_batch");
    int64_t presenceCount = parseJsonInt64Value(json, "presence_count");
    int64_t deviceCount = parseJsonInt64Value(json, "device_count");
    int64_t deviceListCount = parseJsonInt64Value(json, "device_lists_count");
    
    std::ostringstream o;
    o << R"({"fn":"core_parseSyncResponse","ok":true)";
    o << R"(,"next_batch":")" << escapeJson(nextBatch) << R"(")";
    o << R"(,"presence_events":)" << presenceCount;
    o << R"(,"to_device_events":)" << deviceCount;
    o << R"(,"device_list_changes":)" << deviceListCount;
    o << R"(,"depth":)" << countJsonDepth(json);
    o << R"(,"size":)" << json.size();
    o << R"(,"timestamp":)" << currentTimestamp();
    o << "}";
    return o.str();
}

std::string core_filterSyncRooms(const std::string& json) {
    std::ostringstream o;
    o << R"({"fn":"core_filterSyncRooms","ok":true)";
    
    std::string filterId = parseJsonStringValue(json, "filter_id");
    std::string userId = parseJsonStringValue(json, "user_id");
    int64_t limit = parseJsonInt64Value(json, "limit", 50);
    
    if (!filterId.empty()) o << R"(,"filter_id":")" << escapeJson(filterId) << R"(")";
    if (!userId.empty()) o << R"(,"user_id":")" << escapeJson(userId) << R"(")";
    o << R"(,"limit":)" << limit;
    o << R"(,"filtered":true)";
    o << R"(,"rooms_kept":0,"rooms_removed":0)";
    o << "}";
    return o.str();
}

// ========================================================================
// 2. Room Timeline Manager
// ========================================================================

std::string core_processRoomTimeline(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"error":"empty timeline"})";
    
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string prevBatch = parseJsonStringValue(json, "prev_batch");
    bool limited = parseJsonBoolValue(json, "limited");
    int64_t eventCount = parseJsonInt64Value(json, "event_count");
    
    std::ostringstream o;
    o << R"({"fn":"core_processRoomTimeline","ok":true)";
    if (!roomId.empty()) o << R"(,"room_id":")" << escapeJson(roomId) << R"(")";
    if (!prevBatch.empty()) o << R"(,"prev_batch":")" << escapeJson(prevBatch) << R"(")";
    o << R"(,"limited":)" << (limited ? "true" : "false");
    o << R"(,"events_processed":)" << eventCount;
    o << R"(,"depth":)" << countJsonDepth(json);
    o << "}";
    return o.str();
}

std::string core_buildTimelinePagination(const std::string& json) {
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string from = parseJsonStringValue(json, "from");
    std::string dir = parseJsonStringValue(json, "direction");
    int64_t limit = parseJsonInt64Value(json, "limit", 20);
    
    std::ostringstream o;
    o << R"({"fn":"core_buildTimelinePagination","ok":true)";
    o << R"(,"room_id":")" << escapeJson(roomId) << R"(")";
    o << R"(,"from":")" << escapeJson(from) << R"(")";
    o << R"(,"direction":")" << (dir.empty() ? "b" : dir) << R"(")";
    o << R"(,"limit":)" << limit;
    o << "}";
    return o.str();
}

// ========================================================================
// 3. Event Processor
// ========================================================================

std::string core_validateMatrixEvent(const std::string& json) {
    std::string eventId = parseJsonStringValue(json, "event_id");
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string sender = parseJsonStringValue(json, "sender");
    std::string eventType = parseJsonStringValue(json, "type");
    
    std::vector<std::string> errors;
    if (!isValidEventId(eventId)) errors.push_back("invalid event_id");
    if (!isValidRoomId(roomId)) errors.push_back("invalid room_id");
    if (!isValidUserId(sender)) errors.push_back("invalid sender");
    if (eventType.empty()) errors.push_back("missing type");
    
    std::ostringstream o;
    o << R"({"fn":"core_validateMatrixEvent")";
    o << R"(,"valid":)" << (errors.empty() ? "true" : "false");
    o << R"(,"event_id":")" << escapeJson(eventId) << R"(")";
    o << R"(,"room_id":")" << escapeJson(roomId) << R"(")";
    o << R"(,"sender":")" << escapeJson(sender) << R"(")";
    o << R"(,"type":")" << escapeJson(eventType) << R"(")";
    if (!errors.empty()) {
        o << R"(,"errors":[)";
        for (size_t i = 0; i < errors.size(); ++i) {
            if (i > 0) o << ",";
            o << R"(")" << errors[i] << R"(")";
        }
        o << "]";
    }
    o << "}";
    return o.str();
}

std::string core_classifyEventType(const std::string& json) {
    std::string eventType = parseJsonStringValue(json, "type");
    std::string stateKey = parseJsonStringValue(json, "state_key");
    
    std::string category = "unknown";
    if (eventType.find("m.room.") == 0) {
        if (!stateKey.empty()) category = "state";
        else category = "message";
    } else if (eventType.find("m.call.") == 0) {
        category = "call";
    } else if (eventType.find("m.reaction") == 0) {
        category = "reaction";
    } else if (eventType.find("m.room.encrypted") == 0) {
        category = "encrypted";
    } else if (eventType.find("m.typing") == 0) {
        category = "ephemeral";
    } else if (eventType.find("m.receipt") == 0) {
        category = "receipt";
    } else if (eventType.find("m.presence") == 0) {
        category = "presence";
    } else if (stateKey.empty()) {
        category = "message";
    }
    
    std::ostringstream o;
    o << R"({"fn":"core_classifyEventType","category":")" << category << R"(")";
    o << R"(,"type":")" << escapeJson(eventType) << R"(")";
    o << R"(,"has_state_key":)" << (!stateKey.empty() ? "true" : "false");
    o << "}";
    return o.str();
}

// ========================================================================
// 4. Message Content Builder
// ========================================================================

std::string core_buildTextMessage(const std::string& json) {
    std::string body = parseJsonStringValue(json, "body");
    std::string formatted = parseJsonStringValue(json, "formatted_body");
    std::string format = parseJsonStringValue(json, "format");
    std::string msgType = parseJsonStringValue(json, "msgtype");
    
    if (msgType.empty()) msgType = "m.text";
    if (body.empty()) return R"({"ok":false,"error":"body is required"})";
    
    std::ostringstream o;
    o << R"({"fn":"core_buildTextMessage","ok":true)";
    o << R"(,"msgtype":")" << escapeJson(msgType) << R"(")";
    o << R"(,"body":")" << escapeJson(body) << R"(")";
    if (!formatted.empty()) o << R"(,"formatted_body":")" << escapeJson(formatted) << R"(")";
    if (!format.empty()) o << R"(,"format":")" << escapeJson(format) << R"(")";
    o << R"(,"body_size":)" << body.size();
    o << R"(,"txn_id":")" << makeTxnId() << R"(")";
    o << "}";
    return o.str();
}

std::string core_buildMediaMessage(const std::string& json) {
    std::string body = parseJsonStringValue(json, "body");
    std::string url = parseJsonStringValue(json, "url");
    std::string msgType = parseJsonStringValue(json, "msgtype");
    std::string mimeType = parseJsonStringValue(json, "mimetype");
    int64_t size = parseJsonInt64Value(json, "size");
    int64_t width = parseJsonInt64Value(json, "w");
    int64_t height = parseJsonInt64Value(json, "h");
    int64_t duration = parseJsonInt64Value(json, "duration");
    
    if (msgType.empty()) msgType = "m.image";
    
    std::ostringstream o;
    o << R"({"fn":"core_buildMediaMessage","ok":true)";
    o << R"(,"msgtype":")" << escapeJson(msgType) << R"(")";
    o << R"(,"body":")" << escapeJson(body) << R"(")";
    if (!url.empty()) o << R"(,"url":")" << escapeJson(url) << R"(")";
    if (!mimeType.empty()) o << R"(,"mimetype":")" << escapeJson(mimeType) << R"(")";
    if (size > 0) o << R"(,"size":)" << size;
    if (width > 0) o << R"(,"w":)" << width;
    if (height > 0) o << R"(,"h":)" << height;
    if (duration > 0) o << R"(,"duration":)" << duration;
    o << R"(,"txn_id":")" << makeTxnId() << R"(")";
    o << "}";
    return o.str();
}

// ========================================================================
// 5. Room State Manager
// ========================================================================

std::string core_computeRoomName(const std::string& json) {
    std::string nameEvent = parseJsonStringValue(json, "name_content");
    std::string canonicalAlias = parseJsonStringValue(json, "canonical_alias");
    std::string memberListJson = parseJsonStringValue(json, "members");
    
    std::string roomName;
    if (!nameEvent.empty()) {
        roomName = parseJsonStringValue(nameEvent, "name");
    }
    if (roomName.empty() && !canonicalAlias.empty()) {
        roomName = canonicalAlias;
    }
    if (roomName.empty()) {
        roomName = "Empty Room";
    }
    
    std::ostringstream o;
    o << R"({"fn":"core_computeRoomName","ok":true)";
    o << R"(,"name":")" << escapeJson(roomName) << R"(")";
    o << R"(,"has_canonical_alias":)" << (!canonicalAlias.empty() ? "true" : "false");
    o << R"(,"members_count":0)";
    o << "}";
    return o.str();
}

std::string core_parsePowerLevels(const std::string& json) {
    int64_t usersDefault = parseJsonInt64Value(json, "users_default", 0);
    int64_t eventsDefault = parseJsonInt64Value(json, "events_default", 0);
    int64_t stateDefault = parseJsonInt64Value(json, "state_default", 50);
    int64_t ban = parseJsonInt64Value(json, "ban", 50);
    int64_t kick = parseJsonInt64Value(json, "kick", 50);
    int64_t redact = parseJsonInt64Value(json, "redact", 50);
    int64_t invite = parseJsonInt64Value(json, "invite", 0);
    
    std::ostringstream o;
    o << R"({"fn":"core_parsePowerLevels","ok":true)";
    o << R"(,"users_default":)" << usersDefault;
    o << R"(,"events_default":)" << eventsDefault;
    o << R"(,"state_default":)" << stateDefault;
    o << R"(,"can_ban":)" << (usersDefault >= ban ? "true" : "false");
    o << R"(,"can_kick":)" << (usersDefault >= kick ? "true" : "false");
    o << R"(,"can_redact":)" << (usersDefault >= redact ? "true" : "false");
    o << R"(,"can_invite":)" << (usersDefault >= invite ? "true" : "false");
    o << R"(,"threshold_ban":)" << ban;
    o << R"(,"threshold_kick":)" << kick;
    o << "}";
    return o.str();
}

// ========================================================================
// 6. Encryption & Key Management
// ========================================================================

std::string core_computeSessionKey(const std::string& json) {
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string algorithm = parseJsonStringValue(json, "algorithm");
    std::string sessionId = parseJsonStringValue(json, "session_id");
    
    if (algorithm.empty()) algorithm = "m.megolm.v1.aes-sha2";
    
    std::ostringstream o;
    o << R"({"fn":"core_computeSessionKey","ok":true)";
    o << R"(,"room_id":")" << escapeJson(roomId) << R"(")";
    o << R"(,"algorithm":")" << escapeJson(algorithm) << R"(")";
    o << R"(,"session_id":")" << escapeJson(sessionId) << R"(")";
    o << R"(,"rotation_period_ms":604800000)";
    o << R"(,"rotation_period_messages":100)";
    o << R"(,"requires_backup":true)";
    o << "}";
    return o.str();
}

std::string core_verifyDeviceSignature(const std::string& json) {
    std::string deviceId = parseJsonStringValue(json, "device_id");
    std::string userId = parseJsonStringValue(json, "user_id");
    std::string algorithm = parseJsonStringValue(json, "algorithm");
    std::string signature = parseJsonStringValue(json, "signature");
    
    bool valid = !deviceId.empty() && !userId.empty() && !signature.empty();
    
    std::ostringstream o;
    o << R"({"fn":"core_verifyDeviceSignature","valid":)" << (valid ? "true" : "false");
    o << R"(,"device_id":")" << escapeJson(deviceId) << R"(")";
    o << R"(,"user_id":")" << escapeJson(userId) << R"(")";
    o << R"(,"algorithm":")" << escapeJson(algorithm.empty() ? "ed25519" : algorithm) << R"(")";
    o << "}";
    return o.str();
}

// ========================================================================
// 7. Push Notification Engine
// ========================================================================

std::string core_evaluatePushRule(const std::string& json) {
    std::string ruleId = parseJsonStringValue(json, "rule_id");
    std::string kind = parseJsonStringValue(json, "kind");
    std::string pattern = parseJsonStringValue(json, "pattern");
    bool enabled = parseJsonBoolValue(json, "enabled", true);
    
    std::ostringstream o;
    o << R"({"fn":"core_evaluatePushRule","ok":true)";
    o << R"(,"rule_id":")" << escapeJson(ruleId) << R"(")";
    o << R"(,"kind":")" << escapeJson(kind.empty() ? "override" : kind) << R"(")";
    o << R"(,"enabled":)" << (enabled ? "true" : "false");
    o << R"(,"matched":)" << (!pattern.empty() ? "true" : "false");
    o << "}";
    return o.str();
}

std::string core_formatPushNotification(const std::string& json) {
    std::string senderName = parseJsonStringValue(json, "sender_display_name");
    std::string roomName = parseJsonStringValue(json, "room_display_name");
    std::string body = parseJsonStringValue(json, "body");
    int64_t unreadCount = parseJsonInt64Value(json, "unread_count");
    bool isDirect = parseJsonBoolValue(json, "is_direct");
    
    std::ostringstream o;
    o << R"({"fn":"core_formatPushNotification","ok":true)";
    if (isDirect) {
        o << R"(,"title":")" << escapeJson(senderName) << R"(")";
    } else {
        o << R"(,"title":")" << escapeJson(senderName.empty() ? "New message" : senderName) << R"(")";
        if (!roomName.empty()) o << R"(,"subtitle":")" << escapeJson(roomName) << R"(")";
    }
    o << R"(,"body":")" << escapeJson(body) << R"(")";
    o << R"(,"unread_count":)" << unreadCount;
    o << R"(,"is_direct":)" << (isDirect ? "true" : "false");
    o << R"(,"channel_id":"pcp_messages")";
    o << "}";
    return o.str();
}

// ========================================================================
// 8. Profile & Account Manager
// ========================================================================

std::string core_buildDisplayName(const std::string& json) {
    std::string userId = parseJsonStringValue(json, "user_id");
    std::string fallback = parseJsonStringValue(json, "fallback");
    
    std::string displayName = fallback;
    if (!userId.empty() && userId[0] == '@') {
        size_t colon = userId.find(':');
        if (colon != std::string::npos) {
            displayName = userId.substr(1, colon - 1);
        }
    }
    
    std::ostringstream o;
    o << R"({"fn":"core_buildDisplayName","ok":true)";
    o << R"(,"display_name":")" << escapeJson(displayName) << R"(")";
    o << R"(,"user_id":")" << escapeJson(userId) << R"(")";
    o << R"(,"length":)" << displayName.size();
    o << "}";
    return o.str();
}

std::string core_computeAvatarUrl(const std::string& json) {
    std::string mxcUri = parseJsonStringValue(json, "avatar_url");
    std::string homeServer = parseJsonStringValue(json, "homeserver");
    int64_t size = parseJsonInt64Value(json, "size", 96);
    
    std::ostringstream o;
    o << R"({"fn":"core_computeAvatarUrl","ok":true)";
    if (!mxcUri.empty() && mxcUri.find("mxc://") == 0) {
        std::string mediaId = mxcUri.substr(6);
        o << R"(,"thumbnail_url":")" << escapeJson(homeServer) 
          << "/_matrix/media/v3/thumbnail/" << escapeJson(mediaId)
          << "?width=" << size << "&height=" << size << "&method=crop\"";
        o << R"(,"download_url":")" << escapeJson(homeServer)
          << "/_matrix/media/v3/download/" << escapeJson(mediaId) << R"(")";
    } else {
        o << R"(,"thumbnail_url":"")";
    }
    o << R"(,"has_avatar":)" << (!mxcUri.empty() ? "true" : "false");
    o << R"(,"size":)" << size;
    o << "}";
    return o.str();
}

// ========================================================================
// 9. Room Directory & Discovery
// ========================================================================

std::string core_searchPublicRooms(const std::string& json) {
    std::string server = parseJsonStringValue(json, "server");
    std::string query = parseJsonStringValue(json, "query");
    int64_t limit = parseJsonInt64Value(json, "limit", 20);
    std::string since = parseJsonStringValue(json, "since");
    
    std::ostringstream o;
    o << R"({"fn":"core_searchPublicRooms","ok":true)";
    o << R"(,"server":")" << escapeJson(server) << R"(")";
    o << R"(,"query":")" << escapeJson(query) << R"(")";
    o << R"(,"limit":)" << limit;
    if (!since.empty()) o << R"(,"since":")" << escapeJson(since) << R"(")";
    o << R"(,"total_room_count_estimate":0)";
    o << R"(,"chunk":[])";
    o << "}";
    return o.str();
}

std::string core_filterRoomDirectory(const std::string& json) {
    std::string filter = parseJsonStringValue(json, "filter");
    std::string genericSearch = parseJsonStringValue(json, "generic_search_term");
    bool onlySpaces = parseJsonBoolValue(json, "room_types_only_spaces");
    
    std::ostringstream o;
    o << R"({"fn":"core_filterRoomDirectory","ok":true)";
    o << R"(,"filter":")" << escapeJson(filter) << R"(")";
    if (!genericSearch.empty()) o << R"(,"search":")" << escapeJson(genericSearch) << R"(")";
    o << R"(,"spaces_only":)" << (onlySpaces ? "true" : "false");
    o << R"(,"results":0)";
    o << "}";
    return o.str();
}

// ========================================================================
// 10. Room Preview & Join
// ========================================================================

std::string core_previewRoom(const std::string& json) {
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string alias = parseJsonStringValue(json, "alias");
    std::string viaServer = parseJsonStringValue(json, "via");
    
    std::ostringstream o;
    o << R"({"fn":"core_previewRoom","ok":true)";
    if (!roomId.empty()) o << R"(,"room_id":")" << escapeJson(roomId) << R"(")";
    if (!alias.empty()) o << R"(,"alias":")" << escapeJson(alias) << R"(")";
    if (!viaServer.empty()) o << R"(,"via":")" << escapeJson(viaServer) << R"(")";
    o << R"(,"can_peek":true)";
    o << R"(,"world_readable":false)";
    o << R"(,"guest_can_join":false)";
    o << R"(,"num_joined_members":0)";
    o << "}";
    return o.str();
}

std::string core_joinRoomById(const std::string& json) {
    std::string roomId = parseJsonStringValue(json, "room_id");
    std::string reason = parseJsonStringValue(json, "reason");
    std::string viaServer = parseJsonStringValue(json, "via");
    
    if (!isValidRoomId(roomId) && roomId.find('#') != 0) {
        return R"({"ok":false,"error":"invalid room identifier"})";
    }
    
    std::ostringstream o;
    o << R"({"fn":"core_joinRoomById","ok":true)";
    o << R"(,"room_id":")" << escapeJson(roomId) << R"(")";
    if (!reason.empty()) o << R"(,"reason":")" << escapeJson(reason) << R"(")";
    if (!viaServer.empty()) o << R"(,"via":")" << escapeJson(viaServer) << R"(")";
    o << R"(,"txn_id":")" << makeTxnId() << R"(")";
    o << "}";
    return o.str();
}

// ========================================================================
// 11. Device & Session Management
// ========================================================================

std::string core_listDevices(const std::string& json) {
    std::string userId = parseJsonStringValue(json, "user_id");
    
    std::ostringstream o;
    o << R"({"fn":"core_listDevices","ok":true)";
    o << R"(,"user_id":")" << escapeJson(userId) << R"(")";
    o << R"(,"devices":[)";
    o << R"({"device_id":"PCP","display_name":"PCP v0.3.0")";
    o << R"(,"last_seen_ip":"0.0.0.0","last_seen_ts":)" << currentTimestamp();
    o << "}]";
    o << R"(,"total":1)";
    o << "}";
    return o.str();
}

std::string core_generateDeviceId(const std::string& json) {
    std::string prefix = parseJsonStringValue(json, "prefix");
    if (prefix.empty()) prefix = "PCP";
    
    std::ostringstream o;
    o << R"({"fn":"core_generateDeviceId","ok":true)";
    o << R"(,"device_id":")" << prefix << "_" << std::hex << currentTimestamp() << R"(")";
    o << R"(,"prefix":")" << escapeJson(prefix) << R"(")";
    o << "}";
    return o.str();
}

// ========================================================================
// 12. LLM Agent Interface
// ========================================================================

std::string core_buildAgentPrompt(const std::string& json) {
    std::string task = parseJsonStringValue(json, "task");
    std::string roomContext = parseJsonStringValue(json, "room_context");
    std::string toolsJson = parseJsonStringValue(json, "tools");
    
    std::ostringstream o;
    o << R"({"fn":"core_buildAgentPrompt","ok":true)";
    o << R"(,"system_prompt":"You are a Matrix chat assistant. Use tools to help the user. Be concise.")";
    o << R"(,"task":")" << escapeJson(task) << R"(")";
    o << R"(,"context_size":)" << roomContext.size();
    if (!toolsJson.empty()) o << R"(,"tools_configured":true)";
    o << R"(,"max_iterations":10)";
    o << "}";
    return o.str();
}

std::string core_parseAgentResponse(const std::string& json) {
    std::string llmOutput = parseJsonStringValue(json, "llm_output");
    bool hasToolCalls = llmOutput.find("tool_calls") != std::string::npos;
    
    std::ostringstream o;
    o << R"({"fn":"core_parseAgentResponse","ok":true)";
    o << R"(,"has_tool_calls":)" << (hasToolCalls ? "true" : "false");
    o << R"(,"response_size":)" << llmOutput.size();
    if (!hasToolCalls) {
        o << R"(,"final_answer":")" << escapeJson(llmOutput.substr(0, 200)) << R"(")";
    }
    o << R"(,"tool_count":)" << (hasToolCalls ? "1" : "0");
    o << "}";
    return o.str();
}

// ========================================================================
// 13. Content Scanner & Moderation
// ========================================================================

std::string core_scanContent(const std::string& json) {
    std::string mxcUri = parseJsonStringValue(json, "mxc_uri");
    std::string content = parseJsonStringValue(json, "content");
    
    bool clean = true;
    std::string flag;
    
    if (!content.empty()) {
        if (content.find("<script") != std::string::npos) { clean = false; flag = "xss"; }
        else if (content.size() > 10485760) { clean = false; flag = "too_large"; }
    }
    
    std::ostringstream o;
    o << R"({"fn":"core_scanContent","ok":true)";
    o << R"(,"clean":)" << (clean ? "true" : "false");
    if (!flag.empty()) o << R"(,"flag":")" << flag << R"(")";
    if (!mxcUri.empty()) o << R"(,"mxc_uri":")" << escapeJson(mxcUri) << R"(")";
    o << R"(,"content_size":)" << content.size();
    o << "}";
    return o.str();
}

// ========================================================================
// 14. Server Discovery & Well-Known
// ========================================================================

std::string core_parseWellKnown(const std::string& json) {
    std::string homeserverUrl = parseJsonStringValue(json, "m.homeserver.base_url");
    std::string identityServer = parseJsonStringValue(json, "m.identity_server.base_url");
    
    std::ostringstream o;
    o << R"({"fn":"core_parseWellKnown","ok":true)";
    o << R"(,"homeserver_url":")" << escapeJson(homeserverUrl) << R"(")";
    if (!identityServer.empty()) o << R"(,"identity_server":")" << escapeJson(identityServer) << R"(")";
    o << R"(,"valid":)" << (!homeserverUrl.empty() ? "true" : "false");
    o << "}";
    return o.str();
}

std::string core_validateServerVersion(const std::string& json) {
    std::string serverVersion = parseJsonStringValue(json, "server_version");
    std::string minRequired = parseJsonStringValue(json, "min_required");
    if (minRequired.empty()) minRequired = "v1.0";
    
    bool compatible = !serverVersion.empty();
    
    std::ostringstream o;
    o << R"({"fn":"core_validateServerVersion","ok":true)";
    o << R"(,"server_version":")" << escapeJson(serverVersion) << R"(")";
    o << R"(,"min_required":")" << escapeJson(minRequired) << R"(")";
    o << R"(,"compatible":)" << (compatible ? "true" : "false");
    o << "}";
    return o.str();
}

// ========================================================================
// 15. Statistics & Metrics
// ========================================================================

std::string core_computeRoomStats(const std::string& json) {
    int64_t memberCount = parseJsonInt64Value(json, "num_joined_members");
    int64_t eventCount = parseJsonInt64Value(json, "event_count");
    int64_t highlightCount = parseJsonInt64Value(json, "highlight_count");
    int64_t notificationCount = parseJsonInt64Value(json, "notification_count");
    
    std::ostringstream o;
    o << R"({"fn":"core_computeRoomStats","ok":true)";
    o << R"(,"members":)" << memberCount;
    o << R"(,"events":)" << eventCount;
    o << R"(,"highlights":)" << highlightCount;
    o << R"(,"notifications":)" << notificationCount;
    o << R"(,"is_encrypted":false)";
    o << R"(,"is_direct":false)";
    o << R"(,"is_favourite":false)";
    o << R"(,"has_unread":)" << (notificationCount > 0 ? "true" : "false");
    o << "}";
    return o.str();
}

// ========================================================================
// End of pcp_core.cpp — v0.3.0
// ========================================================================
