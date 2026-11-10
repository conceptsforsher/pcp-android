#include "pcp/tdlib_parser.hpp"
#include <android/log.h>

#define TAG "TdParser"

namespace pcp {

static ContentType classifyContent(const std::string& type) {
    if (type.find("Text") != std::string::npos) return ContentType::TEXT;
    if (type.find("Photo") != std::string::npos) return ContentType::PHOTO;
    if (type.find("Video") != std::string::npos) return ContentType::VIDEO;
    if (type.find("Voice") != std::string::npos) return ContentType::VOICE;
    if (type.find("VideoNote") != std::string::npos) return ContentType::VIDEO_NOTE;
    if (type.find("Animation") != std::string::npos) return ContentType::ANIMATION;
    if (type.find("Sticker") != std::string::npos) return ContentType::STICKER;
    if (type.find("Document") != std::string::npos) return ContentType::DOCUMENT;
    if (type.find("Poll") != std::string::npos) return ContentType::POLL;
    if (type.find("Location") != std::string::npos) return ContentType::LOCATION;
    if (type.find("Contact") != std::string::npos) return ContentType::CONTACT;
    if (type.find("Dice") != std::string::npos) return ContentType::DICE;
    if (type.find("Game") != std::string::npos) return ContentType::GAME;
    if (type.find("Call") != std::string::npos) return ContentType::CALL;
    if (type.find("messageChat") != std::string::npos) return ContentType::SYSTEM;
    return ContentType::UNKNOWN;
}

TdUser TdParser::parseUser(const TlObject& json) {
    TdUser u;
    u.id = json.optLong("id", 0);
    u.firstName = json.optString("first_name", "");
    u.lastName = json.optString("last_name", "");
    u.username = json.optString("username", "");
    u.phoneNumber = json.optString("phone_number", "");
    u.isPremium = json.optBool("is_premium", false);
    u.isVerified = json.optBool("is_verified", false);
    u.isContact = json.optBool("is_contact", false);
    u.isMutualContact = json.optBool("is_mutual_contact", false);

    TlObject typeObj = json.optObject("type");
    u.isBot = typeObj.type.find("Bot") != std::string::npos;

    TlObject status = json.optObject("status");
    if (status.type == "userStatusOnline") u.status = UserStatus::ONLINE;
    else if (status.type == "userStatusRecently") u.status = UserStatus::RECENTLY;
    else if (status.type == "userStatusLastWeek") u.status = UserStatus::LAST_WEEK;
    else if (status.type == "userStatusLastMonth") u.status = UserStatus::LAST_MONTH;
    else if (status.type == "userStatusOffline") {
        u.status = UserStatus::OFFLINE;
        u.wasOnline = status.optInt("was_online", 0);
    }

    TlObject emoji = json.optObject("emoji_status");
    if (!emoji.type.empty()) u.emojiStatusId = emoji.optLong("custom_emoji_id", 0);

    return u;
}

TdChat TdParser::parseChat(const TlObject& json) {
    TdChat c;
    c.id = json.optLong("id", 0);
    c.title = json.optString("title", "");

    TlObject typeObj = json.optObject("type");
    c.type = typeObj.type;
    if (c.type.find("Group") != std::string::npos) c.type = "group";
    else if (c.type.find("Supergroup") != std::string::npos) c.type = "supergroup";
    else if (c.type.find("Channel") != std::string::npos) c.type = "channel";
    else if (c.type.find("Secret") != std::string::npos) c.type = "secret";
    else c.type = "private";

    c.unreadCount = json.optInt("unread_count", 0);
    c.unreadMentionCount = json.optInt("unread_mention_count", 0);
    c.isPinned = json.optBool("is_pinned", false);
    c.isMarkedUnread = json.optBool("is_marked_as_unread", false);

    TlObject lastMsg = json.optObject("last_message");
    if (!lastMsg.type.empty()) {
        c.lastMessageId = lastMsg.optLong("id", 0);
        TlObject content = lastMsg.optObject("content");
        TlObject textObj = content.optObject("text");
        c.lastMessageText = textObj.optString("text", content.optString("text", ""));
        if (c.lastMessageText.empty()) {
            auto ct = classifyContent(content.type);
            switch (ct) {
                case ContentType::PHOTO: c.lastMessageText = "[Photo]"; break;
                case ContentType::VIDEO: c.lastMessageText = "[Video]"; break;
                case ContentType::VOICE: c.lastMessageText = "[Voice]"; break;
                case ContentType::STICKER: c.lastMessageText = "[Sticker]"; break;
                default: c.lastMessageText = "[" + content.type + "]"; break;
            }
        }
        c.lastMessageTime = lastMsg.optLong("date", 0);
    }

    TlObject pos = json.optObject("position");
    if (!pos.type.empty()) c.order = pos.optLong("order", 0);

    TlObject draft = json.optObject("draft_message");
    if (!draft.type.empty()) {
        TlObject imt = draft.optObject("input_message_text");
        TlObject txt = imt.optObject("text");
        c.draftText = txt.optString("text", "");
    }

    TlObject notif = json.optObject("notification_settings");
    if (!notif.type.empty()) {
        c.muteFor = notif.optInt("mute_for", 0);
    }

    return c;
}

TdMessage TdParser::parseMessage(const TlObject& json) {
    TdMessage m;
    m.id = json.optLong("id", 0);
    m.chatId = json.optLong("chat_id", 0);
    m.date = json.optInt("date", 0);
    m.editDate = json.optInt("edit_date", 0);
    m.isOutgoing = json.optBool("is_outgoing", false);
    m.isChannelPost = json.optBool("is_channel_post", false);
    m.viewCount = json.optInt("view_count", 0);
    m.forwardCount = json.optInt("forward_count", 0);
    m.replyToMessageId = json.optLong("reply_to_message_id", 0);
    m.threadId = json.optLong("message_thread_id", 0);
    m.viaBotUserId = json.optLong("via_bot_user_id", 0);

    TlObject sender = json.optObject("sender_id");
    m.senderId = sender.optLong("user_id", sender.optLong("chat_id", 0));

    TlObject content = json.optObject("content");
    m.contentType = classifyContent(content.type);

    TlObject textObj = content.optObject("text");
    m.text = textObj.optString("text", content.optString("text", ""));

    TlObject caption = content.optObject("caption");
    m.caption = caption.optString("text", "");

    // Parse rich content
    if (m.contentType == ContentType::PHOTO) {
        TdPhoto p;
        p.hasStickers = content.optBool("has_stickers", false);
        TlObject photo = content.optObject("photo");
        auto sizes = photo.optArray("sizes");
        for (auto& s : sizes) {
            PhotoSize ps;
            ps.type = s.optString("type", "");
            ps.width = s.optInt("width", 0);
            ps.height = s.optInt("height", 0);
            ps.fileId = s.optInt("id", 0);
            ps.fileSize = s.optLong("size", 0);
            p.sizes.push_back(ps);
        }
        m.photo = p;
    } else if (m.contentType == ContentType::VOICE) {
        TlObject vn = content.optObject("voice_note");
        TdVoice v;
        v.duration = vn.optInt("duration", 0);
        v.fileId = vn.optInt("id", 0);
        v.fileSize = vn.optLong("size", 0);
        v.isListened = content.optBool("is_listened", false);
        m.voice = v;
    } else if (m.contentType == ContentType::VIDEO || m.contentType == ContentType::VIDEO_NOTE) {
        TlObject vObj = content.optObject("video");
        TdVideo v;
        v.duration = vObj.optInt("duration", 0);
        v.width = vObj.optInt("width", 0);
        v.height = vObj.optInt("height", 0);
        v.fileId = vObj.optInt("id", 0);
        v.fileSize = vObj.optLong("size", 0);
        v.supportsStreaming = vObj.optBool("supports_streaming", false);
        m.video = v;
    } else if (m.contentType == ContentType::DOCUMENT) {
        TlObject doc = content.optObject("document");
        TdDocument d;
        d.fileName = doc.optString("file_name", "");
        d.mimeType = doc.optString("mime_type", "");
        d.fileId = doc.optInt("id", 0);
        d.fileSize = doc.optLong("size", 0);
        m.document = d;
    } else if (m.contentType == ContentType::STICKER) {
        TlObject s = content.optObject("sticker");
        TdSticker st;
        st.setId = s.optLong("set_id", 0);
        st.width = s.optInt("width", 0);
        st.height = s.optInt("height", 0);
        st.emoji = s.optString("emoji", "");
        st.fileId = s.optInt("id", 0);
        st.fileSize = s.optLong("size", 0);
        st.isAnimated = s.optString("format", "").find("animated") != std::string::npos;
        st.isVideo = s.optString("format", "").find("video") != std::string::npos;
        m.sticker = st;
    } else if (m.contentType == ContentType::POLL) {
        TlObject poll = content.optObject("poll");
        TdPoll p;
        p.id = poll.optLong("id", 0);
        p.question = poll.optString("question", "");
        p.totalVoterCount = poll.optInt("total_voter_count", 0);
        p.isAnonymous = poll.optBool("is_anonymous", true);
        p.isClosed = poll.optBool("is_closed", false);
        p.allowMultipleAnswers = poll.optBool("allow_multiple_answers", false);
        auto opts = poll.optArray("options");
        for (auto& o : opts) {
            PollOption po;
            po.text = o.optString("text", "");
            po.voterCount = o.optInt("voter_count", 0);
            po.votePercentage = o.optInt("vote_percentage", 0);
            po.isChosen = o.optBool("is_chosen", false);
            p.options.push_back(po);
        }
        m.poll = p;
    } else if (m.contentType == ContentType::DICE) {
        TdDice d;
        d.emoji = content.optString("emoji", "");
        d.value = content.optInt("value", 0);
        m.dice = d;
    } else if (m.contentType == ContentType::LOCATION) {
        TlObject loc = content.optObject("location");
        TdLocation l;
        l.latitude = loc.optDouble("latitude", 0);
        l.longitude = loc.optDouble("longitude", 0);
        l.livePeriod = content.optInt("live_period", 0);
        l.heading = content.optInt("heading", 0);
        m.location = l;
    }

    return m;
}

TdStory TdParser::parseStory(const TlObject& json) {
    TdStory s;
    s.id = json.optLong("id", 0);
    s.senderChatId = json.optLong("sender_chat_id", 0);
    s.date = json.optInt("date", 0);
    s.isBeingViewed = json.optBool("is_being_viewed", false);
    s.isPinned = json.optBool("is_pinned", false);

    TlObject content = json.optObject("content");
    s.contentType = classifyContent(content.type);
    TlObject caption = content.optObject("caption");
    s.caption = caption.optString("text", "");

    return s;
}

TdSession TdParser::parseSession(const TlObject& json) {
    TdSession s;
    s.id = json.optLong("id", 0);
    s.isCurrent = json.optBool("is_current", false);
    s.appName = json.optString("application_name", "");
    s.appVersion = json.optString("application_version", "");
    s.deviceModel = json.optString("device_model", "");
    s.platform = json.optString("platform", "");
    s.ip = json.optString("ip", "");
    s.country = json.optString("country", "");
    s.logInDate = json.optInt("log_in_date", 0);
    s.lastActiveDate = json.optInt("last_active_date", 0);
    return s;
}

TdProxy TdParser::parseProxy(const TlObject& json) {
    TdProxy p;
    p.id = json.optInt("id", 0);
    p.server = json.optString("server", "");
    p.port = json.optInt("port", 0);
    p.isEnabled = json.optBool("is_enabled", false);
    p.lastPing = json.optLong("last_ping", 0);

    TlObject type = json.optObject("type");
    if (type.type.find("Socks5") != std::string::npos) p.type = ProxyType::SOCKS5;
    else if (type.type.find("Http") != std::string::npos) p.type = ProxyType::HTTP;
    else p.type = ProxyType::MTPROTO;

    return p;
}

TdNotificationSettings TdParser::parseNotificationSettings(const TlObject& json) {
    TdNotificationSettings ns;
    ns.useDefaultMute = json.optBool("use_default_mute_for", true);
    ns.muteFor = json.optInt("mute_for", 0);
    ns.useDefaultSound = json.optBool("use_default_sound", true);
    ns.soundId = json.optLong("sound_id", 0);
    ns.showPreview = json.optBool("show_preview", true);
    ns.disablePinned = json.optBool("disable_pinned_message_notifications", false);
    ns.disableMention = json.optBool("disable_mention_notifications", false);
    return ns;
}

TdError TdParser::parseError(const TlObject& json) {
    TdError e;
    e.code = json.optInt("code", 0);
    e.message = json.optString("message", "");
    return e;
}

TdConnectionState TdParser::parseConnectionState(const std::string& type) {
    if (type == "connectionStateReady") return TdConnectionState::CONNECTED;
    if (type == "connectionStateConnecting" || type == "connectionStateConnectingToProxy")
        return TdConnectionState::CONNECTING;
    if (type == "connectionStateUpdating") return TdConnectionState::UPDATING;
    if (type == "connectionStateWaitingForNetwork") return TdConnectionState::WAITING_NETWORK;
    return TdConnectionState::DISCONNECTED;
}

ContentType TdParser::parseContentType(const TlObject& content) {
    return classifyContent(content.type);
}

} // namespace pcp
