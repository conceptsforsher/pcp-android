#include "progressive/sync_filter.hpp"
#include "progressive/sync_utils.hpp"
#include <sstream>

namespace progressive {

// ---- RoomEventFilter ----
// Original Kotlin (RoomSyncFilterBuilder.kt:RoomEventFilter):
//   data class RoomEventFilter(
//       val enableUnreadThreadNotifications: Boolean? = null,
//       val lazyLoadMembers: Boolean? = null,
//       val types: List<String>? = null,
//       val notTypes: List<String>? = null
//   )

bool RoomEventFilter::hasData() const {
    return lazyLoadMembers.has_value() ||
           enableUnreadThreadNotifications.has_value() ||
           !types.empty();
}

std::string RoomEventFilter::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (lazyLoadMembers.has_value()) {
        json << R"("lazy_load_members": )" << (*lazyLoadMembers ? "true" : "false");
        first = false;
    }

    if (enableUnreadThreadNotifications.has_value()) {
        if (!first) json << ",";
        json << R"("unread_thread_notifications": )" << (*enableUnreadThreadNotifications ? "true" : "false");
        first = false;
    }

    if (!types.empty()) {
        if (!first) json << ",";
        json << R"("types": [)";
        for (size_t i = 0; i < types.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << types[i] << R"(")";
        }
        json << "]";
        first = false;
    }

    json << "}";
    return json.str();
}

// ---- RoomFilter ----
// Original Kotlin: RoomFilter(timeline, state)

bool RoomFilter::hasData() const {
    return timeline.hasData() || state.hasData();
}

std::string RoomFilter::toJson() const {
    // Only output if there's actual data
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (timeline.hasData()) {
        json << R"("timeline": )" << timeline.toJson();
        first = false;
    }

    if (state.hasData()) {
        if (!first) json << ",";
        json << R"("state": )" << state.toJson();
        first = false;
    }

    json << "}";
    return json.str();
}

// ---- RoomSyncFilter ----
// Original Kotlin (Filter): data class Filter(val room: RoomFilter? = null, ...)

bool RoomSyncFilter::hasData() const {
    return room.hasData() || !eventFields.empty() || !eventFormat.empty();
}

std::string RoomSyncFilter::toJson() const {
    if (!hasData()) return "{}";

    std::ostringstream json;
    json << "{";
    bool first = true;

    if (room.hasData()) {
        json << R"("room": )" << room.toJson();
        first = false;
    }

    if (!eventFields.empty()) {
        if (!first) json << ",";
        json << R"("event_fields": [)";
        for (size_t i = 0; i < eventFields.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << eventFields[i] << R"(")";
        }
        json << "]";
        first = false;
    }

    if (!eventFormat.empty()) {
        if (!first) json << ",";
        json << R"("event_format": ")" << eventFormat << R"(")";
    }

    json << "}";
    return json.str();
}

// ---- RoomSyncFilterBuilder logic (from RoomSyncFilterBuilder.kt:57-94) ----
// Original Kotlin:
//   fun build(homeServerCapabilities: HomeServerCapabilities): Filter {
//       return Filter(room = buildRoomFilter(homeServerCapabilities))
//   }
//   private fun buildRoomFilter(caps): RoomFilter {
//       return RoomFilter(timeline = buildTimelineFilter(caps), state = buildStateFilter())
//   }
//   private fun buildTimelineFilter(caps): RoomEventFilter? {
//       val resolvedUseThreadNotifications = if (caps.canUseThreadReadReceiptsAndNotifications)
//           useThreadNotifications else null
//       return RoomEventFilter(
//           enableUnreadThreadNotifications = resolvedUseThreadNotifications,
//           lazyLoadMembers = lazyLoadMembersForMessageEvents
//       ).orNullIfEmpty()
//   }
//   private fun buildStateFilter(): RoomEventFilter? =
//       RoomEventFilter(lazyLoadMembers = lazyLoadMembersForStateEvents, types = supportedStateTypes).orNullIfEmpty()

RoomSyncFilter buildRoomSyncFilter(
    const SyncFilterParams& params,
    bool canUseThreadReadReceiptsAndNotifications
) {
    RoomSyncFilter filter;

    // Build timeline filter
    // Original: lazyLoadMembers = lazyLoadMembersForMessageEvents
    if (params.lazyLoadMembersForMessageEvents.has_value()) {
        filter.room.timeline.lazyLoadMembers = params.lazyLoadMembersForMessageEvents;
    }

    // Original: enableUnreadThreadNotifications = if (caps.canUseThread...) useThreadNotifications else null
    if (canUseThreadReadReceiptsAndNotifications && params.useThreadNotifications.has_value()) {
        filter.room.timeline.enableUnreadThreadNotifications = params.useThreadNotifications;
    }

    // Original: timeline event types
    filter.room.timeline.types = params.listOfSupportedEventTypes;

    // Build state filter
    // Original: lazyLoadMembers = lazyLoadMembersForStateEvents
    if (params.lazyLoadMembersForStateEvents.has_value()) {
        filter.room.state.lazyLoadMembers = params.lazyLoadMembersForStateEvents;
    }

    // Original: state event types
    filter.room.state.types = params.listOfSupportedStateEventTypes;

    // Default event format
    filter.eventFormat = "client";

    return filter;
}

RoomSyncFilter getDefaultRoomSyncFilter() {
    SyncFilterParams params;
    params.lazyLoadMembersForMessageEvents = true;
    params.lazyLoadMembersForStateEvents = true;

    // Default timeline event types to include
    params.listOfSupportedEventTypes = {
        "m.room.message",
        "m.room.member",
        "m.room.name",
        "m.room.topic",
        "m.room.avatar",
        "m.room.canonical_alias",
        "m.room.join_rules",
        "m.room.history_visibility",
        "m.room.guest_access",
        "m.room.power_levels",
        "m.room.redaction",
        "m.room.encryption",
        "m.room.create",
        "m.sticker",
        "m.reaction",
        "m.call.invite",
        "m.call.answer",
        "m.call.hangup"
    };

    // Default state event types
    params.listOfSupportedStateEventTypes = {
        "m.room.name",
        "m.room.topic",
        "m.room.avatar",
        "m.room.canonical_alias",
        "m.room.join_rules",
        "m.room.history_visibility",
        "m.room.guest_access",
        "m.room.power_levels",
        "m.room.encryption",
        "m.room.create",
        "m.room.member",
        "m.room.tombstone"
    };

    return buildRoomSyncFilter(params, false);
}

bool hasActiveFiltering(const RoomSyncFilter& filter) {
    // Original: orNullIfEmpty() — null if no data
    return filter.hasData();
}

std::string syncFilterToJson(const RoomSyncFilter& filter) {
    return filter.toJson();
}

} // namespace progressive
