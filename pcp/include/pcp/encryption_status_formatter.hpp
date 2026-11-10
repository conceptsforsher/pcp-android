#include <nlohmann/json.hpp>
#pragma once
#include <string>

namespace pcp {

enum class ShieldType { GREEN, RED, YELLOW, BLACK, NONE };

struct EncryptionShield {
    ShieldType type = ShieldType::NONE;
    std::string emoji;
    std::string color;          // hex color
    std::string label;          // "Verified", "Unverified", etc.
};

// Get shield for room encryption level
EncryptionShield getRoomShield(bool allVerified, bool someVerified);

// Get shield for device trust level
EncryptionShield getDeviceShield(bool crossSigningVerified, bool locallyVerified, bool isBlocked);

// Get shield for event (E2EE decoration)
EncryptionShield getEventShield(bool isEncrypted, bool senderVerified);

// Format shield HTML for timeline decoration
std::string formatShieldHtml(const EncryptionShield& shield);

// Check if we should show shield for an event
bool shouldShowShield(bool isEncrypted, bool isOwnMessage, bool roomIsEncrypted);

// Get shield accessibility description
std::string getShieldDescription(const EncryptionShield& shield);

} // namespace pcp
