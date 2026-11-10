#include "pcp/encryption_status_formatter.hpp"
#include <sstream>

namespace pcp {

EncryptionShield getRoomShield(bool allVerified, bool someVerified) {
    if (allVerified) return {ShieldType::GREEN,  "\xF0\x9F\x9F\xA2", "#4CAF50", "Verified"};
    if (someVerified) return {ShieldType::YELLOW, "\xF0\x9F\x9F\xA1", "#FF9800", "Some unverified"};
    return {ShieldType::RED, "\xF0\x9F\x94\xB4", "#F44336", "Unverified"};
}

EncryptionShield getDeviceShield(bool csVerified, bool localVerified, bool blocked) {
    if (blocked) return {ShieldType::RED, "\xF0\x9F\x94\xB4", "#F44336", "Blocked"};
    if (csVerified) return {ShieldType::GREEN, "\xF0\x9F\x9F\xA2", "#4CAF50", "Cross-signing verified"};
    if (localVerified) return {ShieldType::YELLOW, "\xF0\x9F\x9F\xA1", "#FF9800", "Locally verified"};
    return {ShieldType::RED, "\xF0\x9F\x94\xB4", "#F44336", "Unverified"};
}

EncryptionShield getEventShield(bool encrypted, bool senderVerified) {
    if (!encrypted) return {ShieldType::NONE, "\xF0\x9F\x94\x93", "#9E9E9E", "Not encrypted"};
    if (senderVerified) return {ShieldType::GREEN, "\xF0\x9F\x9F\xA2", "#4CAF50", "Encrypted by verified device"};
    return {ShieldType::RED, "\xF0\x9F\x94\xB4", "#F44336", "Encrypted by unverified device"};
}

std::string formatShieldHtml(const EncryptionShield& s) {
    std::ostringstream os;
    os << "<span class=\"encryption-shield\" style=\"color:" << s.color << "\" title=\""
       << s.label << "\">" << s.emoji << "</span>";
    return os.str();
}

bool shouldShowShield(bool encrypted, bool own, bool roomEncrypted) {
    return encrypted || roomEncrypted;
}

std::string getShieldDescription(const EncryptionShield& s) {
    return s.label;
}

} // namespace pcp
