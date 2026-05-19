#ifndef PROGRESSIVE_AUDIO_ENGINE_HPP
#define PROGRESSIVE_AUDIO_ENGINE_HPP

#include <string>
#include <cstdint>

namespace progressive {

struct AudioTrack {
    std::string url;          // MXC or HTTP URL
    std::string title;        // track title (sender name or filename)
    std::string roomId;       // which room it came from
    std::string eventId;      // which event
    int64_t durationMs = 0;   // total duration
    int64_t positionMs = 0;   // current position
    bool isPlaying = false;
    bool isPaused = false;
    float volume = 1.0f;
    float speed = 1.0f;
};

struct AudioState {
    AudioTrack currentTrack;
    bool isActive = false;
    bool isBackground = false;  // app is in background
    float progress = 0.0f;      // 0.0 to 1.0
};

// Format duration in milliseconds to "m:ss" or "h:mm:ss"
std::string formatDuration(int64_t ms);

// Format position info: "1:23 / 3:45"
std::string formatPositionInfo(int64_t positionMs, int64_t durationMs);

// Compute progress percentage as float 0.0-1.0
float computeProgress(int64_t positionMs, int64_t durationMs);

// Validate audio MIME type is supported
bool isSupportedAudioType(const std::string& mimeType);

// Extract file extension from MIME type
std::string mimeToExtension(const std::string& mimeType);

} // namespace progressive

#endif // PROGRESSIVE_AUDIO_ENGINE_HPP
