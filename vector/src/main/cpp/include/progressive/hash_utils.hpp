#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <array>

namespace progressive {

// ---- Existing APIs ----

std::string sha256Hex(const std::string& input);
std::string sha256Hex(const std::vector<uint8_t>& data);

std::string base64Encode(const std::vector<uint8_t>& data);
std::vector<uint8_t> base64Decode(const std::string& input);
std::string hexEncode(const std::vector<uint8_t>& data);
std::vector<uint8_t> hexDecode(const std::string& hex);

std::string hmacSha256(const std::string& key, const std::string& message);

uint32_t crc32(const std::vector<uint8_t>& data);
uint32_t crc32(const std::string& data);
uint32_t adler32(const std::vector<uint8_t>& data);
uint32_t adler32(const std::string& data);

bool verifyHash(const std::string& data, const std::string& expectedHash);
std::string generateToken(int numBytes = 32);
bool constantTimeCompare(const std::string& a, const std::string& b);

std::string base64UrlToBase64(const std::string& base64Url);
std::string base64ToBase64Url(const std::string& base64);
std::string base64ToUnpaddedBase64(const std::string& base64);

// ============================================================
// HASH ALGORITHM — unified hashing interface
// Original Kotlin:
//   org.matrix.android.sdk.api.util.HashUtils
// ============================================================

enum class HashAlgorithm {
    MD5,
    SHA1,
    SHA256,
    SHA384,
    SHA512,
    CRC32,
    XXHASH64,
    MURMUR3,
    BLAKE3
};

struct HashResult {
    std::vector<uint8_t> hashBytes;
    std::string hexString;
    std::string base64String;
};

// Compute hash of data using the specified algorithm.
HashResult computeHash(const std::vector<uint8_t>& data, HashAlgorithm algo);
HashResult computeHash(const std::string& data, HashAlgorithm algo);

// Convert raw hash bytes to hex string.
std::string hashToHex(const std::vector<uint8_t>& hashBytes);

// Convert raw hash bytes to base64 string.
std::string hashToBase64(const std::vector<uint8_t>& hashBytes);

// Verify data against expected hex hash.
bool verifyHash(const std::vector<uint8_t>& data, const std::string& expectedHex, HashAlgorithm algo);

// ============================================================
// HMAC — keyed-hash message authentication
// ============================================================

enum class HmacAlgorithm {
    HMAC_SHA1,
    HMAC_SHA256,
    HMAC_SHA384,
    HMAC_SHA512
};

// Compute HMAC for a message with key and algorithm.
// Returns raw HMAC bytes (not hex-encoded).
std::vector<uint8_t> computeHmac(const std::string& key, const std::string& message,
                                  HmacAlgorithm algo = HmacAlgorithm::HMAC_SHA256);

// Verify an HMAC digest.
bool verifyHmac(const std::string& key, const std::string& message,
                const std::vector<uint8_t>& expectedMac, HmacAlgorithm algo = HmacAlgorithm::HMAC_SHA256);

// ============================================================
// PBKDF2 — password-based key derivation
// ============================================================

struct Pbkdf2Params {
    HashAlgorithm algorithm = HashAlgorithm::SHA256;
    std::string password;
    std::string salt;
    int iterations = 100000;
    int keyLength = 32;  // bytes
};

// Derive a key from a password using PBKDF2.
std::vector<uint8_t> computePbkdf2(const Pbkdf2Params& params);

// ============================================================
// HKDF — HMAC-based key derivation function (RFC 5869)
// ============================================================

namespace HKDF {
    // HKDF-Extract: PRK = HMAC-Hash(salt, IKM)
    std::vector<uint8_t> extract(const std::vector<uint8_t>& salt,
                                  const std::vector<uint8_t>& ikm,
                                  HashAlgorithm hash = HashAlgorithm::SHA256);

    // HKDF-Expand: OKM = HMAC-Hash(PRK, info || counter)
    std::vector<uint8_t> expand(const std::vector<uint8_t>& prk,
                                 const std::vector<uint8_t>& info,
                                 int length,
                                 HashAlgorithm hash = HashAlgorithm::SHA256);
}

// ============================================================
// RANDOM GENERATOR — secure random number generation
// ============================================================

namespace RandomGenerator {
    // Generate N cryptographically-secure random bytes.
    std::vector<uint8_t> generateRandomBytes(int numBytes);

    // Generate a random hex string of specified byte length.
    std::string generateRandomHex(int numBytes = 32);

    // Generate a random base64 string of specified byte length.
    std::string generateRandomBase64(int numBytes = 32);

    // Generate a version-4 (random) UUID.
    // Format: xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx
    std::string generateUUID();

    // Generate a URL-safe secure token (for auth, CSRF, etc.).
    std::string generateSecureToken(int numBytes = 32);
}

// ============================================================
// CHECKSUM — data integrity verification
// ============================================================

enum class ChecksumAlgorithm {
    CRC32,
    ADLER32
};

// Compute a checksum from data.
uint32_t computeChecksum(const std::vector<uint8_t>& data, ChecksumAlgorithm algo);
uint32_t computeChecksum(const std::string& data, ChecksumAlgorithm algo);

// Verify data against a known checksum value.
bool verifyChecksum(const std::vector<uint8_t>& data, uint32_t expectedChecksum, ChecksumAlgorithm algo);
bool verifyChecksum(const std::string& data, uint32_t expectedChecksum, ChecksumAlgorithm algo);

// ============================================================
// MERKLE TREE — hash tree for data verification
// ============================================================

struct MerkleProof {
    std::vector<std::string> siblingHashes;  // sibling hashes along path to root
    int leafIndex = 0;
    std::string leafHash;
};

namespace MerkleHash {
    // Compute the Merkle root from a list of data chunks.
    std::string computeMerkleRoot(const std::vector<std::string>& chunks,
                                   HashAlgorithm algo = HashAlgorithm::SHA256);

    // Build a full Merkle tree (returns all levels, leaf-to-root).
    // Each inner vector is a level: [0] = leaves, [last] = root.
    std::vector<std::vector<std::string>> buildMerkleTree(
        const std::vector<std::string>& chunks,
        HashAlgorithm algo = HashAlgorithm::SHA256);

    // Verify a Merkle proof against a known root.
    bool verifyMerkleProof(const MerkleProof& proof, const std::string& knownRoot,
                           HashAlgorithm algo = HashAlgorithm::SHA256);
}

// ============================================================
// CONTENT HASH — multi-hash content verification (Matrix spec)
// ============================================================

struct ContentHash {
    std::string sha256;
    std::string sha384;
    std::string sha512;
    bool hasSha256 = false;
    bool hasSha384 = false;
    bool hasSha512 = false;
};

// Parse a JSON content hash map as used in Matrix events.
// Input: {"sha256": "base64hash", ...}
ContentHash parseContentHash(const std::string& json);

// Verify content against a ContentHash specification.
// Returns true if ALL present hashes match. If no hashes present, returns false.
bool verifyContentHash(const std::vector<uint8_t>& content, const ContentHash& hash);

} // namespace progressive
