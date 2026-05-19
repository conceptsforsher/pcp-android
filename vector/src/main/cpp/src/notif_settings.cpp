#include "progressive/notif_settings.hpp"
#include <sstream>

namespace progressive {

NotifDecision computeNotifDecision(
    const RoomNotifSettings& roomSettings,
    bool isMention, bool isRoomMention, bool isKeywordMatch,
    bool isInvite, bool isCall, const std::string& senderId, const std::string& myUserId
) {
    NotifDecision decision;

    // Calls always notify
    if (isCall) {
        decision.shouldNotify = true;
        decision.shouldHighlight = true;
        decision.shouldVibrate = true;
        decision.soundName = "call";
        decision.reason = "Incoming call";
        return decision;
    }

    // Invites always notify
    if (isInvite) {
        decision.shouldNotify = true;
        decision.shouldHighlight = true;
        decision.reason = "Room invitation";
        return decision;
    }

    // Own messages never notify
    if (senderId == myUserId) {
        decision.shouldNotify = false;
        decision.reason = "Own message";
        return decision;
    }

    auto mode = roomSettings.mode;

    // Encrypted DMs default to Mentions if not explicitly set
    if (mode == NotifMode::Default && roomSettings.isDirectChat && roomSettings.isEncryptedRoom) {
        mode = NotifMode::Mentions;
    }

    switch (mode) {
        case NotifMode::None:
            decision.shouldNotify = false;
            decision.reason = "Room muted";
            break;

        case NotifMode::Mentions:
            if (isMention || isRoomMention || isKeywordMatch) {
                decision.shouldNotify = true;
                decision.shouldHighlight = isMention || isRoomMention;
                decision.reason = isMention ? "Direct mention" :
                                  isRoomMention ? "@room mention" : "Keyword match";
            } else {
                decision.shouldNotify = false;
                decision.reason = "Not mentioned";
            }
            break;

        case NotifMode::All:
        case NotifMode::Default:
            decision.shouldNotify = true;
            if (isMention || isRoomMention) {
                decision.shouldHighlight = true;
                decision.reason = "Mention in room";
            } else {
                decision.reason = "Room message";
            }
            break;
    }

    // Vibrate for highlights
    decision.shouldVibrate = decision.shouldHighlight;

    return decision;
}

bool shouldUpgradeToMentions(const RoomNotifSettings& settings) {
    return settings.isEncryptedRoom && settings.isDirectChat &&
           settings.mode == NotifMode::Default;
}

std::string formatNotifMode(NotifMode mode) {
    switch (mode) {
        case NotifMode::All:      return "All messages";
        case NotifMode::Mentions:  return "Mentions only";
        case NotifMode::None:     return "Muted";
        case NotifMode::Default:  return "Default";
        default:                  return "Unknown";
    }
}

NotifMode parseNotifMode(const std::string& action) {
    if (action == "all" || action == "notify") return NotifMode::All;
    if (action == "mentions" || action == "highlight") return NotifMode::Mentions;
    if (action == "none" || action == "dont_notify") return NotifMode::None;
    return NotifMode::Default;
}

std::string buildRoomNotifSettingsBody(NotifMode mode) {
    std::ostringstream json;
    json << R"({"actions": [)";
    switch (mode) {
        case NotifMode::All:
            json << R"("notify")";
            break;
        case NotifMode::Mentions:
            json << R"("dont_notify")"; // default action, mentions override elsewhere
            break;
        case NotifMode::None:
            json << R"("dont_notify")";
            break;
        default:
            json << R"("dont_notify")";
            break;
    }
    json << "]}";
    return json.str();
}

bool isNotifModeDifferent(NotifMode oldMode, NotifMode newMode) {
    return oldMode != newMode;
}

NotifMode getDefaultModeForRoom(bool isDirect, bool isEncrypted) {
    if (isDirect && isEncrypted) return NotifMode::Mentions;
    return NotifMode::All;
}

} // namespace progressive
