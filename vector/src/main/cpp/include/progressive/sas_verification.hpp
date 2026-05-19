#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ==== SAS Emoji Verification ====
//
// Wraps libolm's OlmSAS for Short Authentication String verification.
// Used for device-to-device verification via emoji comparison.
// Based on Matrix spec: m.key.verification.start with method="m.sas.v1"

struct SasVerification {
    void* sas = nullptr;          // OlmSAS*
    std::string ourPubkey;        // Base64-encoded Curve25519 public key
    bool theirKeySet = false;
    bool valid = false;
};

// Create a new SAS verification session.
// Returns our public key for sending to the other party.
SasVerification sasCreate();

// Set the other party's public key.
bool sasSetTheirKey(SasVerification& sas, const std::string& theirPubkeyBase64);

// Generate the emoji list for display.
// Returns JSON: [{"emoji":"🐶","description":"Dog"},{"emoji":"🐱","description":"Cat"},...]
// Uses HKDF with info="MATRIX_KEY_VERIFICATION_SAS" to derive the bytes.
std::string sasGetEmojis(SasVerification& sas);

// Calculate the MAC for a SAS verification.
// input: the device key JSON being verified
// info: the MAC info string from the protocol
// Returns: base64-encoded MAC
std::string sasCalculateMac(SasVerification& sas, const std::string& input, const std::string& info);

// Calculate MAC in long KDF format (for SAS verification).
std::string sasCalculateMacLongKdf(SasVerification& sas, const std::string& input, const std::string& info);

// Verify the other party's MAC.
bool sasVerifyMac(SasVerification& sas, const std::string& theirMacBase64,
                  const std::string& input, const std::string& info);

// Destroy a SAS session.
void sasDestroy(SasVerification& sas);

// ==== Emoji Table ====
// Matrix spec: 64 emojis for SAS verification (MSC3086/v1)

struct SasEmoji {
    const char* emoji;
    const char* description;
};

// Get the emoji at a given index (0-63).
SasEmoji sasEmojiForIndex(int index);

// Get all 64 SAS emojis.
const SasEmoji* getSasEmojiTable();

} // namespace progressive
