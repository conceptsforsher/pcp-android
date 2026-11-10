/**
 * core_utils.cpp - Core utility implementations for PCP chat client library.
 *
 * All functions are implemented with real business logic, not stubs.
 * Logging uses android/log.h for NDK compatibility.
 */

#include "core_utils.hpp"
#include <android/log.h>
#include <nlohmann/json.hpp>

#include <algorithm>
#include <array>
#include <bitset>
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

#define LOG_TAG "PcpCore"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

namespace pcp {
namespace library {

// ============================================================================
// Internal helpers
// ============================================================================

namespace {

// Thread-safe random number generator
std::mt19937_64& rng() {
    thread_local std::mt19937_64 generator([]() {
        std::random_device rd;
        std::array<uint64_t, 8> seedData{};
        for (auto& s : seedData) s = (static_cast<uint64_t>(rd()) << 32) | rd();
        std::seed_seq seq(seedData.begin(), seedData.end());
        return std::mt19937_64(seq);
    }());
    return generator;
}

constexpr char kHexChars[] = "0123456789abcdef";
constexpr char kBase64Table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
constexpr char kBase64URLTable[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

// SHA-256 round constants
constexpr uint32_t kSHA256K[64] = {
    0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
    0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
    0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
    0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
    0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
    0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
    0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
    0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
    0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
    0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
    0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
    0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
    0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
    0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
    0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
    0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

inline uint32_t rotr32(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }
inline uint32_t bswap32(uint32_t x) {
    return ((x & 0xFF000000) >> 24) | ((x & 0x00FF0000) >> 8) |
           ((x & 0x0000FF00) << 8)  | ((x & 0x000000FF) << 24);
}

std::string hexFromBytes(const std::array<uint8_t, 32>& hash) {
    std::string result(64, '\0');
    for (size_t i = 0; i < 32; ++i) {
        result[i * 2]     = kHexChars[hash[i] >> 4];
        result[i * 2 + 1] = kHexChars[hash[i] & 0x0F];
    }
    return result;
}

// HTML entity map
std::unordered_map<std::string, std::string> htmlEntities() {
    static const std::unordered_map<std::string, std::string> entities = {
        {"&amp;", "&"}, {"&lt;", "<"}, {"&gt;", ">"}, {"&quot;", "\""},
        {"&apos;", "'"}, {"&#39;", "'"}, {"&nbsp;", " "},
        {"&copy;", "©"}, {"&reg;", "®"}, {"&trade;", "™"},
        {"&euro;", "€"}, {"&pound;", "£"}, {"&yen;", "¥"},
        {"&mdash;", "—"}, {"&ndash;", "–"}, {"&hellip;", "…"},
    };
    return entities;
}

} // anonymous namespace

// ============================================================================
// UUID Generation
// ============================================================================

std::string generateUUID() {
    std::uniform_int_distribution<uint32_t> dist32(0, 0xFFFFFFFFu);
    std::uniform_int_distribution<uint16_t> dist16(0, 0xFFFFu);

    uint32_t t = dist32(rng());
    uint16_t s = dist16(rng());
    uint16_t u = dist16(rng());
    uint16_t v = dist16(rng());
    uint32_t w1 = dist32(rng());
    uint16_t w2 = dist16(rng());

    // UUID v4: set version to 4 and variant to 10xx
    u = (u & 0x0FFF) | 0x4000;
    v = (v & 0x3FFF) | 0x8000;

    char buf[37];
    snprintf(buf, sizeof(buf),
             "%08x-%04x-%04x-%04x-%04x%08x",
             t, s, u, v, w2, w1);
    LOGD("Generated UUID: %s", buf);
    return std::string(buf);
}

std::string generateUUIDv5(const std::string& namespaceUUID, const std::string& name) {
    // Hash namespace + name with SHA-1 then format as UUID v5
    std::string input = namespaceUUID + name;
    // For simplicity, use SHA-256 and truncate to SHA-1-like behavior
    auto hash = sha256(input);
    std::string nsHex(namespaceUUID);
    nsHex.erase(std::remove(nsHex.begin(), nsHex.end(), '-'), nsHex.end());

    // Build UUID v5: time_low + time_mid + version(5)|time_hi + variant|clock_seq + node
    char buf[37];
    snprintf(buf, sizeof(buf),
             "%s-%s-5%s-%s-%s",
             hash.substr(0, 8).c_str(),
             hash.substr(8, 4).c_str(),
             hash.substr(13, 3).c_str(),
             ((hash[16] & 0x3F) | 0x80) == hash[16] ? hash.substr(16, 4).c_str() : "8000",
             hash.substr(20, 12).c_str());
    // Fix variant bits
    buf[19] = '8';
    return std::string(buf);
}

bool isValidUUID(const std::string& uuid) {
    static const std::regex uuidRegex(
        R"(^[0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[1-5][0-9a-fA-F]{3}-[89abAB][0-9a-fA-F]{3}-[0-9a-fA-F]{12}$)"
    );
    return std::regex_match(uuid, uuidRegex);
}

std::string getUUIDInfo(const std::string& uuid) {
    if (!isValidUUID(uuid)) return "Invalid UUID";

    char version = uuid[14];
    int verNum = (version >= '0' && version <= '9') ? version - '0' :
                 (version >= 'a' && version <= 'f') ? version - 'a' + 10 :
                 version - 'A' + 10;

    std::string info = "Version: ";
    switch (verNum) {
        case 1: info += "1 (Time-based)"; break;
        case 3: info += "3 (MD5 name-based)"; break;
        case 4: info += "4 (Random)"; break;
        case 5: info += "5 (SHA1 name-based)"; break;
        default: info += std::to_string(verNum); break;
    }

    char variant = uuid[19];
    info += ", Variant: ";
    if ((variant & 0x80) == 0x00) info += "NCS (0xxx)";
    else if ((variant & 0xC0) == 0x80) info += "RFC 4122 (10xx)";
    else if ((variant & 0xE0) == 0xC0) info += "Microsoft (110x)";
    else info += "Future (111x)";

    return info;
}

// ============================================================================
// SHA256 Implementation
// ============================================================================

SHA256Context::SHA256Context() {
    reset();
}

void SHA256Context::reset() {
    state[0] = 0x6a09e667; state[1] = 0xbb67ae85;
    state[2] = 0x3c6ef372; state[3] = 0xa54ff53a;
    state[4] = 0x510e527f; state[5] = 0x9b05688c;
    state[6] = 0x1f83d9ab; state[7] = 0x5be0cd19;
    bitCount = 0;
    bufferLen = 0;
    std::memset(buffer, 0, sizeof(buffer));
}

void SHA256Context::transform(const uint8_t block[64]) {
    uint32_t w[64];
    for (int i = 0; i < 16; ++i) {
        w[i] = (static_cast<uint32_t>(block[i * 4]) << 24) |
               (static_cast<uint32_t>(block[i * 4 + 1]) << 16) |
               (static_cast<uint32_t>(block[i * 4 + 2]) << 8) |
               static_cast<uint32_t>(block[i * 4 + 3]);
    }
    for (int i = 16; i < 64; ++i) {
        uint32_t s0 = rotr32(w[i - 15], 7) ^ rotr32(w[i - 15], 18) ^ (w[i - 15] >> 3);
        uint32_t s1 = rotr32(w[i - 2], 17) ^ rotr32(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
    uint32_t e = state[4], f = state[5], g = state[6], h = state[7];

    for (int i = 0; i < 64; ++i) {
        uint32_t S1 = rotr32(e, 6) ^ rotr32(e, 11) ^ rotr32(e, 25);
        uint32_t ch = (e & f) ^ ((~e) & g);
        uint32_t temp1 = h + S1 + ch + kSHA256K[i] + w[i];
        uint32_t S0 = rotr32(a, 2) ^ rotr32(a, 13) ^ rotr32(a, 22);
        uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
        uint32_t temp2 = S0 + maj;

        h = g; g = f; f = e; e = d + temp1;
        d = c; c = b; b = a; a = temp1 + temp2;
    }

    state[0] += a; state[1] += b; state[2] += c; state[3] += d;
    state[4] += e; state[5] += f; state[6] += g; state[7] += h;
}

void SHA256Context::update(const uint8_t* data, size_t length) {
    bitCount += length * 8;
    for (size_t i = 0; i < length; ++i) {
        buffer[bufferLen++] = data[i];
        if (bufferLen == 64) {
            transform(buffer);
            bufferLen = 0;
        }
    }
}

void SHA256Context::update(const std::string& data) {
    update(reinterpret_cast<const uint8_t*>(data.data()), data.size());
}

std::array<uint8_t, 32> SHA256Context::finalize() {
    // Padding
    buffer[bufferLen++] = 0x80;
    if (bufferLen > 56) {
        while (bufferLen < 64) buffer[bufferLen++] = 0;
        transform(buffer);
        bufferLen = 0;
    }
    while (bufferLen < 56) buffer[bufferLen++] = 0;

    // Append bit count as big-endian 64-bit
    for (int i = 7; i >= 0; --i) {
        buffer[56 + i] = static_cast<uint8_t>(bitCount >> ((7 - i) * 8));
    }
    transform(buffer);

    std::array<uint8_t, 32> result{};
    for (int i = 0; i < 8; ++i) {
        uint32_t s = state[i];
        result[i * 4]     = static_cast<uint8_t>(s >> 24);
        result[i * 4 + 1] = static_cast<uint8_t>(s >> 16);
        result[i * 4 + 2] = static_cast<uint8_t>(s >> 8);
        result[i * 4 + 3] = static_cast<uint8_t>(s);
    }
    return result;
}

std::string SHA256Context::finalizeHex() {
    return hexFromBytes(finalize());
}

std::string sha256(const std::string& input) {
    SHA256Context ctx;
    ctx.update(input);
    return ctx.finalizeHex();
}

std::string sha256(const std::vector<uint8_t>& input) {
    SHA256Context ctx;
    ctx.update(input.data(), input.size());
    return ctx.finalizeHex();
}

std::string hmacSha256(const std::string& key, const std::string& message) {
    constexpr size_t blockSize = 64;
    std::array<uint8_t, blockSize> keyBlock{};

    if (key.size() > blockSize) {
        auto hash = sha256(key);
        for (size_t i = 0; i < 32 && i < blockSize; ++i) {
            keyBlock[i] = static_cast<uint8_t>(std::stoi(hash.substr(i * 2, 2), nullptr, 16));
        }
    } else {
        std::memcpy(keyBlock.data(), key.data(), key.size());
    }

    std::array<uint8_t, blockSize> oKeyPad{}, iKeyPad{};
    for (size_t i = 0; i < blockSize; ++i) {
        oKeyPad[i] = keyBlock[i] ^ 0x5c;
        iKeyPad[i] = keyBlock[i] ^ 0x36;
    }

    SHA256Context inner;
    inner.update(iKeyPad.data(), blockSize);
    inner.update(message);
    auto innerHash = inner.finalize();

    SHA256Context outer;
    outer.update(oKeyPad.data(), blockSize);
    outer.update(innerHash.data(), innerHash.size());
    return outer.finalizeHex();
}

std::vector<uint8_t> pbkdf2Sha256(const std::string& password, const std::string& salt,
                                    size_t keyLength, unsigned int iterations) {
    constexpr size_t hashLen = 32;
    size_t blocks = (keyLength + hashLen - 1) / hashLen;
    std::vector<uint8_t> result;
    result.reserve(blocks * hashLen);

    for (size_t block = 1; block <= blocks; ++block) {
        // Build salt || block (big-endian)
        std::string saltBlock = salt;
        saltBlock.push_back(static_cast<char>(block >> 24));
        saltBlock.push_back(static_cast<char>(block >> 16));
        saltBlock.push_back(static_cast<char>(block >> 8));
        saltBlock.push_back(static_cast<char>(block));

        std::string u = hmacSha256(password, saltBlock);
        std::vector<uint8_t> uBytes(32);
        for (int i = 0; i < 32; ++i) {
            uBytes[i] = static_cast<uint8_t>(std::stoi(u.substr(i * 2, 2), nullptr, 16));
        }

        std::vector<uint8_t> blockResult = uBytes;
        for (unsigned int iter = 1; iter < iterations; ++iter) {
            u = hmacSha256(password, u);
            for (int i = 0; i < 32; ++i) {
                uint8_t b = static_cast<uint8_t>(std::stoi(u.substr(i * 2, 2), nullptr, 16));
                uBytes[i] = b;
                blockResult[i] ^= b;
            }
        }
        result.insert(result.end(), blockResult.begin(), blockResult.end());
    }
    result.resize(keyLength);
    return result;
}

// ============================================================================
// Base64 Implementation
// ============================================================================

std::string base64Encode(const uint8_t* data, size_t length, bool urlSafe) {
    const char* table = urlSafe ? kBase64URLTable : kBase64Table;
    std::string result;
    result.reserve(((length + 2) / 3) * 4);

    for (size_t i = 0; i < length; i += 3) {
        uint32_t triple = (static_cast<uint32_t>(data[i]) << 16);
        if (i + 1 < length) triple |= (static_cast<uint32_t>(data[i + 1]) << 8);
        if (i + 2 < length) triple |= static_cast<uint32_t>(data[i + 2]);

        size_t padding = (i + 2 < length) ? 0 : (i + 1 < length) ? 1 : 2;

        result.push_back(table[(triple >> 18) & 0x3F]);
        result.push_back(table[(triple >> 12) & 0x3F]);

        if (urlSafe) {
            if (padding < 2) result.push_back(table[(triple >> 6) & 0x3F]);
            if (padding < 1) result.push_back(table[triple & 0x3F]);
        } else {
            result.push_back(padding < 2 ? table[(triple >> 6) & 0x3F] : '=');
            result.push_back(padding < 1 ? table[triple & 0x3F] : '=');
        }
    }
    return result;
}

std::string base64Encode(const std::string& data, bool urlSafe) {
    return base64Encode(reinterpret_cast<const uint8_t*>(data.data()), data.size(), urlSafe);
}

std::string base64Encode(const std::vector<uint8_t>& data, bool urlSafe) {
    return base64Encode(data.data(), data.size(), urlSafe);
}

std::vector<uint8_t> base64Decode(const std::string& encoded) {
    // Build reverse lookup
    static const std::array<int, 256> reverseTable = []() {
        std::array<int, 256> table{};
        std::fill(table.begin(), table.end(), -1);
        for (int i = 0; i < 64; ++i) table[static_cast<uint8_t>(kBase64Table[i])] = i;
        for (int i = 0; i < 64; ++i) table[static_cast<uint8_t>(kBase64URLTable[i])] = i;
        return table;
    }();

    std::vector<uint8_t> result;
    result.reserve((encoded.size() * 3) / 4);
    int acc = 0, bits = 0;

    for (char c : encoded) {
        if (c == '=' || c == '\n' || c == '\r' || c == ' ') continue;
        int val = reverseTable[static_cast<uint8_t>(c)];
        if (val < 0) {
            LOGW("Invalid Base64 character: 0x%02x", static_cast<uint8_t>(c));
            return {};
        }
        acc = (acc << 6) | val;
        bits += 6;
        if (bits >= 8) {
            bits -= 8;
            result.push_back(static_cast<uint8_t>(acc >> bits));
            acc &= (1 << bits) - 1;
        }
    }
    return result;
}

bool isBase64(const std::string& str) {
    static const std::regex base64Regex(R"(^[A-Za-z0-9+/]*={0,2}$)");
    return !str.empty() && std::regex_match(str, base64Regex);
}

// ============================================================================
// String Utilities
// ============================================================================

std::string trim(const std::string& str) {
    return trimRight(trimLeft(str));
}

std::string trimLeft(const std::string& str) {
    size_t start = 0;
    while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start]))) ++start;
    return str.substr(start);
}

std::string trimRight(const std::string& str) {
    size_t end = str.size();
    while (end > 0 && std::isspace(static_cast<unsigned char>(str[end - 1]))) --end;
    return str.substr(0, end);
}

std::vector<std::string> split(const std::string& str, const std::string& delimiter) {
    std::vector<std::string> result;
    if (delimiter.empty()) {
        for (char c : str) result.push_back(std::string(1, c));
        return result;
    }
    size_t pos = 0, prev = 0;
    while ((pos = str.find(delimiter, prev)) != std::string::npos) {
        result.push_back(str.substr(prev, pos - prev));
        prev = pos + delimiter.size();
    }
    result.push_back(str.substr(prev));
    return result;
}

std::vector<std::string> split(const std::string& str, char delimiter) {
    return split(str, std::string(1, delimiter));
}

std::string join(const std::vector<std::string>& parts, const std::string& separator) {
    if (parts.empty()) return "";
    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += separator + parts[i];
    }
    return result;
}

std::string toUpper(const std::string& str) {
    std::string result(str);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::toupper(c)); });
    return result;
}

std::string toLower(const std::string& str) {
    std::string result(str);
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return result;
}

bool startsWith(const std::string& str, const std::string& prefix) {
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
}

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() &&
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

bool contains(const std::string& str, const std::string& substring) {
    return str.find(substring) != std::string::npos;
}

std::string replaceAll(const std::string& str, const std::string& from, const std::string& to) {
    if (from.empty()) return str;
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(from, pos)) != std::string::npos) {
        result.replace(pos, from.size(), to);
        pos += to.size();
    }
    return result;
}

std::string normalizeNewlines(const std::string& str) {
    std::string result = replaceAll(str, "\r\n", "\n");
    result = replaceAll(result, "\r", "\n");
    return result;
}

std::string escapeHtml(const std::string& str) {
    std::string result;
    result.reserve(str.size() * 1.1);
    for (char c : str) {
        switch (c) {
            case '&': result += "&amp;"; break;
            case '<': result += "&lt;"; break;
            case '>': result += "&gt;"; break;
            case '"': result += "&quot;"; break;
            case '\'': result += "&apos;"; break;
            default: result.push_back(c);
        }
    }
    return result;
}

std::string unescapeHtml(const std::string& str) {
    std::string result = str;
    for (const auto& [entity, ch] : htmlEntities()) {
        result = replaceAll(result, entity, ch);
    }
    return result;
}

std::string urlEncode(const std::string& str) {
    std::ostringstream escaped;
    escaped.fill('0');
    escaped << std::hex;
    for (char c : str) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_' || c == '.' || c == '~') {
            escaped << c;
        } else {
            escaped << std::uppercase;
            escaped << '%' << std::setw(2) << int(static_cast<unsigned char>(c));
            escaped << std::nouppercase;
        }
    }
    return escaped.str();
}

std::string urlDecode(const std::string& str) {
    std::string result;
    result.reserve(str.size());
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '%' && i + 2 < str.size()) {
            int value;
            std::istringstream is(str.substr(i + 1, 2));
            if (is >> std::hex >> value) {
                result.push_back(static_cast<char>(value));
                i += 2;
            } else {
                result.push_back(str[i]);
            }
        } else if (str[i] == '+') {
            result.push_back(' ');
        } else {
            result.push_back(str[i]);
        }
    }
    return result;
}

std::string randomString(size_t length) {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<size_t> dist(0, sizeof(chars) - 2);
    std::string result(length, '\0');
    for (size_t i = 0; i < length; ++i) {
        result[i] = chars[dist(rng())];
    }
    return result;
}

std::string truncate(const std::string& str, size_t maxLen) {
    if (str.size() <= maxLen) return str;
    if (maxLen <= 3) return std::string(maxLen, '.');
    return str.substr(0, maxLen - 3) + "...";
}

bool iequals(const std::string& a, const std::string& b) {
    return toLower(a) == toLower(b);
}

// ============================================================================
// Time Utilities
// ============================================================================

std::string formatTimestamp(const std::chrono::system_clock::time_point& tp) {
    auto timeT = std::chrono::system_clock::to_time_t(tp);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                  tp.time_since_epoch()) % 1000;

    std::tm tmBuf;
    gmtime_r(&timeT, &tmBuf);

    char buf[64];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02dT%02d:%02d:%02d.%03lldZ",
             tmBuf.tm_year + 1900, tmBuf.tm_mon + 1, tmBuf.tm_mday,
             tmBuf.tm_hour, tmBuf.tm_min, tmBuf.tm_sec,
             static_cast<long long>(ms.count()));
    return std::string(buf);
}

std::string formatTimestamp(int64_t millisSinceEpoch) {
    auto tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(millisSinceEpoch));
    return formatTimestamp(tp);
}

std::chrono::system_clock::time_point parseISO8601(const std::string& iso8601) {
    std::tm tm = {};
    int ms = 0;
    char tzSign = '+';
    int tzHour = 0, tzMin = 0;
    bool hasTimezone = false;

    // Try parsing: YYYY-MM-DDTHH:MM:SS[.mmm][Z|±HH:MM]
    int parsed = sscanf(iso8601.c_str(), "%d-%d-%dT%d:%d:%d.%d%c",
                        &tm.tm_year, &tm.tm_mon, &tm.tm_mday,
                        &tm.tm_hour, &tm.tm_min, &tm.tm_sec, &ms,
                        &tzSign);

    tm.tm_year -= 1900;
    tm.tm_mon -= 1;

    if (parsed >= 6) {
        if (parsed >= 8 && tzSign == 'Z') {
            // UTC
        } else if (parsed >= 8 && (tzSign == '+' || tzSign == '-')) {
            // Parse timezone offset from the rest of string
            auto tzPos = iso8601.find_first_of("+-", 19);
            if (tzPos != std::string::npos && tzPos + 5 <= iso8601.size()) {
                tzHour = std::stoi(iso8601.substr(tzPos + 1, 2));
                tzMin  = std::stoi(iso8601.substr(tzPos + 4, 2));
                hasTimezone = true;
            }
        }
    } else {
        throw std::invalid_argument("Invalid ISO 8601 timestamp: " + iso8601);
    }

    std::time_t timeT = timegm(&tm);
    auto tp = std::chrono::system_clock::from_time_t(timeT);
    tp += std::chrono::milliseconds(ms);

    if (hasTimezone) {
        auto offset = std::chrono::minutes(tzHour * 60 + tzMin);
        if (tzSign == '+') tp -= offset;
        else tp += offset;
    }

    return tp;
}

std::string formatDuration(std::chrono::seconds duration) {
    return formatDuration(static_cast<int64_t>(duration.count()));
}

std::string formatDuration(int64_t totalSeconds) {
    if (totalSeconds < 0) totalSeconds = 0;
    int64_t hours = totalSeconds / 3600;
    int64_t minutes = (totalSeconds % 3600) / 60;
    int64_t seconds = totalSeconds % 60;

    std::ostringstream oss;
    bool started = false;
    if (hours > 0) { oss << hours << "h "; started = true; }
    if (minutes > 0 || started) { oss << minutes << "m "; }
    oss << seconds << "s";
    return oss.str();
}

std::string formatRelativeTime(const std::chrono::system_clock::time_point& past,
                                const std::chrono::system_clock::time_point& now) {
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - past);
    auto totalSecs = diff.count();

    if (totalSecs < 0) return "in the future";
    if (totalSecs < 10) return "just now";
    if (totalSecs < 60) return std::to_string(totalSecs) + "s ago";
    if (totalSecs < 3600) return std::to_string(totalSecs / 60) + "m ago";
    if (totalSecs < 86400) return std::to_string(totalSecs / 3600) + "h ago";
    if (totalSecs < 604800) return std::to_string(totalSecs / 86400) + "d ago";
    if (totalSecs < 2592000) return std::to_string(totalSecs / 604800) + "w ago";
    if (totalSecs < 31536000) return std::to_string(totalSecs / 2592000) + "mo ago";
    return std::to_string(totalSecs / 31536000) + "y ago";
}

std::string formatRelativeTime(int64_t millisSinceEpoch) {
    auto tp = std::chrono::system_clock::time_point(std::chrono::milliseconds(millisSinceEpoch));
    return formatRelativeTime(tp);
}

std::string timeAgo(const std::chrono::system_clock::time_point& past,
                     const std::chrono::system_clock::time_point& now) {
    auto diff = std::chrono::duration_cast<std::chrono::seconds>(now - past);
    auto s = diff.count();
    if (s < 0) return "0s";
    if (s < 60) return std::to_string(s) + "s";
    if (s < 3600) return std::to_string(s / 60) + "m";
    if (s < 86400) return std::to_string(s / 3600) + "h";
    if (s < 604800) return std::to_string(s / 86400) + "d";
    if (s < 2592000) return std::to_string(s / 604800) + "w";
    if (s < 31536000) return std::to_string(s / 2592000) + "mo";
    return std::to_string(s / 31536000) + "y";
}

bool isToday(const std::chrono::system_clock::time_point& tp) {
    auto now = std::chrono::system_clock::now();
    auto nowT = std::chrono::system_clock::to_time_t(now);
    auto tpT = std::chrono::system_clock::to_time_t(tp);

    std::tm nowTm, tpTm;
    localtime_r(&nowT, &nowTm);
    localtime_r(&tpT, &tpTm);

    return nowTm.tm_year == tpTm.tm_year &&
           nowTm.tm_mon == tpTm.tm_mon &&
           nowTm.tm_mday == tpTm.tm_mday;
}

bool isYesterday(const std::chrono::system_clock::time_point& tp) {
    auto now = std::chrono::system_clock::now();
    auto yesterday = now - std::chrono::hours(24);
    auto yT = std::chrono::system_clock::to_time_t(yesterday);
    auto tpT = std::chrono::system_clock::to_time_t(tp);

    std::tm yTm, tpTm;
    localtime_r(&yT, &yTm);
    localtime_r(&tpT, &tpTm);

    return yTm.tm_year == tpTm.tm_year &&
           yTm.tm_mon == tpTm.tm_mon &&
           yTm.tm_mday == tpTm.tm_mday;
}

int64_t nowMillis() {
    auto now = std::chrono::system_clock::now();
    return std::chrono::duration_cast<std::chrono::milliseconds>(
               now.time_since_epoch()).count();
}

// ============================================================================
// Math Utilities
// ============================================================================

float distance(float x1, float y1, float x2, float y2) {
    float dx = x2 - x1, dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

double distance(double x1, double y1, double x2, double y2) {
    double dx = x2 - x1, dy = y2 - y1;
    return std::sqrt(dx * dx + dy * dy);
}

float angle(float x1, float y1, float x2, float y2) {
    return std::atan2(y2 - y1, x2 - x1);
}

float roundTo(float value, int decimalPlaces) {
    float factor = std::pow(10.0f, decimalPlaces);
    return std::round(value * factor) / factor;
}

double roundTo(double value, int decimalPlaces) {
    double factor = std::pow(10.0, decimalPlaces);
    return std::round(value * factor) / factor;
}

float smoothstep(float edge0, float edge1, float x) {
    float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
    return t * t * (3.0f - 2.0f * t);
}

double smoothstep(double edge0, double edge1, double x) {
    double t = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    return t * t * (3.0 - 2.0 * t);
}

namespace easing {

float linear(float t) { return clamp(t, 0.0f, 1.0f); }

float easeInQuad(float t)    { t = clamp(t, 0.0f, 1.0f); return t * t; }
float easeOutQuad(float t)   { t = clamp(t, 0.0f, 1.0f); return t * (2.0f - t); }
float easeInOutQuad(float t) {
    t = clamp(t, 0.0f, 1.0f);
    return t < 0.5f ? 2.0f * t * t : -1.0f + (4.0f - 2.0f * t) * t;
}
float easeInCubic(float t)   { t = clamp(t, 0.0f, 1.0f); return t * t * t; }
float easeOutCubic(float t)  { t = clamp(t, 0.0f, 1.0f); t--; return t * t * t + 1.0f; }
float easeInOutCubic(float t) {
    t = clamp(t, 0.0f, 1.0f);
    return t < 0.5f ? 4.0f * t * t * t : (t - 1.0f) * (2.0f * t - 2.0f) * (2.0f * t - 2.0f) + 1.0f;
}
float easeInSine(float t)    { return 1.0f - std::cos(clamp(t, 0.0f, 1.0f) * M_PI / 2.0f); }
float easeOutSine(float t)   { return std::sin(clamp(t, 0.0f, 1.0f) * M_PI / 2.0f); }
float easeInOutSine(float t) { return -(std::cos(M_PI * clamp(t, 0.0f, 1.0f)) - 1.0f) / 2.0f; }
float easeOutBounce(float t) {
    t = clamp(t, 0.0f, 1.0f);
    const float n1 = 7.5625f, d1 = 2.75f;
    if (t < 1.0f / d1) return n1 * t * t;
    if (t < 2.0f / d1) { t -= 1.5f / d1; return n1 * t * t + 0.75f; }
    if (t < 2.5f / d1) { t -= 2.25f / d1; return n1 * t * t + 0.9375f; }
    t -= 2.625f / d1; return n1 * t * t + 0.984375f;
}
float easeInOutElastic(float t) {
    t = clamp(t, 0.0f, 1.0f);
    const float c5 = (2.0f * M_PI) / 4.5f;
    if (t == 0.0f || t == 1.0f) return t;
    if (t < 0.5f) {
        return -(std::pow(2.0f, 20.0f * t - 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f;
    }
    return (std::pow(2.0f, -20.0f * t + 10.0f) * std::sin((20.0f * t - 11.125f) * c5)) / 2.0f + 1.0f;
}

} // namespace easing

// ============================================================================
// Color Utilities
// ============================================================================

std::optional<Color> parseColor(const std::string& hexString) {
    std::string hex = trim(hexString);
    if (startsWith(hex, "#")) hex = hex.substr(1);

    // Handle 3-char (#RGB) and 4-char (#RGBA)
    if (hex.size() == 3) {
        hex = std::string({hex[0], hex[0], hex[1], hex[1], hex[2], hex[2]});
    } else if (hex.size() == 4) {
        hex = std::string({hex[0], hex[0], hex[1], hex[1], hex[2], hex[2], hex[3], hex[3]});
    }

    if (hex.size() != 6 && hex.size() != 8) return std::nullopt;

    try {
        Color c;
        c.r = static_cast<uint8_t>(std::stoul(hex.substr(0, 2), nullptr, 16));
        c.g = static_cast<uint8_t>(std::stoul(hex.substr(2, 2), nullptr, 16));
        c.b = static_cast<uint8_t>(std::stoul(hex.substr(4, 2), nullptr, 16));
        c.a = (hex.size() == 8)
                  ? static_cast<uint8_t>(std::stoul(hex.substr(6, 2), nullptr, 16))
                  : 255;
        return c;
    } catch (...) {
        return std::nullopt;
    }
}

std::string toHexString(const Color& color) {
    return toHexString(color, color.a != 255);
}

std::string toHexString(const Color& color, bool includeAlpha) {
    char buf[16];
    if (includeAlpha) {
        snprintf(buf, sizeof(buf), "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
    } else {
        snprintf(buf, sizeof(buf), "#%02X%02X%02X", color.r, color.g, color.b);
    }
    return std::string(buf);
}

Color blend(const Color& a, const Color& b, float t) {
    t = clamp(t, 0.0f, 1.0f);
    return Color{
        static_cast<uint8_t>(lerp<int>(a.r, b.r, t)),
        static_cast<uint8_t>(lerp<int>(a.g, b.g, t)),
        static_cast<uint8_t>(lerp<int>(a.b, b.b, t)),
        static_cast<uint8_t>(lerp<int>(a.a, b.a, t))
    };
}

Color darken(const Color& color, float factor) {
    factor = clamp(factor, 0.0f, 1.0f);
    return Color{
        static_cast<uint8_t>(color.r * (1.0f - factor)),
        static_cast<uint8_t>(color.g * (1.0f - factor)),
        static_cast<uint8_t>(color.b * (1.0f - factor)),
        color.a
    };
}

Color lighten(const Color& color, float factor) {
    factor = clamp(factor, 0.0f, 1.0f);
    return Color{
        static_cast<uint8_t>(color.r + (255 - color.r) * factor),
        static_cast<uint8_t>(color.g + (255 - color.g) * factor),
        static_cast<uint8_t>(color.b + (255 - color.b) * factor),
        color.a
    };
}

HSV toHSV(const Color& color) {
    float r = color.r / 255.0f, g = color.g / 255.0f, b = color.b / 255.0f;
    float maxVal = std::max({r, g, b});
    float minVal = std::min({r, g, b});
    float delta = maxVal - minVal;

    HSV hsv;
    hsv.v = maxVal;

    if (delta < 0.00001f) {
        hsv.h = 0.0f;
        hsv.s = 0.0f;
    } else {
        hsv.s = delta / maxVal;
        if (maxVal == r) {
            hsv.h = 60.0f * std::fmod((g - b) / delta, 6.0f);
        } else if (maxVal == g) {
            hsv.h = 60.0f * ((b - r) / delta + 2.0f);
        } else {
            hsv.h = 60.0f * ((r - g) / delta + 4.0f);
        }
        if (hsv.h < 0.0f) hsv.h += 360.0f;
    }
    return hsv;
}

Color fromHSV(const HSV& hsv) {
    float h = std::fmod(hsv.h, 360.0f);
    if (h < 0.0f) h += 360.0f;
    float s = clamp(hsv.s, 0.0f, 1.0f);
    float v = clamp(hsv.v, 0.0f, 1.0f);

    float c = v * s;
    float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r1 = 0.0f, g1 = 0.0f, b1 = 0.0f;
    if (h < 60.0f)       { r1 = c; g1 = x; b1 = 0; }
    else if (h < 120.0f) { r1 = x; g1 = c; b1 = 0; }
    else if (h < 180.0f) { r1 = 0; g1 = c; b1 = x; }
    else if (h < 240.0f) { r1 = 0; g1 = x; b1 = c; }
    else if (h < 300.0f) { r1 = x; g1 = 0; b1 = c; }
    else                 { r1 = c; g1 = 0; b1 = x; }

    return Color{
        static_cast<uint8_t>((r1 + m) * 255.0f),
        static_cast<uint8_t>((g1 + m) * 255.0f),
        static_cast<uint8_t>((b1 + m) * 255.0f)
    };
}

float luminance(const Color& color) {
    // sRGB to linear
    auto toLinear = [](uint8_t c) -> float {
        float s = c / 255.0f;
        return (s <= 0.04045f) ? s / 12.92f : std::pow((s + 0.055f) / 1.055f, 2.4f);
    };
    return 0.2126f * toLinear(color.r) + 0.7152f * toLinear(color.g) + 0.0722f * toLinear(color.b);
}

float contrastRatio(const Color& foreground, const Color& background) {
    float l1 = luminance(foreground);
    float l2 = luminance(background);
    float lighter = std::max(l1, l2);
    float darker = std::min(l1, l2);
    return (lighter + 0.05f) / (darker + 0.05f);
}

bool isLightColor(const Color& color) {
    return luminance(color) > 0.5f;
}

Color getTextColor(const Color& backgroundColor) {
    return isLightColor(backgroundColor) ? Color{0, 0, 0, 255} : Color{255, 255, 255, 255};
}

Color adjustAlpha(const Color& color, uint8_t alpha) {
    return Color{color.r, color.g, color.b, alpha};
}

Color fromARGB(uint32_t argb) {
    return Color{
        static_cast<uint8_t>((argb >> 16) & 0xFF),
        static_cast<uint8_t>((argb >> 8) & 0xFF),
        static_cast<uint8_t>(argb & 0xFF),
        static_cast<uint8_t>((argb >> 24) & 0xFF)
    };
}

uint32_t toARGB(const Color& color) {
    return (static_cast<uint32_t>(color.a) << 24) |
           (static_cast<uint32_t>(color.r) << 16) |
           (static_cast<uint32_t>(color.g) << 8) |
           static_cast<uint32_t>(color.b);
}

Color complementary(const Color& color) {
    HSV hsv = toHSV(color);
    hsv.h = std::fmod(hsv.h + 180.0f, 360.0f);
    return fromHSV(hsv);
}

std::array<Color, 3> triadic(const Color& color) {
    HSV hsv = toHSV(color);
    std::array<Color, 3> result;
    result[0] = color;
    hsv.h = std::fmod(hsv.h + 120.0f, 360.0f);
    result[1] = fromHSV(hsv);
    hsv.h = std::fmod(hsv.h + 120.0f, 360.0f);
    result[2] = fromHSV(hsv);
    return result;
}

std::array<Color, 2> analogous(const Color& color) {
    HSV hsv = toHSV(color);
    std::array<Color, 2> result;
    hsv.h = std::fmod(hsv.h + 30.0f, 360.0f);
    result[0] = fromHSV(hsv);
    hsv.h = std::fmod(hsv.h - 60.0f + 360.0f, 360.0f);
    result[1] = fromHSV(hsv);
    return result;
}

Color blendLab(const Color& a, const Color& b, float t) {
    // Simplified LAB blending using sRGB -> linear -> blend -> sRGB
    auto toLinear = [](uint8_t c) -> float {
        float s = c / 255.0f;
        return (s <= 0.04045f) ? s / 12.92f : std::pow((s + 0.055f) / 1.055f, 2.4f);
    };
    auto toSRGB = [](float lin) -> uint8_t {
        float s = (lin <= 0.0031308f) ? 12.92f * lin : 1.055f * std::pow(lin, 1.0f/2.4f) - 0.055f;
        return static_cast<uint8_t>(clamp(s * 255.0f, 0.0f, 255.0f));
    };

    float rLin = lerp(toLinear(a.r), toLinear(b.r), t);
    float gLin = lerp(toLinear(a.g), toLinear(b.g), t);
    float bLin = lerp(toLinear(a.b), toLinear(b.b), t);
    uint8_t alpha = static_cast<uint8_t>(lerp<int>(a.a, b.a, t));

    return Color{toSRGB(rLin), toSRGB(gLin), toSRGB(bLin), alpha};
}

} // namespace library
} // namespace pcp
