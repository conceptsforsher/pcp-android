#include "progressive/hash_utils.hpp"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <random>

namespace progressive {

// ---- SHA-256 implementation ----

static const uint32_t SHA256_K[64] = {
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

static std::vector<uint8_t> sha256Raw(const uint8_t* data, size_t len) {
    // Initial hash values
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };

    // Padding
    size_t msgLen = len * 8; // in bits
    size_t paddedLen = ((len + 8) / 64 + 1) * 64;
    std::vector<uint8_t> padded(paddedLen, 0);
    memcpy(padded.data(), data, len);
    padded[len] = 0x80;

    // Append length in big-endian
    for (int i = 0; i < 8; ++i) {
        padded[paddedLen - 1 - i] = (msgLen >> (i * 8)) & 0xFF;
    }

    // Process blocks
    for (size_t i = 0; i < paddedLen; i += 64) {
        uint32_t w[64];
        for (int j = 0; j < 16; ++j) {
            w[j] = (padded[i + j * 4] << 24) |
                   (padded[i + j * 4 + 1] << 16) |
                   (padded[i + j * 4 + 2] << 8) |
                   padded[i + j * 4 + 3];
        }
        for (int j = 16; j < 64; ++j) {
            uint32_t s0 = ((w[j - 15] >> 7) | (w[j - 15] << 25)) ^
                          ((w[j - 15] >> 18) | (w[j - 15] << 14)) ^ (w[j - 15] >> 3);
            uint32_t s1 = ((w[j - 2] >> 17) | (w[j - 2] << 15)) ^
                          ((w[j - 2] >> 19) | (w[j - 2] << 13)) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }

        uint32_t a = h[0], b = h[1], c = h[2], d = h[3];
        uint32_t e = h[4], f = h[5], g = h[6], hh = h[7];

        for (int j = 0; j < 64; ++j) {
            uint32_t S1 = ((e >> 6) | (e << 26)) ^ ((e >> 11) | (e << 21)) ^ ((e >> 25) | (e << 7));
            uint32_t ch = (e & f) ^ ((~e) & g);
            uint32_t temp1 = hh + S1 + ch + SHA256_K[j] + w[j];
            uint32_t S0 = ((a >> 2) | (a << 30)) ^ ((a >> 13) | (a << 19)) ^ ((a >> 22) | (a << 10));
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t temp2 = S0 + maj;

            hh = g; g = f; f = e; e = d + temp1;
            d = c; c = b; b = a; a = temp1 + temp2;
        }

        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }

    std::vector<uint8_t> hash(32);
    for (int i = 0; i < 8; ++i) {
        hash[i * 4]     = (h[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (h[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (h[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = h[i] & 0xFF;
    }
    return hash;
}

std::string sha256Hex(const std::string& input) {
    auto hash = sha256Raw(reinterpret_cast<const uint8_t*>(input.data()), input.size());
    return hexEncode(hash);
}

std::string sha256Hex(const std::vector<uint8_t>& data) {
    auto hash = sha256Raw(data.data(), data.size());
    return hexEncode(hash);
}

std::string hexEncode(const std::vector<uint8_t>& data) {
    std::ostringstream out;
    for (uint8_t b : data) {
        out << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
    }
    return out.str();
}

std::vector<uint8_t> hexDecode(const std::string& hex) {
    std::vector<uint8_t> result;
    for (size_t i = 0; i + 1 < hex.size(); i += 2) {
        unsigned int byte;
        std::stringstream ss(hex.substr(i, 2));
        ss >> std::hex >> byte;
        result.push_back(static_cast<uint8_t>(byte));
    }
    return result;
}

static const char B64_CHARS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64Encode(const std::vector<uint8_t>& data) {
    std::string out;
    int val = 0, bits = -6;
    for (uint8_t c : data) {
        val = (val << 8) + c;
        bits += 8;
        while (bits >= 0) {
            out += B64_CHARS[(val >> bits) & 0x3F];
            bits -= 6;
        }
    }
    if (bits > -6) out += B64_CHARS[((val << 8) >> (bits + 8)) & 0x3F];
    while (out.size() % 4) out += '=';
    return out;
}

std::vector<uint8_t> base64Decode(const std::string& input) {
    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T[static_cast<uint8_t>(B64_CHARS[i])] = i;
    std::vector<uint8_t> out;
    int val = 0, bits = -8;
    for (uint8_t c : input) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        bits += 6;
        if (bits >= 0) {
            out.push_back((val >> bits) & 0xFF);
            bits -= 8;
        }
    }
    return out;
}

std::string hmacSha256(const std::string& key, const std::string& message) {
    const size_t blockSize = 64;
    std::string keyBlock = key;
    if (keyBlock.size() > blockSize) {
        keyBlock = sha256Hex(keyBlock);
    }
    keyBlock.resize(blockSize, 0);

    std::string oKeyPad(blockSize, 0), iKeyPad(blockSize, 0);
    for (size_t i = 0; i < blockSize; ++i) {
        oKeyPad[i] = keyBlock[i] ^ 0x5c;
        iKeyPad[i] = keyBlock[i] ^ 0x36;
    }

    auto innerHash = sha256Raw(reinterpret_cast<const uint8_t*>(iKeyPad.data() + message.size()), 0);
    // Actually: sha256Hex(iKeyPad + message)
    std::string inner = iKeyPad + message;
    auto innerRaw = sha256Raw(reinterpret_cast<const uint8_t*>(inner.data()), inner.size());
    std::string outer = oKeyPad + std::string(innerRaw.begin(), innerRaw.end());
    auto final = sha256Raw(reinterpret_cast<const uint8_t*>(outer.data()), outer.size());
    return std::string(final.begin(), final.end());
}

uint32_t crc32(const std::vector<uint8_t>& data) {
    uint32_t crc = 0xFFFFFFFF;
    for (uint8_t b : data) {
        crc ^= b;
        for (int i = 0; i < 8; ++i) {
            crc = (crc >> 1) ^ (crc & 1 ? 0xEDB88320 : 0);
        }
    }
    return ~crc;
}

uint32_t crc32(const std::string& data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    return crc32(bytes);
}

uint32_t adler32(const std::vector<uint8_t>& data) {
    uint32_t a = 1, b = 0;
    for (uint8_t byte : data) {
        a = (a + byte) % 65521;
        b = (b + a) % 65521;
    }
    return (b << 16) | a;
}

uint32_t adler32(const std::string& data) {
    std::vector<uint8_t> bytes(data.begin(), data.end());
    return adler32(bytes);
}

bool verifyHash(const std::string& data, const std::string& expectedHash) {
    return sha256Hex(data) == expectedHash;
}

std::string generateToken(int numBytes) {
    static const char URL_SAFE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 63);
    std::string token(numBytes, 0);
    for (int i = 0; i < numBytes; ++i) {
        token[i] = URL_SAFE[dis(gen)];
    }
    return token;
}

bool constantTimeCompare(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    int result = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        result |= a[i] ^ b[i];
    }
    return result == 0;
}

// ==== Base64 URL Conversion (from Base64.kt:23-38) ====
std::string base64UrlToBase64(const std::string& base64Url) {
    std::string result = base64Url;
    for (char& c : result) { if (c == '-') c = '+'; else if (c == '_') c = '/'; }
    return result;
}

std::string base64ToBase64Url(const std::string& base64) {
    std::string result;
    for (char c : base64) {
        if (c == '\n') continue;
        if (c == '+') result += '-';
        else if (c == '/') result += '_';
        else if (c == '=') continue;
        else result += c;
    }
    return result;
}

std::string base64ToUnpaddedBase64(const std::string& base64) {
    std::string result;
    for (char c : base64) { if (c == '\n' || c == '=') continue; result += c; }
    return result;
}

} // namespace progressive
