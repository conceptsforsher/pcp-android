#include "progressive/room_summary.hpp"
#include <sstream>
#include <algorithm>
#include <unordered_map>

namespace progressive {

// ================================================================
// Room Summary Update (ported from RoomSummaryUpdater.kt:89-213)
// ================================================================

RoomSummaryInfo updateRoomSummary(
    const std::string& roomId,
    const RoomSyncData& syncData,
    const RoomStateEvents& stateEvents,
    const std::vector<ThreadUnreadData>& threadUnreads,
    const std::string& inviterId,
    const std::string& readMarkerId,
    const std::string& latestEventId,
    int64_t latestEventTs)
{
    RoomSummaryInfo s;
    s.roomId = roomId;

    // --- Membership counts from sync summary ---
    // Original Kotlin:
    //   if (roomSummary.invitedMembersCount != null)
    //       roomSummaryEntity.invitedMembersCount = roomSummary.invitedMembersCount
    //   if (roomSummary.joinedMembersCount != null)
    //       roomSummaryEntity.joinedMembersCount = roomSummary.joinedMembersCount
    if (syncData.joinedMemberCount >= 0)
        s.joinedMembers = syncData.joinedMemberCount;
    if (syncData.invitedMemberCount >= 0)
        s.invitedMembers = syncData.invitedMemberCount;

    // --- Notification counts ---
    // Original Kotlin:
    //   roomSummaryEntity.highlightCount = unreadNotifications?.highlightCount ?: 0
    //   roomSummaryEntity.notificationCount = unreadNotifications?.notificationCount ?: 0
    s.highlightCount = syncData.unreadHighlightCount;
    s.notificationCount = syncData.unreadNotificationCount;

    // --- Thread notification counts ---
    // Original Kotlin:
    //   roomSummaryEntity.threadHighlightCount = unreadThreadNotifications
    //       ?.count { (it.value.highlightCount ?: 0) > 0 } ?: 0
    //   roomSummaryEntity.threadNotificationCount = unreadThreadNotifications
    //       ?.count { (it.value.notificationCount ?: 0) > 0 } ?: 0
    int threadNotifyCount = 0;
    int threadHighlightCount = 0;
    computeThreadNotificationCounts(threadUnreads, threadNotifyCount, threadHighlightCount);
    s.threadNotificationCount = threadNotifyCount;
    s.threadHighlightCount = threadHighlightCount;

    // --- State events ---
    // Original Kotlin:
    //   roomSummaryEntity.setDisplayName(roomDisplayNameResolver.resolve(realm, roomId))
    //   roomSummaryEntity.avatarUrl = roomAvatarResolver.resolve(realm, roomId)
    //   roomSummaryEntity.name = ContentMapper.map(lastNameEvent?.content).toModel<RoomNameContent>()?.name
    //   roomSummaryEntity.topic = ContentMapper.map(lastTopicEvent?.content).toModel<RoomTopicContent>()?.topic
    s.displayName = stateEvents.roomName;
    s.topic = stateEvents.roomTopic;
    s.canonicalAlias = stateEvents.canonicalAlias;

    // --- Room type ---
    // Original Kotlin: roomType = ContentMapper.map(roomCreateEvent?.content).toModel<RoomCreateContent>()?.type
    // roomSummaryEntity.roomType = roomType
    s.isSpace = (stateEvents.roomType == "m.space");

    // --- Encryption ---
    // Original Kotlin:
    //   val wasEncrypted = roomSummaryEntity.isEncrypted
    //   roomSummaryEntity.isEncrypted = encryptionEvent != null
    //   roomSummaryEntity.e2eAlgorithm = ...algorithm
    //   roomSummaryEntity.encryptionEventTs = encryptionEvent?.originServerTs
    s.isEncrypted = stateEvents.hasEncryptionEvent;
    if (stateEvents.hasEncryptionEvent) {
        s.encryptionAlgorithm = stateEvents.encryptionAlgorithm;
        s.encryptionEventTs = stateEvents.encryptionEventTs;
    }

    // --- Join rule ---
    // Original Kotlin:
    //   roomSummaryEntity.joinRules = ...joinRules
    s.isPublic = (stateEvents.joinRule == "public");

    // --- Inviter ---
    // Original Kotlin:
    //   if (roomSummaryEntity.membership == Membership.INVITE && inviterId != null)
    //       roomSummaryEntity.inviterId = inviterId
    //   else if (roomSummaryEntity.membership != Membership.INVITE)
    //       roomSummaryEntity.inviterId = null
    if (s.isInvited && !inviterId.empty()) {
        s.inviterId = inviterId;
    }

    // --- Last activity ---
    // Original Kotlin:
    //   val lastActivityFromEvent = latestPreviewableEvent?.root?.originServerTs
    //   if (lastActivityFromEvent != null) roomSummaryEntity.lastActivityTime = lastActivityFromEvent
    if (latestEventTs > 0) {
        s.lastActivityTs = latestEventTs;
    }

    // --- Read marker ---
    s.readMarkerId = readMarkerId;

    return s;
}

// ================================================================
// Has Unread Messages (ported from RoomSummaryUpdater.kt:156-158)
// ================================================================

bool computeHasUnreadMessages(
    int notificationCount,
    const std::string& latestEventId,
    const std::string& readMarkerId,
    const std::string& userId,
    const std::string& roomId,
    bool canUseThreadReceipts)
{
    // Original Kotlin:
    //   roomSummaryEntity.hasUnreadMessages = roomSummaryEntity.notificationCount > 0 ||
    //       latestPreviewableEvent?.let {
    //           !isEventRead(realm.configuration, userId, roomId, it.eventId,
    //               shouldCheckIfReadInEventsThread)
    //       } ?: false

    // Simple case: unread count > 0
    if (notificationCount > 0) return true;

    // If no latest event, nothing to check
    if (latestEventId.empty()) return false;

    // Check if the latest event has been read
    // If read marker is empty (nothing read yet) → has unread
    if (readMarkerId.empty()) return true;

    // Compare event IDs — if latest event differs from read marker, there are unreads
    // This is a simplified version of isEventRead() which would do a more complex
    // comparison of event ordering in the timeline.
    return (latestEventId != readMarkerId);
}

// ================================================================
// Thread Notification Counts (ported from RoomSummaryUpdater.kt:116-122)
// ================================================================

void computeThreadNotificationCounts(
    const std::vector<ThreadUnreadData>& threadUnreads,
    int& outThreadNotificationCount,
    int& outThreadHighlightCount)
{
    outThreadNotificationCount = 0;
    outThreadHighlightCount = 0;

    // Original Kotlin:
    //   roomSummaryEntity.threadHighlightCount = unreadThreadNotifications
    //       ?.count { (it.value.highlightCount ?: 0) > 0 } ?: 0
    //   roomSummaryEntity.threadNotificationCount = unreadThreadNotifications
    //       ?.count { (it.value.notificationCount ?: 0) > 0 } ?: 0
    for (const auto& t : threadUnreads) {
        if (t.highlightCount > 0) outThreadHighlightCount++;
        if (t.notificationCount > 0) outThreadNotificationCount++;
    }
}

// ================================================================
// Should Hide From User (ported from RoomSummaryUpdater.kt:129-130)
// ================================================================

bool shouldHideFromUser(bool isUpgradedRoomJoined, bool isVirtualRoom) {
    // Original Kotlin:
    //   roomSummaryEntity.isHiddenFromUser =
    //       roomSummaryEntity.versioningState == VersioningState.UPGRADED_ROOM_JOINED ||
    //       roomAccountDataDataSource.getAccountDataEvent(
    //           roomId, RoomAccountDataTypes.EVENT_TYPE_VIRTUAL_ROOM) != null
    return isUpgradedRoomJoined || isVirtualRoom;
}

// ================================================================
// Preview Filter (ported from RoomSummaryEventsHelper.kt)
// ================================================================

RoomPreviewFilter buildDefaultPreviewFilter() {
    RoomPreviewFilter filter;
    filter.filterUseless = true;
    filter.filterEdits = true;
    filter.filterRedacted = false;

    // Original Kotlin: RoomSummaryConstants.PREVIEWABLE_TYPES
    // Standard Matrix message types that can be previewed in room list
    constexpr const char* previewable[] = {
        "m.room.message",
        "m.sticker",
        "m.room.encrypted",
        "m.room.member",
        "m.room.name",
        "m.room.topic",
        "m.room.avatar",
        "m.room.canonical_alias",
        "m.room.join_rules",
        "m.room.create",
        "m.call.invite",
        "m.call.answer",
        "m.call.hangup",
        "m.room.tombstone",
        "m.poll.start",
        "m.poll.end",
        "m.room.pinned_events",
        "m.space.child",
        "m.space.parent",
        "m.beacon_info",
        "org.matrix.msc3381.poll.start",
        "org.matrix.msc3381.poll.end",
    };

    for (const auto& t : previewable) {
        filter.allowedTypes.insert(t);
    }

    return filter;
}

bool isPreviewableEventType(const std::string& eventType, const RoomPreviewFilter& filter) {
    // Original Kotlin: previewFilters logic in RoomSummaryEventsHelper
    // Checks if event type is in the allowed set
    return filter.allowedTypes.find(eventType) != filter.allowedTypes.end();
}

// ================================================================
// Room List Stats
// ================================================================

RoomListStats computeRoomListStats(const std::vector<RoomSummaryInfo>& rooms) {
    RoomListStats stats;
    for (const auto& room : rooms) {
        stats.totalRooms++;
        if (room.hasUnread) stats.totalUnread++;
        if (room.highlightCount > 0 || room.hasMention) stats.totalHighlights += room.highlightCount;
        if (room.isDirect) stats.totalDirectChats++;
        if (!room.isDirect && !room.isSpace) stats.totalGroupRooms++;
        if (room.isSpace) stats.totalSpaces++;
        if (room.isInvited) stats.totalInvites++;
        if (room.isFavourite) stats.totalFavourites++;
    }
    return stats;
}

int computeRoomPriority(const RoomSummaryInfo& room) {
    int priority = 0;

    // Favourites always first
    if (room.isFavourite) priority += 1000;

    // Invites are important
    if (room.isInvited) priority += 900;

    // Pings/highlights
    if (room.highlightCount > 0 || room.hasMention) priority += 800;

    // Unread messages
    if (room.hasUnread) priority += 700;

    // Direct chats slightly above group rooms
    if (room.isDirect) priority += 100;

    // Add small timestamp component (normalized)
    if (room.lastActivityTs > 0) {
        priority += static_cast<int>((room.lastActivityTs >> 20) & 0xFF);
    }

    return priority;
}

void sortRoomsByPriority(std::vector<RoomSummaryInfo>& rooms) {
    for (auto& room : rooms) {
        room.sortPriority = computeRoomPriority(room);
    }
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.sortPriority > b.sortPriority;
    });
}

void sortRoomsByActivity(std::vector<RoomSummaryInfo>& rooms) {
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.lastActivityTs > b.lastActivityTs;
    });
}

void sortRoomsByName(std::vector<RoomSummaryInfo>& rooms) {
    std::sort(rooms.begin(), rooms.end(), [](const auto& a, const auto& b) {
        return a.displayName < b.displayName;
    });
}

bool needsAttention(const RoomSummaryInfo& room) {
    return room.isInvited || room.hasMention || room.highlightCount > 0;
}

std::string getNotificationBadge(const RoomSummaryInfo& room) {
    if (room.highlightCount > 0 || room.hasMention) {
        return "!";
    }
    if (room.notificationCount > 0) {
        return room.notificationCount > 99 ? "99+" : std::to_string(room.notificationCount);
    }
    return "";
}

std::string formatLastMessagePreview(const std::string& sender, const std::string& body, bool isEncrypted) {
    std::ostringstream out;
    if (!sender.empty()) out << sender << ": ";
    if (isEncrypted && body.empty()) out << "[Encrypted message]";
    else if (body.empty()) out << "[No preview]";
    else if (body.size() > 80) out << body.substr(0, 77) << "...";
    else out << body;
    return out.str();
}

std::string formatRoomPreview(const RoomSummaryInfo& room, bool showSender) {
    std::ostringstream out;

    if (room.isEncrypted) out << "\xF0\x9F\x94\x92 ";  // lock emoji

    out << room.displayName;

    if (!room.lastMessageBody.empty()) {
        out << "\n";
        if (showSender && !room.lastMessageSender.empty()) {
            out << room.lastMessageSender << ": ";
        }
        out << (room.lastMessageBody.size() > 60
            ? room.lastMessageBody.substr(0, 57) + "..."
            : room.lastMessageBody);
    }

    return out.str();
}

std::string roomListStatsToJson(const RoomListStats& stats) {
    std::ostringstream json;
    json << "{";
    json << R"("totalRooms": )" << stats.totalRooms << ",";
    json << R"("totalUnread": )" << stats.totalUnread << ",";
    json << R"("totalHighlights": )" << stats.totalHighlights << ",";
    json << R"("totalDirectChats": )" << stats.totalDirectChats << ",";
    json << R"("totalGroupRooms": )" << stats.totalGroupRooms << ",";
    json << R"("totalSpaces": )" << stats.totalSpaces << ",";
    json << R"("totalInvites": )" << stats.totalInvites << ",";
    json << R"("totalFavourites": )" << stats.totalFavourites;
    json << "}";
    return json.str();
}

} // namespace progressive
