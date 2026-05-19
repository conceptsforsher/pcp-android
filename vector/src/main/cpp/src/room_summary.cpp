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

// ================================================================
// Room Summary Query (EXPAND)
//
// Ported from RoomSummaryDataSource.kt roomSummariesQuery() +
// RoomSummaryQueryParams.kt
// ================================================================

std::string buildRoomSummaryQuery(const RoomSummaryQueryParams& params) {
    // Original Kotlin: RoomSummaryDataSource.roomSummariesQuery() builds a Realm query
    // with multiple filter clauses. We serialize to a JSON filter spec.
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "{";

    // memberships filter
    if (!params.memberships.empty()) {
        json << R"("memberships":[)";
        for (size_t i = 0; i < params.memberships.size(); i++) {
            if (i > 0) json << ",";
            json << R"(")" << esc(params.memberships[i]) << R"(")";
        }
        json << "]";
    } else {
        json << R"("memberships":[])";
    }

    // roomIds filter
    if (!params.roomIds.empty()) {
        json << R"(,"roomIds":[)";
        for (size_t i = 0; i < params.roomIds.size(); i++) {
            if (i > 0) json << ",";
            json << R"(")" << esc(params.roomIds[i]) << R"(")";
        }
        json << "]";
    }

    json << R"(,"includeDrafts":)" << (params.includeDrafts ? "true" : "false");
    json << R"(,"excludeVirtual":)" << (params.excludeVirtual ? "true" : "false");
    json << R"(,"onlyDirect":)" << (params.onlyDirect ? "true" : "false");
    json << R"(,"onlyGroups":)" << (params.onlyGroups ? "true" : "false");
    json << R"(,"onlyWithNotifications":)" << (params.onlyWithNotifications ? "true" : "false");
    json << R"(,"onlyFavourites":)" << (params.onlyFavourites ? "true" : "false");
    json << R"(,"onlySpaces":)" << (params.onlySpaces ? "true" : "false");
    json << R"(,"excludeSpaces":)" << (params.excludeSpaces ? "true" : "false");

    if (!params.spaceId.empty())
        json << R"(,"spaceId":")" << esc(params.spaceId) << R"(")";

    json << R"(,"orphanRooms":)" << (params.orphanRooms ? "true" : "false");

    if (!params.displayNameFilter.empty())
        json << R"(,"displayNameFilter":")" << esc(params.displayNameFilter) << R"(")";

    if (!params.canonicalAliasFilter.empty())
        json << R"(,"canonicalAliasFilter":")" << esc(params.canonicalAliasFilter) << R"(")";

    json << "}";
    return json.str();
}

bool roomSummaryMatchesFilter(const RoomSummaryInfo& room, const RoomSummaryQueryParams& params) {
    // Original Kotlin: RoomSummaryDataSource.roomSummariesQuery() Realm filter chain
    // Evaluated in-memory in C++ for offline/embedded use.

    // Hidden rooms are always excluded (corresponds to IS_HIDDEN_FROM_USER == false)
    if (room.isHiddenFromUser) return false;

    // Membership filter
    if (!params.memberships.empty()) {
        bool found = false;
        for (const auto& m : params.memberships) {
            if (room.membership == m) { found = true; break; }
        }
        if (!found) return false;
    }

    // Room ID filter (exact match in list)
    if (!params.roomIds.empty()) {
        bool found = false;
        for (const auto& rid : params.roomIds) {
            if (room.roomId == rid) { found = true; break; }
        }
        if (!found) return false;
    }

    // Draft filter
    if (!params.includeDrafts && room.hasDraft) return false;

    // Virtual / local-echo exclusion
    if (params.excludeVirtual && room.isHiddenFromUser) return false;

    // Direct / Group filter (mutually exclusive in Kotlin RoomCategoryFilter)
    if (params.onlyDirect && !room.isDirect) return false;
    if (params.onlyGroups && room.isDirect) return false;
    if (params.onlyGroups && room.isSpace) return false;

    // Notifications filter
    if (params.onlyWithNotifications && room.notificationCount == 0) return false;

    // Favourites filter
    if (params.onlyFavourites && !room.isFavourite) return false;

    // Space filter
    if (params.onlySpaces && !room.isSpace) return false;
    if (params.excludeSpaces && room.isSpace) return false;

    // Display name fuzzy filter
    if (!params.displayNameFilter.empty()) {
        if (room.displayName.find(params.displayNameFilter) == std::string::npos)
            return false;
    }

    // Canonical alias exact filter
    if (!params.canonicalAliasFilter.empty()) {
        if (room.canonicalAlias != params.canonicalAliasFilter)
            return false;
    }

    return true;
}

// ================================================================
// Room Summary Update Batch (EXPAND)
//
// Ported from RoomSummaryUpdater.kt sync diff logic:
// When sync returns, we compare current summaries against new ones
// to emit add/update/remove events for the UI.
// ================================================================

RoomSummaryUpdateBatch computeRoomSummaryDiff(
    const std::vector<RoomSummaryInfo>& oldList,
    const std::vector<RoomSummaryInfo>& newList)
{
    // Original Kotlin: RoomSummaryUpdater iterates realm results and
    // detects which rooms were added, changed, or removed.
    RoomSummaryUpdateBatch batch;

    // Build lookup maps by roomId
    std::unordered_map<std::string, const RoomSummaryInfo*> oldMap;
    for (const auto& r : oldList) oldMap[r.roomId] = &r;

    std::unordered_map<std::string, const RoomSummaryInfo*> newMap;
    for (const auto& r : newList) newMap[r.roomId] = &r;

    // Detect added and updated rooms
    for (const auto& nr : newList) {
        auto it = oldMap.find(nr.roomId);
        if (it == oldMap.end()) {
            // Room not in old list → ADDED
            batch.added.push_back(nr.roomId);
        } else {
            // Room in both lists — check for changes
            const auto* orp = it->second;
            if (orp->displayName != nr.displayName ||
                orp->notificationCount != nr.notificationCount ||
                orp->highlightCount != nr.highlightCount ||
                orp->lastActivityTs != nr.lastActivityTs ||
                orp->membership != nr.membership ||
                orp->hasUnread != nr.hasUnread ||
                orp->isDirect != nr.isDirect ||
                orp->isFavourite != nr.isFavourite ||
                orp->isPublic != nr.isPublic ||
                orp->topic != nr.topic ||
                orp->canonicalAlias != nr.canonicalAlias ||
                orp->avatarUrl != nr.avatarUrl ||
                orp->isEncrypted != nr.isEncrypted ||
                orp->joinedMembers != nr.joinedMembers ||
                orp->invitedMembers != nr.invitedMembers) {
                batch.updated.push_back(nr.roomId);
            }
        }
    }

    // Detect removed rooms
    for (const auto& or_ : oldList) {
        if (newMap.find(or_.roomId) == newMap.end()) {
            batch.removed.push_back(or_.roomId);
        }
    }

    return batch;
}

RoomSummaryUpdateEvent getRoomSummaryUpdateType(
    const RoomSummaryInfo* oldRoom,
    const RoomSummaryInfo* newRoom)
{
    // Original Kotlin: inferred from single-room live data update callback
    if (!oldRoom && newRoom) return RoomSummaryUpdateEvent::ADDED;
    if (oldRoom && !newRoom) return RoomSummaryUpdateEvent::REMOVED;
    if (!oldRoom && !newRoom) return RoomSummaryUpdateEvent::NONE;

    // Both exist — check for material changes
    if (oldRoom->displayName != newRoom->displayName ||
        oldRoom->notificationCount != newRoom->notificationCount ||
        oldRoom->highlightCount != newRoom->highlightCount ||
        oldRoom->lastActivityTs != newRoom->lastActivityTs ||
        oldRoom->membership != newRoom->membership ||
        oldRoom->hasUnread != newRoom->hasUnread ||
        oldRoom->isDirect != newRoom->isDirect ||
        oldRoom->isFavourite != newRoom->isFavourite ||
        oldRoom->topic != newRoom->topic ||
        oldRoom->canonicalAlias != newRoom->canonicalAlias ||
        oldRoom->isEncrypted != newRoom->isEncrypted ||
        oldRoom->joinedMembers != newRoom->joinedMembers ||
        oldRoom->invitedMembers != newRoom->invitedMembers) {
        return RoomSummaryUpdateEvent::UPDATED;
    }

    return RoomSummaryUpdateEvent::NONE;
}

} // namespace progressive
