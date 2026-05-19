#ifndef PROGRESSIVE_WAVEFORM_HPP
#define PROGRESSIVE_WAVEFORM_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

struct WaveformBar {
    double amplitude = 0.0; // 0.0 to 1.0 normalized
    int barHeight = 0;      // 1-10 height for UI rendering
    bool isActive = false;  // currently being played
};

// Generate waveform bars from raw PCM data (16-bit samples).
// Produces `numBars` bars for UI rendering.
std::vector<WaveformBar> generateWaveform(
    const std::vector<int16_t>& samples,
    int numBars = 40,
    int sampleRate = 44100
);

// Generate waveform from amplitude values (already processed).
std::vector<WaveformBar> generateWaveformFromAmplitudes(
    const std::vector<double>& amplitudes,
    int numBars = 40
);

// Compute RMS (root mean square) volume from PCM samples.
double computeRmsVolume(const std::vector<int16_t>& samples);

// Compute peak volume from PCM samples (max amplitude / 32768).
double computePeakVolume(const std::vector<int16_t>& samples);

// Detect silence in PCM samples (all samples below threshold).
bool isSilence(const std::vector<int16_t>& samples, double threshold = 0.01);

// Format waveform bars as JSON array for UI.
std::string waveformToJson(const std::vector<WaveformBar>& bars);

// Compute playback progress through waveform bars.
double computeWaveformProgress(const std::vector<WaveformBar>& bars, int64_t positionMs, int64_t totalMs);

// Mark active bars based on playback position.
void markActiveBars(std::vector<WaveformBar>& bars, int64_t positionMs, int64_t totalMs);

// Compute optimal bar count for voice message duration.
int suggestBarCount(int64_t durationMs);

// Normalize amplitudes to 0.0-1.0 range.
std::vector<double> normalizeAmplitudes(const std::vector<double>& amplitudes);

// Sanitize waveform values for Matrix voice message spec.
// Faithful port from org.matrix.android.sdk.internal.session.room.send.WaveFormSanitizer.kt (86L)
// Original: sanitize(waveForm: List<Int>?): List<Int>?
//   - Forces 30-120 values (repeats short, subsamples long)
//   - Makes all values positive (abs)
//   - Caps max value at 1024
//   - Scales down proportionally if over max
std::vector<int> sanitizeWaveform(const std::vector<int>& waveform);

} // namespace progressive

#endif // PROGRESSIVE_WAVEFORM_HPP
