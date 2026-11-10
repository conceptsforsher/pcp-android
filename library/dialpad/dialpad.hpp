#pragma once
#include <string>
#include <vector>
#include <functional>

namespace pcp::dialpad {

enum class DialpadKey {
    KEY_0='0', KEY_1='1', KEY_2='2', KEY_3='3', KEY_4='4',
    KEY_5='5', KEY_6='6', KEY_7='7', KEY_8='8', KEY_9='9',
    KEY_STAR='*', KEY_HASH='#',
    KEY_PLUS='+', KEY_PAUSE=',', KEY_WAIT=';'
};

struct DTMFConfig {
    int toneDurationMs = 200;
    int pauseDurationMs = 100;
    int sampleRate = 8000;
    float amplitude = 0.5f;
};

class Dialpad {
public:
    Dialpad();
    ~Dialpad();

    void pressKey(DialpadKey key);
    void pressDigit(char digit);
    void clear();
    void backspace();
    std::string getNumber() const { return m_number; }

    using KeyCallback = std::function<void(DialpadKey)>;
    void onKeyPressed(KeyCallback cb) { m_keyCallback = std::move(cb); }

    using NumberCallback = std::function<void(const std::string&)>;
    void onNumberChanged(NumberCallback cb) { m_numberCallback = std::move(cb); }

    // DTMF tone generation
    std::vector<int16_t> generateTone(DialpadKey key, const DTMFConfig& config = {});
    std::vector<int16_t> generateSequence(const std::string& digits,
                                            const DTMFConfig& config = {});

    // Number formatting
    static std::string formatNumber(const std::string& number);
    static std::string stripFormatting(const std::string& number);
    static bool isValidNumber(const std::string& number);

private:
    std::string m_number;
    KeyCallback m_keyCallback;
    NumberCallback m_numberCallback;

    static std::pair<int, int> dtmfFrequency(DialpadKey key);
};

} // namespace pcp::dialpad
