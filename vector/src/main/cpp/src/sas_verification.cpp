#include "progressive/sas_verification.hpp"
#include "progressive/crypto_algorithms.hpp"
#include <olm/olm.h>
#include <olm/sas.h>
#include <cstring>
#include <sstream>
#include <android/log.h>

#define LOG_TAG "SasVerif"
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)

namespace progressive {

// ==== Base64 helpers ====
static const char B64_C[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static std::string b64Encode(const uint8_t* data, size_t len) {
    std::string r;
    int val = 0, vb = -6;
    for (size_t i = 0; i < len; i++) {
        val = (val << 8) + data[i]; vb += 8;
        while (vb >= 0) { r.push_back(B64_C[(val>>vb)&0x3F]); vb -= 6; }
    }
    if (vb > -6) r.push_back(B64_C[((val<<8)>>(vb+8))&0x3F]);
    while (r.size()%4) r.push_back('=');
    return r;
}

static std::vector<uint8_t> b64Decode(const std::string& in) {
    std::vector<uint8_t> r;
    int val = 0, vb = -8;
    for (char c : in) {
        if (c == '=') break;
        const char* p = strchr(B64_C, c); if (!p) continue;
        val = (val<<6)+(int)(p-B64_C); vb += 6;
        if (vb >= 0) { r.push_back((uint8_t)((val>>vb)&0xFF)); vb -= 8; }
    }
    return r;
}

// ==== SAS Verification ====

SasVerification sasCreate() {
    SasVerification result;

    size_t size = olm_sas_size();
    void* sasBuf = malloc(size);
    if (!sasBuf) return result;

    auto* sas = olm_sas(sasBuf);

    size_t randLen = olm_create_sas_random_length(sas);
    std::vector<uint8_t> random(randLen, 0);
    for (size_t i = 0; i < randLen; i++) random[i] = (uint8_t)(i * 13 + 31);

    size_t ret = olm_create_sas(sas, random.data(), randLen);
    if (ret == olm_error()) {
        LOGW("olm_create_sas failed: %s", olm_sas_last_error(sas));
        free(sasBuf);
        return result;
    }

    // Get our pubkey
    size_t keyLen = olm_sas_pubkey_length(sas);
    std::string pubkey(keyLen, '\0');
    olm_sas_get_pubkey(sas, &pubkey[0], keyLen);
    pubkey.resize(keyLen);

    result.sas = sasBuf;
    result.ourPubkey = b64Encode(reinterpret_cast<const uint8_t*>(pubkey.data()), pubkey.size());
    result.valid = true;
    return result;
}

bool sasSetTheirKey(SasVerification& sas, const std::string& theirPubkeyBase64) {
    if (!sas.valid || !sas.sas) return false;

    auto key = b64Decode(theirPubkeyBase64);
    auto* olmSas = olm_sas(sas.sas);
    size_t ret = olm_sas_set_their_key(olmSas, key.data(), key.size());
    if (ret == olm_error()) {
        LOGW("olm_sas_set_their_key failed: %s", olm_sas_last_error(olmSas));
        return false;
    }
    sas.theirKeySet = true;
    return true;
}

std::string sasGetEmojis(SasVerification& sas) {
    if (!sas.valid || !sas.sas || !sas.theirKeySet) return "[]";

    auto* olmSas = olm_sas(sas.sas);

    // Generate bytes using HKDF-like info string
    const char* info = "MATRIX_KEY_VERIFICATION_SAS";
    size_t infoLen = strlen(info);

    // Generate 6 bytes of shared secret
    uint8_t bytes[6];
    size_t ret = olm_sas_generate_bytes(olmSas, info, infoLen, bytes, 6);
    if (ret == olm_error()) {
        LOGW("olm_sas_generate_bytes failed");
        return "[]";
    }

    // Map bytes to emoji indices (mod 64)
    std::ostringstream os;
    os << "[";
    for (int i = 0; i < 6; i++) {
        if (i > 0) os << ",";
        int idx = bytes[i] % 64;
        auto emoji = sasEmojiForIndex(idx);
        os << R"({"emoji":")" << emoji.emoji
           << R"(","description":")" << emoji.description << "\"}";
    }
    os << "]";
    return os.str();
}

std::string sasCalculateMac(SasVerification& sas, const std::string& input, const std::string& info) {
    if (!sas.valid || !sas.sas || !sas.theirKeySet) return "";

    auto* olmSas = olm_sas(sas.sas);
    size_t macLen = olm_sas_mac_length(olmSas);

    std::string mac(macLen, '\0');
    size_t ret = olm_sas_calculate_mac(olmSas,
        reinterpret_cast<const uint8_t*>(input.data()), input.size(),
        info.data(), info.size(),
        &mac[0], macLen);
    if (ret == olm_error()) {
        LOGW("olm_sas_calculate_mac failed");
        return "";
    }
    mac.resize(ret);
    return b64Encode(reinterpret_cast<const uint8_t*>(mac.data()), mac.size());
}

std::string sasCalculateMacLongKdf(SasVerification& sas, const std::string& input, const std::string& info) {
    if (!sas.valid || !sas.sas || !sas.theirKeySet) return "";

    auto* olmSas = olm_sas(sas.sas);
    size_t macLen = olm_sas_mac_length(olmSas);

    std::string mac(macLen, '\0');
    size_t ret = olm_sas_calculate_mac_long_kdf(olmSas,
        reinterpret_cast<const uint8_t*>(input.data()), input.size(),
        info.data(), info.size(),
        &mac[0], macLen);
    if (ret == olm_error()) return "";
    mac.resize(ret);
    return b64Encode(reinterpret_cast<const uint8_t*>(mac.data()), mac.size());
}

bool sasVerifyMac(SasVerification& sas, const std::string& theirMacBase64,
                  const std::string& input, const std::string& info) {
    auto ourMac = sasCalculateMac(sas, input, info);
    if (ourMac.empty()) return false;

    // In SAS verification, we compare MACs by computing and checking
    // The libolm SAS API doesn't have a direct verify-mac function.
    // Instead, both sides compute MAC and compare out-of-band or
    // exchange MACs and verify equality.
    auto theirMac = b64Decode(theirMacBase64);
    auto ourMacBytes = b64Decode(ourMac);
    if (theirMac.size() != ourMacBytes.size()) return false;
    for (size_t i = 0; i < theirMac.size(); i++) {
        if (theirMac[i] != ourMacBytes[i]) return false;
    }
    return true;
}

void sasDestroy(SasVerification& sas) {
    if (sas.sas) {
        olm_clear_sas(olm_sas(sas.sas));
        free(sas.sas);
        sas.sas = nullptr;
    }
    sas.valid = false;
}

// ==== Emoji Table ====
// Matrix spec SAS emoji v1 (64 emojis, indexed 0-63)

static const SasEmoji EMOJI_TABLE[64] = {
    {"🐶", "Dog"},      {"🐱", "Cat"},      {"🦁", "Lion"},     {"🐎", "Horse"},
    {"🦄", "Unicorn"},  {"🐷", "Pig"},       {"🐘", "Elephant"}, {"🐰", "Rabbit"},
    {"🐼", "Panda"},    {"🐓", "Rooster"},   {"🐧", "Penguin"},  {"🐢", "Turtle"},
    {"🐟", "Fish"},     {"🐙", "Octopus"},   {"🦋", "Butterfly"},{"🌷", "Tulip"},
    {"🌲", "Evergreen"},{"🌵", "Cactus"},    {"🍄", "Mushroom"}, {"🌍", "Globe"},
    {"🌙", "Moon"},     {"☁️", "Cloud"},      {"🔥", "Fire"},     {"🍌", "Banana"},
    {"🍎", "Apple"},    {"🍓", "Strawberry"},{"🌽", "Corn"},     {"🍕", "Pizza"},
    {"🎂", "Cake"},     {"❤️", "Heart"},      {"😀", "Smiley"},   {"🤖", "Robot"},
    {"🎩", "Top Hat"},  {"👓", "Glasses"},   {"🔧", "Wrench"},   {"🎸", "Guitar"},
    {"🚗", "Car"},      {"🚂", "Train"},     {"✈️", "Airplane"}, {"🚀", "Rocket"},
    {"🏆", "Trophy"},   {"⚽", "Ball"},       {"🔑", "Key"},      {"🔨", "Hammer"},
    {"☎️", "Telephone"},{"🏳️", "Flag"},      {"🎯", "Bullseye"}, {"🛡️", "Shield"},
    {"🔒", "Padlock"},  {"✏️", "Pencil"},    {"✂️", "Scissors"}, {"📌", "Pushpin"},
    {"🧦", "Socks"},    {"🧩", "Puzzle"},    {"🧭", "Compass"},  {"📅", "Calendar"},
    {"📎", "Paperclip"},{"🔔", "Bell"},       {"🎄", "Tree"},     {"🧲", "Magnet"},
};

SasEmoji sasEmojiForIndex(int index) {
    if (index < 0 || index >= 64) return EMOJI_TABLE[0];
    return EMOJI_TABLE[index];
}

const SasEmoji* getSasEmojiTable() {
    return EMOJI_TABLE;
}

} // namespace progressive
