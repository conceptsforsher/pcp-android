#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>

namespace progressive {

// ---- Encrypted File Parser (Matrix E2EE Attachments) ----
// Faithful port from original Kotlin:
//   org.matrix.android.sdk.api.session.crypto.model.EncryptedFileKey.kt (80 lines)
//   org.matrix.android.sdk.api.session.crypto.model.EncFileInfo.kt (84 lines)
//
// Matrix spec for encrypted attachments:
//   https://matrix.org/docs/spec/client_server/latest#sending-encrypted-attachments
//
// The encrypted file JSON format:
//   {"v":"v2","key":{"alg":"A256CTR","ext":true,"k":"base64key",...},"iv":"base64iv","hashes":{"sha256":"..."},"url":"mxc://..."}

struct EncryptedFileKey {
    // Original Kotlin (EncryptedFileKey.kt:24-52):
    //   data class EncryptedFileKey(alg, ext, keyOps, kty, k)
    std::string alg;                 // Must be "A256CTR"
    bool ext = false;                // Must be true (W3C extension)
    std::vector<std::string> keyOps; // Must contain "encrypt" and "decrypt"
    std::string kty;                 // Must be "oct"
    std::string k;                   // The key, base64-url encoded
    bool valid = false;

    // Check if this JWK is valid per Matrix spec.
    // Original Kotlin (EncryptedFileKey.kt:isValid):
    //   alg == "A256CTR" && ext == true && keyOps contains encrypt+decrypt
    //   && kty == "oct" && k is not blank
    bool isValid() const;
};

struct EncryptedFileInfo {
    // Original Kotlin (EncryptedFileInfo.kt:26-83):
    //   data class EncryptedFileInfo(url, key, iv, hashes, v)
    // Plus mimetype from attachment context
    std::string url;                     // MXC URL to the encrypted file
    EncryptedFileKey key;                // JWK key object
    std::string iv;                      // Initialisation Vector (unpadded base64)
    std::map<std::string, std::string> hashes; // Hash map, must contain "sha256"
    std::string version;                 // Must be "v2"
    std::string mimetype;                // Optional: MIME type
    bool valid = false;

    // Check if this encrypted file descriptor is valid.
    // Original Kotlin (EncryptedFileInfo.kt:isValid):
    //   url not blank, key.isValid(), iv not blank,
    //   hashes contains "sha256", v == "v2"
    bool isValid() const;

    // JNI flat fields (compat with legacy EncFileInfo accessors)
    std::string getAlg() const { return key.alg; }
    std::string getKty() const { return key.kty; }
    std::string getK() const { return key.k; }
    std::string getExt() const { return key.ext ? "true" : "false"; }
    std::string getKeyOps() const;
};

// ---- EncryptedAttachmentInfo (generated key material) ----
// Ported from:
//   org.matrix.android.sdk.internal.crypto.attachments.MXEncryptedAttachments
//
// Original Kotlin (MXEncryptedAttachments.kt:encryptAttachment):
//   Generates a random 32-byte key + 16-byte IV (upper 8 random, lower 8 zero).

struct EncryptedAttachmentInfo {
    std::vector<uint8_t> key;    // 32-byte raw AES-256 key
    std::vector<uint8_t> iv;     // 16-byte raw IV (upper 8 random, lower 8 zero)
    std::string sha256;          // SHA-256 of ciphertext, unpadded base64
    std::string mimetype;        // MIME type
    EncryptedFileKey fileKey;    // JWK representation of the key
    EncryptedFileInfo fileInfo;  // Full encrypted file descriptor
    bool valid = false;
};

// ---- EncryptionResult ----
// Original Kotlin (EncryptionResult.kt:24-27):
//   internal data class EncryptionResult(encryptedFileInfo, encryptedByteArray)

struct EncryptionResult {
    EncryptedFileInfo encryptedFileInfo;
    std::vector<uint8_t> encryptedData;
};

// ---- ElementToDecrypt ----
// Original Kotlin (ElementToDecrypt.kt:41-45):
//   @Parcelize
//   data class ElementToDecrypt(iv: String, k: String, sha256: String) : Parcelable

struct ElementToDecrypt {
    std::string iv;      // Base64 IV
    std::string k;       // Base64-url key
    std::string sha256;  // SHA-256 hash (unpadded base64)
};

// ==== Parse functions ====

// Parse an EncryptedFileKey from a JSON object.
EncryptedFileKey parseEncryptedFileKey(const std::string& json);

// Parse an EncryptedFileInfo from JSON.
EncryptedFileInfo parseEncryptedFileInfo(const std::string& json);

// ==== Build/Serialize functions ====

// Build full EncryptedFileInfo JSON (with url, key, iv, hashes, v, mimetype).
std::string buildEncryptedFileKey(const EncryptedFileKey& key);
std::string buildEncryptedFileInfo(const EncryptedFileInfo& info);

// Build encrypted m.room.message content JSON.
// Original Kotlin: the encrypted file info is nested as `file` in content.
std::string buildEncryptedContent(const EncryptedFileInfo& fileInfo,
                                   const std::string& msgtype = "m.file",
                                   const std::string& body = "",
                                   const std::string& filename = "");

// ==== Validation ====

// Validate a JWK key per Matrix spec requirements.
bool isValidJwkKey(const EncryptedFileKey& key);

// Validate an encrypted file descriptor.
bool isValidEncryptedFile(const EncryptedFileInfo& info);

// ==== Key Extraction ====

// Extract the base64-url encoded key from a JWK.
std::string extractFileKey(const EncryptedFileKey& key);

// Extract the base64-encoded IV from encrypted file info.
std::string extractFileIv(const EncryptedFileInfo& info);

// ==== Format for JNI/Kotlin UI compat ====

std::string encryptedFileKeyToJson(const EncryptedFileKey& key);
std::string encryptedFileInfoToJson(const EncryptedFileInfo& info);

// Original Kotlin (ElementToDecrypt.kt:23-34):
//   fun EncryptedFileInfo.toElementToDecrypt(): ElementToDecrypt?
ElementToDecrypt toElementToDecrypt(const EncryptedFileInfo& info);

// ==== Encrypted Attachment Crypto (AES-256-CTR) ====

// Generate a random 32-byte AES-256 key and 16-byte IV.
// The IV has the upper 8 bytes random, lower 8 bytes zeroed, per Matrix spec.
// Original Kotlin (MXEncryptedAttachments.kt:generate key+IV logic, lines 172-183):
//   val initVectorBytes = ByteArray(16) { 0 }
//   val ivRandomPart = ByteArray(8); secureRandom.nextBytes(ivRandomPart)
//   System.arraycopy(ivRandomPart, 0, initVectorBytes, 0, 8)
//   val key = ByteArray(32); secureRandom.nextBytes(key)
EncryptedAttachmentInfo generateAttachmentKey(const std::string& mimetype = "");

// Encrypt a plaintext buffer using AES-256-CTR.
// Returns EncryptionResult with the encrypted data and encrypted file info.
// The hash in fileInfo is the SHA-256 of the ciphertext.
// Original Kotlin (MXEncryptedAttachments.kt:encryptAttachment, lines 167-230):
EncryptionResult encryptAttachment(const std::vector<uint8_t>& plaintext,
                                    const std::string& mimetype = "");

// Decrypt an encrypted buffer using AES-256-CTR.
// Verifies the SHA-256 hash of the ciphertext against the expected value.
// Returns true on success, decrypting into `output`.
// Original Kotlin (MXEncryptedAttachments.kt:decryptAttachment, lines 241-300):
bool decryptAttachment(const std::vector<uint8_t>& ciphertext,
                       const std::string& keyBase64Url,
                       const std::string& ivBase64,
                       const std::string& expectedSha256,
                       std::vector<uint8_t>& output);

} // namespace progressive
