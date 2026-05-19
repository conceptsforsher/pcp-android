#include "progressive/voice_broadcast.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====================================================================
// VoiceBroadcastState
// ====================================================================

// Original Kotlin: voicebroadcast/VoiceBroadcastState.kt
const char* voiceBroadcastStateToString(VoiceBroadcastState s) {
    switch (s) {
        case VoiceBroadcastState::NOT_STARTED: return "not_started";
        case VoiceBroadcastState::STARTED:     return "started";
        case VoiceBroadcastState::PAUSED:      return "paused";
        case VoiceBroadcastState::RESUMED:     return "resumed";
        case VoiceBroadcastState::STOPPED:     return "stopped";
    }
    return "not_started";
}

VoiceBroadcastState voiceBroadcastStateFromString(const std::string& s) {
    if (s == "not_started") return VoiceBroadcastState::NOT_STARTED;
    if (s == "started")     return VoiceBroadcastState::STARTED;
    if (s == "paused")      return VoiceBroadcastState::PAUSED;
    if (s == "resumed")     return VoiceBroadcastState::RESUMED;
    if (s == "stopped")     return VoiceBroadcastState::STOPPED;
    return VoiceBroadcastState::NOT_STARTED;
}

// ====================================================================
// VoiceBroadcastPlaybackState
// ====================================================================

// Original Kotlin: voicebroadcast/VoiceBroadcastPlaybackState.kt
const char* voiceBroadcastPlaybackStateToString(VoiceBroadcastPlaybackState s) {
    switch (s) {
        case VoiceBroadcastPlaybackState::STOPPED:   return "stopped";
        case VoiceBroadcastPlaybackState::PLAYING:   return "playing";
        case VoiceBroadcastPlaybackState::PAUSED:    return "paused";
        case VoiceBroadcastPlaybackState::BUFFERING: return "buffering";
    }
    return "stopped";
}

VoiceBroadcastPlaybackState voiceBroadcastPlaybackStateFromString(const std::string& s) {
    if (s == "stopped")   return VoiceBroadcastPlaybackState::STOPPED;
    if (s == "playing")   return VoiceBroadcastPlaybackState::PLAYING;
    if (s == "paused")    return VoiceBroadcastPlaybackState::PAUSED;
    if (s == "buffering") return VoiceBroadcastPlaybackState::BUFFERING;
    return VoiceBroadcastPlaybackState::STOPPED;
}

// ====================================================================
// JSON Helpers (local, manual — no dependency on json_parser)
// ====================================================================

static std::string jExtractStr(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    size_t end = pos;
    while (end < json.size() && json[end] != '"') {
        if (json[end] == '\\') end++;
        end++;
    }
    return json.substr(pos, end - pos);
}

static int64_t jExtractInt(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return 0;
    pos = json.find(':', pos);
    if (pos == std::string::npos) return 0;
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size()) return 0;
    int64_t val = 0;
    bool neg = false;
    if (json[pos] == '-') { neg = true; pos++; }
    while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
        val = val * 10 + (json[pos] - '0');
        pos++;
    }
    return neg ? -val : val;
}

static std::string jExtractObj(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r')) pos++;
    if (pos >= json.size() || json[pos] != '{') return "";
    int depth = 1;
    size_t start = pos;
    pos++;
    while (pos < json.size() && depth > 0) {
        if (json[pos] == '{') depth++;
        else if (json[pos] == '}') depth--;
        pos++;
    }
    return json.substr(start, pos - start);
}

static std::string jEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

// ====================================================================
// State Computation
// ====================================================================

// Original Kotlin: computeVoiceBroadcastState()
VoiceBroadcastState computeVoiceBroadcastState(const std::vector<VoiceBroadcastEvent>& events) {
    if (events.empty()) return VoiceBroadcastState::NOT_STARTED;

    // Find the most recent info event (highest timestamp)
    const VoiceBroadcastEvent* lastInfo = nullptr;
    for (const auto& ev : events) {
        if (ev.isInfoEvent()) {
            if (!lastInfo || ev.timestamp > lastInfo->timestamp)
                lastInfo = &ev;
        }
    }
    if (lastInfo) return lastInfo->state;
    return VoiceBroadcastState::NOT_STARTED;
}

// ====================================================================
// JSON Builders
// ====================================================================

// Original Kotlin: buildVoiceBroadcastInfoContent()
std::string buildVoiceBroadcastInfoContent(const VoiceBroadcastInfoContent& info) {
    std::ostringstream os;
    os << "{";
    os << "\"voice_broadcast_id\":\"" << jEscape(info.voiceBroadcastId) << "\"";
    if (!info.deviceId.empty())
        os << ",\"device_id\":\"" << jEscape(info.deviceId) << "\"";
    os << ",\"state\":\"" << jEscape(info.state) << "\"";
    if (info.lastChunkSequence > 0)
        os << ",\"last_chunk_sequence\":" << info.lastChunkSequence;
    if (info.startedTimestamp > 0)
        os << ",\"started_timestamp\":" << info.startedTimestamp;
    if (info.chunkLength > 0)
        os << ",\"chunk_length\":" << info.chunkLength;
    os << "}";
    return os.str();
}

// Original Kotlin: buildVoiceBroadcastChunkContent()
std::string buildVoiceBroadcastChunkContent(const VoiceBroadcastChunkContent& chunk) {
    std::ostringstream os;
    os << "{";
    os << "\"sequence_number\":" << chunk.sequenceNumber;
    if (!chunk.voiceBroadcastId.empty())
        os << ",\"voice_broadcast_id\":\"" << jEscape(chunk.voiceBroadcastId) << "\"";
    if (!chunk.mxcUrl.empty()) {
        os << ",\"url\":\"" << jEscape(chunk.mxcUrl) << "\"";
        os << ",\"mxc_url\":\"" << jEscape(chunk.mxcUrl) << "\"";
    }
    if (chunk.durationMs > 0)
        os << ",\"duration\":" << chunk.durationMs;
    if (!chunk.waveform.empty()) {
        os << ",\"waveform\":[";
        for (size_t i = 0; i < chunk.waveform.size(); i++) {
            if (i > 0) os << ",";
            os << chunk.waveform[i];
        }
        os << "]";
    }
    os << "}";
    return os.str();
}

// Original Kotlin: buildVoiceBroadcastChunkEvent()
std::string buildVoiceBroadcastChunkEvent(const std::string& roomId,
                                          const std::string& broadcastId,
                                          const VoiceBroadcastChunkContent& chunk) {
    std::ostringstream os;
    os << "{";
    os << "\"type\":\"io.element.voicebroadcast.chunk\",";
    os << "\"room_id\":\"" << jEscape(roomId) << "\",";
    os << "\"content\":" << buildVoiceBroadcastChunkContent(chunk);
    os << "}";
    return os.str();
}

// ====================================================================
// Parsers
// ====================================================================

// Original Kotlin: parseVoiceBroadcastInfo()
VoiceBroadcastInfoContent parseVoiceBroadcastInfo(const std::string& json) {
    VoiceBroadcastInfoContent info;
    info.voiceBroadcastId = jExtractStr(json, "voice_broadcast_id");
    info.deviceId = jExtractStr(json, "device_id");
    info.state = jExtractStr(json, "state");
    info.lastChunkSequence = static_cast<int>(jExtractInt(json, "last_chunk_sequence"));
    info.startedTimestamp = jExtractInt(json, "started_timestamp");
    info.chunkLength = static_cast<int>(jExtractInt(json, "chunk_length"));
    // Fallback: state may be in root directly (for aggregate JSON)
    if (info.voiceBroadcastId.empty())
        info.voiceBroadcastId = jExtractStr(json, "voiceBroadcastId");
    return info;
}

// Original Kotlin: parseVoiceBroadcastChunk()
VoiceBroadcastChunkContent parseVoiceBroadcastChunk(const std::string& json) {
    VoiceBroadcastChunkContent chunk;
    chunk.sequenceNumber = static_cast<int>(jExtractInt(json, "sequence_number"));
    chunk.voiceBroadcastId = jExtractStr(json, "voice_broadcast_id");
    chunk.mxcUrl = jExtractStr(json, "mxc_url");
    if (chunk.mxcUrl.empty()) chunk.mxcUrl = jExtractStr(json, "url");
    chunk.durationMs = jExtractInt(json, "duration");

    // Parse waveform array
    auto wfPos = json.find("\"waveform\"");
    if (wfPos != std::string::npos) {
        wfPos = json.find('[', wfPos);
        if (wfPos != std::string::npos) {
            size_t pos = wfPos + 1;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n' || json[pos] == '\r')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                int val = 0;
                bool neg = false;
                if (json[pos] == '-') { neg = true; pos++; }
                while (pos < json.size() && json[pos] >= '0' && json[pos] <= '9') {
                    val = val * 10 + (json[pos] - '0');
                    pos++;
                }
                chunk.waveform.push_back(neg ? -val : val);
            }
        }
    }

    return chunk;
}

// Original Kotlin: parseVoiceBroadcastChunkEvent()
VoiceBroadcastChunkContent parseVoiceBroadcastChunkEvent(const std::string& eventJson) {
    // First try extracting from content envelope
    auto contentJson = jExtractObj(eventJson, "content");
    if (!contentJson.empty()) {
        return parseVoiceBroadcastChunk(contentJson);
    }
    // Fallback: parse top-level directly
    return parseVoiceBroadcastChunk(eventJson);
}

// Original Kotlin: parseVoiceBroadcastInfo() — full aggregate
VoiceBroadcastInfo parseVoiceBroadcastInfoJson(const std::string& json) {
    VoiceBroadcastInfo info;
    info.voiceBroadcastId = jExtractStr(json, "voice_broadcast_id");
    info.deviceId = jExtractStr(json, "device_id");
    info.state = voiceBroadcastStateFromString(jExtractStr(json, "state"));
    info.lastChunkSequence = static_cast<int>(jExtractInt(json, "last_chunk_sequence"));
    info.startedTimestamp = jExtractInt(json, "started_timestamp");
    info.chunkLength = static_cast<int>(jExtractInt(json, "chunk_length"));

    // Parse chunks array if present
    auto chunksArr = json.find("\"chunks\"");
    if (chunksArr != std::string::npos) {
        chunksArr = json.find('[', chunksArr);
        if (chunksArr != std::string::npos) {
            size_t pos = chunksArr + 1;
            while (pos < json.size()) {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n' || json[pos] == '\r')) pos++;
                if (pos >= json.size() || json[pos] == ']') break;
                if (json[pos] == '{') {
                    int depth = 1;
                    size_t start = pos;
                    pos++;
                    while (pos < json.size() && depth > 0) {
                        if (json[pos] == '{') depth++;
                        else if (json[pos] == '}') depth--;
                        pos++;
                    }
                    std::string chunkJson = json.substr(start, pos - start);
                    VoiceBroadcastChunk c;
                    c.sequenceNumber = static_cast<int>(jExtractInt(chunkJson, "sequence_number"));
                    c.mxcUrl = jExtractStr(chunkJson, "mxc_url");
                    if (c.mxcUrl.empty()) c.mxcUrl = jExtractStr(chunkJson, "url");
                    c.durationMs = jExtractInt(chunkJson, "duration");
                    info.chunks.push_back(c);
                } else {
                    break;
                }
            }
        }
    }

    return info;
}

// ====================================================================
// Playback Controls
// ====================================================================

// Original Kotlin: VoiceBroadcastPlayer.play()
bool playVoiceBroadcast(const std::string& broadcastId) {
    (void)broadcastId;
    // Real implementation hooks into audio engine for playback
    return false;
}

// Original Kotlin: VoiceBroadcastPlayer.pause()
bool pauseVoiceBroadcast(const std::string& broadcastId) {
    (void)broadcastId;
    return false;
}

// Original Kotlin: VoiceBroadcastPlayer.stop()
bool stopVoiceBroadcast(const std::string& broadcastId) {
    (void)broadcastId;
    return false;
}

// Original Kotlin: VoiceBroadcastPlayer.seek()
bool seekVoiceBroadcast(const std::string& broadcastId, int64_t positionMs) {
    (void)broadcastId;
    (void)positionMs;
    return false;
}

// Original Kotlin: VoiceBroadcastPlayer.getProgress()
std::pair<int64_t, int64_t> getVoiceBroadcastProgress(const std::string& broadcastId) {
    (void)broadcastId;
    return {0, 0};
}

// ====================================================================
// Helpers
// ====================================================================

// Original Kotlin: formatVoiceBroadcastDuration()
std::string formatVoiceBroadcastDuration(int64_t totalMs) {
    int64_t totalSec = totalMs / 1000;
    int hours = static_cast<int>(totalSec / 3600);
    int mins = static_cast<int>((totalSec % 3600) / 60);
    int secs = static_cast<int>(totalSec % 60);

    std::ostringstream os;
    if (hours > 0) {
        os << hours << ":";
        if (mins < 10) os << "0";
    }
    os << mins << ":";
    if (secs < 10) os << "0";
    os << secs;
    return os.str();
}

// Original Kotlin: assembleVoiceBroadcastChunks()
std::vector<VoiceBroadcastChunk> assembleVoiceBroadcastChunks(
    const std::vector<VoiceBroadcastChunkContent>& chunks) {
    std::vector<VoiceBroadcastChunk> result;
    for (const auto& cc : chunks) {
        VoiceBroadcastChunk c;
        c.sequenceNumber = cc.sequenceNumber;
        c.mxcUrl = cc.mxcUrl;
        c.durationMs = cc.durationMs;
        c.waveform = cc.waveform;
        result.push_back(c);
    }
    // Sort by sequence number
    std::sort(result.begin(), result.end(),
        [](const VoiceBroadcastChunk& a, const VoiceBroadcastChunk& b) {
            return a.sequenceNumber < b.sequenceNumber;
        });
    return result;
}

// Original Kotlin: isVoiceBroadcastEvent()
bool isVoiceBroadcastEvent(const std::string& eventType) {
    return eventType == "io.element.voicebroadcast.info" ||
           eventType == "io.element.voicebroadcast.chunk" ||
           eventType == "org.matrix.android.sdk.voicebroadcast.info";
}

// Original Kotlin: getVoiceBroadcastId()
std::string getVoiceBroadcastId(const std::string& eventContentJson) {
    std::string id = jExtractStr(eventContentJson, "voice_broadcast_id");
    if (id.empty()) id = jExtractStr(eventContentJson, "voiceBroadcastId");
    return id;
}

// ====================================================================
// Current User Checks
// ====================================================================

// Original Kotlin: isCurrentUserBroadcasting()
bool isCurrentUserBroadcasting(const std::string& currentUserId,
                               const std::vector<VoiceBroadcastEvent>& events) {
    for (const auto& ev : events) {
        if (ev.senderId == currentUserId && ev.isInfoEvent() &&
            (ev.state == VoiceBroadcastState::STARTED ||
             ev.state == VoiceBroadcastState::RESUMED)) {
            return true;
        }
    }
    return false;
}

// Original Kotlin: canStartVoiceBroadcast()
bool canStartVoiceBroadcast(const std::string& currentUserId,
                            const std::string& roomId,
                            const std::vector<VoiceBroadcastEvent>& events) {
    for (const auto& ev : events) {
        if (ev.roomId == roomId &&
            ev.senderId == currentUserId &&
            (ev.state == VoiceBroadcastState::STARTED ||
             ev.state == VoiceBroadcastState::RESUMED)) {
            return false;
        }
    }
    return true;
}

// ====================================================================
// Aggregation
// ====================================================================

// Original Kotlin: aggregateVoiceBroadcast()
VoiceBroadcastSummary aggregateVoiceBroadcast(const std::vector<VoiceBroadcastEvent>& events) {
    VoiceBroadcastSummary summary;

    if (events.empty()) return summary;

    summary.state = computeVoiceBroadcastState(events);

    // Collect chunk events and info events
    std::vector<const VoiceBroadcastEvent*> infoEvents;
    std::vector<const VoiceBroadcastEvent*> chunkEvents;

    for (const auto& ev : events) {
        if (summary.broadcastId.empty() && !ev.voiceBroadcastId.empty())
            summary.broadcastId = ev.voiceBroadcastId;
        if (summary.creatorId.empty() && !ev.senderId.empty())
            summary.creatorId = ev.senderId;

        if (ev.isInfoEvent()) {
            infoEvents.push_back(&ev);
        } else if (ev.isChunkEvent()) {
            chunkEvents.push_back(&ev);
        }
    }

    // Total chunks
    summary.totalChunks = static_cast<int>(chunkEvents.size());

    // Total duration from chunks
    for (const auto* ce : chunkEvents) {
        summary.totalDurationMs += ce->chunk.durationMs;
    }

    // Timestamps from info events
    for (const auto* ie : infoEvents) {
        if (ie->timestamp > 0) {
            if (summary.startedTs == 0 || ie->timestamp < summary.startedTs)
                summary.startedTs = ie->timestamp;
            if (ie->timestamp > summary.endedTs)
                summary.endedTs = ie->timestamp;
        }
    }

    // If stopped, use last known info event time as ended
    if (summary.state == VoiceBroadcastState::STOPPED && summary.endedTs == 0) {
        for (auto it = infoEvents.rbegin(); it != infoEvents.rend(); ++it) {
            if ((*it)->state == VoiceBroadcastState::STOPPED) {
                summary.endedTs = (*it)->timestamp;
                break;
            }
        }
    }

    return summary;
}

} // namespace progressive
