#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>

namespace progressive {

// ---- Verification Phase (State Machine) ----
// Original Kotlin: EVerificationState.kt (34L)

enum class VerificationState {
    Requested,           // Incoming request, waiting for user action
    WaitingForReady,     // Outgoing request sent, waiting for other side to respond
    Ready,               // Both sides exchanged readiness
    Started,             // Verification started
    Done,                // Verification completed successfully
    Cancelled,           // Verification was cancelled
    HandledByOther       // Request was handled by another session
};

inline std::string verificationStateToString(VerificationState s) {
    switch (s) {
        case VerificationState::Requested:       return "Requested";
        case VerificationState::WaitingForReady: return "WaitingForReady";
        case VerificationState::Ready:           return "Ready";
        case VerificationState::Started:         return "Started";
        case VerificationState::Done:            return "Done";
        case VerificationState::Cancelled:       return "Cancelled";
        case VerificationState::HandledByOther:  return "HandledByOther";
    }
    return "Unknown";
}

inline VerificationState verificationStateFromString(const std::string& s) {
    if (s == "Requested")       return VerificationState::Requested;
    if (s == "WaitingForReady") return VerificationState::WaitingForReady;
    if (s == "Ready")           return VerificationState::Ready;
    if (s == "Started")         return VerificationState::Started;
    if (s == "Done")            return VerificationState::Done;
    if (s == "Cancelled")       return VerificationState::Cancelled;
    if (s == "HandledByOther")  return VerificationState::HandledByOther;
    return VerificationState::Requested;
}

// ---- Verification Method ----
// Original Kotlin: VerificationMethod.kt (37L)

enum class VerificationMethod {
    SAS,              // m.sas.v1
    QR_CODE_SHOW,     // m.qr_code.show.v1
    QR_CODE_SCAN,     // m.qr_code.scan.v1
    RECIPROCATE,      // m.reciprocate.v1
    UNKNOWN
};

inline std::string verificationMethodToString(VerificationMethod m) {
    switch (m) {
        case VerificationMethod::SAS:           return "SAS";
        case VerificationMethod::QR_CODE_SHOW:  return "QR_CODE_SHOW";
        case VerificationMethod::QR_CODE_SCAN:  return "QR_CODE_SCAN";
        case VerificationMethod::RECIPROCATE:   return "RECIPROCATE";
        case VerificationMethod::UNKNOWN:       return "UNKNOWN";
    }
    return "UNKNOWN";
}

inline std::string verificationMethodToMatrixString(VerificationMethod m) {
    switch (m) {
        case VerificationMethod::SAS:           return "m.sas.v1";
        case VerificationMethod::QR_CODE_SHOW:  return "m.qr_code.show.v1";
        case VerificationMethod::QR_CODE_SCAN:  return "m.qr_code.scan.v1";
        case VerificationMethod::RECIPROCATE:   return "m.reciprocate.v1";
        case VerificationMethod::UNKNOWN:       return "";
    }
    return "";
}

inline VerificationMethod verificationMethodFromMatrixString(const std::string& methodStr) {
    if (methodStr == "m.sas.v1")           return VerificationMethod::SAS;
    if (methodStr == "m.qr_code.show.v1")  return VerificationMethod::QR_CODE_SHOW;
    if (methodStr == "m.qr_code.scan.v1")  return VerificationMethod::QR_CODE_SCAN;
    if (methodStr == "m.reciprocate.v1")   return VerificationMethod::RECIPROCATE;
    return VerificationMethod::UNKNOWN;
}

// ---- Cancel Code ----
// Original Kotlin: CancelCode.kt (37L)

enum class CancelCode {
    User,
    Timeout,
    UnknownTransaction,
    UnknownMethod,
    MismatchedCommitment,
    MismatchedSas,
    UnexpectedMessage,
    InvalidMessage,
    MismatchedKeys,
    UserError,
    MismatchedUser,
    QrCodeInvalid,
    AcceptedByAnotherDevice
};

inline std::string cancelCodeToMatrixString(CancelCode c) {
    switch (c) {
        case CancelCode::User:                   return "m.user";
        case CancelCode::Timeout:                return "m.timeout";
        case CancelCode::UnknownTransaction:     return "m.unknown_transaction";
        case CancelCode::UnknownMethod:          return "m.unknown_method";
        case CancelCode::MismatchedCommitment:   return "m.mismatched_commitment";
        case CancelCode::MismatchedSas:          return "m.mismatched_sas";
        case CancelCode::UnexpectedMessage:      return "m.unexpected_message";
        case CancelCode::InvalidMessage:         return "m.invalid_message";
        case CancelCode::MismatchedKeys:         return "m.key_mismatch";
        case CancelCode::UserError:              return "m.user_error";
        case CancelCode::MismatchedUser:         return "m.user_mismatch";
        case CancelCode::QrCodeInvalid:          return "m.qr_code.invalid";
        case CancelCode::AcceptedByAnotherDevice: return "m.accepted";
    }
    return "m.user";
}

inline CancelCode cancelCodeFromMatrixString(const std::string& s) {
    if (s == "m.user")                   return CancelCode::User;
    if (s == "m.timeout")                return CancelCode::Timeout;
    if (s == "m.unknown_transaction")    return CancelCode::UnknownTransaction;
    if (s == "m.unknown_method")         return CancelCode::UnknownMethod;
    if (s == "m.mismatched_commitment")  return CancelCode::MismatchedCommitment;
    if (s == "m.mismatched_sas")         return CancelCode::MismatchedSas;
    if (s == "m.unexpected_message")     return CancelCode::UnexpectedMessage;
    if (s == "m.invalid_message")        return CancelCode::InvalidMessage;
    if (s == "m.key_mismatch")           return CancelCode::MismatchedKeys;
    if (s == "m.user_error")             return CancelCode::UserError;
    if (s == "m.user_mismatch")          return CancelCode::MismatchedUser;
    if (s == "m.qr_code.invalid")        return CancelCode::QrCodeInvalid;
    if (s == "m.accepted")               return CancelCode::AcceptedByAnotherDevice;
    return CancelCode::User;
}

// ---- SAS Verification State (sub-state machine) ----
// Original Kotlin: SasVerificationState — tracks SAS flow progress
enum class SasVerificationState {
    INITIAL,
    KEY_AGREEMENT_STARTED,
    KEY_AGREEMENT_FINISHED,
    SAS_SENT,
    SAS_RECEIVED,
    VERIFIED,
    CANCELLED
};

inline std::string sasVerificationStateToString(SasVerificationState s) {
    switch (s) {
        case SasVerificationState::INITIAL:                return "INITIAL";
        case SasVerificationState::KEY_AGREEMENT_STARTED:  return "KEY_AGREEMENT_STARTED";
        case SasVerificationState::KEY_AGREEMENT_FINISHED: return "KEY_AGREEMENT_FINISHED";
        case SasVerificationState::SAS_SENT:               return "SAS_SENT";
        case SasVerificationState::SAS_RECEIVED:           return "SAS_RECEIVED";
        case SasVerificationState::VERIFIED:               return "VERIFIED";
        case SasVerificationState::CANCELLED:              return "CANCELLED";
    }
    return "UNKNOWN";
}

inline SasVerificationState sasVerificationStateFromString(const std::string& s) {
    if (s == "INITIAL")                return SasVerificationState::INITIAL;
    if (s == "KEY_AGREEMENT_STARTED")  return SasVerificationState::KEY_AGREEMENT_STARTED;
    if (s == "KEY_AGREEMENT_FINISHED") return SasVerificationState::KEY_AGREEMENT_FINISHED;
    if (s == "SAS_SENT")               return SasVerificationState::SAS_SENT;
    if (s == "SAS_RECEIVED")           return SasVerificationState::SAS_RECEIVED;
    if (s == "VERIFIED")               return SasVerificationState::VERIFIED;
    if (s == "CANCELLED")              return SasVerificationState::CANCELLED;
    return SasVerificationState::INITIAL;
}

struct VerificationEmoji {
    std::string emoji;
    std::string description;
};

struct VerificationSas {
    std::vector<VerificationEmoji> emojis;
    std::vector<int> decimals;
    std::string method;
    bool matches = true;
};

// ---- Verification Session (live state container) ----
// Renamed from VerificationState to avoid conflict with enum above

struct VerificationSession {
    std::string transactionId;
    std::string otherUserId;
    std::string otherDeviceId;
    bool isIncoming = false;
    bool isReady = false;
    bool isStarted = false;
    bool isDone = false;
    bool isCancelled = false;
    std::string cancelReason;
    VerificationSas sas;
    VerificationMethod method = VerificationMethod::SAS;
    VerificationState state = VerificationState::Requested;
};

// ---- Verification Info Structs (parsed event data) ----
// Original Kotlin: ValidVerificationInfoRequest.kt (24L)

struct VerificationRequestInfo {
    std::string transactionId;
    std::string fromDevice;
    std::string fromUser;
    std::vector<std::string> methods;
    int64_t timestamp = 0;
    std::string roomId;
    bool isIncoming = false;     // Original Kotlin: whether this is an incoming request
};

// Original Kotlin: ValidVerificationInfoReady.kt (23L)

struct VerificationReadyInfo {
    std::string transactionId;
    std::string fromDevice;
    std::vector<std::string> methods;
};

struct VerificationStartInfo {
    std::string transactionId;
    std::string fromDevice;
    std::string method;
    std::vector<std::string> keyAgreementProtocols;
    std::vector<std::string> hashes;
    std::vector<std::string> messageAuthenticationCodes;
    std::vector<std::string> shortAuthenticationStrings;
};

struct VerificationAcceptInfo {
    std::string transactionId;
    std::string commitment;
    std::string keyAgreementProtocol;
    std::string hash;
    std::string messageAuthenticationCode;
    std::string shortAuthenticationString;
};

struct VerificationKeyInfo {
    std::string transactionId;
    std::string key;
};

struct VerificationMacInfo {
    std::string transactionId;
    std::map<std::string, std::string> mac;
    std::string keys;
};

struct VerificationCancelInfo {
    std::string transactionId;
    std::string reason;
    std::string code;
};

// ---- JSON Builders ----

std::string buildVerificationRequest(const std::string& fromDevice,
    const std::string& transactionId, const std::vector<std::string>& methods,
    const std::string& fromUser, const std::string& roomId = "");

std::string buildVerificationReady(const std::string& fromDevice,
    const std::string& transactionId, const std::vector<std::string>& methods);

std::string buildVerificationStart(const std::string& fromDevice,
    const std::string& transactionId, const std::string& method = "m.sas.v1",
    const std::vector<std::string>& keyAgreementProtocols = {"curve25519-hkdf-sha256"},
    const std::vector<std::string>& hashes = {"sha256"},
    const std::vector<std::string>& messageAuthenticationCodes = {"hkdf-hmac-sha256"},
    const std::vector<std::string>& shortAuthenticationStrings = {"decimal", "emoji"});

std::string buildVerificationAccept(const std::string& transactionId,
    const std::string& commitment,
    const std::string& keyAgreementProtocol = "curve25519-hkdf-sha256",
    const std::string& hash = "sha256",
    const std::string& messageAuthenticationCode = "hkdf-hmac-sha256",
    const std::string& shortAuthenticationString = "decimal");

std::string buildVerificationKey(const std::string& transactionId,
    const std::string& key);

std::string buildVerificationMac(const std::string& transactionId,
    const std::map<std::string, std::string>& mac, const std::string& keys);

std::string buildVerificationCancel(const std::string& transactionId,
    const std::string& reason = "User cancelled",
    const std::string& code = "m.user");

// ---- JSON Parsers ----

VerificationRequestInfo parseVerificationRequest(const std::string& json);
VerificationReadyInfo parseVerificationReady(const std::string& json);
VerificationStartInfo parseVerificationStart(const std::string& json);
VerificationMacInfo parseVerificationMac(const std::string& json);
VerificationKeyInfo parseVerificationKey(const std::string& json);
VerificationCancelInfo parseVerificationCancel(const std::string& json);

// ---- SAS / Emoji Utilities ----

std::vector<VerificationEmoji> getVerificationEmojis();
VerificationSas computeSasEmojis(const std::string& sasBytes);
std::vector<int> computeSasDecimals(const std::string& sasBytes);
std::string formatSasEmojis(const VerificationSas& sas);
std::string formatSasDecimals(const VerificationSas& sas);
bool sasMatches(const VerificationSas& a, const VerificationSas& b);

// ---- Verification Helpers ----

VerificationMethod parseVerificationMethod(const std::string& methodStr);
std::string formatVerificationSession(const VerificationSession& state);

std::string buildVerificationStartBody(const std::string& fromDevice,
    const std::string& transactionId, const std::string& method = "m.sas.v1");

std::string buildVerificationMacBody(const std::string& transactionId,
    const std::string& mac, const std::string& keys);

std::string buildVerificationCancelBody(const std::string& transactionId,
    const std::string& reason = "User cancelled");

std::vector<std::string> getSupportedVerificationMethods();
bool isVerificationMethodSupported(const std::string& method);

} // namespace progressive
