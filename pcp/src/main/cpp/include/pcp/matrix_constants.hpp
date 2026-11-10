#pragma once
#include <string>
#include <cstdint>

namespace pcp {

// ==== Matrix Login Flow Constants ====
// Ported from: org.matrix.android.sdk.api.auth.data.LoginFlowTypes.kt
namespace LoginFlow {
    constexpr const char* PASSWORD = "m.login.password";
    constexpr const char* OAUTH2 = "m.login.oauth2";  
    constexpr const char* EMAIL_CODE = "m.login.email.code";
    constexpr const char* EMAIL_URL = "m.login.email.url";
    constexpr const char* EMAIL_IDENTITY = "m.login.email.identity";
    constexpr const char* MSISDN = "m.login.msisdn";
    constexpr const char* RECAPTCHA = "m.login.recaptcha";
    constexpr const char* DUMMY = "m.login.dummy";
    constexpr const char* TERMS = "m.login.terms";
    constexpr const char* TOKEN = "m.login.token";
    constexpr const char* SSO = "m.login.sso";
}

// ==== Matrix Push Rule Constants ====
// Ported from: org.matrix.android.sdk.api.session.pushrules.RuleIds.kt
// Ref: https://matrix.org/docs/spec/client_server/latest#predefined-rules
namespace PushRule {
    constexpr const char* DISABLE_ALL = ".m.rule.master";
    constexpr const char* SUPPRESS_NOTICES = ".m.rule.suppress_notices";
    constexpr const char* INVITE_FOR_ME = ".m.rule.invite_for_me";
    constexpr const char* MEMBER_EVENT = ".m.rule.member_event";
    constexpr const char* CONTAINS_DISPLAY_NAME = ".m.rule.contains_display_name";
    constexpr const char* TOMBSTONE = ".m.rule.tombstone";
    constexpr const char* ROOM_NOTIF = ".m.rule.roomnotif";
    constexpr const char* CONTAINS_USER_NAME = ".m.rule.contains_user_name";
    constexpr const char* KEYWORD = ".m.rule.keyword";
    constexpr const char* CALL = ".m.rule.call";
    constexpr const char* ENCRYPTED = ".m.rule.encrypted";
    constexpr const char* ONE_TO_ONE_ROOM = ".m.rule.room_one_to_one";
    constexpr const char* ONE_TO_ONE_ENCRYPTED = ".m.rule.encrypted_one_to_one";
    constexpr const char* MENTION_USER = ".m.rule.is_user_mention";
    constexpr const char* MENTION_ROOM = ".m.rule.is_room_mention";
}

// ==== Matrix Encryption Algorithm Constants ====
// Ported from: org.matrix.android.sdk.api.crypto.CryptoConstants.kt
//              org.matrix.android.sdk.api.session.room.model.RoomEncryptionAlgorithm.kt
namespace EncryptionAlgorithm {
    constexpr const char* OLM_V1 = "m.olm.v1.curve25519-aes-sha2";
    constexpr const char* MEGOLM_V1 = "m.megolm.v1.aes-sha2";
    constexpr const char* MEGOLM_V2 = "m.megolm.v2.aes-sha2";
    constexpr const char* ROOM_ENC_V1 = "m.room.v1";
    constexpr const char* SAS_V1 = "m.sas.v1";
    constexpr const char* CURVE25519 = "curve25519";
    constexpr const char* ED25519 = "ed25519";
    constexpr const char* SIGNED_CURVE25519 = "signed_curve25519";
    constexpr const char* SHORT_AUTH_STRING = "m.sas.v1";
}

// ==== Matrix Registration Stages ====
// Ported from: org.matrix.android.sdk.api.auth.registration.RegistrationFlowResponse.kt
namespace RegistrationStage {
    constexpr const char* PASSWORD = "m.login.password";
    constexpr const char* EMAIL = "m.login.email.identity";
    constexpr const char* MSISDN = "m.login.msisdn";
    constexpr const char* RECAPTCHA = "m.login.recaptcha";
    constexpr const char* DUMMY = "m.login.dummy";
    constexpr const char* TERMS = "m.login.terms";
    constexpr const char* REGISTRATION_TOKEN = "m.login.registration_token";
    constexpr const char* OAUTH2 = "m.login.oauth2";
}

// ==== Matrix Device Key Algorithms ====
// Ported from: CryptoConstants.kt
namespace KeyAlgorithm {
    constexpr const char* ED25519 = "ed25519";
    constexpr const char* CURVE25519 = "curve25519";
    constexpr const char* SIGNED_CURVE25519 = "signed_curve25519";
}

// ==== Helper functions ====

// Check if a login flow requires terms acceptance.
// Original Kotlin: LoginFlowTypes.TERMS
inline bool isTermsRequired(const std::string& flowType) {
    return flowType == LoginFlow::TERMS;
}

// Check if a push rule ID is a default (predefined) rule.
inline bool isDefaultPushRule(const std::string& ruleId) {
    return ruleId.rfind(".m.rule.", 0) == 0;
}

// Check if encryption algorithm is Olm-based.
inline bool isOlmAlgorithm(const std::string& algo) {
    return algo == EncryptionAlgorithm::OLM_V1;
}

// Check if encryption algorithm is Megolm-based.
inline bool isMegolmAlgorithm(const std::string& algo) {
    return algo == EncryptionAlgorithm::MEGOLM_V1 || algo == EncryptionAlgorithm::MEGOLM_V2;
}

} // namespace pcp
