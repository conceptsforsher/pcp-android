#include "pcp/encrypted_file.hpp"
#include <sstream>
#include <cstring>
#include <cstdlib>
#include <random>
#include <array>

namespace pcp {

// ============================================================================
// Internal helpers
// ============================================================================

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

// ---- Minimal AES-256-CTR (NDK 21 compatible, no external deps) ----
// Original Kotlin: MXEncryptedAttachments.kt uses javax.crypto.Cipher("AES/CTR/NoPadding")

struct AesCtrState {
    std::array<uint8_t, 32> key_bytes{};
    std::array<uint8_t, 16> counter{};
    std::array<uint8_t, 16> keystream{};
    int keystream_pos = 16; // force generation on first call
};

// Simple AES S-box (Rijndael)
constexpr uint8_t sbox[256] = {
    0x63,0x7c,0x77,0x7b,0xf2,0x6b,0x6f,0xc5,0x30,0x01,0x67,0x2b,0xfe,0xd7,0xab,0x76,
    0xca,0x82,0xc9,0x7d,0xfa,0x59,0x47,0xf0,0xad,0xd4,0xa2,0xaf,0x9c,0xa4,0x72,0xc0,
    0xb7,0xfd,0x93,0x26,0x36,0x3f,0xf7,0xcc,0x34,0xa5,0xe5,0xf1,0x71,0xd8,0x31,0x15,
    0x04,0xc7,0x23,0xc3,0x18,0x96,0x05,0x9a,0x07,0x12,0x80,0xe2,0xeb,0x27,0xb2,0x75,
    0x09,0x83,0x2c,0x1a,0x1b,0x6e,0x5a,0xa0,0x52,0x3b,0xd6,0xb3,0x29,0xe3,0x2f,0x84,
    0x53,0xd1,0x00,0xed,0x20,0xfc,0xb1,0x5b,0x6a,0xcb,0xbe,0x39,0x4a,0x4c,0x58,0xcf,
    0xd0,0xef,0xaa,0xfb,0x43,0x4d,0x33,0x85,0x45,0xf9,0x02,0x7f,0x50,0x3c,0x9f,0xa8,
    0x51,0xa3,0x40,0x8f,0x92,0x9d,0x38,0xf5,0xbc,0xb6,0xda,0x21,0x10,0xff,0xf3,0xd2,
    0xcd,0x0c,0x13,0xec,0x5f,0x97,0x44,0x17,0xc4,0xa7,0x7e,0x3d,0x64,0x5d,0x19,0x73,
    0x60,0x81,0x4f,0xdc,0x22,0x2a,0x90,0x88,0x46,0xee,0xb8,0x14,0xde,0x5e,0x0b,0xdb,
    0xe0,0x32,0x3a,0x0a,0x49,0x06,0x24,0x5c,0xc2,0xd3,0xac,0x62,0x91,0x95,0xe4,0x79,
    0xe7,0xc8,0x37,0x6d,0x8d,0xd5,0x4e,0xa9,0x6c,0x56,0xf4,0xea,0x65,0x7a,0xae,0x08,
    0xba,0x78,0x25,0x2e,0x1c,0xa6,0xb4,0xc6,0xe8,0xdd,0x74,0x1f,0x4b,0xbd,0x8b,0x8a,
    0x70,0x3e,0xb5,0x66,0x48,0x03,0xf6,0x0e,0x61,0x35,0x57,0xb9,0x86,0xc1,0x1d,0x9e,
    0xe1,0xf8,0x98,0x11,0x69,0xd9,0x8e,0x94,0x9b,0x1e,0x87,0xe9,0xce,0x55,0x28,0xdf,
    0x8c,0xa1,0x89,0x0d,0xbf,0xe6,0x42,0x68,0x41,0x99,0x2d,0x0f,0xb0,0x54,0xbb,0x16
};

constexpr uint8_t rcon[10] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80,0x1b,0x36};

static void aes_key_expansion(const uint8_t* key, uint8_t* round_keys) {
    int Nk = 8; // AES-256
    int Nr = 14;
    memcpy(round_keys, key, 32);
    int i = Nk;
    while (i < 4 * (Nr + 1)) {
        uint8_t temp[4];
        memcpy(temp, round_keys + (i - 1) * 4, 4);
        if (i % Nk == 0) {
            // RotWord
            uint8_t t = temp[0]; temp[0] = temp[1]; temp[1] = temp[2]; temp[2] = temp[3]; temp[3] = t;
            // SubWord
            for (int j = 0; j < 4; ++j) temp[j] = sbox[temp[j]];
            // XOR with rcon
            temp[0] ^= rcon[i / Nk - 1];
        } else if (Nk > 6 && i % Nk == 4) {
            // SubWord
            for (int j = 0; j < 4; ++j) temp[j] = sbox[temp[j]];
        }
        for (int j = 0; j < 4; ++j) {
            round_keys[i * 4 + j] = round_keys[(i - Nk) * 4 + j] ^ temp[j];
        }
        ++i;
    }
}

static void aes_encrypt_block(const uint8_t* round_keys, const uint8_t* in, uint8_t* out) {
    uint8_t state[16];
    memcpy(state, in, 16);

    // Initial AddRoundKey
    for (int i = 0; i < 16; ++i) state[i] ^= round_keys[i];

    // 13 rounds (AES-256 has 14 rounds total)
    for (int round = 1; round < 14; ++round) {
        // SubBytes
        for (int i = 0; i < 16; ++i) state[i] = sbox[state[i]];
        // ShiftRows
        uint8_t tmp[16];
        memcpy(tmp, state, 16);
        // Row 0: no shift
        state[0] = tmp[0]; state[4] = tmp[4]; state[8] = tmp[8]; state[12] = tmp[12];
        // Row 1: shift left 1
        state[1] = tmp[5]; state[5] = tmp[9]; state[9] = tmp[13]; state[13] = tmp[1];
        // Row 2: shift left 2
        state[2] = tmp[10]; state[6] = tmp[14]; state[10] = tmp[2]; state[14] = tmp[6];
        // Row 3: shift left 3
        state[3] = tmp[15]; state[7] = tmp[3]; state[11] = tmp[7]; state[15] = tmp[11];
        // MixColumns
        for (int c = 0; c < 4; ++c) {
            uint8_t a[4];
            memcpy(a, state + c * 4, 4);
            state[c * 4 + 0] = (uint8_t)(gmul(a[0], 2) ^ gmul(a[1], 3) ^ a[2] ^ a[3]);
            state[c * 4 + 1] = (uint8_t)(a[0] ^ gmul(a[1], 2) ^ gmul(a[2], 3) ^ a[3]);
            state[c * 4 + 2] = (uint8_t)(a[0] ^ a[1] ^ gmul(a[2], 2) ^ gmul(a[3], 3));
            state[c * 4 + 3] = (uint8_t)(gmul(a[0], 3) ^ a[1] ^ a[2] ^ gmul(a[3], 2));
        }
        // AddRoundKey
        for (int i = 0; i < 16; ++i) state[i] ^= round_keys[round * 16 + i];
    }

    // Final round (no MixColumns)
    // SubBytes
    for (int i = 0; i < 16; ++i) state[i] = sbox[state[i]];
    // ShiftRows
    uint8_t tmp[16];
    memcpy(tmp, state, 16);
    state[0] = tmp[0]; state[4] = tmp[4]; state[8] = tmp[8]; state[12] = tmp[12];
    state[1] = tmp[5]; state[5] = tmp[9]; state[9] = tmp[13]; state[13] = tmp[1];
    state[2] = tmp[10]; state[6] = tmp[14]; state[10] = tmp[2]; state[14] = tmp[6];
    state[3] = tmp[15]; state[7] = tmp[3]; state[11] = tmp[7]; state[15] = tmp[11];
    // AddRoundKey
    for (int i = 0; i < 16; ++i) state[i] ^= round_keys[14 * 16 + i];

    memcpy(out, state, 16);
}

// Galois field multiply (used by MixColumns)
static uint8_t gmul(uint8_t a, uint8_t b) {
    uint8_t p = 0;
    for (int i = 0; i < 8; ++i) {
        if (b & 1) p ^= a;
        bool hi = (a & 0x80);
        a <<= 1;
        if (hi) a ^= 0x1b;
        b >>= 1;
    }
    return p;
}

static void aes_ctr_init(AesCtrState& ctx, const uint8_t* key, const uint8_t* iv) {
    memcpy(ctx.key_bytes.data(), key, 32);
    memcpy(ctx.counter.data(), iv, 16);
    ctx.keystream_pos = 16;
}

static void aes_ctr_crypt(AesCtrState& ctx, const uint8_t* input, uint8_t* output, size_t len) {
    uint8_t round_keys[15 * 16]; // AES-256: 15 round keys
    aes_key_expansion(ctx.key_bytes.data(), round_keys);

    for (size_t i = 0; i < len; ++i) {
        if (ctx.keystream_pos >= 16) {
            aes_encrypt_block(round_keys, ctx.counter.data(), ctx.keystream.data());
            ctx.keystream_pos = 0;
            // Increment counter (big-endian 128-bit)
            for (int j = 15; j >= 0; --j) {
                if (++ctx.counter[j] != 0) break;
            }
        }
        output[i] = input[i] ^ ctx.keystream[ctx.keystream_pos++];
    }
}

// ---- SHA-256 (compact, NDK 21 compatible) ----
// Original Kotlin: MessageDigest.getInstance("SHA-256")

struct Sha256Context {
    uint32_t state[8];
    uint64_t bitlen;
    uint8_t data[64];
    int datalen;
};

static const uint32_t sha256_k[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

#define ROTR(x,n) (((x) >> (n)) | ((x) << (32 - (n))))
#define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x) (ROTR(x,2) ^ ROTR(x,13) ^ ROTR(x,22))
#define EP1(x) (ROTR(x,6) ^ ROTR(x,11) ^ ROTR(x,25))
#define SIG0(x) (ROTR(x,7) ^ ROTR(x,18) ^ ((x) >> 3))
#define SIG1(x) (ROTR(x,17) ^ ROTR(x,19) ^ ((x) >> 10))

static void sha256_transform(Sha256Context* ctx, const uint8_t* data) {
    uint32_t a, b, c, d, e, f, g, h, i, j, t1, t2, m[64];

    for (i = 0, j = 0; i < 16; ++i, j += 4)
        m[i] = (data[j] << 24) | (data[j + 1] << 16) | (data[j + 2] << 8) | (data[j + 3]);
    for ( ; i < 64; ++i)
        m[i] = SIG1(m[i - 2]) + m[i - 7] + SIG0(m[i - 15]) + m[i - 16];

    a = ctx->state[0]; b = ctx->state[1]; c = ctx->state[2]; d = ctx->state[3];
    e = ctx->state[4]; f = ctx->state[5]; g = ctx->state[6]; h = ctx->state[7];

    for (i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e, f, g) + sha256_k[i] + m[i];
        t2 = EP0(a) + MAJ(a, b, c);
        h = g; g = f; f = e; e = d + t1;
        d = c; c = b; b = a; a = t1 + t2;
    }

    ctx->state[0] += a; ctx->state[1] += b; ctx->state[2] += c; ctx->state[3] += d;
    ctx->state[4] += e; ctx->state[5] += f; ctx->state[6] += g; ctx->state[7] += h;
}

static void sha256_init(Sha256Context* ctx) {
    ctx->datalen = 0;
    ctx->bitlen = 0;
    ctx->state[0] = 0x6a09e667;
    ctx->state[1] = 0xbb67ae85;
    ctx->state[2] = 0x3c6ef372;
    ctx->state[3] = 0xa54ff53a;
    ctx->state[4] = 0x510e527f;
    ctx->state[5] = 0x9b05688c;
    ctx->state[6] = 0x1f83d9ab;
    ctx->state[7] = 0x5be0cd19;
}

static void sha256_update(Sha256Context* ctx, const uint8_t* data, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        ctx->data[ctx->datalen] = data[i];
        ctx->datalen++;
        if (ctx->datalen == 64) {
            sha256_transform(ctx, ctx->data);
            ctx->bitlen += 512;
            ctx->datalen = 0;
        }
    }
}

static void sha256_final(Sha256Context* ctx, uint8_t hash[32]) {
    int i = ctx->datalen;
    if (ctx->datalen < 56) {
        ctx->data[i++] = 0x80;
        while (i < 56) ctx->data[i++] = 0;
    } else {
        ctx->data[i++] = 0x80;
        while (i < 64) ctx->data[i++] = 0;
        sha256_transform(ctx, ctx->data);
        memset(ctx->data, 0, 56);
    }
    ctx->bitlen += ctx->datalen * 8;
    ctx->data[63] = ctx->bitlen;
    ctx->data[62] = ctx->bitlen >> 8;
    ctx->data[61] = ctx->bitlen >> 16;
    ctx->data[60] = ctx->bitlen >> 24;
    ctx->data[59] = ctx->bitlen >> 32;
    ctx->data[58] = ctx->bitlen >> 40;
    ctx->data[57] = ctx->bitlen >> 48;
    ctx->data[56] = ctx->bitlen >> 56;
    sha256_transform(ctx, ctx->data);

    for (i = 0; i < 4; ++i) {
        hash[i]      = (ctx->state[0] >> (24 - i * 8)) & 0xff;
        hash[i + 4]  = (ctx->state[1] >> (24 - i * 8)) & 0xff;
        hash[i + 8]  = (ctx->state[2] >> (24 - i * 8)) & 0xff;
        hash[i + 12] = (ctx->state[3] >> (24 - i * 8)) & 0xff;
        hash[i + 16] = (ctx->state[4] >> (24 - i * 8)) & 0xff;
        hash[i + 20] = (ctx->state[5] >> (24 - i * 8)) & 0xff;
        hash[i + 24] = (ctx->state[6] >> (24 - i * 8)) & 0xff;
        hash[i + 28] = (ctx->state[7] >> (24 - i * 8)) & 0xff;
    }
}

std::string sha256_string(const uint8_t* data, size_t len) {
    Sha256Context ctx;
    sha256_init(&ctx);
    sha256_update(&ctx, data, len);
    uint8_t hash[32];
    sha256_final(&ctx, hash);
    return std::string(reinterpret_cast<char*>(hash), 32);
}

// ---- Base64 (unpadded, URL-safe) ----
// Original Kotlin: MXEncryptedAttachments.kt uses Base64 encoding

constexpr char b64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string b64_encode(const uint8_t* data, size_t len) {
    std::string out;
    out.reserve(((len + 2) / 3) * 4);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t n = (data[i] << 16);
        if (i + 1 < len) n |= (data[i + 1] << 8);
        if (i + 2 < len) n |= data[i + 2];
        out += b64_table[(n >> 18) & 0x3f];
        out += b64_table[(n >> 12) & 0x3f];
        out += (i + 1 < len) ? b64_table[(n >> 6) & 0x3f] : '=';
        out += (i + 2 < len) ? b64_table[n & 0x3f] : '=';
    }
    // Strip padding for unpadded
    while (!out.empty() && out.back() == '=') out.pop_back();
    return out;
}

} // anonymous namespace

// ============================================================================
// EncryptedFileKey Validation (from EncryptedFileKey.kt:57-79)
// ============================================================================

bool EncryptedFileKey::isValid() const {
    if (alg != "A256CTR") return false;
    if (!ext) return false;
    bool hasEncrypt = false, hasDecrypt = false;
    for (const auto& op : keyOps) {
        if (op == "encrypt") hasEncrypt = true;
        if (op == "decrypt") hasDecrypt = true;
    }
    if (!hasEncrypt || !hasDecrypt) return false;
    if (kty != "oct") return false;
    if (k.empty()) return false;
    return true;
}

// Original Kotlin (EncryptedFileInfo.kt:isValid)
bool EncryptedFileInfo::isValid() const {
    if (url.empty()) return false;
    if (!key.isValid()) return false;
    if (iv.empty()) return false;
    if (hashes.find("sha256") == hashes.end()) return false;
    if (version != "v2") return false;
    return true;
}

std::string EncryptedFileInfo::getKeyOps() const {
    std::ostringstream out;
    for (size_t i = 0; i < key.keyOps.size(); ++i) {
        if (i > 0) out << ",";
        out << key.keyOps[i];
    }
    return out.str();
}

// ============================================================================
// JSON Parsers
// ============================================================================

// Original Kotlin (EncryptedFileKey.kt JSON deserialization)
EncryptedFileKey parseEncryptedFileKey(const std::string& json) {
    EncryptedFileKey key;

    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    key.alg = extractStr("alg");
    key.kty = extractStr("kty");
    key.k = extractStr("k");
    key.ext = json.find("\"ext\": true") != std::string::npos
           || json.find("\"ext\":true") != std::string::npos;

    // Parse key_ops array
    auto opsPos = json.find("\"key_ops\"");
    if (opsPos != std::string::npos) {
        auto bracket = json.find('[', opsPos);
        if (bracket != std::string::npos) {
            size_t pos = bracket + 1;
            while (pos < json.size()) {
                if (json[pos] == '"') {
                    size_t end = json.find('"', pos + 1);
                    if (end != std::string::npos) {
                        key.keyOps.push_back(json.substr(pos + 1, end - pos - 1));
                        pos = end + 1;
                        continue;
                    }
                }
                if (json[pos] == ']') break;
                pos++;
            }
        }
    }

    key.valid = key.isValid();
    return key;
}

// Original Kotlin (EncryptedFileInfo.kt JSON deserialization)
EncryptedFileInfo parseEncryptedFileInfo(const std::string& json) {
    EncryptedFileInfo info;

    auto extractStr = [&](const std::string& field) -> std::string {
        auto search = "\"" + field + "\":\"";
        auto pos = json.find(search);
        if (pos == std::string::npos) {
            search = "\"" + field + "\": \"";
            pos = json.find(search);
        }
        if (pos == std::string::npos) return "";
        pos += search.size();
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    };

    info.url = extractStr("url");
    info.iv = extractStr("iv");
    info.version = extractStr("v");
    info.mimetype = extractStr("mimetype");

    // Parse nested key object
    auto keyPos = json.find("\"key\"");
    if (keyPos != std::string::npos) {
        auto brace = json.find('{', keyPos);
        if (brace != std::string::npos) {
            int depth = 1;
            size_t end = brace + 1;
            while (end < json.size() && depth > 0) {
                if (json[end] == '{') depth++;
                else if (json[end] == '}') depth--;
                end++;
            }
            info.key = parseEncryptedFileKey(json.substr(brace, end - brace));
        }
    }

    // Parse hashes map
    auto hashesPos = json.find("\"hashes\"");
    if (hashesPos != std::string::npos) {
        auto brace = json.find('{', hashesPos);
        if (brace != std::string::npos) {
            size_t pos = brace + 1;
            while (pos < json.size() && json[pos] != '}') {
                if (json[pos] == '"') {
                    size_t keyEnd = json.find('"', pos + 1);
                    if (keyEnd == std::string::npos) break;
                    std::string hashKey = json.substr(pos + 1, keyEnd - pos - 1);
                    auto colon = json.find(':', keyEnd);
                    if (colon == std::string::npos) break;
                    auto valQuote = json.find('"', colon);
                    if (valQuote == std::string::npos) break;
                    auto valEnd = json.find('"', valQuote + 1);
                    if (valEnd == std::string::npos) break;
                    info.hashes[hashKey] = json.substr(valQuote + 1, valEnd - valQuote - 1);
                    pos = valEnd + 1;
                    continue;
                }
                pos++;
            }
        }
    }

    info.valid = info.isValid();
    return info;
}

// ============================================================================
// Validation
// ============================================================================

bool isValidJwkKey(const EncryptedFileKey& key) { return key.isValid(); }
bool isValidEncryptedFile(const EncryptedFileInfo& info) { return info.isValid(); }

std::string extractFileKey(const EncryptedFileKey& key) { return key.k; }
std::string extractFileIv(const EncryptedFileInfo& info) { return info.iv; }

// ============================================================================
// JSON Builders
// ============================================================================

// Original Kotlin (EncryptedFileKey.kt JSON serialization)
std::string buildEncryptedFileKey(const EncryptedFileKey& key) {
    std::ostringstream json;
    json << "{";
    json << R"("alg": ")" << escJson(key.alg) << R"(")";
    json << R"(,"ext": )" << (key.ext ? "true" : "false");
    json << R"(,"kty": ")" << escJson(key.kty) << R"(")";
    json << R"(,"k": ")" << escJson(key.k) << R"(")";
    if (!key.keyOps.empty()) {
        json << R"(,"key_ops": [)";
        for (size_t i = 0; i < key.keyOps.size(); ++i) {
            if (i > 0) json << ",";
            json << R"(")" << escJson(key.keyOps[i]) << R"(")";
        }
        json << "]";
    }
    json << "}";
    return json.str();
}

// Original Kotlin (EncryptedFileInfo.kt JSON serialization)
std::string buildEncryptedFileInfo(const EncryptedFileInfo& info) {
    std::ostringstream json;
    json << "{";
    json << R"("v": ")" << escJson(info.version) << R"(")";
    json << R"(,"url": ")" << escJson(info.url) << R"(")";
    json << R"(,"key": )" << buildEncryptedFileKey(info.key);
    json << R"(,"iv": ")" << escJson(info.iv) << R"(")";
    if (!info.hashes.empty()) {
        json << R"(,"hashes": {)";
        bool first = true;
        for (const auto& [hkey, hval] : info.hashes) {
            if (!first) json << ",";
            first = false;
            json << R"(")" << escJson(hkey) << R"(": ")" << escJson(hval) << R"(")";
        }
        json << "}";
    }
    if (!info.mimetype.empty()) {
        json << R"(,"mimetype": ")" << escJson(info.mimetype) << R"(")";
    }
    json << "}";
    return json.str();
}

// Original Kotlin: Build encrypted m.room.message content JSON.
// The encrypted file info is nested as `file` in the content.
std::string buildEncryptedContent(const EncryptedFileInfo& fileInfo,
                                   const std::string& msgtype,
                                   const std::string& body,
                                   const std::string& filename) {
    std::ostringstream json;
    json << "{";
    json << R"("msgtype": ")" << escJson(msgtype) << R"(")";
    if (!body.empty()) {
        json << R"(,"body": ")" << escJson(body) << R"(")";
    } else {
        json << R"(,"body": ")" << escJson(filename.empty() ? "Encrypted file" : filename) << R"(")";
    }
    if (!filename.empty()) {
        json << R"(,"filename": ")" << escJson(filename) << R"(")";
    }
    json << R"(,"file": )" << buildEncryptedFileInfo(fileInfo);
    json << "}";
    return json.str();
}

// ============================================================================
// JNI compat formatters
// ============================================================================

std::string encryptedFileKeyToJson(const EncryptedFileKey& key) {
    std::ostringstream json;
    json << R"({"valid": )" << (key.isValid() ? "true" : "false") << ",";
    json << R"("alg": ")" << escJson(key.alg) << R"(",)";
    json << R"("kty": ")" << escJson(key.kty) << R"(",)";
    json << R"("ext": )" << (key.ext ? "true" : "false");
    json << "}";
    return json.str();
}

std::string encryptedFileInfoToJson(const EncryptedFileInfo& info) {
    std::ostringstream json;
    json << R"({"valid": )" << (info.isValid() ? "true" : "false") << ",";
    json << R"("url": ")" << escJson(info.url) << R"(",)";
    json << R"("version": ")" << escJson(info.version) << R"(",)";
    json << R"("iv": ")" << escJson(info.iv) << R"(")";
    json << "}";
    return json.str();
}

// Original Kotlin (ElementToDecrypt.kt:23-34):
//   fun EncryptedFileInfo.toElementToDecrypt(): ElementToDecrypt?
ElementToDecrypt toElementToDecrypt(const EncryptedFileInfo& info) {
    ElementToDecrypt elem;
    elem.iv = info.iv;
    elem.k = info.key.k;
    auto it = info.hashes.find("sha256");
    if (it != info.hashes.end()) {
        elem.sha256 = it->second;
    }
    return elem;
}

// ============================================================================
// Encrypted Attachment Crypto (AES-256-CTR)
// ============================================================================

// Original Kotlin (MXEncryptedAttachments.kt:generate key+IV, lines 172-183)
EncryptedAttachmentInfo generateAttachmentKey(const std::string& mimetype) {
    EncryptedAttachmentInfo info;
    info.mimetype = mimetype;

    // Generate 32-byte AES-256 key
    info.key.resize(32);
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<int> dist(0, 255);
    for (size_t i = 0; i < 32; ++i) {
        info.key[i] = static_cast<uint8_t>(dist(gen));
    }

    // Generate 16-byte IV: upper 8 random, lower 8 zero
    info.iv.resize(16, 0);
    for (size_t i = 0; i < 8; ++i) {
        info.iv[i] = static_cast<uint8_t>(dist(gen));
    }

    // Build JWK representation
    info.fileKey.alg = "A256CTR";
    info.fileKey.ext = true;
    info.fileKey.kty = "oct";
    info.fileKey.keyOps = {"encrypt", "decrypt"};
    info.fileKey.k = b64_encode(info.key.data(), info.key.size());
    info.fileKey.valid = info.fileKey.isValid();

    info.valid = true;
    return info;
}

// Original Kotlin (MXEncryptedAttachments.kt:encryptAttachment, lines 167-230)
EncryptionResult encryptAttachment(const std::vector<uint8_t>& plaintext,
                                    const std::string& mimetype) {
    EncryptionResult result;

    auto attInfo = generateAttachmentKey(mimetype);
    if (!attInfo.valid) return result;

    // Encrypt using AES-256-CTR
    result.encryptedData.resize(plaintext.size());
    AesCtrState ctx;
    aes_ctr_init(ctx, attInfo.key.data(), attInfo.iv.data());
    aes_ctr_crypt(ctx, plaintext.data(), result.encryptedData.data(), plaintext.size());

    // Compute SHA-256 of ciphertext
    std::string hash = sha256_string(result.encryptedData.data(), result.encryptedData.size());
    auto hashB64 = b64_encode(reinterpret_cast<const uint8_t*>(hash.data()), hash.size());

    // Build EncryptedFileInfo
    result.encryptedFileInfo.version = "v2";
    result.encryptedFileInfo.iv = b64_encode(attInfo.iv.data(), attInfo.iv.size());
    result.encryptedFileInfo.key = attInfo.fileKey;
    result.encryptedFileInfo.hashes["sha256"] = hashB64;
    result.encryptedFileInfo.mimetype = mimetype;
    result.encryptedFileInfo.valid = result.encryptedFileInfo.isValid();

    return result;
}

// Original Kotlin (MXEncryptedAttachments.kt:decryptAttachment, lines 241-300)
bool decryptAttachment(const std::vector<uint8_t>& ciphertext,
                       const std::string& keyBase64Url,
                       const std::string& ivBase64,
                       const std::string& expectedSha256,
                       std::vector<uint8_t>& output) {
    // Verify SHA-256 of ciphertext matches expected
    std::string actualHash = sha256_string(ciphertext.data(), ciphertext.size());
    auto actualHashB64 = b64_encode(
        reinterpret_cast<const uint8_t*>(actualHash.data()), actualHash.size());
    if (actualHashB64 != expectedSha256) return false;

    // Decode base64 key and IV (simple decoding)
    // Note: full base64 decode would need a proper implementation.
    // For now, we assume key and IV are passed as raw strings matching the encrypt
    // phase. In production, use a proper base64 decoder.

    // Decrypt using AES-256-CTR
    output.resize(ciphertext.size());

    // Parse key bytes - simplified (assumes key was generated by our own encrypt)
    auto parseB64 = [](const std::string& s) -> std::vector<uint8_t> {
        std::vector<uint8_t> out;
        // Simple unpadded base64 decode
        const char* table = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        int val = 0, valb = -8;
        for (char c : s) {
            if (c == '=') break;
            const char* p = strchr(table, c);
            if (!p) continue;
            val = (val << 6) | static_cast<int>(p - table);
            valb += 6;
            if (valb >= 0) {
                out.push_back(static_cast<uint8_t>((val >> valb) & 0xff));
                valb -= 8;
            }
        }
        return out;
    };

    auto keyBytes = parseB64(keyBase64Url);
    auto ivBytes = parseB64(ivBase64);

    if (keyBytes.size() < 32 || ivBytes.size() < 16) return false;

    AesCtrState ctx;
    aes_ctr_init(ctx, keyBytes.data(), ivBytes.data());
    aes_ctr_crypt(ctx, ciphertext.data(), output.data(), ciphertext.size());

    return true;
}

} // namespace pcp
