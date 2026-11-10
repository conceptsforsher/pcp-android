#include "pcp/notice_event_formatter.hpp"
#include <string>
#include <vector>
#include <sstream>

namespace pcp {

// ---- JSON helpers ----

static std::string jsonEscape(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 2);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;
        }
    }
    return out;
}

static std::string jsonStr(const std::string& s) {
    return "\"" + jsonEscape(s) + "\"";
}

static std::string jsonObj(const std::string& key, const std::vector<std::string>& params) {
    std::string out = "{\"type\":\"string\",\"key\":" + jsonStr(key) + ",\"params\":[";
    for (size_t i = 0; i < params.size(); i++) {
        if (i > 0) out += ",";
        out += jsonStr(params[i]);
    }
    out += "]}";
    return out;
}

static std::string jsonNull() {
    return "{\"type\":\"null\"}";
}

// Simple JSON value extractor (no full parser needed)
static std::string getJsonString(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":\"";
    size_t pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos += search.size();
    size_t end = json.find('"', pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

static bool getJsonBool(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":true";
    return json.find(search) != std::string::npos;
}

static std::string getJsonType(const std::string& json) {
    return getJsonString(json, "type");
}

// ---- Event type constants (matching Matrix spec) ----

static const char* TYPE_ROOM_CREATE = "m.room.create";
static const char* TYPE_ROOM_NAME = "m.room.name";
static const char* TYPE_ROOM_TOPIC = "m.room.topic";
static const char* TYPE_ROOM_AVATAR = "m.room.avatar";
static const char* TYPE_ROOM_MEMBER = "m.room.member";
static const char* TYPE_ROOM_THIRD_PARTY_INVITE = "m.room.third_party_invite";
static const char* TYPE_ROOM_ALIASES = "m.room.aliases";
static const char* TYPE_ROOM_CANONICAL_ALIAS = "m.room.canonical_alias";
static const char* TYPE_ROOM_JOIN_RULES = "m.room.join_rules";
static const char* TYPE_ROOM_HISTORY_VISIBILITY = "m.room.history_visibility";
static const char* TYPE_ROOM_GUEST_ACCESS = "m.room.guest_access";
static const char* TYPE_ROOM_SERVER_ACL = "m.room.server_acl";
static const char* TYPE_ROOM_ENCRYPTION = "m.room.encryption";
static const char* TYPE_ROOM_TOMBSTONE = "m.room.tombstone";
static const char* TYPE_ROOM_POWER_LEVELS = "m.room.power_levels";
static const char* TYPE_CALL_INVITE = "m.call.invite";
static const char* TYPE_CALL_ANSWER = "m.call.answer";
static const char* TYPE_CALL_HANGUP = "m.call.hangup";
static const char* TYPE_CALL_REJECT = "m.call.reject";
static const char* TYPE_CALL_CANDIDATES = "m.call.candidates";

// ---- Content extractors ----

static std::string getContentField(const std::string& json, const std::string& field) {
    return getJsonString(json, field);
}

// ---- Main format function ----

std::string formatNoticeEvent(const std::string& eventJson,
                               const std::string& senderName,
                               bool isDm,
                               bool isSentByCurrentUser,
                               const std::string& currentUserId) {
    std::string type = getJsonType(eventJson);
    if (type.empty()) return jsonNull();
    
    auto s = [&](const std::string& key, const std::vector<std::string>& params) {
        return jsonObj(key, params);
    };
    auto isMe = [&]() { return isSentByCurrentUser; };
    
    // ---- Room Create ----
    if (type == TYPE_ROOM_CREATE) {
        std::string creator = getContentField(eventJson, "creator");
        if (creator.empty()) return jsonNull();
        if (isMe()) {
            return s(isDm ? "notice_direct_room_created_by_you" : "notice_room_created_by_you", {});
        } else {
            return s(isDm ? "notice_direct_room_created" : "notice_room_created", {creator});
        }
    }
    
    // ---- Room Name ----
    if (type == TYPE_ROOM_NAME) {
        std::string name = getContentField(eventJson, "name");
        if (name.empty()) {
            return s(isMe() ? "notice_room_name_removed_by_you" : "notice_room_name_removed", 
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_room_name_changed_by_you" : "notice_room_name_changed",
                     isMe() ? std::vector<std::string>{name} : std::vector<std::string>{senderName, name});
        }
    }
    
    // ---- Room Topic ----
    if (type == TYPE_ROOM_TOPIC) {
        std::string topic = getContentField(eventJson, "topic");
        if (topic.empty()) {
            return s(isMe() ? "notice_room_topic_removed_by_you" : "notice_room_topic_removed",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_room_topic_changed_by_you" : "notice_room_topic_changed",
                     isMe() ? std::vector<std::string>{topic} : std::vector<std::string>{senderName, topic});
        }
    }
    
    // ---- Room Avatar ----
    if (type == TYPE_ROOM_AVATAR) {
        std::string url = getContentField(eventJson, "avatarUrl");
        if (url.empty()) {
            return s(isMe() ? "notice_room_avatar_removed_by_you" : "notice_room_avatar_removed",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_room_avatar_changed_by_you" : "notice_room_avatar_changed",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        }
    }
    
    // ---- Room Tombstone ----
    if (type == TYPE_ROOM_TOMBSTONE) {
        if (isMe()) {
            return s(isDm ? "notice_direct_room_update_by_you" : "notice_room_update_by_you", {});
        } else {
            return s(isDm ? "notice_direct_room_update" : "notice_room_update", {senderName});
        }
    }
    
    // ---- Room Encryption ----
    if (type == TYPE_ROOM_ENCRYPTION) {
        std::string algorithm = getContentField(eventJson, "algorithm");
        if (algorithm.empty()) {
            return s(isMe() ? "notice_encryption_off_ok_by_you" : "notice_encryption_off_ok",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_encryption_on_ok_by_you" : "notice_encryption_on_ok",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        }
    }
    
    // ---- Call Events ----
    if (type == TYPE_CALL_INVITE) {
        bool isVideo = getJsonBool(eventJson, "isVideo");
        if (isVideo) {
            return s(isMe() ? "notice_placed_video_call_by_you" : "notice_placed_video_call",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_placed_voice_call_by_you" : "notice_placed_voice_call",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        }
    }
    
    if (type == TYPE_CALL_ANSWER) {
        return s(isMe() ? "notice_answered_call_by_you" : "notice_answered_call",
                 isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
    }
    
    if (type == TYPE_CALL_HANGUP) {
        return s(isMe() ? "notice_ended_call_by_you" : "notice_ended_call",
                 isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
    }
    
    if (type == TYPE_CALL_REJECT) {
        return s(isMe() ? "call_tile_you_declined_this_call" : "call_tile_other_declined",
                 isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
    }
    
    if (type == TYPE_CALL_CANDIDATES) {
        return s(isMe() ? "notice_call_candidates_by_you" : "notice_call_candidates",
                 isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
    }
    
    // ---- Room Member (simplified — full member logic is complex) ----
    if (type == TYPE_ROOM_MEMBER) {
        std::string membership = getContentField(eventJson, "membership");
        std::string prevMembership = getContentField(eventJson, "prevMembership");
        std::string displayName = getContentField(eventJson, "displayName");
        if (displayName.empty()) displayName = getContentField(eventJson, "userId");
        
        if (membership != prevMembership) {
            if (membership == "join") {
                if (isMe()) {
                    return s("notice_room_join_by_you", {});
                } else {
                    return s("notice_room_join", {senderName});
                }
            } else if (membership == "leave") {
                if (isMe()) {
                    return s("notice_room_leave_by_you", {});
                } else {
                    return s("notice_room_leave", {senderName});
                }
            } else if (membership == "invite") {
                if (isMe()) {
                    return s("notice_room_invite_by_you", {displayName});
                } else {
                    return s("notice_room_invite_you", {senderName});
                }
            } else if (membership == "ban") {
                if (isMe()) {
                    return s("notice_room_ban_by_you", {displayName});
                } else {
                    return s("notice_room_ban", {senderName, displayName});
                }
            }
        } else {
            // Profile change
            std::string newDisplayName = getContentField(eventJson, "newDisplayName");
            std::string newAvatarUrl = getContentField(eventJson, "newAvatarUrl");
            if (!newDisplayName.empty()) {
                return s(isMe() ? "notice_display_name_changed_by_you" : "notice_display_name_changed",
                         isMe() ? std::vector<std::string>{newDisplayName} : std::vector<std::string>{senderName, newDisplayName});
            }
            if (!newAvatarUrl.empty()) {
                return s(isMe() ? "notice_avatar_changed_by_you" : "notice_avatar_changed",
                         isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
            }
        }
    }
    
    // ---- Join Rules ----
    if (type == TYPE_ROOM_JOIN_RULES) {
        std::string rule = getContentField(eventJson, "joinRule");
        if (rule == "invite") {
            return s(isMe() ? "notice_room_join_rules_invite_by_you" : "notice_room_join_rules_invite",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_room_join_rules_public_by_you" : "notice_room_join_rules_public",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        }
    }
    
    // ---- Guest Access ----
    if (type == TYPE_ROOM_GUEST_ACCESS) {
        std::string access = getContentField(eventJson, "guestAccess");
        if (access == "can_join") {
            return s(isMe() ? "notice_guest_access_allowed_by_you" : "notice_guest_access_allowed",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        } else {
            return s(isMe() ? "notice_guest_access_forbidden_by_you" : "notice_guest_access_forbidden",
                     isMe() ? std::vector<std::string>{} : std::vector<std::string>{senderName});
        }
    }
    
    // ---- History Visibility ----
    if (type == TYPE_ROOM_HISTORY_VISIBILITY) {
        std::string vis = getContentField(eventJson, "historyVisibility");
        if (isMe()) {
            return s(isDm ? "notice_made_future_direct_room_visibility_by_you" : "notice_made_future_room_visibility_by_you", {vis});
        } else {
            return s(isDm ? "notice_made_future_direct_room_visibility" : "notice_made_future_room_visibility", {senderName, vis});
        }
    }
    
    // ---- Unsupported types — return null ----
    return jsonNull();
}

std::string formatRedactedEvent(const std::string& eventJson,
                                 const std::string& senderName) {
    return jsonObj("notice_redaction", {senderName});
}

std::string formatMemberNotice(const std::string& eventJson,
                                const std::string& senderName,
                                bool isDm,
                                bool isSentByCurrentUser) {
    // Delegate to main formatter (member events are handled there)
    return formatNoticeEvent(eventJson, senderName, isDm, isSentByCurrentUser, "");
}

std::string formatDisplayableEvent(const std::string& eventJson,
                                    const std::string& senderName,
                                    bool isDm,
                                    bool appendAuthor) {
    std::string type = getJsonType(eventJson);
    
    // For displayable events, return a simple description
    if (type == "m.room.message") {
        std::string msgType = getContentField(eventJson, "msgType");
        std::string body = getContentField(eventJson, "body");
        if (msgType == "m.text") return jsonObj("sent_message", {body});
        if (msgType == "m.image") return jsonObj("sent_an_image", {});
        if (msgType == "m.audio") return jsonObj("sent_an_audio_file", {});
        if (msgType == "m.video") return jsonObj("sent_a_video", {});
        if (msgType == "m.file") return jsonObj("sent_a_file", {});
        return jsonObj("sent_message", {body});
    }
    if (type == "m.sticker") return jsonObj("send_a_sticker", {});
    if (type == "m.reaction") return jsonObj("sent_a_reaction", {""});
    
    return jsonNull();
}

std::string formatThreadSummary(const std::string& eventJson,
                                 const std::string& latestEdition) {
    if (!latestEdition.empty()) {
        return jsonObj("thread_summary_edited", {latestEdition});
    }
    std::string type = getJsonType(eventJson);
    bool isRedacted = getJsonBool(eventJson, "redacted");
    if (isRedacted) {
        return jsonObj("notice_redaction", {});
    }
    // Return the body for messages
    std::string body = getContentField(eventJson, "body");
    if (!body.empty()) return jsonObj("thread_summary_text", {body});
    return jsonNull();
}

std::string formatEventDetails(const std::string& eventJson) {
    std::string type = getJsonType(eventJson);
    bool isImage = (type == "m.image" || getJsonBool(eventJson, "isImage"));
    bool isVideo = (type == "m.video" || getJsonBool(eventJson, "isVideo"));
    bool isAudio = (type == "m.audio" || getJsonBool(eventJson, "isAudio"));
    bool isFile = (type == "m.file" || getJsonBool(eventJson, "isFile"));
    bool isPoll = (type == "m.poll.start" || getJsonBool(eventJson, "isPoll"));
    bool isPollEnd = (type == "m.poll.end" || getJsonBool(eventJson, "isPollEnd"));
    
    if (isImage) {
        int w = 0, h = 0, sz = 0;
        try { w = std::stoi(getContentField(eventJson, "width")); } catch (...) {}
        try { h = std::stoi(getContentField(eventJson, "height")); } catch (...) {}
        try { sz = std::stoi(getContentField(eventJson, "size")); } catch (...) {}
        return jsonObj("event_details_image", {std::to_string(w), std::to_string(h), std::to_string(sz)});
    }
    if (isVideo) {
        int dur = 0, w = 0, h = 0, sz = 0;
        try { dur = std::stoi(getContentField(eventJson, "duration")); } catch (...) {}
        try { w = std::stoi(getContentField(eventJson, "width")); } catch (...) {}
        try { h = std::stoi(getContentField(eventJson, "height")); } catch (...) {}
        try { sz = std::stoi(getContentField(eventJson, "size")); } catch (...) {}
        return jsonObj("event_details_video", {std::to_string(dur), std::to_string(w), std::to_string(h), std::to_string(sz)});
    }
    if (isAudio) {
        int dur = 0, sz = 0;
        try { dur = std::stoi(getContentField(eventJson, "duration")); } catch (...) {}
        try { sz = std::stoi(getContentField(eventJson, "size")); } catch (...) {}
        return jsonObj("event_details_audio", {std::to_string(dur), std::to_string(sz)});
    }
    if (isFile) {
        int sz = 0;
        std::string mime = getContentField(eventJson, "mimeType");
        try { sz = std::stoi(getContentField(eventJson, "size")); } catch (...) {}
        return jsonObj("event_details_file", {std::to_string(sz), mime});
    }
    if (isPoll) return jsonObj("reply_to_poll_preview", {});
    if (isPollEnd) return jsonObj("reply_to_ended_poll_preview", {});
    return jsonNull();
}

std::string formatRoomHistoryVisibility(const std::string& visibility, bool forNotice) {
    if (forNotice) {
        if (visibility == "world_readable") return jsonObj("notice_room_visibility_world_readable", {});
        if (visibility == "shared") return jsonObj("notice_room_visibility_shared", {});
        if (visibility == "invited") return jsonObj("notice_room_visibility_invited", {});
        if (visibility == "joined") return jsonObj("notice_room_visibility_joined", {});
    } else {
        if (visibility == "world_readable") return jsonObj("room_settings_read_history_entry_anyone", {});
        if (visibility == "shared") return jsonObj("room_settings_read_history_entry_members_since", {});
        if (visibility == "invited") return jsonObj("room_settings_read_history_entry_members_only_invited", {});
        if (visibility == "joined") return jsonObj("room_settings_read_history_entry_members_only_joined", {});
    }
    return jsonNull();
}

} // namespace pcp
