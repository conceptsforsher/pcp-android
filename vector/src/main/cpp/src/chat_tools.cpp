#include "progressive/chat_tools.hpp"
#include <sstream>
#include <algorithm>
#include <chrono>
#include <unordered_map>

namespace progressive {

// ---- UserHideManager ----

void UserHideManager::hideFor(const std::string& userId, const std::string& displayName, int minutes) {
    // Remove existing hide for this user
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const UserHideEntry& e) { return e.userId == userId; }
    ), entries_.end());

    UserHideEntry entry;
    entry.userId = userId;
    entry.displayName = displayName;
    entry.remainingSeconds = minutes * 60;
    entry.hiddenUntilMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count() + (minutes * 60 * 1000LL);
    entries_.push_back(entry);
}

bool UserHideManager::isHidden(const std::string& userId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    for (const auto& e : entries_) {
        if (e.userId == userId && e.hiddenUntilMs > now) return true;
    }
    return false;
}

int UserHideManager::getRemainingSeconds(const std::string& userId) const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    for (const auto& e : entries_) {
        if (e.userId == userId && e.hiddenUntilMs > now) {
            return static_cast<int>((e.hiddenUntilMs - now) / 1000);
        }
    }
    return 0;
}

std::vector<UserHideEntry> UserHideManager::getActiveHides() const {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    std::vector<UserHideEntry> result;
    for (const auto& e : entries_) {
        if (e.hiddenUntilMs > now) {
            auto copy = e;
            copy.remainingSeconds = static_cast<int>((e.hiddenUntilMs - now) / 1000);
            result.push_back(copy);
        }
    }
    return result;
}

void UserHideManager::cleanExpired() {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    entries_.erase(std::remove_if(entries_.begin(), entries_.end(),
        [&](const UserHideEntry& e) { return e.hiddenUntilMs <= now; }
    ), entries_.end());
}

std::string UserHideManager::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto active = getActiveHides();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < active.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"userId": ")" << esc(active[i].userId) << R"(")";
        json << R"(,"displayName": ")" << esc(active[i].displayName) << R"(")";
        json << R"(,"remainingSeconds": )" << active[i].remainingSeconds << "}";
    }
    json << "]";
    return json.str();
}

void UserHideManager::clear() {
    entries_.clear();
}

// ---- MessageQueue ----

void MessageQueue::enqueue(const QueuedMessage& msg) {
    queue_.push_back(msg);
}

void MessageQueue::setOrder(const std::string& msgId, int order) {
    for (auto& m : queue_) {
        if (m.msgId == msgId) {
            m.order = order;
            return;
        }
    }
}

void MessageQueue::markFailed(const std::string& msgId, const std::string& error) {
    for (auto& m : queue_) {
        if (m.msgId == msgId) {
            m.failed = true;
            m.lastError = error;
            m.retries++;
            return;
        }
    }
}

void MessageQueue::markSent(const std::string& msgId) {
    queue_.erase(std::remove_if(queue_.begin(), queue_.end(),
        [&](const QueuedMessage& m) { return m.msgId == msgId; }
    ), queue_.end());
}

const QueuedMessage* MessageQueue::getNext() const {
    // Find the non-failed message with lowest order, or first pending
    for (const auto& m : queue_) {
        if (!m.failed) return &m;
    }
    // Retry failed ones that haven't exceeded max retries
    for (const auto& m : queue_) {
        if (m.failed && m.retries < m.maxRetries) return &m;
    }
    return nullptr;
}

std::vector<QueuedMessage> MessageQueue::getAll() const {
    auto result = queue_;
    std::sort(result.begin(), result.end(), [](const QueuedMessage& a, const QueuedMessage& b) {
        return a.order < b.order;
    });
    return result;
}

int MessageQueue::pendingCount() const {
    return static_cast<int>(queue_.size());
}

void MessageQueue::clear() {
    queue_.clear();
}

std::string MessageQueue::exportJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    auto sorted = getAll();
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < sorted.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"msgId": ")" << esc(sorted[i].msgId) << R"(")";
        json << R"(,"body": ")" << esc(sorted[i].body) << R"(")";
        json << R"(,"order": )" << sorted[i].order;
        json << R"(,"failed": )" << (sorted[i].failed ? "true" : "false");
        json << R"(,"retries": )" << sorted[i].retries << "}";
    }
    json << "]";
    return json.str();
}

// ---- Auto-Scroll ----

ScrollPlan computeScrollPlan(const AutoScrollConfig& config, int totalLines, int lineHeightPx) {
    ScrollPlan plan;
    plan.totalLines = totalLines;
    int totalPx = totalLines * lineHeightPx;

    if (totalLines <= 0 || config.durationMinutes <= 0) return plan;

    plan.linesPerMinute = totalLines / config.durationMinutes;
    plan.estimatedFullScrollMin = config.durationMinutes;

    if (config.smoothScroll) {
        // Smooth: calculate pixels per tick
        double totalMs = config.durationMinutes * 60.0 * 1000.0;
        plan.scrollPxPerTick = static_cast<int>((totalPx / totalMs) * 1000.0);
        if (plan.scrollPxPerTick < 1) plan.scrollPxPerTick = 1;
    } else {
        // Pages: calculate lines per jump
        int jumpsPerMinute = 60 / 2; // jump every 2 seconds
        plan.linesPerMinute = totalLines / config.durationMinutes;
    }

    return plan;
}

// ---- Image Crop ----

bool isValidCrop(int imgW, int imgH, int cropX, int cropY, int cropW, int cropH) {
    if (imgW <= 0 || imgH <= 0) return false;
    if (cropX < 0 || cropY < 0) return false;
    if (cropW <= 0 || cropH <= 0) return false;
    if (cropX + cropW > imgW) return false;
    if (cropY + cropH > imgH) return false;
    return true;
}

// Original Kotlin: getToolLabel — human-readable label by locale-neutral enum
std::string getToolLabel(ChatTool tool) {
    switch (tool) {
        case ChatTool::SEARCH:                return "Search";
        case ChatTool::MARK_UNREAD:           return "Mark as unread";
        case ChatTool::COPY_LINK:             return "Copy link";
        case ChatTool::SHARE:                 return "Share";
        case ChatTool::FAVOURITE:             return "Favourite";
        case ChatTool::NOTIFICATION_SETTINGS:  return "Notification settings";
        case ChatTool::LEAVE_ROOM:            return "Leave room";
        case ChatTool::ROOM_SETTINGS:         return "Room settings";
        case ChatTool::INVITE_PEOPLE:         return "Invite people";
        case ChatTool::REPORT_CONTENT:        return "Report content";
        case ChatTool::MARK_ALL_READ:         return "Mark all as read";
        case ChatTool::JUMP_TO_DATE:          return "Jump to date";
        case ChatTool::MUTE:                  return "Mute";
        case ChatTool::UNMUTE:                return "Unmute";
    }
    return "Unknown";
}

// Original Kotlin: getToolIcon — icon resource name for tool
std::string getToolIcon(ChatTool tool) {
    switch (tool) {
        case ChatTool::SEARCH:                return "ic_search";
        case ChatTool::MARK_UNREAD:           return "ic_mark_unread";
        case ChatTool::COPY_LINK:             return "ic_link";
        case ChatTool::SHARE:                 return "ic_share";
        case ChatTool::FAVOURITE:             return "ic_favorite";
        case ChatTool::NOTIFICATION_SETTINGS:  return "ic_notifications";
        case ChatTool::LEAVE_ROOM:            return "ic_exit_to_app";
        case ChatTool::ROOM_SETTINGS:         return "ic_settings";
        case ChatTool::INVITE_PEOPLE:         return "ic_person_add";
        case ChatTool::REPORT_CONTENT:        return "ic_report";
        case ChatTool::MARK_ALL_READ:         return "ic_done_all";
        case ChatTool::JUMP_TO_DATE:          return "ic_date_range";
        case ChatTool::MUTE:                  return "ic_volume_off";
        case ChatTool::UNMUTE:                return "ic_volume_up";
    }
    return "ic_help";
}

// Original Kotlin: getAvailableTools — list tools enabled for context
std::vector<ChatToolInfo> getAvailableTools(const ChatToolContext& ctx) {
    std::vector<ChatToolInfo> tools;

    auto add = [&](ChatTool t, bool reqRoom, bool reqEvent, bool destructive) {
        ChatToolInfo info;
        info.tool = t;
        info.label = getToolLabel(t);
        info.icon = getToolIcon(t);
        info.requiresRoom = reqRoom;
        info.requiresEvent = reqEvent;
        info.isDestructive = destructive;
        info.isEnabled = isToolAvailable(t, ctx);
        tools.push_back(info);
    };

    add(ChatTool::SEARCH, true, false, false);
    add(ChatTool::MARK_UNREAD, true, false, false);
    add(ChatTool::COPY_LINK, false, true, false);
    add(ChatTool::SHARE, false, true, false);
    if (!ctx.isFavourite) {
        add(ChatTool::FAVOURITE, true, false, false);
    }
    add(ChatTool::NOTIFICATION_SETTINGS, true, false, false);
    add(ChatTool::ROOM_SETTINGS, true, false, false);
    add(ChatTool::INVITE_PEOPLE, true, false, false);
    add(ChatTool::REPORT_CONTENT, false, true, false);
    add(ChatTool::MARK_ALL_READ, true, false, false);
    add(ChatTool::JUMP_TO_DATE, true, false, false);
    if (ctx.isMuted) {
        add(ChatTool::UNMUTE, true, false, false);
    } else {
        add(ChatTool::MUTE, true, false, false);
    }
    if (ctx.isJoinedRoom) {
        add(ChatTool::LEAVE_ROOM, true, false, true);
    }

    return tools;
}

// Original Kotlin: isToolAvailable — check tool availability in context
bool isToolAvailable(ChatTool tool, const ChatToolContext& ctx) {
    switch (tool) {
        case ChatTool::SEARCH:
            return ctx.isJoinedRoom;
        case ChatTool::MARK_UNREAD:
            return ctx.isJoinedRoom;
        case ChatTool::COPY_LINK:
            return !ctx.eventId.empty();
        case ChatTool::SHARE:
            return !ctx.eventId.empty();
        case ChatTool::FAVOURITE:
            return ctx.isJoinedRoom && !ctx.isFavourite;
        case ChatTool::NOTIFICATION_SETTINGS:
            return ctx.isJoinedRoom;
        case ChatTool::LEAVE_ROOM:
            return ctx.isJoinedRoom && ctx.userMembership == "join";
        case ChatTool::ROOM_SETTINGS:
            return ctx.isJoinedRoom && ctx.userPowerLevel >= 50;
        case ChatTool::INVITE_PEOPLE:
            return ctx.isJoinedRoom && ctx.userPowerLevel >= 0;
        case ChatTool::REPORT_CONTENT:
            return !ctx.eventId.empty();
        case ChatTool::MARK_ALL_READ:
            return ctx.isJoinedRoom;
        case ChatTool::JUMP_TO_DATE:
            return ctx.isJoinedRoom;
        case ChatTool::MUTE:
            return ctx.isJoinedRoom && !ctx.isMuted;
        case ChatTool::UNMUTE:
            return ctx.isJoinedRoom && ctx.isMuted;
    }
    return false;
}

// Original Kotlin: executeToolAction — placeholder action dispatch
std::string executeToolAction(ChatTool tool, const ChatToolContext& ctx) {
    std::ostringstream result;
    result << R"({"tool": ")" << getToolLabel(tool)
           << R"(", "roomId": ")" << ctx.roomId
           << R"(", "success": true})";
    return result.str();
}

// Original Kotlin: ChatToolCategory — grouping for tool menus
enum class ChatToolCategory {
    NAVIGATION,
    ROOM_ACTIONS,
    MESSAGE_ACTIONS,
    MODERATION,
    NOTIFICATIONS,
    DESTRUCTIVE
};

// Original Kotlin: getToolCategory — map tool to its category
ChatToolCategory getToolCategory(ChatTool tool) {
    switch (tool) {
        case ChatTool::SEARCH:
        case ChatTool::JUMP_TO_DATE:
        case ChatTool::MARK_UNREAD:
        case ChatTool::MARK_ALL_READ:
            return ChatToolCategory::NAVIGATION;
        case ChatTool::ROOM_SETTINGS:
        case ChatTool::INVITE_PEOPLE:
        case ChatTool::FAVOURITE:
        case ChatTool::LEAVE_ROOM:
            return ChatToolCategory::ROOM_ACTIONS;
        case ChatTool::COPY_LINK:
        case ChatTool::SHARE:
        case ChatTool::REPORT_CONTENT:
            return ChatToolCategory::MESSAGE_ACTIONS;
        case ChatTool::MUTE:
        case ChatTool::UNMUTE:
        case ChatTool::NOTIFICATION_SETTINGS:
            return ChatToolCategory::NOTIFICATIONS;
    }
    return ChatToolCategory::NAVIGATION;
}

// Original Kotlin: getToolDescription — longer description for each tool
std::string getToolDescription(ChatTool tool) {
    switch (tool) {
        case ChatTool::SEARCH:
            return "Search messages in this room";
        case ChatTool::MARK_UNREAD:
            return "Mark this room as unread";
        case ChatTool::COPY_LINK:
            return "Copy permalink to this event";
        case ChatTool::SHARE:
            return "Share this event externally";
        case ChatTool::FAVOURITE:
            return "Add this room to favourites";
        case ChatTool::NOTIFICATION_SETTINGS:
            return "Configure notification preferences";
        case ChatTool::LEAVE_ROOM:
            return "Leave this room permanently";
        case ChatTool::ROOM_SETTINGS:
            return "View and edit room settings";
        case ChatTool::INVITE_PEOPLE:
            return "Invite others to this room";
        case ChatTool::REPORT_CONTENT:
            return "Report inappropriate content";
        case ChatTool::MARK_ALL_READ:
            return "Mark all messages as read";
        case ChatTool::JUMP_TO_DATE:
            return "Jump to messages from a specific date";
        case ChatTool::MUTE:
            return "Mute notifications from this room";
        case ChatTool::UNMUTE:
            return "Unmute notifications from this room";
    }
    return "";
}

// Original Kotlin: getToolOrder — display order weight (lower = higher)
int getToolOrder(ChatTool tool) {
    switch (tool) {
        case ChatTool::SEARCH: return 10;
        case ChatTool::JUMP_TO_DATE: return 20;
        case ChatTool::MARK_UNREAD: return 30;
        case ChatTool::MARK_ALL_READ: return 40;
        case ChatTool::COPY_LINK: return 50;
        case ChatTool::SHARE: return 60;
        case ChatTool::FAVOURITE: return 70;
        case ChatTool::NOTIFICATION_SETTINGS: return 80;
        case ChatTool::MUTE: return 90;
        case ChatTool::UNMUTE: return 100;
        case ChatTool::INVITE_PEOPLE: return 110;
        case ChatTool::ROOM_SETTINGS: return 120;
        case ChatTool::REPORT_CONTENT: return 130;
        case ChatTool::LEAVE_ROOM: return 140;
    }
    return 999;
}

// Original Kotlin: sortToolsByOrder — sort ChatToolInfo vector by display order
void sortToolsByOrder(std::vector<ChatToolInfo>& tools) {
    std::sort(tools.begin(), tools.end(), [](const ChatToolInfo& a, const ChatToolInfo& b) {
        return getToolOrder(a.tool) < getToolOrder(b.tool);
    });
}

// Original Kotlin: buildRoomPermalink — construct matrix.to permalink
std::string buildRoomPermalink(const std::string& roomId, const std::string& eventId) {
    std::string link = "https://matrix.to/#/" + roomId;
    if (!eventId.empty()) {
        link += "/" + eventId;
    }
    return link;
}

// Original Kotlin: buildShareContent — construct share text for an event
std::string buildShareContent(const std::string& roomId, const std::string& eventId, const std::string& body) {
    std::ostringstream out;
    out << body << "\n\n";
    out << buildRoomPermalink(roomId, eventId);
    return out.str();
}

// Original Kotlin: getUserMembershipLevel — numeric level for membership comparison
int getUserMembershipLevel(const std::string& membership) {
    if (membership == "join") return 100;
    if (membership == "invite") return 50;
    if (membership == "leave") return 10;
    if (membership == "ban") return 0;
    if (membership == "knock") return 25;
    return -1;
}

// Original Kotlin: contextToJson — serialize ChatToolContext to JSON
std::string contextToJson(const ChatToolContext& ctx) {
    std::ostringstream json;
    json << R"({"roomId": ")" << ctx.roomId
         << R"(", "isJoined": )" << (ctx.isJoinedRoom ? "true" : "false")
         << R"(, "isEncrypted": )" << (ctx.isEncryptedRoom ? "true" : "false")
         << R"(, "membership": ")" << ctx.userMembership
         << R"(", "powerLevel": )" << ctx.userPowerLevel
         << R"(, "isFavourite": )" << (ctx.isFavourite ? "true" : "false")
         << R"(, "isMuted": )" << (ctx.isMuted ? "true" : "false")
         << "}";
    return json.str();
}

} // namespace progressive
