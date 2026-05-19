#ifndef PROGRESSIVE_VERIFICATION_UTILS_HPP
#define PROGRESSIVE_VERIFICATION_UTILS_HPP

#include <string>
#include <vector>

namespace progressive {

// ---- Device Verification ----

enum class VerificationMethod { Sas, QrCode, Reciprocate, Unknown };

struct VerificationEmoji {
    std::string emoji;
    std::string description;   // e.g. "Dog", "Cat", "Rocket"
};

struct VerificationSas {
    std::vector<VerificationEmoji> emojis;   // 7 emojis for comparison
    std::vector<int> decimals;               // 7 three-digit numbers (alternative)
    std::string method;                       // "m.sas.v1"
    bool matches = true;                     // both sides see same values
};

struct VerificationState {
    std::string transactionId;
    std::string otherUserId;
    std::string otherDeviceId;
    bool isIncoming = false;       // we received the request
    bool isReady = false;          // both sides have exchanged keys
    bool isStarted = false;        // SAS computation started
    bool isDone = false;           // verification complete
    bool isCancelled = false;
    std::string cancelReason;
    VerificationSas sas;
    VerificationMethod method = VerificationMethod::Sas;
};

// Parse verification emoji list from Matrix spec.
std::vector<VerificationEmoji> getVerificationEmojis();

// Compute SAS emoji codes from 6 bytes.
VerificationSas computeSasEmojis(const std::string& sasBytes);

// Compute SAS decimal codes from 6 bytes.
std::vector<int> computeSasDecimals(const std::string& sasBytes);

// Format SAS emojis for display.
std::string formatSasEmojis(const VerificationSas& sas);

// Format SAS decimals for display.
std::string formatSasDecimals(const VerificationSas& sas);

// Check if two SAS values match.
bool sasMatches(const VerificationSas& a, const VerificationSas& b);

// Get verification method from string.
VerificationMethod parseVerificationMethod(const std::string& methodStr);

// Format verification state for display.
std::string formatVerificationState(const VerificationState& state);

// Build verification start request body.
std::string buildVerificationStartBody(const std::string& fromDevice,
    const std::string& transactionId, const std::string& method = "m.sas.v1");

// Build verification MAC request body.
std::string buildVerificationMacBody(const std::string& transactionId,
    const std::string& mac, const std::string& keys);

// Build verification cancel request body.
std::string buildVerificationCancelBody(const std::string& transactionId,
    const std::string& reason = "User cancelled");

// Get all supported verification methods.
std::vector<std::string> getSupportedVerificationMethods();

// Check if a verification method is supported.
bool isVerificationMethodSupported(const std::string& method);

} // namespace progressive

#endif // PROGRESSIVE_VERIFICATION_UTILS_HPP
