#include "dialpad.hpp"
#include <cmath>
#include <algorithm>
#include <sstream>
#include <android/log.h>

#define DP_LOG_TAG "Dialpad"
#define DP_LOGI(...) __android_log_print(ANDROID_LOG_INFO, DP_LOG_TAG, __VA_ARGS__)

namespace pcp::dialpad {

Dialpad::Dialpad() = default;
Dialpad::~Dialpad() = default;

void Dialpad::pressKey(DialpadKey key) {
    m_number += static_cast<char>(key);
    if (m_keyCallback) m_keyCallback(key);
    if (m_numberCallback) m_numberCallback(m_number);
}

void Dialpad::pressDigit(char digit) {
    if (digit >= '0' && digit <= '9') {
        pressKey(static_cast<DialpadKey>(digit));
    } else if (digit == '*') {
        pressKey(DialpadKey::KEY_STAR);
    } else if (digit == '#') {
        pressKey(DialpadKey::KEY_HASH);
    } else if (digit == '+') {
        pressKey(DialpadKey::KEY_PLUS);
    }
}

void Dialpad::clear() {
    m_number.clear();
    if (m_numberCallback) m_numberCallback(m_number);
}

void Dialpad::backspace() {
    if (!m_number.empty()) {
        m_number.pop_back();
        if (m_numberCallback) m_numberCallback(m_number);
    }
}

// DTMF frequencies: low and high frequency for each key
std::pair<int, int> Dialpad::dtmfFrequency(DialpadKey key) {
    // DTMF frequency table
    //        1209Hz  1336Hz  1477Hz  1633Hz
    // 697Hz  1       2       3       A
    // 770Hz  4       5       6       B
    // 852Hz  7       8       9       C
    // 941Hz  *       0       #       D

    int lowFreq, highFreq;
    switch (key) {
        case DialpadKey::KEY_1: lowFreq = 697; highFreq = 1209; break;
        case DialpadKey::KEY_2: lowFreq = 697; highFreq = 1336; break;
        case DialpadKey::KEY_3: lowFreq = 697; highFreq = 1477; break;
        case DialpadKey::KEY_4: lowFreq = 770; highFreq = 1209; break;
        case DialpadKey::KEY_5: lowFreq = 770; highFreq = 1336; break;
        case DialpadKey::KEY_6: lowFreq = 770; highFreq = 1477; break;
        case DialpadKey::KEY_7: lowFreq = 852; highFreq = 1209; break;
        case DialpadKey::KEY_8: lowFreq = 852; highFreq = 1336; break;
        case DialpadKey::KEY_9: lowFreq = 852; highFreq = 1477; break;
        case DialpadKey::KEY_0: lowFreq = 941; highFreq = 1336; break;
        case DialpadKey::KEY_STAR: lowFreq = 941; highFreq = 1209; break;
        case DialpadKey::KEY_HASH: lowFreq = 941; highFreq = 1477; break;
        default: return {0, 0};
    }
    return {lowFreq, highFreq};
}

std::vector<int16_t> Dialpad::generateTone(DialpadKey key, const DTMFConfig& config) {
    auto [lowFreq, highFreq] = dtmfFrequency(key);
    if (lowFreq == 0 || highFreq == 0) return {};

    int numSamples = config.sampleRate * config.toneDurationMs / 1000;
    std::vector<int16_t> samples(numSamples);

    for (int i = 0; i < numSamples; i++) {
        double t = static_cast<double>(i) / config.sampleRate;
        double sample = config.amplitude * 0.5 *
            (sin(2.0 * M_PI * lowFreq * t) + sin(2.0 * M_PI * highFreq * t));
        samples[i] = static_cast<int16_t>(sample * 32767.0);
    }
    return samples;
}

std::vector<int16_t> Dialpad::generateSequence(const std::string& digits,
                                                  const DTMFConfig& config) {
    std::vector<int16_t> result;
    int pauseSamples = config.sampleRate * config.pauseDurationMs / 1000;
    std::vector<int16_t> silence(pauseSamples, 0);

    for (char c : digits) {
        auto tone = generateTone(static_cast<DialpadKey>(c), config);
        if (!tone.empty()) {
            result.insert(result.end(), tone.begin(), tone.end());
            result.insert(result.end(), silence.begin(), silence.end());
        }
    }
    return result;
}

std::string Dialpad::formatNumber(const std::string& number) {
    std::string clean = stripFormatting(number);
    // Basic international format
    if (clean.size() >= 10) {
        // Format as: +X (XXX) XXX-XXXX
        return clean;
    }
    return clean;
}

std::string Dialpad::stripFormatting(const std::string& number) {
    std::string result;
    for (char c : number) {
        if (isdigit(c) || c == '+' || c == '*' || c == '#') result += c;
    }
    return result;
}

bool Dialpad::isValidNumber(const std::string& number) {
    std::string clean = stripFormatting(number);
    if (clean.empty()) return false;
    for (char c : clean) {
        if (!isdigit(c) && c != '+' && c != '*' && c != '#') return false;
    }
    return true;
}

} // namespace pcp::dialpad
