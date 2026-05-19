#ifndef PROGRESSIVE_HASH_UTILS_HPP
#define PROGRESSIVE_HASH_UTILS_HPP

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// SHA-256 hash of a string or bytes.
std::string sha256Hex(const std::string& input);
std::string sha256Hex(const std::vector<uint8_t>& data);

// SHA-256 hex digest.
std::string sha256Hex(const std::string& input);
std::string sha256Hex(const std::vector<uint8_t>& data);

// Base64-encode binary data.
std::string base64Encode(const std::vector<uint8_t>& data);

// Base64-decode string to binary.
std::vector<uint8_t> base64Decode(const std::string& input);

// Hex-encode binary data.
std::string hexEncode(const std::vector<uint8_t>& data);

// Hex-decode string to binary.
std::vector<uint8_t> hexDecode(const std::string& hex);

// HMAC-SHA256.
std::string hmacSha256(const std::string& key, const std::string& message);

// CRC32 checksum.
uint32_t crc32(const std::vector<uint8_t>& data);
uint32_t crc32(const std::string& data);

// Adler-32 checksum.
uint32_t adler32(const std::vector<uint8_t>& data);
uint32_t adler32(const std::string& data);

// Verify a SHA-256 hash matches data.
bool verifyHash(const std::string& data, const std::string& expectedHash);

// Generate a random token (URL-safe base64, N bytes of entropy).
std::string generateToken(int numBytes = 32);

// Simple constant-time comparison (prevents timing attacks).
bool constantTimeCompare(const std::string& a, const std::string& b);

// Convert base64-URL to standard base64: - → +, _ → /
// From: org.matrix.android.sdk.internal.util.Base64.kt (38L)
std::string base64UrlToBase64(const std::string& base64Url);

// Convert standard base64 to base64-URL: + → -, / → _, = removed, newlines stripped
std::string base64ToBase64Url(const std::string& base64);

// Convert standard base64 to unpadded (remove = and newlines).
std::string base64ToUnpaddedBase64(const std::string& base64);

} // namespace progressive

#endif // PROGRESSIVE_HASH_UTILS_HPP
