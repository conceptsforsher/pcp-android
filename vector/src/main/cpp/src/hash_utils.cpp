#include "progressive/hash_utils.hpp"
#include <sstream>
#include <iomanip>
#include <cstring>
#include <random>
#include <array>

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
    uint32_t h[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    size_t msgLen = len * 8;
    size_t paddedLen = ((len + 8) / 64 + 1) * 64;
    std::vector<uint8_t> padded(paddedLen, 0);
    memcpy(padded.data(), data, len);
    padded[len] = 0x80;
    for (int i = 0; i < 8; ++i)
        padded[paddedLen - 1 - i] = (msgLen >> (i * 8)) & 0xFF;
    for (size_t i = 0; i < paddedLen; i += 64) {
        uint32_t w[64];
        for (int j = 0; j < 16; ++j) {
            w[j] = (padded[i + j * 4] << 24) | (padded[i + j * 4 + 1] << 16) |
                   (padded[i + j * 4 + 2] << 8) | padded[i + j * 4 + 3];
        }
        for (int j = 16; j < 64; ++j) {
            uint32_t s0 = ((w[j - 15] >> 7) | (w[j - 15] << 25)) ^
                          ((w[j - 15] >> 18) | (w[j - 15] << 14)) ^ (w[j - 15] >> 3);
            uint32_t s1 = ((w[j - 2] >> 17) | (w[j - 2] << 15)) ^
                          ((w[j - 2] >> 19) | (w[j - 2] << 13)) ^ (w[j - 2] >> 10);
            w[j] = w[j - 16] + s0 + w[j - 7] + s1;
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3],
                 e = h[4], f = h[5], g = h[6], hh = h[7];
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
    for (uint8_t b : data) out << std::hex << std::setfill('0') << std::setw(2) << static_cast<int>(b);
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
        while (bits >= 0) { out += B64_CHARS[(val >> bits) & 0x3F]; bits -= 6; }
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
        val = (val << 6) + T[c]; bits += 6;
        if (bits >= 0) { out.push_back((val >> bits) & 0xFF); bits -= 8; }
    }
    return out;
}

std::string hmacSha256(const std::string& key, const std::string& message) {
    const size_t blockSize = 64;
    std::string keyBlock = key;
    if (keyBlock.size() > blockSize) keyBlock = sha256Hex(keyBlock);
    keyBlock.resize(blockSize, 0);
    std::string oKeyPad(blockSize, 0), iKeyPad(blockSize, 0);
    for (size_t i = 0; i < blockSize; ++i) {
        oKeyPad[i] = keyBlock[i] ^ 0x5c;
        iKeyPad[i] = keyBlock[i] ^ 0x36;
    }
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
        for (int i = 0; i < 8; ++i)
            crc = (crc >> 1) ^ (crc & 1 ? 0xEDB88320 : 0);
    }
    return ~crc;
}

uint32_t crc32(const std::string& data) {
    return crc32(std::vector<uint8_t>(data.begin(), data.end()));
}

uint32_t adler32(const std::vector<uint8_t>& data) {
    uint32_t a = 1, b = 0;
    for (uint8_t byte : data) { a = (a + byte) % 65521; b = (b + a) % 65521; }
    return (b << 16) | a;
}

uint32_t adler32(const std::string& data) {
    return adler32(std::vector<uint8_t>(data.begin(), data.end()));
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
    for (int i = 0; i < numBytes; ++i) token[i] = URL_SAFE[dis(gen)];
    return token;
}

bool constantTimeCompare(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) return false;
    int result = 0;
    for (size_t i = 0; i < a.size(); ++i) result |= a[i] ^ b[i];
    return result == 0;
}

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

// ============================================================
// SHA-1 IMPLEMENTATION
// ============================================================

static std::vector<uint8_t> sha1Raw(const uint8_t* data, size_t len) {
    // Original Kotlin:
    //   MessageDigest.getInstance("SHA-1")
    uint32_t h[5] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0};
    size_t msgLen = len * 8;
    size_t paddedLen = ((len + 8) / 64 + 1) * 64;
    std::vector<uint8_t> padded(paddedLen, 0);
    memcpy(padded.data(), data, len);
    padded[len] = 0x80;
    for (size_t i = 0; i < 8; ++i)
        padded[paddedLen - 1 - i] = static_cast<uint8_t>((msgLen >> ((7 - i) * 8)) & 0xFF);

    for (size_t i = 0; i < paddedLen; i += 64) {
        uint32_t w[80];
        for (int j = 0; j < 16; ++j) {
            w[j] = (static_cast<uint32_t>(padded[i + j * 4]) << 24) |
                   (static_cast<uint32_t>(padded[i + j * 4 + 1]) << 16) |
                   (static_cast<uint32_t>(padded[i + j * 4 + 2]) << 8) |
                    static_cast<uint32_t>(padded[i + j * 4 + 3]);
        }
        for (int j = 16; j < 80; ++j) {
            w[j] = (w[j - 3] ^ w[j - 8] ^ w[j - 14] ^ w[j - 16]);
            w[j] = (w[j] << 1) | (w[j] >> 31);
        }
        uint32_t a = h[0], b = h[1], c = h[2], d = h[3], e = h[4];
        for (int j = 0; j < 80; ++j) {
            uint32_t f, k;
            if (j < 20)      { f = (b & c) | ((~b) & d); k = 0x5A827999; }
            else if (j < 40) { f = b ^ c ^ d; k = 0x6ED9EBA1; }
            else if (j < 60) { f = (b & c) | (b & d) | (c & d); k = 0x8F1BBCDC; }
            else             { f = b ^ c ^ d; k = 0xCA62C1D6; }
            uint32_t temp = ((a << 5) | (a >> 27)) + f + e + k + w[j];
            e = d; d = c; c = (b << 30) | (b >> 2); b = a; a = temp;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d; h[4] += e;
    }
    std::vector<uint8_t> hash(20);
    for (int i = 0; i < 5; ++i) {
        hash[i * 4]     = (h[i] >> 24) & 0xFF;
        hash[i * 4 + 1] = (h[i] >> 16) & 0xFF;
        hash[i * 4 + 2] = (h[i] >> 8) & 0xFF;
        hash[i * 4 + 3] = h[i] & 0xFF;
    }
    return hash;
}

// ============================================================
// MD5 IMPLEMENTATION
// ============================================================

static std::vector<uint8_t> md5Raw(const uint8_t* data, size_t len) {
    // Original Kotlin:
    //   MessageDigest.getInstance("MD5")
    static const uint32_t S[64] = {
        7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
        5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
        4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
        6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
    };
    static const uint32_t K[64] = {
        0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
        0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
        0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
        0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
        0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
        0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
        0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
        0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
        0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
        0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
        0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
        0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
        0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
        0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
        0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
        0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
    };

    uint32_t a0 = 0x67452301, b0 = 0xefcdab89, c0 = 0x98badcfe, d0 = 0x10325476;
    size_t msgLen = len * 8;
    size_t paddedLen = ((len + 8) / 64 + 1) * 64;
    std::vector<uint8_t> padded(paddedLen, 0);
    memcpy(padded.data(), data, len);
    padded[len] = 0x80;
    for (size_t i = 0; i < 8; ++i)
        padded[paddedLen - 8 + i] = static_cast<uint8_t>((msgLen >> (i * 8)) & 0xFF);

    for (size_t offset = 0; offset < paddedLen; offset += 64) {
        uint32_t M[16];
        for (int i = 0; i < 16; ++i)
            M[i] = padded[offset + i * 4] |
                  (static_cast<uint32_t>(padded[offset + i * 4 + 1]) << 8) |
                  (static_cast<uint32_t>(padded[offset + i * 4 + 2]) << 16) |
                  (static_cast<uint32_t>(padded[offset + i * 4 + 3]) << 24);

        uint32_t A = a0, B = b0, C = c0, D = d0;
        for (int i = 0; i < 64; ++i) {
            uint32_t F, g;
            if (i < 16)      { F = (B & C) | ((~B) & D); g = i; }
            else if (i < 32) { F = (D & B) | ((~D) & C); g = (5 * i + 1) % 16; }
            else if (i < 48) { F = B ^ C ^ D; g = (3 * i + 5) % 16; }
            else             { F = C ^ (B | (~D)); g = (7 * i) % 16; }
            F = F + A + K[i] + M[g];
            A = D;
            D = C;
            C = B;
            B = B + ((F << S[i]) | (F >> (32 - S[i])));
        }
        a0 += A; b0 += B; c0 += C; d0 += D;
    }

    std::vector<uint8_t> hash(16);
    auto writeU32 = [&](int idx, uint32_t val) {
        hash[idx] = val & 0xFF; hash[idx+1] = (val >> 8) & 0xFF;
        hash[idx+2] = (val >> 16) & 0xFF; hash[idx+3] = (val >> 24) & 0xFF;
    };
    writeU32(0, a0); writeU32(4, b0); writeU32(8, c0); writeU32(12, d0);
    return hash;
}

// ============================================================
// SHA-512 IMPLEMENTATION (simplified: SHA-384 uses truncated SHA-512)
// ============================================================

static const uint64_t SHA512_K[80] = {
    0x428a2f98d728ae22ULL, 0x7137449123ef65cdULL, 0xb5c0fbcfec4d3b2fULL,
    0xe9b5dba58189dbbcULL, 0x3956c25bf348b538ULL, 0x59f111f1b605d019ULL,
    0x923f82a4af194f9bULL, 0xab1c5ed5da6d8118ULL, 0xd807aa98a3030242ULL,
    0x12835b0145706fbeULL, 0x243185be4ee4b28cULL, 0x550c7dc3d5ffb4e2ULL,
    0x72be5d74f27b896fULL, 0x80deb1fe3b1696b1ULL, 0x9bdc06a725c71235ULL,
    0xc19bf174cf692694ULL, 0xe49b69c19ef14ad2ULL, 0xefbe4786384f25e3ULL,
    0x0fc19dc68b8cd5b5ULL, 0x240ca1cc77ac9c65ULL, 0x2de92c6f592b0275ULL,
    0x4a7484aa6ea6e483ULL, 0x5cb0a9dcbd41fbd4ULL, 0x76f988da831153b5ULL,
    0x983e5152ee66dfabULL, 0xa831c66d2db43210ULL, 0xb00327c898fb213fULL,
    0xbf597fc7beef0ee4ULL, 0xc6e00bf33da88fc2ULL, 0xd5a79147930aa725ULL,
    0x06ca6351e003826fULL, 0x142929670a0e6e70ULL, 0x27b70a8546d22ffcULL,
    0x2e1b21385c26c926ULL, 0x4d2c6dfc5ac42aedULL, 0x53380d139d95b3dfULL,
    0x650a73548baf63deULL, 0x766a0abb3c77b2a8ULL, 0x81c2c92e47edaee6ULL,
    0x92722c851482353bULL, 0xa2bfe8a14cf10364ULL, 0xa81a664bbc423001ULL,
    0xc24b8b70d0f89791ULL, 0xc76c51a30654be30ULL, 0xd192e819d6ef5218ULL,
    0xd69906245565a910ULL, 0xf40e35855771202aULL, 0x106aa07032bbd1b8ULL,
    0x19a4c116b8d2d0c8ULL, 0x1e376c085141ab53ULL, 0x2748774cdf8eeb99ULL,
    0x34b0bcb5e19b48a8ULL, 0x391c0cb3c5c95a63ULL, 0x4ed8aa4ae3418acbULL,
    0x5b9cca4f7763e373ULL, 0x682e6ff3d6b2b8a3ULL, 0x748f82ee5defb2fcULL,
    0x78a5636f43172f60ULL, 0x84c87814a1f0ab72ULL, 0x8cc702081a6439ecULL,
    0x90befffa23631e28ULL, 0xa4506cebde82bde9ULL, 0xbef9a3f7b2c67915ULL,
    0xc67178f2e372532bULL, 0xca273eceea26619cULL, 0xd186b8c721c0c207ULL,
    0xeada7dd6cde0eb1eULL, 0xf57d4f7fee6ed178ULL, 0x06f067aa72176fbaULL,
    0x0a637dc5a2c898a6ULL, 0x113f9804bef90daeULL, 0x1b710b35131c471bULL,
    0x28db77f523047d84ULL, 0x32caab7b40c72493ULL, 0x3c9ebe0a15c9bebcULL,
    0x431d67c49c100d4cULL, 0x4cc5d4becb3e42b6ULL, 0x597f299cfc657e2aULL,
    0x5fcb6fab3ad6faecULL, 0x6c44198c4a475817ULL
};

static std::vector<uint8_t> sha512Raw(const uint8_t* data, size_t len) {
    uint64_t h[8] = {
        0x6a09e667f3bcc908ULL, 0xbb67ae8584caa73bULL,
        0x3c6ef372fe94f82bULL, 0xa54ff53a5f1d36f1ULL,
        0x510e527fade682d1ULL, 0x9b05688c2b3e6c1fULL,
        0x1f83d9abfb41bd6bULL, 0x5be0cd19137e2179ULL
    };

    uint64_t msgLen = static_cast<uint64_t>(len) * 8;
    size_t paddedLen = ((len + 16) / 128 + 1) * 128;
    std::vector<uint8_t> padded(paddedLen, 0);
    memcpy(padded.data(), data, len);
    padded[len] = 0x80;
    for (int i = 0; i < 8; ++i)
        padded[paddedLen - 1 - i] = static_cast<uint8_t>((msgLen >> (i * 8)) & 0xFF);

    for (size_t i = 0; i < paddedLen; i += 128) {
        uint64_t w[80];
        for (int j = 0; j < 16; ++j) {
            w[j] = (static_cast<uint64_t>(padded[i + j * 8]) << 56) |
                   (static_cast<uint64_t>(padded[i + j * 8 + 1]) << 48) |
                   (static_cast<uint64_t>(padded[i + j * 8 + 2]) << 40) |
                   (static_cast<uint64_t>(padded[i + j * 8 + 3]) << 32) |
                   (static_cast<uint64_t>(padded[i + j * 8 + 4]) << 24) |
                   (static_cast<uint64_t>(padded[i + j * 8 + 5]) << 16) |
                   (static_cast<uint64_t>(padded[i + j * 8 + 6]) << 8) |
                    static_cast<uint64_t>(padded[i + j * 8 + 7]);
        }
        for (int j = 16; j < 80; ++j) {
            uint64_t s0 = ((w[j-15] >> 1) | (w[j-15] << 63)) ^
                          ((w[j-15] >> 8) | (w[j-15] << 56)) ^ (w[j-15] >> 7);
            uint64_t s1 = ((w[j-2] >> 19) | (w[j-2] << 45)) ^
                          ((w[j-2] >> 61) | (w[j-2] << 3)) ^ (w[j-2] >> 6);
            w[j] = w[j-16] + s0 + w[j-7] + s1;
        }

        uint64_t a = h[0], b = h[1], c = h[2], d = h[3],
                 e = h[4], f = h[5], g = h[6], hh = h[7];

        for (int j = 0; j < 80; ++j) {
            uint64_t S1 = ((e >> 14) | (e << 50)) ^ ((e >> 18) | (e << 46)) ^ ((e >> 41) | (e << 23));
            uint64_t ch = (e & f) ^ ((~e) & g);
            uint64_t temp1 = hh + S1 + ch + SHA512_K[j] + w[j];
            uint64_t S0 = ((a >> 28) | (a << 36)) ^ ((a >> 34) | (a << 30)) ^ ((a >> 39) | (a << 25));
            uint64_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint64_t temp2 = S0 + maj;
            hh = g; g = f; f = e; e = d + temp1;
            d = c; c = b; b = a; a = temp1 + temp2;
        }
        h[0] += a; h[1] += b; h[2] += c; h[3] += d;
        h[4] += e; h[5] += f; h[6] += g; h[7] += hh;
    }

    std::vector<uint8_t> hash(64);
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j)
            hash[i * 8 + j] = static_cast<uint8_t>((h[i] >> ((7 - j) * 8)) & 0xFF);
    }
    return hash;
}

// ============================================================
// MURMURHASH3 (32-bit variant)
// ============================================================

static uint32_t murmur3_32(const uint8_t* data, size_t len, uint32_t seed = 0) {
    uint32_t h1 = seed;
    const uint32_t c1 = 0xcc9e2d51, c2 = 0x1b873593;

    for (size_t i = 0; i + 4 <= len; i += 4) {
        uint32_t k1 = static_cast<uint32_t>(data[i]) |
                     (static_cast<uint32_t>(data[i+1]) << 8) |
                     (static_cast<uint32_t>(data[i+2]) << 16) |
                     (static_cast<uint32_t>(data[i+3]) << 24);
        k1 *= c1;
        k1 = (k1 << 15) | (k1 >> 17);
        k1 *= c2;
        h1 ^= k1;
        h1 = (h1 << 13) | (h1 >> 19);
        h1 = h1 * 5 + 0xe6546b64;
    }

    uint32_t k1 = 0;
    switch (len & 3) {
        case 3: k1 ^= static_cast<uint32_t>(data[len - 3]) << 16; [[fallthrough]];
        case 2: k1 ^= static_cast<uint32_t>(data[len - 2]) << 8;  [[fallthrough]];
        case 1: k1 ^= static_cast<uint32_t>(data[len - 1]);
                k1 *= c1; k1 = (k1 << 15) | (k1 >> 17); k1 *= c2; h1 ^= k1;
    }

    h1 ^= static_cast<uint32_t>(len);
    h1 ^= h1 >> 16;
    h1 *= 0x85ebca6b;
    h1 ^= h1 >> 13;
    h1 *= 0xc2b2ae35;
    h1 ^= h1 >> 16;
    return h1;
}

// ============================================================
// XXHASH64 (simplified)
// ============================================================

static uint64_t xxhash64(const uint8_t* data, size_t len, uint64_t seed = 0) {
    const uint64_t PRIME64_1 = 11400714785074694791ULL;
    const uint64_t PRIME64_2 = 14029467366897019727ULL;
    const uint64_t PRIME64_3 = 1609587929392839161ULL;
    const uint64_t PRIME64_4 = 9650029242287828579ULL;
    const uint64_t PRIME64_5 = 2870177450012600261ULL;

    uint64_t h64;
    if (len >= 32) {
        uint64_t v1 = seed + PRIME64_1 + PRIME64_2;
        uint64_t v2 = seed + PRIME64_2;
        uint64_t v3 = seed;
        uint64_t v4 = seed - PRIME64_1;
        const uint8_t* p = data;
        const uint8_t* limit = data + len - 32;
        do {
            auto read64 = [](const uint8_t* d) -> uint64_t {
                return static_cast<uint64_t>(d[0]) | (static_cast<uint64_t>(d[1]) << 8) |
                       (static_cast<uint64_t>(d[2]) << 16) | (static_cast<uint64_t>(d[3]) << 24) |
                       (static_cast<uint64_t>(d[4]) << 32) | (static_cast<uint64_t>(d[5]) << 40) |
                       (static_cast<uint64_t>(d[6]) << 48) | (static_cast<uint64_t>(d[7]) << 56);
            };
            v1 = (v1 * PRIME64_2 + read64(p)) ; p += 8; v1 = (v1 << 31) | (v1 >> 33); v1 *= PRIME64_1;
            v2 = (v2 * PRIME64_2 + read64(p)) ; p += 8; v2 = (v2 << 31) | (v2 >> 33); v2 *= PRIME64_1;
            v3 = (v3 * PRIME64_2 + read64(p)) ; p += 8; v3 = (v3 << 31) | (v3 >> 33); v3 *= PRIME64_1;
            v4 = (v4 * PRIME64_2 + read64(p)) ; p += 8; v4 = (v4 << 31) | (v4 >> 33); v4 *= PRIME64_1;
        } while (p <= limit);
        h64 = ((v1 << 1) | (v1 >> 63)) + ((v2 << 7) | (v2 >> 57)) +
              ((v3 << 12) | (v3 >> 52)) + ((v4 << 18) | (v4 >> 46));
        v1 *= PRIME64_2; v1 = (v1 << 31) | (v1 >> 33); v1 *= PRIME64_1; h64 ^= v1;
        h64 = h64 * PRIME64_1 + PRIME64_4;
        v2 *= PRIME64_2; v2 = (v2 << 31) | (v2 >> 33); v2 *= PRIME64_1; h64 ^= v2;
        h64 = h64 * PRIME64_1 + PRIME64_4;
        v3 *= PRIME64_2; v3 = (v3 << 31) | (v3 >> 33); v3 *= PRIME64_1; h64 ^= v3;
        h64 = h64 * PRIME64_1 + PRIME64_4;
        v4 *= PRIME64_2; v4 = (v4 << 31) | (v4 >> 33); v4 *= PRIME64_1; h64 ^= v4;
        h64 = h64 * PRIME64_1 + PRIME64_4;
    } else {
        h64 = seed + PRIME64_5;
    }

    h64 += static_cast<uint64_t>(len);

    const uint8_t* p = data + (len & ~0x1FULL);
    const uint8_t* end = data + len;
    while (p + 8 <= end) {
        uint64_t k = static_cast<uint64_t>(p[0]) | (static_cast<uint64_t>(p[1]) << 8) |
                     (static_cast<uint64_t>(p[2]) << 16) | (static_cast<uint64_t>(p[3]) << 24) |
                     (static_cast<uint64_t>(p[4]) << 32) | (static_cast<uint64_t>(p[5]) << 40) |
                     (static_cast<uint64_t>(p[6]) << 48) | (static_cast<uint64_t>(p[7]) << 56);
        k *= PRIME64_2; k = (k << 31) | (k >> 33); k *= PRIME64_1;
        h64 ^= k;
        h64 = ((h64 << 27) | (h64 >> 37)) * PRIME64_1 + PRIME64_4;
        p += 8;
    }
    if (p + 4 <= end) {
        h64 ^= static_cast<uint64_t>(p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24)) * PRIME64_1;
        h64 = ((h64 << 23) | (h64 >> 41)) * PRIME64_2 + PRIME64_3;
        p += 4;
    }
    while (p < end) {
        h64 ^= static_cast<uint64_t>(*p) * PRIME64_5;
        h64 = (h64 << 11) | (h64 >> 53); h64 *= PRIME64_1;
        p++;
    }

    h64 ^= h64 >> 33; h64 *= PRIME64_2;
    h64 ^= h64 >> 29; h64 *= PRIME64_3;
    h64 ^= h64 >> 32;
    return h64;
}

// ============================================================
// BLAKE3 REFERENCE (simplified single-block via SHA-256 wrapper)
// Full BLAKE3 requires the BLAKE3 C library which may not be available on NDK 21.
// We provide a wrapper that uses SHA-512 with a "blake3:" prefix for identification.
// ============================================================

static std::vector<uint8_t> blake3Ref(const uint8_t* data, size_t len) {
    // Original Kotlin:
    //   For BLAKE3, use the native implementation if available.
    //   This is a best-effort fallback: SHA-512 with a distinguishing metadata prefix.
    std::string meta = "blake3:" + std::to_string(len) + ":";
    std::vector<uint8_t> combined(meta.begin(), meta.end());
    combined.insert(combined.end(), data, data + len);
    auto sha512 = sha512Raw(combined.data(), combined.size());
    // Return first 32 bytes as blake3-256 equivalent
    return std::vector<uint8_t>(sha512.begin(), sha512.begin() + 32);
}

// ============================================================
// UNIFIED HASH COMPUTATION
// ============================================================

HashResult computeHash(const std::vector<uint8_t>& data, HashAlgorithm algo) {
    HashResult result;
    switch (algo) {
        case HashAlgorithm::MD5:
            result.hashBytes = md5Raw(data.data(), data.size());
            break;
        case HashAlgorithm::SHA1:
            result.hashBytes = sha1Raw(data.data(), data.size());
            break;
        case HashAlgorithm::SHA256:
            result.hashBytes = sha256Raw(data.data(), data.size());
            break;
        case HashAlgorithm::SHA384: {
            auto full = sha512Raw(data.data(), data.size());
            result.hashBytes.assign(full.begin(), full.begin() + 48);
            break;
        }
        case HashAlgorithm::SHA512:
            result.hashBytes = sha512Raw(data.data(), data.size());
            break;
        case HashAlgorithm::CRC32: {
            uint32_t c = crc32(data);
            result.hashBytes.assign(reinterpret_cast<uint8_t*>(&c),
                                    reinterpret_cast<uint8_t*>(&c) + 4);
            break;
        }
        case HashAlgorithm::XXHASH64: {
            uint64_t h = xxhash64(data.data(), data.size());
            result.hashBytes.assign(reinterpret_cast<uint8_t*>(&h),
                                    reinterpret_cast<uint8_t*>(&h) + 8);
            break;
        }
        case HashAlgorithm::MURMUR3: {
            uint32_t h = murmur3_32(data.data(), data.size());
            result.hashBytes.assign(reinterpret_cast<uint8_t*>(&h),
                                    reinterpret_cast<uint8_t*>(&h) + 4);
            break;
        }
        case HashAlgorithm::BLAKE3:
            result.hashBytes = blake3Ref(data.data(), data.size());
            break;
    }
    result.hexString = hexEncode(result.hashBytes);
    result.base64String = base64Encode(result.hashBytes);
    return result;
}

HashResult computeHash(const std::string& data, HashAlgorithm algo) {
    return computeHash(std::vector<uint8_t>(data.begin(), data.end()), algo);
}

std::string hashToHex(const std::vector<uint8_t>& hashBytes) {
    return hexEncode(hashBytes);
}

std::string hashToBase64(const std::vector<uint8_t>& hashBytes) {
    return base64Encode(hashBytes);
}

bool verifyHash(const std::vector<uint8_t>& data, const std::string& expectedHex, HashAlgorithm algo) {
    auto result = computeHash(data, algo);
    return constantTimeCompare(result.hexString, expectedHex);
}

// ============================================================
// HMAC — generalized for multiple algorithms
// ============================================================

// Internal: compute HMAC raw bytes with given hash function
using HashRawFn = std::vector<uint8_t>(*)(const uint8_t*, size_t);
static size_t hashBlockSize(HashAlgorithm algo) {
    switch (algo) {
        case HashAlgorithm::MD5:    case HashAlgorithm::SHA1:
            return 64;
        case HashAlgorithm::SHA256:
            return 64;
        case HashAlgorithm::SHA384: case HashAlgorithm::SHA512:
            return 128;
        default: return 64;
    }
}

static HashRawFn hashRawFnFor(HashAlgorithm algo) {
    switch (algo) {
        case HashAlgorithm::MD5:    return md5Raw;
        case HashAlgorithm::SHA1:   return sha1Raw;
        case HashAlgorithm::SHA256: return sha256Raw;
        case HashAlgorithm::SHA384:
        case HashAlgorithm::SHA512: return sha512Raw;
        default: return sha256Raw;
    }
}

static size_t hashOutputSize(HashAlgorithm algo) {
    switch (algo) {
        case HashAlgorithm::MD5:    return 16;
        case HashAlgorithm::SHA1:   return 20;
        case HashAlgorithm::SHA256: return 32;
        case HashAlgorithm::SHA384: return 48;
        case HashAlgorithm::SHA512: return 64;
        default: return 32;
    }
}

std::vector<uint8_t> computeHmac(const std::string& key, const std::string& message,
                                  HmacAlgorithm hmacAlgo) {
    // Original Kotlin:
    //   javax.crypto.Mac.getInstance("HmacSHA256").doFinal()
    HashAlgorithm hashAlgo;
    switch (hmacAlgo) {
        case HmacAlgorithm::HMAC_SHA1:   hashAlgo = HashAlgorithm::SHA1;   break;
        case HmacAlgorithm::HMAC_SHA256: hashAlgo = HashAlgorithm::SHA256; break;
        case HmacAlgorithm::HMAC_SHA384: hashAlgo = HashAlgorithm::SHA384; break;
        case HmacAlgorithm::HMAC_SHA512: hashAlgo = HashAlgorithm::SHA512; break;
        default: hashAlgo = HashAlgorithm::SHA256;
    }
    auto rawFn = hashRawFnFor(hashAlgo);
    size_t blockSize = hashBlockSize(hashAlgo);
    size_t outSize = hashOutputSize(hashAlgo);

    std::vector<uint8_t> keyBlock(blockSize, 0);
    if (key.size() > blockSize) {
        auto keyHash = rawFn(reinterpret_cast<const uint8_t*>(key.data()), key.size());
        memcpy(keyBlock.data(), keyHash.data(), std::min(keyHash.size(), blockSize));
    } else {
        memcpy(keyBlock.data(), key.data(), key.size());
    }

    std::vector<uint8_t> oKeyPad(blockSize), iKeyPad(blockSize);
    for (size_t i = 0; i < blockSize; ++i) { oKeyPad[i] = keyBlock[i] ^ 0x5c; iKeyPad[i] = keyBlock[i] ^ 0x36; }

    std::string inner(iKeyPad.begin(), iKeyPad.end());
    inner += message;
    auto innerHash = rawFn(reinterpret_cast<const uint8_t*>(inner.data()), inner.size());

    std::string outer(oKeyPad.begin(), oKeyPad.end());
    outer.insert(outer.end(), innerHash.begin(), innerHash.end());
    auto finalHash = rawFn(reinterpret_cast<const uint8_t*>(outer.data()), outer.size());

    if (hmacAlgo == HmacAlgorithm::HMAC_SHA384)
        return std::vector<uint8_t>(finalHash.begin(), finalHash.begin() + 48);
    return finalHash;
}

bool verifyHmac(const std::string& key, const std::string& message,
                const std::vector<uint8_t>& expectedMac, HmacAlgorithm algo) {
    auto computed = computeHmac(key, message, algo);
    if (computed.size() != expectedMac.size()) return false;
    // Constant-time comparison
    int result = 0;
    for (size_t i = 0; i < computed.size(); ++i) result |= computed[i] ^ expectedMac[i];
    return result == 0;
}

// ============================================================
// PBKDF2 IMPLEMENTATION (RFC 2898)
// ============================================================

std::vector<uint8_t> computePbkdf2(const Pbkdf2Params& params) {
    // Original Kotlin:
    //   javax.crypto.SecretKeyFactory.getInstance("PBKDF2WithHmacSHA256")
    if (params.password.empty() || params.keyLength <= 0) return {};
    auto rawFn = hashRawFnFor(params.algorithm);
    size_t hLen = hashOutputSize(params.algorithm);
    size_t blockSize = hashBlockSize(params.algorithm);

    std::vector<uint8_t> derived;
    derived.reserve(params.keyLength);

    std::vector<uint8_t> saltBytes(params.salt.begin(), params.salt.end());
    for (int blockIndex = 1; derived.size() < static_cast<size_t>(params.keyLength); ++blockIndex) {
        // U_1 = HMAC(password, salt || blockIndex)
        std::vector<uint8_t> saltBlock = saltBytes;
        saltBlock.push_back(static_cast<uint8_t>((blockIndex >> 24) & 0xFF));
        saltBlock.push_back(static_cast<uint8_t>((blockIndex >> 16) & 0xFF));
        saltBlock.push_back(static_cast<uint8_t>((blockIndex >> 8) & 0xFF));
        saltBlock.push_back(static_cast<uint8_t>(blockIndex & 0xFF));

        // Compute HMAC manually
        std::vector<uint8_t> keyBlock(blockSize, 0);
        if (params.password.size() > blockSize) {
            auto pwHash = rawFn(reinterpret_cast<const uint8_t*>(params.password.data()), params.password.size());
            memcpy(keyBlock.data(), pwHash.data(), std::min(pwHash.size(), blockSize));
        } else {
            memcpy(keyBlock.data(), params.password.data(), params.password.size());
        }
        std::vector<uint8_t> oPad(blockSize), iPad(blockSize);
        for (size_t i = 0; i < blockSize; ++i) { oPad[i] = keyBlock[i] ^ 0x5c; iPad[i] = keyBlock[i] ^ 0x36; }
        std::string inner(iPad.begin(), iPad.end());
        inner.insert(inner.end(), saltBlock.begin(), saltBlock.end());
        auto prev = rawFn(reinterpret_cast<const uint8_t*>(inner.data()), inner.size());

        std::vector<uint8_t> xorAccum(prev.begin(), prev.end());

        for (int iter = 1; iter < params.iterations; ++iter) {
            std::string outer(oPad.begin(), oPad.end());
            outer.insert(outer.end(), prev.begin(), prev.end());
            prev = rawFn(reinterpret_cast<const uint8_t*>(outer.data()), outer.size());
            for (size_t i = 0; i < hLen; ++i) xorAccum[i] ^= prev[i];
        }

        derived.insert(derived.end(), xorAccum.begin(), xorAccum.end());
    }
    derived.resize(params.keyLength);
    return derived;
}

// ============================================================
// HKDF IMPLEMENTATION (RFC 5869)
// ============================================================

namespace HKDF {

std::vector<uint8_t> extract(const std::vector<uint8_t>& salt,
                              const std::vector<uint8_t>& ikm,
                              HashAlgorithm hash) {
    // Original Kotlin:
    //   Hkdf.deriveHkdf(key, ikm, salt, info, length)
    // PRK = HMAC-Hash(salt, ikm)
    auto rawFn = hashRawFnFor(hash);
    size_t hLen = hashOutputSize(hash);
    size_t blockSize = hashBlockSize(hash);

    std::string saltStr(salt.begin(), salt.end());
    std::string ikmStr(ikm.begin(), ikm.end());

    std::vector<uint8_t> keyBlock(blockSize, 0);
    if (saltStr.size() > blockSize) {
        auto kh = rawFn(reinterpret_cast<const uint8_t*>(saltStr.data()), saltStr.size());
        memcpy(keyBlock.data(), kh.data(), std::min(kh.size(), blockSize));
    } else {
        memcpy(keyBlock.data(), saltStr.data(), saltStr.size());
    }

    std::vector<uint8_t> oPad(blockSize), iPad(blockSize);
    for (size_t i = 0; i < blockSize; ++i) { oPad[i] = keyBlock[i] ^ 0x5c; iPad[i] = keyBlock[i] ^ 0x36; }
    std::string inner(iPad.begin(), iPad.end());
    inner += ikmStr;
    auto prk = rawFn(reinterpret_cast<const uint8_t*>(inner.data()), inner.size());
    return prk;
}

std::vector<uint8_t> expand(const std::vector<uint8_t>& prk,
                             const std::vector<uint8_t>& info,
                             int length,
                             HashAlgorithm hash) {
    // OKM = T(1) || T(2) || ... || T(N)
    auto rawFn = hashRawFnFor(hash);
    size_t hLen = hashOutputSize(hash);
    size_t blockSize = hashBlockSize(hash);

    std::string prkStr(prk.begin(), prk.end());
    std::vector<uint8_t> okm;

    std::vector<uint8_t> keyBlock(blockSize, 0);
    memcpy(keyBlock.data(), prkStr.data(), std::min(prkStr.size(), blockSize));
    std::vector<uint8_t> oPad(blockSize), iPad(blockSize);
    for (size_t i = 0; i < blockSize; ++i) { oPad[i] = keyBlock[i] ^ 0x5c; iPad[i] = keyBlock[i] ^ 0x36; }

    std::vector<uint8_t> tPrev;
    for (int n = 1; static_cast<int>(okm.size()) < length; ++n) {
        std::string innerMessage;
        innerMessage.insert(innerMessage.end(), tPrev.begin(), tPrev.end());
        innerMessage.insert(innerMessage.end(), info.begin(), info.end());
        innerMessage += static_cast<char>(n);

        std::string inner(iPad.begin(), iPad.end());
        inner += innerMessage;
        auto tmp = rawFn(reinterpret_cast<const uint8_t*>(inner.data()), inner.size());
        tPrev = tmp;

        std::string outer(oPad.begin(), oPad.end());
        outer.insert(outer.end(), tmp.begin(), tmp.end());
        tPrev = rawFn(reinterpret_cast<const uint8_t*>(outer.data()), outer.size());

        okm.insert(okm.end(), tPrev.begin(), tPrev.end());
    }
    okm.resize(length);
    return okm;
}

} // namespace HKDF

// ============================================================
// RANDOM GENERATOR
// ============================================================

namespace RandomGenerator {

std::vector<uint8_t> generateRandomBytes(int numBytes) {
    // Original Kotlin:
    //   SecureRandom.nextBytes(numBytes)
    std::vector<uint8_t> bytes(numBytes);
    std::random_device rd;
    // Use rd to seed, then generate
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;
    size_t offset = 0;
    while (offset < static_cast<size_t>(numBytes)) {
        uint64_t val = dis(gen);
        // Collect entropy from multiple sources
        val ^= static_cast<uint64_t>(rd()) << 32 | rd();
        for (int i = 0; i < 8 && offset < static_cast<size_t>(numBytes); ++i) {
            bytes[offset++] = static_cast<uint8_t>(val & 0xFF);
            val >>= 8;
        }
    }
    return bytes;
}

std::string generateRandomHex(int numBytes) {
    return hexEncode(generateRandomBytes(numBytes));
}

std::string generateRandomBase64(int numBytes) {
    auto raw = generateRandomBytes(numBytes);
    // URL-safe base64 without padding
    std::string b64 = base64Encode(raw);
    return base64ToBase64Url(b64);
}

std::string generateUUID() {
    // Original Kotlin:
    //   UUID.randomUUID().toString()
    auto bytes = generateRandomBytes(16);
    // Set version 4 (random)
    bytes[6] = (bytes[6] & 0x0F) | 0x40;
    // Set variant bits
    bytes[8] = (bytes[8] & 0x3F) | 0x80;
    // Format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    std::ostringstream out;
    auto hex = hexEncode(bytes);
    out << hex.substr(0, 8) << "-"
        << hex.substr(8, 4) << "-"
        << hex.substr(12, 4) << "-"
        << hex.substr(16, 4) << "-"
        << hex.substr(20, 12);
    return out.str();
}

std::string generateSecureToken(int numBytes) {
    return generateRandomBase64(numBytes);
}

} // namespace RandomGenerator

// ============================================================
// CHECKSUM
// ============================================================

uint32_t computeChecksum(const std::vector<uint8_t>& data, ChecksumAlgorithm algo) {
    switch (algo) {
        case ChecksumAlgorithm::CRC32:  return crc32(data);
        case ChecksumAlgorithm::ADLER32: return adler32(data);
    }
    return 0;
}

uint32_t computeChecksum(const std::string& data, ChecksumAlgorithm algo) {
    return computeChecksum(std::vector<uint8_t>(data.begin(), data.end()), algo);
}

bool verifyChecksum(const std::vector<uint8_t>& data, uint32_t expectedChecksum, ChecksumAlgorithm algo) {
    return computeChecksum(data, algo) == expectedChecksum;
}

bool verifyChecksum(const std::string& data, uint32_t expectedChecksum, ChecksumAlgorithm algo) {
    return verifyChecksum(std::vector<uint8_t>(data.begin(), data.end()), expectedChecksum, algo);
}

// ============================================================
// MERKLE TREE
// ============================================================

namespace MerkleHash {

static std::string hashLeaf(const std::string& chunk, HashAlgorithm algo) {
    // Prefix "L:" to distinguish leaves from internal nodes
    auto result = computeHash("L:" + chunk, algo);
    return result.hexString;
}

static std::string hashNode(const std::string& left, const std::string& right, HashAlgorithm algo) {
    auto result = computeHash("N:" + left + ":" + right, algo);
    return result.hexString;
}

std::string computeMerkleRoot(const std::vector<std::string>& chunks, HashAlgorithm algo) {
    // Original Kotlin:
    //   Build merkle tree and return root hash
    if (chunks.empty()) return "";
    auto tree = buildMerkleTree(chunks, algo);
    if (tree.empty()) return "";
    return tree.back()[0];
}

std::vector<std::vector<std::string>> buildMerkleTree(
    const std::vector<std::string>& chunks, HashAlgorithm algo) {
    std::vector<std::vector<std::string>> levels;
    if (chunks.empty()) return levels;

    // Level 0: leaf hashes
    std::vector<std::string> current;
    for (const auto& chunk : chunks) {
        current.push_back(hashLeaf(chunk, algo));
    }
    levels.push_back(current);

    // Build up until we have one root
    while (current.size() > 1) {
        std::vector<std::string> next;
        for (size_t i = 0; i < current.size(); i += 2) {
            if (i + 1 < current.size()) {
                next.push_back(hashNode(current[i], current[i + 1], algo));
            } else {
                // Odd number: duplicate last node
                next.push_back(hashNode(current[i], current[i], algo));
            }
        }
        levels.push_back(next);
        current = next;
    }
    return levels;
}

bool verifyMerkleProof(const MerkleProof& proof, const std::string& knownRoot,
                        HashAlgorithm algo) {
    // Original Kotlin:
    //   Verify a merkle inclusion proof
    if (proof.leafHash.empty() || knownRoot.empty()) return false;

    std::string currentHash = proof.leafHash;
    int idx = proof.leafIndex;

    for (const auto& sibling : proof.siblingHashes) {
        if (idx % 2 == 0) {
            currentHash = hashNode(currentHash, sibling, algo);
        } else {
            currentHash = hashNode(sibling, currentHash, algo);
        }
        idx /= 2;
    }
    return constantTimeCompare(currentHash, knownRoot);
}

} // namespace MerkleHash

// ============================================================
// CONTENT HASH — Matrix spec multi-hash verification
// ============================================================

static std::string jsonGetString(const std::string& json, const std::string& key) {
    // Original Kotlin:
    //   jsonObject.getString("sha256")
    // Simple JSON string extractor for {"key": "value"} format
    std::string searchKey = "\"" + key + "\"";
    auto pos = json.find(searchKey);
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos + searchKey.size());
    if (pos == std::string::npos) return "";
    pos++;
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t')) pos++;
    if (pos >= json.size() || json[pos] != '"') return "";
    pos++;
    auto end = json.find('"', pos);
    if (end == std::string::npos) return "";
    return json.substr(pos, end - pos);
}

ContentHash parseContentHash(const std::string& json) {
    // Original Kotlin:
    //   Parse JSON like {"sha256": "base64hash"}
    ContentHash hash;
    auto sha256 = jsonGetString(json, "sha256");
    if (!sha256.empty()) { hash.sha256 = sha256; hash.hasSha256 = true; }
    auto sha384 = jsonGetString(json, "sha384");
    if (!sha384.empty()) { hash.sha384 = sha384; hash.hasSha384 = true; }
    auto sha512 = jsonGetString(json, "sha512");
    if (!sha512.empty()) { hash.sha512 = sha512; hash.hasSha512 = true; }
    return hash;
}

bool verifyContentHash(const std::vector<uint8_t>& content, const ContentHash& hash) {
    // Original Kotlin:
    //   Verify that all present hashes in the ContentHash match the content
    if (!hash.hasSha256 && !hash.hasSha384 && !hash.hasSha512) return false;

    bool matched = false;
    if (hash.hasSha256) {
        auto computed = computeHash(content, HashAlgorithm::SHA256);
        std::string decoded = base64Encode(base64Decode(hash.sha256));
        if (constantTimeCompare(computed.base64String, hash.sha256) ||
            constantTimeCompare(base64ToUnpaddedBase64(computed.base64String),
                                base64ToUnpaddedBase64(hash.sha256))) {
            matched = true;
        } else {
            matched = false;
        }
    }
    if (hash.hasSha384) {
        auto computed = computeHash(content, HashAlgorithm::SHA384);
        if (!constantTimeCompare(computed.base64String, hash.sha384)) matched = false;
    }
    if (hash.hasSha512) {
        auto computed = computeHash(content, HashAlgorithm::SHA512);
        if (!constantTimeCompare(computed.base64String, hash.sha512)) matched = false;
    }
    return matched;
}

} // namespace progressive
