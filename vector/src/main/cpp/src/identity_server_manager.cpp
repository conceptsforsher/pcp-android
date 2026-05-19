#include "progressive/identity_server_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>

namespace progressive {

// ====== Enum conversions ======

const char* threePidMediumToString(ThreePidMedium medium) {
    return medium == ThreePidMedium::EMAIL ? "email" : "msisdn";
}

ThreePidMedium threePidMediumFromString(const std::string& s) {
    if (s == "email") return ThreePidMedium::EMAIL;
    if (s == "msisdn") return ThreePidMedium::MSISDN;
    return ThreePidMedium::EMAIL;
}

const char* sharedStateToString(IS_SharedState state) {
    switch (state) {
        case IS_SharedState::SHARED: return "shared";
        case IS_SharedState::NOT_SHARED: return "not_shared";
        case IS_SharedState::BINDING_IN_PROGRESS: return "binding_in_progress";
    }
    return "not_shared";
}

IS_SharedState sharedStateFromString(const std::string& s) {
    if (s == "shared") return IS_SharedState::SHARED;
    if (s == "binding_in_progress") return IS_SharedState::BINDING_IN_PROGRESS;
    return IS_SharedState::NOT_SHARED;
}

// ====== IS_ThreePid ======
// Original: toMedium()

std::string IS_ThreePid::toMedium() const {
    return threePidMediumToString(medium);
}

// Original: getCountryCode() — only for MSISDN
std::string IS_ThreePid::getCountryCode() const {
    if (medium != ThreePidMedium::MSISDN) return "";
    // Simple heuristic: first 1-3 digits
    std::string clean;
    for (char c : value) if (c >= '0' && c <= '9') clean += c;
    if (clean.size() >= 11) return clean.substr(0, 1); // US/Canada
    if (clean.size() >= 12) return clean.substr(0, 2); // Most EU
    if (clean.size() >= 13) return clean.substr(0, 3);
    return "1"; // default
}

IS_ThreePid IS_ThreePid::parse(const std::string& input) {
    IS_ThreePid pid;
    pid.value = input;
    if (isEmail(input)) {
        pid.medium = ThreePidMedium::EMAIL;
        pid.valid = true;
    } else if (isMsisdn(input)) {
        pid.medium = ThreePidMedium::MSISDN;
        pid.valid = true;
    }
    return pid;
}

bool IS_ThreePid::isEmail(const std::string& input) {
    auto at = input.find('@');
    return at != std::string::npos && at > 0 && at < input.size() - 1;
}

bool IS_ThreePid::isMsisdn(const std::string& input) {
    // Starts with + or contains only digits and common separators
    if (!input.empty() && input[0] == '+') return true;
    for (char c : input) {
        if (c >= '0' && c <= '9') continue;
        if (c == ' ' || c == '-' || c == '(' || c == ')') continue;
        return false;
    }
    return input.size() >= 7; // Minimum phone number length
}

// ====== JSON helpers ======

std::string IdentityServerManager::extractStr(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return "";
    pp = json.find('"', pp + key.size() + 2);
    if (pp == std::string::npos) return "";
    pp++;
    size_t e = pp;
    while (e < json.size() && json[e] != '"') e++;
    return json.substr(pp, e - pp);
}

int64_t IdentityServerManager::extractInt(const std::string& json, const std::string& key) {
    auto pp = json.find("\"" + key + "\"");
    if (pp == std::string::npos) return 0;
    pp = json.find(':', pp);
    if (pp == std::string::npos) return 0;
    pp++;
    while (pp < json.size() && (json[pp] == ' ' || json[pp] == '\t')) pp++;
    int64_t v = 0;
    while (pp < json.size() && json[pp] >= '0' && json[pp] <= '9') { v=v*10+(json[pp]-'0'); pp++; }
    return v;
}

bool IdentityServerManager::extractBool(const std::string& json, const std::string& key) {
    return json.find("\"" + key + "\":true") != std::string::npos;
}

// ====== Constructor ======

IdentityServerManager::IdentityServerManager() {}

// ====== Server Configuration ======
// Original: getDefaultIdentityServer / getCurrentIdentityServerUrl

void IdentityServerManager::setDefaultServer(const std::string& url) { config_.defaultServerUrl = url; }
void IdentityServerManager::setCurrentServer(const std::string& url) { config_.currentServerUrl = url; }
std::string IdentityServerManager::getCurrentServerUrl() const { return config_.currentServerUrl; }
std::string IdentityServerManager::getDefaultServerUrl() const { return config_.defaultServerUrl; }

// Original: setNewIdentityServer(url) → final url (prepend "https://")
std::string IdentityServerManager::setNewIdentityServer(const std::string& url, std::string& error) {
    std::string finalUrl = url;
    if (finalUrl.find("://") == std::string::npos) {
        finalUrl = "https://" + finalUrl;
    }

    if (!isValidServerUrl(finalUrl)) {
        error = "Invalid identity server URL: " + finalUrl;
        return "";
    }

    // Disconnect old server
    disconnect();

    config_.currentServerUrl = finalUrl;
    config_.userConsent = false; // Reset consent on server change
    return finalUrl;
}

void IdentityServerManager::disconnect() {
    config_.currentServerUrl.clear();
    config_.isValid = false;
    config_.userConsent = false;
    bindings_.clear();
}

// ====== Server Validation ======

std::string IdentityServerManager::buildStatusCheckUrl(const std::string& serverUrl) const {
    std::string base = serverUrl;
    while (!base.empty() && base.back() == '/') base.pop_back();
    return base + "/_matrix/identity/api/v2";
}

bool IdentityServerManager::parseStatusResponse(const std::string& json) const {
    auto version = extractStr(json, "version");
    return !version.empty(); // Any version response = server is alive
}

bool IdentityServerManager::isValidServerUrl(const std::string& url) const {
    return url.find("https://") == 0 && url.size() > 8;
}

// ====== ThreePID Binding ======
// Original: startBindThreePid / cancelBindThreePid / finalizeBindThreePid

std::string IdentityServerManager::buildBindRequest(const IS_ThreePid& threePid) const {
    std::ostringstream os;
    os << R"({"medium":")" << threePid.toMedium()
       << R"(","address":")" << threePid.value << R"("})";
    return os.str();
}

std::string IdentityServerManager::buildUnbindRequest(const IS_ThreePid& threePid) const {
    std::ostringstream os;
    os << R"({"medium":")" << threePid.toMedium()
       << R"(","address":")" << threePid.value << R"("})";
    return os.str();
}

std::string IdentityServerManager::buildSubmitTokenRequest(const IS_ThreePid& threePid, const std::string& sid,
                                                            const std::string& clientSecret, int token) const {
    std::ostringstream os;
    os << R"({"sid":")" << sid
       << R"(","client_secret":")" << clientSecret
       << R"(","token":")" << token << R"(")";
    os << "}";
    return os.str();
}

IS_ThreePidBindingStatus IdentityServerManager::parseBindResponse(const std::string& json, const IS_ThreePid& threePid) const {
    IS_ThreePidBindingStatus status;
    status.threePid = threePid;

    auto sid = extractStr(json, "sid");
    if (!sid.empty()) {
        status.sid = sid;
        status.shareState = IS_SharedState::BINDING_IN_PROGRESS;
        status.boundAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    }

    auto err = extractStr(json, "errcode");
    if (!err.empty()) {
        status.errorMessage = err + ": " + extractStr(json, "error");
    }

    return status;
}

void IdentityServerManager::registerBinding(const std::string& sid, const IS_ThreePid& threePid) {
    IS_ThreePidBindingStatus status;
    status.threePid = threePid;
    status.sid = sid;
    status.shareState = IS_SharedState::BINDING_IN_PROGRESS;
    status.boundAtMs = static_cast<int64_t>(std::time(nullptr)) * 1000;
    bindings_[sid] = status;
}

IS_ThreePidBindingStatus IdentityServerManager::getBinding(const std::string& sid) const {
    auto it = bindings_.find(sid);
    if (it != bindings_.end()) return it->second;
    IS_ThreePidBindingStatus empty;
    empty.sid = sid;
    return empty;
}

void IdentityServerManager::cancelBinding(const std::string& sid) {
    bindings_.erase(sid);
}

void IdentityServerManager::finalizeBinding(const std::string& sid) {
    auto it = bindings_.find(sid);
    if (it != bindings_.end()) {
        it->second.isBound = true;
        it->second.shareState = IS_SharedState::SHARED;
    }
}

void IdentityServerManager::removeBinding(const std::string& sid) {
    bindings_.erase(sid);
}

// ====== 3PID Lookup ======
// Original: lookUp(threePids) → List<IS_FoundThreePid>

std::string IdentityServerManager::buildLookupRequest(const std::vector<IS_ThreePid>& threePids) const {
    std::ostringstream os;
    os << R"({"threepids":[)";
    for (size_t i = 0; i < threePids.size(); i++) {
        if (i > 0) os << ",";
        os << R"(["")" << threePidMediumToString(threePids[i].medium)
           << R"(",")" << threePids[i].value << R"("])";
    }
    os << "]}";
    return os.str();
}

std::vector<IS_FoundThreePid> IdentityServerManager::parseLookupResponse(const std::string& json) const {
    std::vector<IS_FoundThreePid> results;

    size_t pos = json.find("\"mappings\"");
    if (pos == std::string::npos) pos = json.find("\"results\"");

    if (pos != std::string::npos) {
        pos = json.find('{', pos);
        if (pos != std::string::npos) {
            // Parse mappings: {"medium+address": "mxid"}
            pos++;
            while (pos < json.size() && json[pos] != '}') {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',')) pos++;
                if (pos >= json.size() || json[pos] == '}') break;

                if (json[pos] == '"') {
                    pos++;
                    size_t keyEnd = pos;
                    while (keyEnd < json.size() && json[keyEnd] != '"') keyEnd++;
                    std::string key = json.substr(pos, keyEnd - pos);

                    // Find value
                    auto valPos = json.find('"', keyEnd + 1);
                    if (valPos != std::string::npos) {
                        valPos++;
                        size_t valEnd = valPos;
                        while (valEnd < json.size() && json[valEnd] != '"') valEnd++;
                        std::string mxid = json.substr(valPos, valEnd - valPos);

                        IS_FoundThreePid found;
                        found.matrixId = mxid;
                        found.threePid = IS_ThreePid::parse(key);
                        found.valid = !mxid.empty();
                        results.push_back(found);

                        pos = valEnd + 1;
                    } else {
                        pos = keyEnd + 1;
                    }
                } else {
                    pos++;
                }
            }
        }
    }

    return results;
}

// ====== User Consent ======

void IdentityServerManager::setUserConsent(bool consent) { config_.userConsent = consent; }
bool IdentityServerManager::getUserConsent() const { return config_.userConsent; }

std::string IdentityServerManager::buildConsentRequest(bool consent) const {
    return consent ? R"({"consent":true})" : R"({"consent_revoked":true})";
}

// ====== Share Status ======

IS_SharedState IdentityServerManager::getShareStatus(const IS_ThreePid& threePid) const {
    for (const auto& [sid, binding] : bindings_) {
        if (binding.threePid.medium == threePid.medium &&
            binding.threePid.value == threePid.value) {
            return binding.shareState;
        }
    }
    return IS_SharedState::NOT_SHARED;
}

void IdentityServerManager::setShareStatus(const IS_ThreePid& threePid, IS_SharedState state) {
    for (auto& [sid, binding] : bindings_) {
        if (binding.threePid.medium == threePid.medium &&
            binding.threePid.value == threePid.value) {
            binding.shareState = state;
            return;
        }
    }
}

// ====== Invitation Signing ======

std::string IdentityServerManager::buildSignInvitationRequest(const std::string& token, const std::string& secret) const {
    std::ostringstream os;
    os << R"({"token":")" << token << R"(","secret":")" << secret << R"("})";
    return os.str();
}

SignInvitationResult IdentityServerManager::parseSignInvitationResponse(const std::string& json) const {
    SignInvitationResult result;
    result.mxid = extractStr(json, "mxid");
    result.token = extractStr(json, "token");
    result.signatures = extractStr(json, "signatures");
    result.valid = !result.mxid.empty() && !result.signatures.empty();
    return result;
}

// ====== Serialization ======

std::string IdentityServerManager::threePidToJson(const IS_ThreePid& threePid) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"medium":")" << threePid.toMedium()
       << R"(","value":")" << esc(threePid.value)
       << R"(","valid":)" << (threePid.valid ? "true" : "false")
       << "}";
    return os.str();
}

std::string IdentityServerManager::bindingToJson(const IS_ThreePidBindingStatus& status) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"sid":")" << esc(status.sid)
       << R"(","medium":")" << status.threePid.toMedium()
       << R"(","value":")" << esc(status.threePid.value)
       << R"(","state":")" << sharedStateToString(status.shareState)
       << R"(,"is_bound":)" << (status.isBound ? "true" : "false")
       << R"(,"bound_at":)" << status.boundAtMs;
    if (!status.errorMessage.empty()) os << R"(,"error":")" << esc(status.errorMessage) << R"(")";
    os << "}";
    return os.str();
}

std::string IdentityServerManager::foundPidToJson(const IS_FoundThreePid& found) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"medium":")" << found.threePid.toMedium()
       << R"(","value":")" << esc(found.threePid.value)
       << R"(","matrix_id":")" << esc(found.matrixId)
       << R"(","valid":)" << (found.valid ? "true" : "false")
       << "}";
    return os.str();
}

} // namespace progressive
