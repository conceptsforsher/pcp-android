#pragma once
#include <string>
#include <vector>

namespace pcp {

struct EncryptionStatus {
    bool isEncrypted = false;
    bool isVerified = false;
    bool isTrusted = false;
    std::string algorithm;
    std::string senderDeviceId;
    std::string senderDeviceName;
};

// Determine encryption status from event JSON
EncryptionStatus getEncryptionStatus(const std::string& eventJson,
                                       const std::string& senderDeviceInfo = "");

// Format encryption shield emoji
std::string formatEncryptionShield(const EncryptionStatus& status);

// Format encryption description for tooltip
std::string formatEncryptionDescription(const EncryptionStatus& status);

// Check if event should show encryption badge in timeline
bool shouldShowEncryptionBadge(const EncryptionStatus& status);

} // namespace pcp
