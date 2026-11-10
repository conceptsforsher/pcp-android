#include "pcp/event_encryption_helper.hpp"
#include <sstream>

namespace pcp {

EncryptionStatus getEncryptionStatus(const std::string& json, const std::string& deviceInfo) {
    EncryptionStatus s;
    auto algoPos = json.find("\"algorithm\"");
    if (algoPos != std::string::npos) {
        s.isEncrypted = json.find("megolm", algoPos) != std::string::npos;
        s.algorithm = "m.megolm.v1.aes-sha2";
    }
    if (!deviceInfo.empty()) {
        s.isVerified = deviceInfo.find("\"verified\":true") != std::string::npos;
        s.isTrusted = deviceInfo.find("\"trusted\":true") != std::string::npos;
        auto devPos = deviceInfo.find("\"device_id\":\"");
        if (devPos != std::string::npos) {
            devPos += 13;
            auto end = deviceInfo.find('"', devPos);
            if (end != std::string::npos) s.senderDeviceId = deviceInfo.substr(devPos, end - devPos);
        }
    }
    return s;
}

std::string formatEncryptionShield(const EncryptionStatus& s) {
    if (!s.isEncrypted) return "\xF0\x9F\x94\x93"; // open lock
    if (s.isVerified) return "\xF0\x9F\x9F\xA2\xF0\x9F\x94\x92"; // green locked
    if (s.isTrusted) return "\xF0\x9F\x9F\xA1\xF0\x9F\x94\x92"; // yellow locked
    return "\xF0\x9F\x94\xB4\xF0\x9F\x94\x92"; // red locked
}

std::string formatEncryptionDescription(const EncryptionStatus& s) {
    if (!s.isEncrypted) return "Not encrypted";
    if (s.isVerified) return "Encrypted by verified device";
    if (s.isTrusted) return "Encrypted by trusted device";
    return "Encrypted by unverified device";
}

bool shouldShowEncryptionBadge(const EncryptionStatus& s) {
    return s.isEncrypted;
}

} // namespace pcp
