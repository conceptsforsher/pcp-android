#include "progressive/pin_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== JSON helpers ======

static std::string extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

static bool extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

PinManagerFull::PinManagerFull() {}

// ====== Helpers ======

std::vector<std::string>& PinManagerFull::getPinnedIds(const std::string& roomId) {
    return pinned_[roomId];
}

std::string PinManagerFull::buildPreview(const std::string& body, const std::string& senderName,
                                      const std::string& eventType, bool hasImage,
                                      bool hasFile, bool hasVideo) const {
    std::string sender = senderName.empty() ? "" : senderName + ": ";

    if (hasImage) return sender + "[Image]";
    if (hasVideo) return sender + "[Video]";
    if (hasFile) return sender + "[File]";
    if (eventType == "m.sticker") return sender + "[Sticker]";

    // Truncate body to max 80 chars
    std::string text = body;
    // Remove newlines for preview
    std::string clean;
    for (char c : text) {
        if (c == '\n') clean += ' ';
        else clean += c;
    }

    if (clean.size() > 80) clean = clean.substr(0, 77) + "...";
    return sender + clean;
}

// ====== Pin/Unpin Lifecycle ======

std::string PinManagerFull::pinEvent(const std::string& roomId, const std::string& eventId,
                                  const std::string& pinnedBy, int userPowerLevel,
                                  std::string& error) {
    if (!canManagePins(userPowerLevel)) {
        error = "Power level " + std::to_string(userPowerLevel) +
                " insufficient (need " + std::to_string(REQUIRED_POWER_LEVEL) + ")";
        return "";
    }

    auto& ids = getPinnedIds(roomId);

    // Check if already pinned
    for (const auto& id : ids) {
        if (id == eventId) {
            error = "Event already pinned";
            return "";
        }
    }

    // Check max limit
    if (static_cast<int>(ids.size()) >= MAX_PINNED) {
        error = "Maximum pinned events (" + std::to_string(MAX_PINNED) + ") reached";
        return "";
    }

    ids.push_back(eventId);

    // Store metadata
    PinnedEventInfo info;
    info.eventId = eventId;
    info.roomId = roomId;
    info.pinnedBy = pinnedBy;
    info.pinnedAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    info.displayOrder = static_cast<int>(ids.size());
    metadata_[roomId][eventId] = info;

    return buildPinnedEventsContent(ids);
}

std::string PinManagerFull::unpinEvent(const std::string& roomId, const std::string& eventId,
                                    const std::string& removedBy, int userPowerLevel,
                                    std::string& error) {
    if (!canManagePins(userPowerLevel)) {
        error = "Power level " + std::to_string(userPowerLevel) +
                " insufficient (need " + std::to_string(REQUIRED_POWER_LEVEL) + ")";
        return "";
    }

    auto& ids = getPinnedIds(roomId);

    auto it = std::find(ids.begin(), ids.end(), eventId);
    if (it == ids.end()) {
        error = "Event not pinned";
        return "";
    }

    ids.erase(it);

    // Update display order
    for (size_t i = 0; i < ids.size(); i++) {
        auto mi = metadata_[roomId].find(ids[i]);
        if (mi != metadata_[roomId].end()) mi->second.displayOrder = static_cast<int>(i);
    }

    // Remove metadata
    metadata_[roomId].erase(eventId);

    return buildPinnedEventsContent(ids);
}

std::string PinManagerFull::togglePin(const std::string& roomId, const std::string& eventId,
                                   const std::string& userId, int userPowerLevel,
                                   std::string& error) {
    if (isEventPinned(roomId, eventId)) {
        return unpinEvent(roomId, eventId, userId, userPowerLevel, error);
    }
    return pinEvent(roomId, eventId, userId, userPowerLevel, error);
}

// ====== State Loading ======

void PinManagerFull::loadState(const std::string& roomId, const std::string& stateContentJson) {
    auto eventIds = parsePinnedEventIds(stateContentJson);
    auto& ids = getPinnedIds(roomId);
    ids.clear();

    for (size_t i = 0; i < eventIds.size(); i++) {
        ids.push_back(eventIds[i]);
        PinnedEventInfo info;
        info.eventId = eventIds[i];
        info.roomId = roomId;
        info.displayOrder = static_cast<int>(i);
        metadata_[roomId][eventIds[i]] = info;
    }
}

void PinManagerFull::setEventMetadata(const std::string& roomId, const std::string& eventId,
                                   const std::string& senderId, const std::string& senderName,
                                   const std::string& body, const std::string& eventType,
                                   int64_t originalTimestampMs, bool isEncrypted) {
    auto ri = metadata_.find(roomId);
    if (ri == metadata_.end()) return;

    auto ei = ri->second.find(eventId);
    if (ei == ri->second.end()) return;

    ei->second.senderId = senderId;
    ei->second.senderName = senderName;
    ei->second.body = body;
    ei->second.eventType = eventType;
    ei->second.originalTimestampMs = originalTimestampMs;
    ei->second.isEncrypted = isEncrypted;

    // Detect attachments
    auto lbody = body;
    std::transform(lbody.begin(), lbody.end(), lbody.begin(), ::tolower);
    ei->second.hasImage = body.find("\"url\":\"mxc://") != std::string::npos &&
                          (body.find("\"msgtype\":\"m.image\"") != std::string::npos ||
                           lbody.find(".jpg") != std::string::npos ||
                           lbody.find(".png") != std::string::npos);
    ei->second.hasFile = body.find("\"msgtype\":\"m.file\"") != std::string::npos;
    ei->second.hasVideo = body.find("\"msgtype\":\"m.video\"") != std::string::npos;

    // Build preview
    ei->second.previewText = buildPreview(body, senderName, eventType,
                                           ei->second.hasImage, ei->second.hasFile,
                                           ei->second.hasVideo);
}

// ====== Queries ======

std::vector<PinnedEventInfo> PinManagerFull::getPinnedEvents(const std::string& roomId) const {
    std::vector<PinnedEventInfo> result;
    auto pi = pinned_.find(roomId);
    if (pi == pinned_.end()) return result;

    auto mi = metadata_.find(roomId);
    if (mi == metadata_.end()) return result;

    for (const auto& eventId : pi->second) {
        auto ei = mi->second.find(eventId);
        if (ei != mi->second.end()) {
            result.push_back(ei->second);
        }
    }

    // Sort by display order (newest first = reverse)
    std::sort(result.begin(), result.end(), [](const PinnedEventInfo& a, const PinnedEventInfo& b) {
        return a.displayOrder > b.displayOrder; // newest first
    });

    return result;
}

bool PinManagerFull::isEventPinned(const std::string& roomId, const std::string& eventId) const {
    auto pi = pinned_.find(roomId);
    if (pi == pinned_.end()) return false;
    return std::find(pi->second.begin(), pi->second.end(), eventId) != pi->second.end();
}

int PinManagerFull::getPinnedCount(const std::string& roomId) const {
    auto pi = pinned_.find(roomId);
    if (pi == pinned_.end()) return 0;
    return static_cast<int>(pi->second.size());
}

bool PinManagerFull::canManagePins(int userPowerLevel) const {
    return userPowerLevel >= REQUIRED_POWER_LEVEL;
}

// ====== Formatting ======

std::string PinManagerFull::formatPinnedList(const std::string& roomId) const {
    auto events = getPinnedEvents(roomId);
    std::ostringstream os;
    os << events.size() << " pinned message" << (events.size() != 1 ? "s" : "") << "\n";
    for (size_t i = 0; i < events.size(); i++) {
        os << (i + 1) << ". " << events[i].previewText << "\n";
    }
    return os.str();
}

std::string PinManagerFull::formatPinnedPreview(const PinnedEventInfo& event) const {
    return event.previewText.empty() ? event.body.substr(0, 80) : event.previewText;
}

// ====== Serialization ======

std::string PinManagerFull::pinnedEventsToJson(const std::string& roomId) const {
    auto events = getPinnedEvents(roomId);
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"room_id":")" << esc(roomId)
       << R"(","pinned_count":)" << events.size()
       << R"(,"events":[)";
    for (size_t i = 0; i < events.size(); i++) {
        if (i > 0) os << ",";
        os << R"({"event_id":")" << esc(events[i].eventId)
           << R"(","sender_name":")" << esc(events[i].senderName)
           << R"(","preview":")" << esc(events[i].previewText)
           << R"(","pinned_by":")" << esc(events[i].pinnedBy)
           << R"(,"type":")" << esc(events[i].eventType)
           << R"(,"has_image":)" << (events[i].hasImage ? "true" : "false")
           << "}";
    }
    os << "]}";
    return os.str();
}

std::string PinManagerFull::buildPinnedEventsContent(const std::vector<std::string>& eventIds) {
    std::ostringstream os;
    os << R"({"pinned":[)";
    for (size_t i = 0; i < eventIds.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << eventIds[i] << "\"";
    }
    os << "]}";
    return os.str();
}

std::vector<std::string> PinManagerFull::parsePinnedEventIds(const std::string& stateContentJson) {
    std::vector<std::string> ids;
    size_t pos = stateContentJson.find("\"pinned\"");
    if (pos == std::string::npos) return ids;

    pos = stateContentJson.find('[', pos);
    if (pos == std::string::npos) return ids;

    pos++;
    while (pos < stateContentJson.size()) {
        while (pos < stateContentJson.size() && (stateContentJson[pos] == ' ' || stateContentJson[pos] == ',')) pos++;
        if (pos >= stateContentJson.size() || stateContentJson[pos] == ']') break;
        if (stateContentJson[pos] == '"') {
            pos++;
            size_t e = pos;
            while (e < stateContentJson.size() && stateContentJson[e] != '"') e++;
            ids.push_back(stateContentJson.substr(pos, e - pos));
            pos = e + 1;
        } else pos++;
    }

    return ids;
}

int PinManagerFull::totalPinnedEvents() const {
    int total = 0;
    for (const auto& [roomId, ids] : pinned_) total += static_cast<int>(ids.size());
    return total;
}

} // namespace progressive
