#include "progressive/waveform.hpp"
#include <sstream>
#include <cmath>
#include <algorithm>

namespace progressive {

std::vector<WaveformBar> generateWaveform(
    const std::vector<int16_t>& samples,
    int numBars, int sampleRate
) {
    if (samples.empty() || numBars <= 0) return {};

    int samplesPerBar = static_cast<int>(samples.size()) / numBars;
    if (samplesPerBar < 1) samplesPerBar = 1;

    std::vector<double> amplitudes;
    amplitudes.reserve(numBars);

    for (int i = 0; i < numBars; ++i) {
        int start = i * samplesPerBar;
        int end = std::min(start + samplesPerBar, static_cast<int>(samples.size()));

        // Compute RMS for this segment
        double sumSq = 0.0;
        int count = 0;
        for (int j = start; j < end; ++j) {
            double normalized = static_cast<double>(samples[j]) / 32768.0;
            sumSq += normalized * normalized;
            count++;
        }
        double rms = count > 0 ? std::sqrt(sumSq / count) : 0.0;
        amplitudes.push_back(rms);
    }

    return generateWaveformFromAmplitudes(amplitudes, numBars);
}

std::vector<WaveformBar> generateWaveformFromAmplitudes(
    const std::vector<double>& amplitudes, int numBars
) {
    if (amplitudes.empty()) return {};

    auto normalized = normalizeAmplitudes(amplitudes);

    std::vector<WaveformBar> bars;
    bars.reserve(numBars);

    int barCount = std::min(numBars, static_cast<int>(normalized.size()));
    for (int i = 0; i < barCount; ++i) {
        WaveformBar bar;
        bar.amplitude = normalized[i];
        // Map 0.0-1.0 to bar height 1-10
        bar.barHeight = std::max(1, static_cast<int>(normalized[i] * 10.0 + 0.5));
        bars.push_back(bar);
    }

    return bars;
}

double computeRmsVolume(const std::vector<int16_t>& samples) {
    if (samples.empty()) return 0.0;
    double sumSq = 0.0;
    for (auto s : samples) {
        double normalized = static_cast<double>(s) / 32768.0;
        sumSq += normalized * normalized;
    }
    return std::sqrt(sumSq / samples.size());
}

double computePeakVolume(const std::vector<int16_t>& samples) {
    if (samples.empty()) return 0.0;
    int16_t peak = 0;
    for (auto s : samples) {
        if (std::abs(s) > peak) peak = std::abs(s);
    }
    return static_cast<double>(peak) / 32768.0;
}

bool isSilence(const std::vector<int16_t>& samples, double threshold) {
    return computeRmsVolume(samples) < threshold;
}

std::string waveformToJson(const std::vector<WaveformBar>& bars) {
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < bars.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"h": )" << bars[i].barHeight;
        json << R"(,"a": )" << bars[i].amplitude;
        json << R"(,"active": )" << (bars[i].isActive ? "true" : "false") << "}";
    }
    json << "]";
    return json.str();
}

double computeWaveformProgress(const std::vector<WaveformBar>& bars, int64_t positionMs, int64_t totalMs) {
    if (bars.empty() || totalMs <= 0) return 0.0;
    return static_cast<double>(positionMs) / static_cast<double>(totalMs);
}

void markActiveBars(std::vector<WaveformBar>& bars, int64_t positionMs, int64_t totalMs) {
    if (bars.empty() || totalMs <= 0) return;

    double progress = computeWaveformProgress(bars, positionMs, totalMs);
    int activeCount = static_cast<int>(progress * bars.size());

    for (int i = 0; i < static_cast<int>(bars.size()); ++i) {
        bars[i].isActive = (i < activeCount);
    }
}

int suggestBarCount(int64_t durationMs) {
    // Roughly 2 bars per second, minimum 10, maximum 100
    int bars = static_cast<int>(durationMs / 500);
    return std::max(10, std::min(100, bars));
}

std::vector<double> normalizeAmplitudes(const std::vector<double>& amplitudes) {
    if (amplitudes.empty()) return {};

    double maxAmp = 0.0;
    for (double a : amplitudes) {
        if (a > maxAmp) maxAmp = a;
    }

    if (maxAmp <= 0.0) return std::vector<double>(amplitudes.size(), 0.0);

    std::vector<double> result;
    result.reserve(amplitudes.size());
    for (double a : amplitudes) {
        result.push_back(a / maxAmp);
    }
    return result;
}

// ==== Waveform Sanitizer (from WaveFormSanitizer.kt:36-85) ====
// Original Kotlin:
//   fun sanitize(waveForm: List<Int>?): List<Int>? {
//       // MIN_NUMBER_OF_VALUES = 30, MAX_NUMBER_OF_VALUES = 120, MAX_VALUE = 1024
//       // 1. Extend short lists by repeating values
//       // 2. Trim long lists by subsampling (keep every Nth)
//       // 3. Make all values positive (abs)
//       // 4. Cap max at 1024 and scale down if over

std::vector<int> sanitizeWaveform(const std::vector<int>& waveform) {
    const int MIN_VALUES = 30;
    const int MAX_VALUES = 120;
    const int MAX_VALUE = 1024;

    if (waveform.empty()) return {};

    // Step 1: Adjust size to be in [30, 120]
    std::vector<int> sized;
    if (static_cast<int>(waveform.size()) < MIN_VALUES) {
        // Original: repeat values to reach at least 30
        int repeatTimes = (MIN_VALUES + static_cast<int>(waveform.size()) - 1) / static_cast<int>(waveform.size());
        for (int val : waveform) {
            for (int r = 0; r < repeatTimes; ++r) sized.push_back(val);
        }
    } else if (static_cast<int>(waveform.size()) > MAX_VALUES) {
        // Original: subsample — keep every Nth value
        int keepOneOf = (static_cast<int>(waveform.size()) + MAX_VALUES - 1) / MAX_VALUES;
        for (size_t i = 0; i < waveform.size(); ++i) {
            if (i % keepOneOf == 0) sized.push_back(waveform[i]);
        }
    } else {
        sized = waveform;
    }

    // Step 2: Make all values positive (abs)
    std::vector<int> positive;
    for (int val : sized) positive.push_back(std::abs(val));

    // Step 3: Find max, scale down if > MAX_VALUE
    int maxVal = 0;
    for (int val : positive) if (val > maxVal) maxVal = val;
    if (maxVal == 0) maxVal = MAX_VALUE;

    if (maxVal > MAX_VALUE) {
        // Original: scale down proportionally
        std::vector<int> scaled;
        for (int val : positive) {
            scaled.push_back(val * MAX_VALUE / maxVal);
        }
        return scaled;
    }

    return positive;
}

} // namespace progressive
