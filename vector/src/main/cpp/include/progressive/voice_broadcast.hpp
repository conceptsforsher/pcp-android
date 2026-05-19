#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <functional>
#include <utility>

namespace progressive {

// ================================================================
// Voice Broadcast Models & Services
//
// Implements io.element.voicebroadcast.* state events (Element calls).
// Ref: https://github.com/vector-im/element-call/blob/main/doc/voice-broadcasts.md
//
// Original Kotlin: MessageType.MSGTYPE_VOICE_BROADCAST_INFO
//   "io.element.voicebroadcast.info"
// ================================================================

// ==== VoiceBroadcastState ====
//
// Original Kotlin: voicebroadcast/VoiceBroadcastState.kt
enum class VoiceBroadcastState {
    NOT_STARTED = 0,   // "not_started"
    STARTED = 1,       // "started"
    PAUSED = 2,        // "paused"
    RESUMED = 3,       // "resumed"
    STOPPED = 4        // "stopped"
};

const char* voiceBroadcastStateToString(VoiceBroadcastState s);
VoiceBroadcastState voiceBroadcastStateFromString(const std::string& s);

// ==== VoiceBroadcastPlaybackState ====
//
// Original Kotlin: voicebroadcast/VoiceBroadcastPlaybackState.kt
enum class VoiceBroadcastPlaybackState {
    STOPPED = 0,    // playback stopped
    PLAYING = 1,    // actively playing
    PAUSED = 2,     // playback paused
    BUFFERING = 3   // loading/buffering audio
};

const char* voiceBroadcastPlaybackStateToString(VoiceBroadcastPlaybackState s);
VoiceBroadcastPlaybackState voiceBroadcastPlaybackStateFromString(const std::string& s);

// ==== VoiceBroadcastChunk ====
//
// Original Kotlin: voicebroadcast/model/VoiceBroadcastChunk.kt
struct VoiceBroadcastChunk {
    int sequenceNumber = 0;              // chunk sequence number
    std::string mxcUrl;                  // MXC URI for the audio chunk
    int64_t durationMs = 0;              // duration in milliseconds
    std::vector<int> waveform;           // waveform data (amplitude samples)

    bool isValid() const { return sequenceNumber > 0 && !mxcUrl.empty(); }
};

// ==== VoiceBroadcastRecorder ====
//
// Original Kotlin: voicebroadcast/VoiceBroadcastRecorder.kt
struct VoiceBroadcastRecorder {
    bool isRecording = false;
    std::string deviceId;
    int64_t startedAt = 0;
    int chunkSequence = 0;
    int64_t lastChunkDuration = 0;
};

// ==== VoiceBroadcastPlaylist ====
//
// Original Kotlin: voicebroadcast/VoiceBroadcastPlaylist.kt
struct VoiceBroadcastPlaylist {
    std::vector<VoiceBroadcastChunk> chunks;
    int currentIndex = 0;
    int64_t totalDuration = 0;
};

// ==== VoiceBroadcastInfo ====
//
// Original Kotlin: voicebroadcast/model/VoiceBroadcastInfo.kt
struct VoiceBroadcastInfo {
    std::string voiceBroadcastId;        // unique broadcast identifier
    std::string deviceId;                // device that created this
    VoiceBroadcastState state = VoiceBroadcastState::NOT_STARTED;
    int lastChunkSequence = 0;           // latest chunk sequence number
    int64_t startedTimestamp = 0;        // when broadcast started (epoch ms)
    int chunkLength = 0;                 // chunk length in ms
    std::vector<VoiceBroadcastChunk> chunks; // all chunks in this broadcast

    bool isActive() const {
        return state == VoiceBroadcastState::STARTED ||
               state == VoiceBroadcastState::RESUMED;
    }
};

// ==== VoiceBroadcastEvent ====
//
// Original Kotlin: voicebroadcast/model/VoiceBroadcastEvent.kt
struct VoiceBroadcastEvent {
    std::string eventId;                 // Matrix event ID
    std::string roomId;                  // room where event was sent
    std::string senderId;                // sender user ID
    std::string deviceId;                // sender device ID
    std::string voiceBroadcastId;        // broadcast ID (from content)
    VoiceBroadcastState state = VoiceBroadcastState::NOT_STARTED;
    int chunkSequence = 0;               // chunk sequence (if chunk event)
    VoiceBroadcastChunk chunk;           // chunk data (if chunk event)
    int64_t timestamp = 0;               // event timestamp (epoch ms)

    bool isInfoEvent() const { return chunkSequence == 0; }
    bool isChunkEvent() const { return chunkSequence > 0; }
};

// ==== VoiceBroadcastInfoContent ====
//
// Original Kotlin: voicebroadcast/model/VoiceBroadcastInfoContent.kt
// State event content for "io.element.voicebroadcast.info"
struct VoiceBroadcastInfoContent {
    std::string voiceBroadcastId;        // "voice_broadcast_id" key
    std::string deviceId;                // "device_id" key
    std::string state;                   // "state" key
    int lastChunkSequence = 0;           // "last_chunk_sequence" key
    int64_t startedTimestamp = 0;        // "started_timestamp" key (epoch ms)
    int chunkLength = 0;                 // "chunk_length" key (ms)
};

// ==== VoiceBroadcastChunkContent ====
//
// Original Kotlin: voicebroadcast/model/VoiceBroadcastChunkContent.kt
// Chunk event content — sent per chunk as a message-like event
struct VoiceBroadcastChunkContent {
    int sequenceNumber = 0;              // "sequence_number" key
    std::string voiceBroadcastId;        // "voice_broadcast_id" key
    std::string mxcUrl;                  // "mxc_url" key (or use m.file url)
    int64_t durationMs = 0;              // "duration" key (ms)
    std::vector<int> waveform;           // "waveform" key (array of ints)
};

// ==== VoiceBroadcastSummary ====
//
// Original Kotlin: voicebroadcast/VoiceBroadcastSummary.kt
// Computed aggregate for timeline display
struct VoiceBroadcastSummary {
    std::string broadcastId;             // voice broadcast ID
    std::string creatorId;               // MXID of creator
    std::string creatorName;             // display name of creator
    VoiceBroadcastState state = VoiceBroadcastState::NOT_STARTED;
    int totalChunks = 0;                 // total number of chunks
    int64_t totalDurationMs = 0;         // total duration in ms
    int64_t startedTs = 0;               // actual start timestamp (ms)
    int64_t endedTs = 0;                 // actual end timestamp (ms)

    bool isLive() const {
        return state == VoiceBroadcastState::STARTED ||
               state == VoiceBroadcastState::RESUMED;
    }
    int64_t recordedDurationMs() const {
        if (startedTs > 0 && endedTs > startedTs)
            return endedTs - startedTs;
        return totalDurationMs;
    }
};

// ==== VoiceBroadcastListener ====
//
// Original Kotlin: voicebroadcast/VoiceBroadcastListener.kt
// Callback interface for voice broadcast state changes.
struct VoiceBroadcastListener {
    std::function<void(const std::string& broadcastId, VoiceBroadcastPlaybackState state)> onPlaybackStateChanged;
    std::function<void(const std::string& broadcastId, int chunkIndex)> onChunkPlayed;
    std::function<void(const std::string& broadcastId, const std::string& error)> onError;
    std::function<void(const std::string& broadcastId, int64_t positionMs, int64_t durationMs)> onProgress;
};

// ================================================================
// Voice Broadcast Functions
// ================================================================

// ---- State Computation ----

// Determine overall broadcast state from timeline events.
// Scans events and returns the aggregate VoiceBroadcastState.
VoiceBroadcastState computeVoiceBroadcastState(const std::vector<VoiceBroadcastEvent>& events);

// ---- JSON Builders ----

// Build "io.element.voicebroadcast.info" state event content JSON.
std::string buildVoiceBroadcastInfoContent(const VoiceBroadcastInfoContent& info);

// Build chunk event content JSON.
std::string buildVoiceBroadcastChunkContent(const VoiceBroadcastChunkContent& chunk);

// Build a full chunk event JSON for sending over the wire.
// Original Kotlin: voicebroadcast/VoiceBroadcastChunkEvent.kt
std::string buildVoiceBroadcastChunkEvent(const std::string& roomId,
                                          const std::string& broadcastId,
                                          const VoiceBroadcastChunkContent& chunk);

// ---- Parsers ----

// Parse voice broadcast info from state event content JSON.
VoiceBroadcastInfoContent parseVoiceBroadcastInfo(const std::string& json);

// Parse voice broadcast chunk from chunk event content JSON.
VoiceBroadcastChunkContent parseVoiceBroadcastChunk(const std::string& json);

// Parse a full VoiceBroadcastInfo from events.
VoiceBroadcastInfo parseVoiceBroadcastInfoJson(const std::string& json);

// Parse a complete chunk event from a message event JSON.
// Original Kotlin: VoiceBroadcastChunkEvent.kt parser
VoiceBroadcastChunkContent parseVoiceBroadcastChunkEvent(const std::string& eventJson);

// ---- Playback Controls ----
//
// Original Kotlin: VoiceBroadcastPlayer.kt

// Start playback of a voice broadcast.
bool playVoiceBroadcast(const std::string& broadcastId);

// Pause current voice broadcast playback.
bool pauseVoiceBroadcast(const std::string& broadcastId);

// Stop current voice broadcast playback.
bool stopVoiceBroadcast(const std::string& broadcastId);

// Seek to a position in the current voice broadcast.
bool seekVoiceBroadcast(const std::string& broadcastId, int64_t positionMs);

// Get playback progress for a voice broadcast.
std::pair<int64_t, int64_t> getVoiceBroadcastProgress(const std::string& broadcastId);

// ---- Helpers ----

// Format total broadcast duration as "MM:SS" or "HH:MM:SS".
std::string formatVoiceBroadcastDuration(int64_t totalMs);

// Assemble all chunks into a timeline ordered by sequence number.
std::vector<VoiceBroadcastChunk> assembleVoiceBroadcastChunks(
    const std::vector<VoiceBroadcastChunkContent>& chunks);

// Check if an event type string corresponds to a voice broadcast event.
bool isVoiceBroadcastEvent(const std::string& eventType);

// Extract the voice broadcast ID from event content JSON.
std::string getVoiceBroadcastId(const std::string& eventContentJson);

// ---- Aggregation ----

// Compute a VoiceBroadcastSummary from timeline events.
VoiceBroadcastSummary aggregateVoiceBroadcast(const std::vector<VoiceBroadcastEvent>& events);

// ---- Current User Checks ----
//
// Original Kotlin: voicebroadcast/VoiceBroadcastHelper.kt

// Check if the current user is actively broadcasting.
bool isCurrentUserBroadcasting(const std::string& currentUserId,
                               const std::vector<VoiceBroadcastEvent>& events);

// Check if the current user is eligible to start a new voice broadcast.
bool canStartVoiceBroadcast(const std::string& currentUserId,
                            const std::string& roomId,
                            const std::vector<VoiceBroadcastEvent>& events);

} // namespace progressive
