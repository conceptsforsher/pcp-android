#include "progressive/verification_utils.hpp"
#include <sstream>
#include <iomanip>
#include <cstdlib>

namespace progressive {

// ---- internal JSON helpers ----

namespace {

std::string escJson(const std::string& s) {
    std::string out;
    for (char c : s) {
        if (c == '"') out += "\\\"";
        else if (c == '\\') out += "\\\\";
        else out += c;
    }
    return out;
}

// Extract a string value for a given key from JSON (same as json_parser)
std::string parseJsonStringVal(const std::string& json, const std::string& key) {
    std::string search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return {};
    pos += search.size();
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
        ++pos;
    if (pos >= json.size() || json[pos] != ':') return {};
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == '\r'))
        ++pos;
    if (pos >= json.size()) return {};
    if (json[pos] == '"') {
        ++pos;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return {};
        return json.substr(pos, end - pos);
    }
    auto end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}' && json[end] != ' ' &&
           json[end] != '\t' && json[end] != '\n' && json[end] != '\r') {
        ++end;
    }
    return json.substr(pos, end - pos);
}

// Extract an int64 value for a given key
int64_t parseJsonInt64Val(const std::string& json, const std::string& key) {
    auto s = parseJsonStringVal(json, key);
    if (s.empty()) return 0;
    return static_cast<int64_t>(std::atoll(s.c_str()));
}

// Extract a JSON array of strings for a given key
std::vector<std::string> parseJsonStringArray(const std::string& json, const std::string& key) {
    std::vector<std::string> result;
    std::string search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return result;
    pos += search.size();
    while (pos < json.size() && json[pos] != ':') ++pos;
    if (pos >= json.size()) return result;
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size() || json[pos] != '[') return result;
    ++pos;
    while (pos < json.size() && json[pos] != ']') {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == ',')) ++pos;
        if (pos >= json.size() || json[pos] == ']') break;
        if (json[pos] == '"') {
            ++pos;
            auto end = json.find('"', pos);
            if (end != std::string::npos) {
                result.push_back(json.substr(pos, end - pos));
                pos = end + 1;
            }
        } else {
            auto end = pos;
            while (end < json.size() && json[end] != ',' && json[end] != ']' && json[end] != ' ') ++end;
            result.push_back(json.substr(pos, end - pos));
            pos = end;
        }
    }
    return result;
}

// Extract a JSON object of string->string for a given key
std::map<std::string, std::string> parseJsonStringMap(const std::string& json, const std::string& key) {
    std::map<std::string, std::string> result;
    std::string search = '"' + key + '"';
    auto pos = json.find(search);
    if (pos == std::string::npos) return result;
    pos += search.size();
    while (pos < json.size() && json[pos] != ':') ++pos;
    if (pos >= json.size()) return result;
    ++pos;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
    if (pos >= json.size() || json[pos] != '{') return result;
    ++pos;
    while (pos < json.size() && json[pos] != '}') {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n' || json[pos] == ',')) ++pos;
        if (pos >= json.size() || json[pos] == '}') break;
        if (json[pos] == '"') {
            ++pos;
            auto keyEnd = json.find('"', pos);
            if (keyEnd == std::string::npos) break;
            std::string mapKey = json.substr(pos, keyEnd - pos);
            pos = keyEnd + 1;
            while (pos < json.size() && json[pos] != ':') ++pos;
            ++pos;
            while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) ++pos;
            if (json[pos] == '"') {
                ++pos;
                auto valEnd = json.find('"', pos);
                if (valEnd != std::string::npos) {
                    result[mapKey] = json.substr(pos, valEnd - pos);
                    pos = valEnd + 1;
                }
            } else {
                auto valEnd = pos;
                while (valEnd < json.size() && json[valEnd] != ',' && json[valEnd] != '}' && json[valEnd] != ' ') ++valEnd;
                result[mapKey] = json.substr(pos, valEnd - pos);
                pos = valEnd;
            }
        }
    }
    return result;
}

std::string buildJsonStringArray(const std::vector<std::string>& items) {
    std::ostringstream out;
    out << "[";
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) out << ", ";
        out << '"' << escJson(items[i]) << '"';
    }
    out << "]";
    return out.str();
}

std::string buildJsonStringMap(const std::map<std::string, std::string>& items) {
    std::ostringstream out;
    out << "{";
    bool first = true;
    for (const auto& [k, v] : items) {
        if (!first) out << ", ";
        first = false;
        out << '"' << escJson(k) << "\": \"" << escJson(v) << '"';
    }
    out << "}";
    return out.str();
}

} // anonymous namespace

// ---- Verification Methods ----

std::vector<std::string> getSupportedVerificationMethods() {
    // Original Kotlin: VerificationMethod.kt, SasVerificationTransaction.kt constants
    return {"m.sas.v1", "m.qr_code.scan.v1", "m.qr_code.show.v1", "m.reciprocate.v1"};
}

bool isVerificationMethodSupported(const std::string& method) {
    for (const auto& m : getSupportedVerificationMethods()) {
        if (m == method) return true;
    }
    return false;
}

VerificationMethod parseVerificationMethod(const std::string& methodStr) {
    return verificationMethodFromMatrixString(methodStr);
}

// ---- JSON Builders ----

std::string buildVerificationRequest(const std::string& fromDevice,
    const std::string& transactionId, const std::vector<std::string>& methods,
    const std::string& fromUser, const std::string& roomId) {
    // Original Kotlin: VerificationRequest sends m.key.verification.request
    std::ostringstream json;
    json << "{";
    json << R"("from_device": ")" << escJson(fromDevice) << R"(")";
    json << R"(, "transaction_id": ")" << escJson(transactionId) << R"(")";
    json << R"(, "methods": )" << buildJsonStringArray(methods);
    json << R"(, "timestamp": )" << std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    if (!fromUser.empty()) {
        json << R"(, "from_user": ")" << escJson(fromUser) << R"(")";
    }
    if (!roomId.empty()) {
        json << R"(, "room_id": ")" << escJson(roomId) << R"(")";
    }
    json << "}";
    return json.str();
}

std::string buildVerificationReady(const std::string& fromDevice,
    const std::string& transactionId, const std::vector<std::string>& methods) {
    // Original Kotlin: VerificationRequest.acceptWithMethods() → m.key.verification.ready
    std::ostringstream json;
    json << "{";
    json << R"("from_device": ")" << escJson(fromDevice) << R"(")";
    json << R"(, "transaction_id": ")" << escJson(transactionId) << R"(")";
    json << R"(, "methods": )" << buildJsonStringArray(methods);
    json << "}";
    return json.str();
}

std::string buildVerificationStart(const std::string& fromDevice,
    const std::string& transactionId, const std::string& method,
    const std::vector<std::string>& keyAgreementProtocols,
    const std::vector<std::string>& hashes,
    const std::vector<std::string>& messageAuthenticationCodes,
    const std::vector<std::string>& shortAuthenticationStrings) {
    // Original Kotlin: SasVerificationTransaction start flow
    std::ostringstream json;
    json << "{";
    json << R"("from_device": ")" << escJson(fromDevice) << R"(")";
    json << R"(, "transaction_id": ")" << escJson(transactionId) << R"(")";
    json << R"(, "method": ")" << escJson(method) << R"(")";
    json << R"(, "key_agreement_protocols": )" << buildJsonStringArray(keyAgreementProtocols);
    json << R"(, "hashes": )" << buildJsonStringArray(hashes);
    json << R"(, "message_authentication_codes": )" << buildJsonStringArray(messageAuthenticationCodes);
    json << R"(, "short_authentication_string": )" << buildJsonStringArray(shortAuthenticationStrings);
    json << "}";
    return json.str();
}

std::string buildVerificationAccept(const std::string& transactionId,
    const std::string& commitment,
    const std::string& keyAgreementProtocol,
    const std::string& hash,
    const std::string& messageAuthenticationCode,
    const std::string& shortAuthenticationString) {
    // Original Kotlin: SasVerification.accept() → m.key.verification.accept
    std::ostringstream json;
    json << "{";
    json << R"("transaction_id": ")" << escJson(transactionId) << R"(")";
    json << R"(, "commitment": ")" << escJson(commitment) << R"(")";
    json << R"(, "key_agreement_protocol": ")" << escJson(keyAgreementProtocol) << R"(")";
    json << R"(, "hash": ")" << escJson(hash) << R"(")";
    json << R"(, "message_authentication_code": ")" << escJson(messageAuthenticationCode) << R"(")";
    json << R"(, "short_authentication_string": ")" << escJson(shortAuthenticationString) << R"(")";
    json << "}";
    return json.str();
}

std::string buildVerificationKey(const std::string& transactionId, const std::string& key) {
    // Original Kotlin: m.key.verification.key
    std::ostringstream json;
    json << "{";
    json << R"("transaction_id": ")" << escJson(transactionId) << R"(")";
    json << R"(, "key": ")" << escJson(key) << R"(")";
    json << "}";
    return json.str();
}

std::string buildVerificationMac(const std::string& transactionId,
    const std::map<std::string, std::string>& mac, const std::string& keys) {
    // Original Kotlin: SasVerification.confirm() → m.key.verification.mac
    std::ostringstream json;
    json << "{";
    json << R"("transaction_id": ")" << escJson(transactionId) << R"(")";
    json << R"(, "mac": )" << buildJsonStringMap(mac);
    json << R"(, "keys": ")" << escJson(keys) << R"(")";
    json << "}";
    return json.str();
}

std::string buildVerificationCancel(const std::string& transactionId,
    const std::string& reason, const std::string& code) {
    // Original Kotlin: m.key.verification.cancel
    std::ostringstream json;
    json << "{";
    json << R"("transaction_id": ")" << escJson(transactionId) << R"(")";
    if (!reason.empty()) {
        json << R"(, "reason": ")" << escJson(reason) << R"(")";
    }
    json << R"(, "code": ")" << escJson(code) << R"(")";
    json << "}";
    return json.str();
}

// ---- JSON Parsers ----

VerificationRequestInfo parseVerificationRequest(const std::string& json) {
    // Original Kotlin: ValidVerificationInfoRequest.kt
    VerificationRequestInfo info;
    info.transactionId = parseJsonStringVal(json, "transaction_id");
    info.fromDevice = parseJsonStringVal(json, "from_device");
    info.fromUser = parseJsonStringVal(json, "from_user");
    info.methods = parseJsonStringArray(json, "methods");
    info.timestamp = parseJsonInt64Val(json, "timestamp");
    info.roomId = parseJsonStringVal(json, "room_id");
    return info;
}

VerificationReadyInfo parseVerificationReady(const std::string& json) {
    // Original Kotlin: ValidVerificationInfoReady.kt
    VerificationReadyInfo info;
    info.transactionId = parseJsonStringVal(json, "transaction_id");
    info.fromDevice = parseJsonStringVal(json, "from_device");
    info.methods = parseJsonStringArray(json, "methods");
    return info;
}

VerificationStartInfo parseVerificationStart(const std::string& json) {
    VerificationStartInfo info;
    info.transactionId = parseJsonStringVal(json, "transaction_id");
    info.fromDevice = parseJsonStringVal(json, "from_device");
    info.method = parseJsonStringVal(json, "method");
    info.keyAgreementProtocols = parseJsonStringArray(json, "key_agreement_protocols");
    info.hashes = parseJsonStringArray(json, "hashes");
    info.messageAuthenticationCodes = parseJsonStringArray(json, "message_authentication_codes");
    info.shortAuthenticationStrings = parseJsonStringArray(json, "short_authentication_string");
    return info;
}

VerificationMacInfo parseVerificationMac(const std::string& json) {
    VerificationMacInfo info;
    info.transactionId = parseJsonStringVal(json, "transaction_id");
    info.mac = parseJsonStringMap(json, "mac");
    info.keys = parseJsonStringVal(json, "keys");
    return info;
}

VerificationKeyInfo parseVerificationKey(const std::string& json) {
    VerificationKeyInfo info;
    info.transactionId = parseJsonStringVal(json, "transaction_id");
    info.key = parseJsonStringVal(json, "key");
    return info;
}

VerificationCancelInfo parseVerificationCancel(const std::string& json) {
    VerificationCancelInfo info;
    info.transactionId = parseJsonStringVal(json, "transaction_id");
    info.reason = parseJsonStringVal(json, "reason");
    info.code = parseJsonStringVal(json, "code");
    return info;
}

// ---- SAS / Emoji Utilities ----

std::vector<VerificationEmoji> getVerificationEmojis() {
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
        int idx = byte & 0x3F;
        if (idx < static_cast<int>(allEmojis.size())) {
            sas.emojis.push_back(allEmojis[idx]);
        }
    }

    return sas;
}

std::vector<int> computeSasDecimals(const std::string& sasBytes) {
    std::vector<int> decimals;
    for (size_t i = 0; i + 2 < sasBytes.size(); i += 3) {
        int value = ((unsigned char)sasBytes[i] << 16) |
                    ((unsigned char)sasBytes[i + 1] << 8) |
                    (unsigned char)sasBytes[i + 2];
        int decimal = value % 1000;
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
    std::ostringstream out;
    for (size_t i = 0; i < sas.decimals.size(); ++i) {
        if (i > 0) out << " - ";
        out << std::setfill('0') << std::setw(3) << (sas.decimals[i] % 1000);
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

// ---- Formatting ----

std::string formatVerificationSession(const VerificationSession& state) {
    std::ostringstream out;
    out << "Verification with " << state.otherUserId
        << " [" << verificationStateToString(state.state) << "]";
    if (state.isDone) out << " — Complete";
    else if (state.isCancelled) out << " — Cancelled";
    else if (state.isStarted) out << " — In progress";
    else if (state.isReady) out << " — Ready to start";
    else out << " — Waiting";
    return out.str();
}

// ---- Legacy Builders (kept for compatibility) ----

std::string buildVerificationStartBody(const std::string& fromDevice,
    const std::string& transactionId, const std::string& method) {
    return buildVerificationStart(fromDevice, transactionId, method);
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
    return buildVerificationCancel(transactionId, reason);
}

} // namespace progressive
