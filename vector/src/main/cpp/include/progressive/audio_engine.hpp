#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ---- Existing APIs ----

struct AudioTrack {
    std::string url;
    std::string title;
    std::string roomId;
    std::string eventId;
    int64_t durationMs = 0;
    int64_t positionMs = 0;
    bool isPlaying = false;
    bool isPaused = false;
    float volume = 1.0f;
    float speed = 1.0f;
};

struct AudioState {
    AudioTrack currentTrack;
    bool isActive = false;
    bool isBackground = false;
    float progress = 0.0f;
};

std::string formatDuration(int64_t ms);
std::string formatPositionInfo(int64_t positionMs, int64_t durationMs);
float computeProgress(int64_t positionMs, int64_t durationMs);
bool isSupportedAudioType(const std::string& mimeType);
std::string mimeToExtension(const std::string& mimeType);

// ============================================================
// AUDIO FORMAT — detect and classify audio file formats
// Original Kotlin:
//   Audio format detection from headers / MIME / extension
// ============================================================

enum class AudioFormat {
    MP3,
    AAC,
    OGG_VORBIS,
    OPUS,
    FLAC,
    WAV,
    WMA,
    M4A,
    AMR,
    UNKNOWN
};

// Detect audio format from a filename or file extension.
AudioFormat detectAudioFormat(const std::string& filenameOrPath);

// Detect audio format from raw file header bytes (first N bytes of file).
AudioFormat detectAudioFormat(const std::vector<uint8_t>& headerBytes);

// Convert AudioFormat to a human-readable string.
std::string audioFormatToString(AudioFormat fmt);

// Convert AudioFormat to its primary MIME type.
std::string audioFormatToMime(AudioFormat fmt);

// ============================================================
// AUDIO METADATA — ID3 and container metadata parsing
// ============================================================

struct AudioMetadata {
    std::string title;
    std::string artist;
    std::string album;
    std::string genre;
    int year = 0;
    int trackNumber = 0;
    int64_t durationMs = 0;
    int bitRate = 0;       // kbps
    int sampleRate = 0;    // Hz
    int channels = 0;
    std::string codec;     // "mp3", "aac", etc.
    int64_t fileSize = 0;  // bytes
    AudioFormat format = AudioFormat::UNKNOWN;
    bool valid = false;
};

// Parse audio metadata from file bytes or tagged metadata JSON.
// Handles common ID3v1/v2 header parsing for MP3, FLAC metadata blocks,
// and basic WAV/OGG header extraction.
AudioMetadata parseAudioMetadata(const std::vector<uint8_t>& rawBytes);

// Parse a minimal metadata struct from a JSON representation.
AudioMetadata parseAudioMetadata(const std::string& json);

// ============================================================
// AUDIO WAVEFORM — visual waveform data generation
// ============================================================

struct AudioWaveform {
    std::vector<float> samples;     // normalized amplitude samples (−1.0 to +1.0)
    float maxAmplitude = 0.0f;
    int64_t durationMs = 0;
    int sampleRate = 0;
    int numSamples = 0;
};

// Generate a waveform from raw audio PCM samples.
AudioWaveform generateWaveform(const std::vector<int16_t>& pcmSamples,
                                 int sampleRate, int64_t durationMs,
                                 int targetNumSamples = 100);

// Scale a waveform to a target number of sample points.
AudioWaveform scaleWaveform(const AudioWaveform& waveform, int targetNumSamples);

// ============================================================
// AUDIO PLAYER — playback state management
// ============================================================

enum class AudioPlayerState {
    STOPPED,
    PLAYING,
    PAUSED,
    BUFFERING,
    ERROR
};

struct AudioPlayerInfo {
    AudioPlayerState state = AudioPlayerState::STOPPED;
    int64_t currentPositionMs = 0;
    int64_t durationMs = 0;
    bool isLooping = false;
    float volume = 1.0f;
    float playbackSpeed = 1.0f;
    std::string errorMessage;
};

// Format audio duration for display: "m:ss" or "h:mm:ss".
std::string formatAudioDuration(int64_t ms);

// Format full position info: "1:23 / 3:45".
std::string formatAudioPositionInfo(int64_t positionMs, int64_t durationMs);

// ============================================================
// AUDIO RECORDER — voice message recording
// ============================================================

enum class AudioRecorderState {
    IDLE,
    RECORDING,
    PAUSED
};

struct AudioRecorderConfig {
    AudioFormat format = AudioFormat::AAC;
    int sampleRate = 44100;
    int bitRate = 128000;      // kbps
    int channels = 1;          // mono for voice
    int64_t maxDurationMs = 300000; // 5 minutes default
    bool useVoiceProcessing = true;
};

// ============================================================
// AUDIO SESSION — combined recording session metadata
// ============================================================

struct AudioSession {
    std::string sessionId;
    AudioRecorderState state = AudioRecorderState::IDLE;
    AudioFormat format = AudioFormat::AAC;
    int64_t durationMs = 0;
    int64_t fileSize = 0;
    AudioWaveform waveform;
    AudioMetadata metadata;
};

// Check if a voice message job is for an audio message.
bool isAudioVoiceMessage(const std::string& mimeType);

// Build an AudioPlayerInfo from a JSON string.
AudioPlayerInfo buildAudioInfo(const std::string& json);

// Parse AudioPlayerInfo to a JSON string.
std::string parseAudioInfo(const AudioPlayerInfo& info);

// ============================================================
// AUDIO PLAYBACK SPEED — speed presets for voice messages
// ============================================================

enum class AudioPlaybackSpeed {
    HALF = 0,      // 0.5x
    NORMAL = 1,    // 1.0x
    ONE_POINT_FIVE = 2, // 1.5x
    DOUBLE = 3     // 2.0x
};

// Get the speed multiplier from the enum value.
float playbackSpeedValue(AudioPlaybackSpeed speed);

// Get the display label for a playback speed.
std::string playbackSpeedLabel(AudioPlaybackSpeed speed);

// ============================================================
// AUDIO VISUALIZER — frequency/time domain analysis
// ============================================================

namespace AudioVisualizer {
    // Compute frequency-domain data via DFT (simple FFT-like binning).
    // Returns magnitudes for `numBins` frequency bins.
    std::vector<float> computeFrequencyData(const std::vector<int16_t>& pcmSamples,
                                              int sampleRate, int numBins = 64);

    // Compute time-domain amplitude data (windowed RMS).
    // Returns amplitude envelope with `numPoints` samples.
    std::vector<float> computeTimeData(const std::vector<int16_t>& pcmSamples,
                                         int numPoints = 100);
}

} // namespace progressive
