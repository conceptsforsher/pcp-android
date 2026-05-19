#include "progressive/sender_notif_filter.hpp"
#include <sstream>
#include <algorithm>

namespace progressive {

// ==== Notification Decision Logic ====

bool shouldNotifyForSender(
    const SenderNotifSettings& settings,
    const std::string& senderId,
    bool hasMention,
    bool hasHighlight)
{
    switch (settings.mode) {
        case SenderNotifMode::NotifyAll:
            // Always notify — even without mention/highlight
            return true;

        case SenderNotifMode::NotifyMentions:
            // Only notify for @mentions and highlight keywords
            return hasMention || hasHighlight;

        case SenderNotifMode::NotifyList:
            // Only notify if sender is in allowed list
            if (isSenderAllowed(settings, senderId)) return true;
            // Always notify for mentions even if sender not in list
            return hasMention;

        case SenderNotifMode::MuteList:
            // Notify everyone EXCEPT muted senders
            if (isSenderMuted(settings, senderId)) {
                // Exception: always notify for @mentions from muted senders
                return hasMention;
            }
            return true;
    }
    return true;
}

// ==== Sender List Management ====

void muteSender(SenderNotifSettings& settings, const std::string& senderId) {
    if (!isSenderMuted(settings, senderId)) {
        settings.mutedSenders.push_back(senderId);
    }
    settings.mode = SenderNotifMode::MuteList;
}

void unmuteSender(SenderNotifSettings& settings, const std::string& senderId) {
    settings.mutedSenders.erase(
        std::remove(settings.mutedSenders.begin(), settings.mutedSenders.end(), senderId),
        settings.mutedSenders.end());
    if (settings.mutedSenders.empty() && settings.allowedSenders.empty()) {
        settings.mode = SenderNotifMode::NotifyAll;
    }
}

void allowSender(SenderNotifSettings& settings, const std::string& senderId) {
    if (!isSenderAllowed(settings, senderId)) {
        settings.allowedSenders.push_back(senderId);
    }
    settings.mode = SenderNotifMode::NotifyList;
}

void disallowSender(SenderNotifSettings& settings, const std::string& senderId) {
    settings.allowedSenders.erase(
        std::remove(settings.allowedSenders.begin(), settings.allowedSenders.end(), senderId),
        settings.allowedSenders.end());
    if (settings.allowedSenders.empty() && settings.mutedSenders.empty()) {
        settings.mode = SenderNotifMode::NotifyAll;
    }
}

bool isSenderMuted(const SenderNotifSettings& settings, const std::string& senderId) {
    return std::find(settings.mutedSenders.begin(), settings.mutedSenders.end(), senderId)
           != settings.mutedSenders.end();
}

bool isSenderAllowed(const SenderNotifSettings& settings, const std::string& senderId) {
    return std::find(settings.allowedSenders.begin(), settings.allowedSenders.end(), senderId)
           != settings.allowedSenders.end();
}

// ==== Serialization ====

std::string senderNotifModeToString(SenderNotifMode mode) {
    switch (mode) {
        case SenderNotifMode::NotifyAll: return "notify_all";
        case SenderNotifMode::NotifyMentions: return "notify_mentions";
        case SenderNotifMode::NotifyList: return "notify_list";
        case SenderNotifMode::MuteList: return "mute_list";
    }
    return "notify_all";
}

SenderNotifMode senderNotifModeFromString(const std::string& mode) {
    if (mode == "notify_all") return SenderNotifMode::NotifyAll;
    if (mode == "notify_mentions") return SenderNotifMode::NotifyMentions;
    if (mode == "notify_list") return SenderNotifMode::NotifyList;
    if (mode == "mute_list") return SenderNotifMode::MuteList;
    return SenderNotifMode::NotifyAll;
}

std::string senderNotifSettingsToJson(const SenderNotifSettings& settings) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"roomId": ")" << esc(settings.roomId) << R"(",)";
    json << R"("mode": ")" << senderNotifModeToString(settings.mode) << R"(",)";
    json << R"("muteUnknown": )" << (settings.muteUnknownSenders ? "true" : "false") << ",";
    json << R"("allowedSenders": [)";
    for (size_t i = 0; i < settings.allowedSenders.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(settings.allowedSenders[i]) << R"(")";
    }
    json << "],";
    json << R"("mutedSenders": [)";
    for (size_t i = 0; i < settings.mutedSenders.size(); ++i) {
        if (i > 0) json << ",";
        json << R"(")" << esc(settings.mutedSenders[i]) << R"(")";
    }
    json << "]}";
    return json.str();
}

SenderNotifSettings parseSenderNotifSettings(const std::string& json) {
    SenderNotifSettings settings;

    auto extractStr = [&](const std::string& key) -> std::string {
        auto search = "\"" + key + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + key + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    settings.roomId = extractStr("roomId");
    settings.mode = senderNotifModeFromString(extractStr("mode"));
    settings.muteUnknownSenders = json.find("\"muteUnknown\": true") != std::string::npos;

    // Parse allowedSenders array
    auto allowedPos = json.find("\"allowedSenders\"");
    if (allowedPos != std::string::npos) {
        size_t pos = json.find('[', allowedPos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        settings.allowedSenders.push_back(json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                pos++;
            }
        }
    }

    // Parse mutedSenders array
    auto mutedPos = json.find("\"mutedSenders\"");
    if (mutedPos != std::string::npos) {
        size_t pos = json.find('[', mutedPos);
        if (pos != std::string::npos) {
            pos++;
            while (pos < json.size() && json[pos] != ']') {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        settings.mutedSenders.push_back(json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                pos++;
            }
        }
    }

    return settings;
}

SenderNotifSettings getDefaultSenderNotifSettings(const std::string& roomId) {
    SenderNotifSettings settings;
    settings.roomId = roomId;
    return settings;
}

} // namespace progressive
