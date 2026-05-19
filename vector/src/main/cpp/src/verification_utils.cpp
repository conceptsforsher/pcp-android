#include "progressive/verification_utils.hpp"
#include <sstream>
#include <iomanip>

namespace progressive {

std::vector<VerificationEmoji> getVerificationEmojis() {
    // Standard SAS emoji list from Matrix spec
    return {
        {"🐶", "Dog"}, {"🐱", "Cat"}, {"🦁", "Lion"}, {"🐎", "Horse"},
        {"🦄", "Unicorn"}, {"🐷", "Pig"}, {"🐘", "Elephant"}, {"🐰", "Rabbit"},
        {"🐼", "Panda"}, {"🐓", "Rooster"}, {"🐧", "Penguin"}, {"🐢", "Turtle"},
        {"🐙", "Octopus"}, {"🐳", "Whale"}, {"🦋", "Butterfly"}, {"🌻", "Sunflower"},
        {"🌴", "Palm Tree"}, {"🌵", "Cactus"}, {"🍇", "Grapes"}, {"🍉", "Watermelon"},
        {"🍋", "Lemon"}, {"🍌", "Banana"}, {"🍍", "Pineapple"}, {"🍎", "Red Apple"},
        {"🍒", "Cherries"}, {"🍓", "Strawberry"}, {"🌽", "Corn"}, {"🍕", "Pizza"},
        {"🎂", "Birthday Cake"}, {"🏆", "Trophy"}, {"🎓", "Graduation Cap"},
        {"🎸", "Guitar"}, {"🎺", "Trumpet"}, {"🔔", "Bell"}, {"🎵", "Musical Note"},
        {"🎄", "Christmas Tree"}, {"🎃", "Pumpkin"}, {"🌎", "Earth"}, {"🌙", "Moon"},
        {"☀️", "Sun"}, {"⭐", "Star"}, {"⚡", "Lightning"}, {"🔥", "Fire"},
        {"🌈", "Rainbow"}, {"❄️", "Snowflake"}, {"💧", "Droplet"}, {"🎈", "Balloon"},
        {"🔑", "Key"}, {"🔒", "Lock"}, {"✏️", "Pencil"}, {"📌", "Pin"},
        {"⌚", "Watch"}, {"📷", "Camera"}, {"🔋", "Battery"}, {"💡", "Light Bulb"},
        {"🏁", "Checkered Flag"}, {"🚀", "Rocket"}, {"🚲", "Bicycle"}, {"🚗", "Car"},
        {"⛵", "Sailboat"}, {"✈️", "Airplane"}, {"🚂", "Train"}, {"🚦", "Traffic Light"}
    };
}

VerificationSas computeSasEmojis(const std::string& sasBytes) {
    VerificationSas sas;
    sas.method = "m.sas.v1";
    auto allEmojis = getVerificationEmojis();

    for (size_t i = 0; i < sasBytes.size() && sas.emojis.size() < 7; ++i) {
        unsigned char byte = sasBytes[i];
        // Each byte maps to one of 64 emojis
        int idx = byte & 0x3F; // 0-63
        if (idx < static_cast<int>(allEmojis.size())) {
            sas.emojis.push_back(allEmojis[idx]);
        }
    }

    return sas;
}

std::vector<int> computeSasDecimals(const std::string& sasBytes) {
    std::vector<int> decimals;
    // 3 bytes → 1 three-digit number
    for (size_t i = 0; i + 2 < sasBytes.size(); i += 3) {
        int value = ((unsigned char)sasBytes[i] << 16) |
                    ((unsigned char)sasBytes[i + 1] << 8) |
                    (unsigned char)sasBytes[i + 2];
        int decimal = value % 1000;
        // Format as three digits
        if (decimal < 100) decimal += 1000;
        decimals.push_back(decimal);
        if (decimals.size() >= 7) break;
    }
    return decimals;
}

std::string formatSasEmojis(const VerificationSas& sas) {
    std::ostringstream out;
    for (size_t i = 0; i < sas.emojis.size(); ++i) {
        if (i > 0) out << "  ";
        out << sas.emojis[i].emoji;
    }
    return out.str();
}

std::string formatSasDecimals(const VerificationSas& sas) {
    auto decimals = computeSasDecimals(sas.method); // placeholder
    std::ostringstream out;
    for (size_t i = 0; i < decimals.size(); ++i) {
        if (i > 0) out << " - ";
        out << std::setfill('0') << std::setw(3) << (decimals[i] % 1000);
    }
    return out.str();
}

bool sasMatches(const VerificationSas& a, const VerificationSas& b) {
    if (a.emojis.size() != b.emojis.size()) return false;
    for (size_t i = 0; i < a.emojis.size(); ++i) {
        if (a.emojis[i].emoji != b.emojis[i].emoji) return false;
    }
    return true;
}

VerificationMethod parseVerificationMethod(const std::string& methodStr) {
    if (methodStr == "m.sas.v1") return VerificationMethod::Sas;
    if (methodStr == "m.qr_code.scan.v1" || methodStr == "m.qr_code.show.v1")
        return VerificationMethod::QrCode;
    if (methodStr == "m.reciprocate.v1") return VerificationMethod::Reciprocate;
    return VerificationMethod::Unknown;
}

std::string formatVerificationState(const VerificationState& state) {
    std::ostringstream out;
    out << "Verification with " << state.otherUserId;
    if (state.isDone) out << " — Complete";
    else if (state.isCancelled) out << " — Cancelled";
    else if (state.isStarted) out << " — In progress";
    else if (state.isReady) out << " — Ready to start";
    else out << " — Waiting";
    return out.str();
}

std::string buildVerificationStartBody(const std::string& fromDevice,
    const std::string& transactionId, const std::string& method) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"from_device": ")" << esc(fromDevice) << R"(")";
    json << R"(,"transaction_id": ")" << esc(transactionId) << R"(")";
    json << R"(,"method": ")" << esc(method) << R"(")";
    json << R"(,"timestamp": )" << std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    json << "}";
    return json.str();
}

std::string buildVerificationMacBody(const std::string& transactionId,
    const std::string& mac, const std::string& keys) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"transaction_id": ")" << esc(transactionId) << R"(")";
    json << R"(,"mac": {)" << mac << "}";
    json << R"(,"keys": ")" << esc(keys) << R"(")";
    json << "}";
    return json.str();
}

std::string buildVerificationCancelBody(const std::string& transactionId,
    const std::string& reason) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    return R"({"transaction_id": ")" + esc(transactionId) +
           R"(", "reason": ")" + esc(reason) + R"("})";
}

std::vector<std::string> getSupportedVerificationMethods() {
    return {"m.sas.v1", "m.qr_code.scan.v1", "m.qr_code.show.v1", "m.reciprocate.v1"};
}

bool isVerificationMethodSupported(const std::string& method) {
    for (const auto& m : getSupportedVerificationMethods()) {
        if (m == method) return true;
    }
    return false;
}

} // namespace progressive
