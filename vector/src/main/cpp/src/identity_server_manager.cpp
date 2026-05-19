#include "progressive/identity_server_manager.hpp"
#include <sstream>
#include <algorithm>
#include <ctime>
#include <cstring>

namespace progressive {

// ====== SHA-256 Implementation (public domain, FIPS 180-4) ======
// Original Kotlin: Sha256Converter.kt -> MessageDigest.getInstance("SHA-256") -> toBase64NoPadding()
// NDK 21 compatible self-contained implementation (no external crypto libs required).

namespace {
    constexpr uint32_t SHA256_K[64] = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0c97,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    inline uint32_t rotr32(uint32_t x, uint32_t n) { return (x >> n) | (x << (32 - n)); }

    void sha256Transform(uint32_t* state, const unsigned char* data) {
        uint32_t w[64];
        for (int i = 0; i < 16; i++) {
            w[i] = (static_cast<uint32_t>(data[i*4]) << 24) |
                   (static_cast<uint32_t>(data[i*4+1]) << 16) |
                   (static_cast<uint32_t>(data[i*4+2]) << 8) |
                    static_cast<uint32_t>(data[i*4+3]);
        }
        for (int i = 16; i < 64; i++) {
            uint32_t s0 = rotr32(w[i-15], 7) ^ rotr32(w[i-15], 18) ^ (w[i-15] >> 3);
            uint32_t s1 = rotr32(w[i-2], 17) ^ rotr32(w[i-2], 19) ^ (w[i-2] >> 10);
            w[i] = w[i-16] + s0 + w[i-7] + s1;
        }
        uint32_t a = state[0], b = state[1], c = state[2], d = state[3];
        uint32_t e = state[4], f = state[5], g = state[6], h = state[7];
        for (int i = 0; i < 64; i++) {
            uint32_t S1 = rotr32(e,6) ^ rotr32(e,11) ^ rotr32(e,25);
            uint32_t ch = (e & f) ^ (~e & g);
            uint32_t t1 = h + S1 + ch + SHA256_K[i] + w[i];
            uint32_t S0 = rotr32(a,2) ^ rotr32(a,13) ^ rotr32(a,22);
            uint32_t maj = (a & b) ^ (a & c) ^ (b & c);
            uint32_t t2 = S0 + maj;
            h = g; g = f; f = e; e = d + t1;
            d = c; c = b; b = a; a = t1 + t2;
        }
        state[0] += a; state[1] += b; state[2] += c; state[3] += d;
        state[4] += e; state[5] += f; state[6] += g; state[7] += h;
    }

    std::string sha256Raw(const std::string& input) {
        uint32_t state[8] = {
            0x6a09e667,0xbb67ae85,0x3c6ef372,0xa54ff53a,
            0x510e527f,0x9b05688c,0x1f83d9ab,0x5be0cd19
        };
        unsigned char block[128];
        size_t pos = 0;
        size_t len = input.size();

        while (pos + 64 <= len) {
            sha256Transform(state, reinterpret_cast<const unsigned char*>(input.data() + pos));
            pos += 64;
        }

        size_t rem = len - pos;
        std::memset(block, 0, 64);
        std::memcpy(block, input.data() + pos, rem);
        block[rem] = 0x80;

        if (rem >= 56) {
            sha256Transform(state, block);
            std::memset(block, 0, 64);
        }

        uint64_t bitlen = len * 8;
        for (int i = 0; i < 8; i++) {
            block[56 + i] = static_cast<unsigned char>(bitlen >> (56 - i*8));
        }
        sha256Transform(state, block);

        std::string hash(32, '\0');
        for (int i = 0; i < 8; i++) {
            hash[i*4]   = static_cast<char>(state[i] >> 24);
            hash[i*4+1] = static_cast<char>(state[i] >> 16);
            hash[i*4+2] = static_cast<char>(state[i] >> 8);
            hash[i*4+3] = static_cast<char>(state[i]);
        }
        return hash;
    }

    // Base64 encoder (standard table, no padding, URL-safe output)
    // Original Kotlin: toBase64NoPadding() + base64ToBase64Url() converts +->-, /->_
    constexpr char B64_TABLE[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

    std::string base64UrlNoPad(const std::string& data) {
        // Original Kotlin: sha256.digest(str.toByteArray()).toBase64NoPadding() + base64ToBase64Url
        // Produces URL-safe base64 with no '=' padding
        std::string out;
        size_t len = data.size();
        for (size_t i = 0; i < len; i += 3) {
            uint32_t val = static_cast<unsigned char>(data[i]) << 16;
            if (i + 1 < len) val |= static_cast<unsigned char>(data[i+1]) << 8;
            if (i + 2 < len) val |= static_cast<unsigned char>(data[i+2]);
            out += B64_TABLE[(val >> 18) & 0x3F];
            out += B64_TABLE[(val >> 12) & 0x3F];
            if (i + 1 < len) out += B64_TABLE[(val >> 6) & 0x3F];
            if (i + 2 < len) out += B64_TABLE[val & 0x3F];
        }
        return out;
    }

    // JSON string escape helper
    std::string esc(const std::string& s) {
        std::string out;
        for (char c : s) {
            switch (c) {
                case '"':  out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n";  break;
                case '\r': out += "\\r";  break;
                case '\t': out += "\\t";  break;
                default:   out += c;
            }
        }
        return out;
    }
} // anonymous namespace

// ====== Enum conversions ======
// threePidMediumToString / threePidMediumFromString are now in identity_utils.cpp

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
    if (!input.empty() && input[0] == '+') return true;
    for (char c : input) {
        if (c >= '0' && c <= '9') continue;
        if (c == ' ' || c == '-' || c == '(' || c == ')') continue;
        return false;
    }
    return input.size() >= 7;
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

// Original: setNewIdentityServer(url) -> final url (prepend "https://")
std::string IdentityServerManager::setNewIdentityServer(const std::string& url, std::string& error) {
    std::string finalUrl = url;
    if (finalUrl.find("://") == std::string::npos) {
        finalUrl = "https://" + finalUrl;
    }

    if (!isValidServerUrl(finalUrl)) {
        error = "Invalid identity server URL: " + finalUrl;
        return "";
    }

    // Original Kotlin: disconnect previous, set new, reset consent
    disconnect();

    config_.currentServerUrl = finalUrl;
    config_.userConsent = false; // Original Kotlin: identityStore.setUserConsent(false)
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
    return !version.empty();
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
// Original: lookUp(threePids) -> List<IS_FoundThreePid>

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
            pos++;
            while (pos < json.size() && json[pos] != '}') {
                while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',')) pos++;
                if (pos >= json.size() || json[pos] == '}') break;

                if (json[pos] == '"') {
                    pos++;
                    size_t keyEnd = pos;
                    while (keyEnd < json.size() && json[keyEnd] != '"') keyEnd++;
                    std::string key = json.substr(pos, keyEnd - pos);

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

// ====== NEW: Identity API Request Builder ======
// Original Kotlin: IdentityAPI.kt, IdentityAuthAPI.kt - Retrofit @POST("{endpoint}")

std::string IdentityServerManager::buildIdentityApiRequest(
    const std::string& endpoint,
    const std::string& body,
    const std::string& accessToken) const {
    // Original Kotlin: Retrofit creates the full HTTP request automatically.
    // In C++ we return the serialized request info that the JNI/HTTP layer will use.
    // Format: {"endpoint":"...","body":{...},"authorization":"Bearer ..."}
    //
    // The caller (JNI) should:
    //   1. Build URL: config_.currentServerUrl + endpoint
    //   2. Set Authorization: Bearer <accessToken> header if token provided
    //   3. POST with body as JSON

    std::ostringstream os;
    os << "{\"endpoint\":\"" << esc(endpoint) << "\"";
    if (!accessToken.empty()) {
        os << ",\"authorization\":\"Bearer " << esc(accessToken) << "\"";
    }
    if (!body.empty()) {
        os << ",\"body\":" << body;
    }
    os << "}";
    return os.str();
}

// ====== NEW: Bulk Lookup Body (member) ======
// Original Kotlin: IdentityBulkLookupTask.kt -> identityAPI.lookup(IdentityLookUpParams(...))

std::string IdentityServerManager::buildBulkLookupBody(const std::vector<ThreePid>& threepids) const {
    // Original Kotlin: builds "threepids" JSON array format
    // Delegates to free function in identity_utils.hpp by explicit namespace qualifier
    return progressive::buildBulkLookupBody(threepids);
}

// ====== NEW: Hashed Lookup Request (member) ======
// Original Kotlin: IdentityBulkLookupTask.kt lookUpInternal() ->
//   IdentityLookUpParams(hashedAddresses, "sha256", pepper)

std::string IdentityServerManager::buildHashedLookupRequest(
    const std::vector<std::string>& hashedAddresses,
    const std::string& algorithm,
    const std::string& pepper) const {
    // Original Kotlin: {"addresses":[...], "algorithm":"sha256", "pepper":"..."}
    // Ref: https://github.com/matrix-org/matrix-doc/blob/hs/hash-identity/proposals/2134-identity-hash-lookup.md
    std::ostringstream os;
    os << "{\"addresses\":[";
    for (size_t i = 0; i < hashedAddresses.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << esc(hashedAddresses[i]) << "\"";
    }
    os << "],\"algorithm\":\"" << esc(algorithm)
       << "\",\"pepper\":\"" << esc(pepper) << "\"}";
    return os.str();
}

// ====== NEW: Parse Bulk Lookup Response (member) ======
// Original Kotlin: IdentityBulkLookupTask.kt handleSuccess() mapping hashed addresses back

IdentityBulkLookupResponse IdentityServerManager::parseBulkLookupResponse(
    const std::string& json,
    const std::vector<ThreePid>& originalPids) const {
    // Original Kotlin: maps IdentityLookUpResponse.mappings keys back to threePids via indexOf
    IdentityBulkLookupResponse response;

    auto pos = json.find("\"mappings\"");
    if (pos == std::string::npos) return response;

    pos = json.find('{', pos);
    if (pos == std::string::npos) return response;
    pos++;

    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n' || json[pos] == '\t')) pos++;
        if (pos >= json.size() || json[pos] == '}') break;

        if (json[pos] == '"') {
            pos++;
            size_t keyEnd = pos;
            while (keyEnd < json.size() && json[keyEnd] != '"') keyEnd++;
            std::string hashedKey = json.substr(pos, keyEnd - pos);

            auto colon = json.find(':', keyEnd);
            if (colon == std::string::npos) break;
            auto vStart = json.find('"', colon);
            if (vStart == std::string::npos) break;
            vStart++;
            size_t vEnd = vStart;
            while (vEnd < json.size() && json[vEnd] != '"') vEnd++;
            std::string mxid = json.substr(vStart, vEnd - vStart);

            FoundThreePid found;
            found.mxid = mxid;
            // For hashed lookup, the key is a SHA-256 hash - we can't directly map back
            // without re-hashing all originals. The caller should provide pre-hashed indexes.
            found.threePid = ThreePid{}; // Caller resolves via hashedAddresses index mapping
            if (!found.mxid.empty()) {
                response.threepids.push_back(found);
            }

            pos = vEnd + 1;
        } else {
            pos++;
        }
    }

    return response;
}

// ====== NEW: Register Body ======
// Original Kotlin: IdentityRegisterTask.kt -> identityAuthAPI.register(openIdToken)
// POST /_matrix/identity/v2/account/register body = OpenIdToken

std::string IdentityServerManager::buildRegisterBody(const IdentityRegisterRequest& req) const {
    // Original Kotlin: The body is the OpenIdToken object (access_token, token_type, matrix_server_name, expires_in)
    // The key fields are: {"access_token":"...","token_type":"Bearer","matrix_server_name":"...","expires_in":3600}
    std::ostringstream os;
    os << "{\"access_token\":\"" << esc(req.idAccessToken) << "\"";
    if (!req.address.empty()) {
        os << ",\"address\":\"" << esc(req.address) << "\"";
    }
    if (!req.clientSecret.empty()) {
        os << ",\"client_secret\":\"" << esc(req.clientSecret) << "\"";
    }
    if (req.sendAttempt > 0) {
        os << ",\"send_attempt\":" << req.sendAttempt;
    }
    if (!req.nextLink.empty()) {
        os << ",\"next_link\":\"" << esc(req.nextLink) << "\"";
    }
    os << "}";
    return os.str();
}

// ====== NEW: Parse Register Response ======
// Original Kotlin: IdentityRegisterResponse(token)

IdentityRegisterResponse IdentityServerManager::parseRegisterResponse(const std::string& json) const {
    // Original Kotlin: {"token": "..."}
    IdentityRegisterResponse resp;
    resp.sid = extractStr(json, "token"); // original response field is "token"
    return resp;
}

// ====== NEW: Parse Identity Binding Response ======
// Original Kotlin: identity binding API - 3PID association metadata

IdentityBindingResponse IdentityServerManager::parseIdentityBindingResponse(const std::string& json) const {
    IdentityBindingResponse resp;
    resp.address = extractStr(json, "address");
    resp.medium  = extractStr(json, "medium");
    resp.mxid    = extractStr(json, "mxid");
    resp.notBefore = extractInt(json, "not_before");
    resp.notAfter  = extractInt(json, "not_after");
    resp.ts        = extractInt(json, "ts");
    return resp;
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
    std::ostringstream os;
    os << R"({"medium":")" << threePid.toMedium()
       << R"(","value":")" << esc(threePid.value)
       << R"(","valid":)" << (threePid.valid ? "true" : "false")
       << "}";
    return os.str();
}

std::string IdentityServerManager::bindingToJson(const IS_ThreePidBindingStatus& status) const {
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
    std::ostringstream os;
    os << R"({"medium":")" << found.threePid.toMedium()
       << R"(","value":")" << esc(found.threePid.value)
       << R"(","matrix_id":")" << esc(found.matrixId)
       << R"(","valid":)" << (found.valid ? "true" : "false")
       << "}";
    return os.str();
}

// ====== Free Functions ======

// ---- isIdentityServerAvailable ----
// Original Kotlin: IdentityPingTask.kt - calls identityAuthAPI.ping(), catches 404 for v1 fallback

bool isIdentityServerAvailable(const std::string& identityServerUrl) {
    // Original Kotlin: IdentityPingTask calls ping() -> 200 = success, 404 -> try v1 -> outdated
    // In C++ utility layer we validate syntactically + check URL format.
    // The actual HTTP ping must be performed by the JNI/Java layer using buildStatusCheckUrl().
    // Returns false if the URL is empty or malformed.
    if (identityServerUrl.empty()) return false;
    if (identityServerUrl.find("://") == std::string::npos) return false;
    if (identityServerUrl.size() < 10) return false;
    return true;
}

// ---- formatThreePidForDisplay ----
// Original Kotlin: ThreePid display formatting - "email@example.com" or formatted phone number

std::string formatThreePidForDisplay(const ThreePid& pid) {
    // Original Kotlin: email is shown as-is, phone numbers may be formatted
    // (Element Android uses libphonenumber for MSISDN formatting)
    if (pid.address.empty()) return "";

    if (pid.medium == ThreePidMedium::EMAIL) {
        return pid.address;
    }

    if (pid.medium == ThreePidMedium::MSISDN) {
        // Basic E.164 formatting: "+12345678901" -> "+1 234 567 8901"
        std::string addr = pid.address;
        if (addr.size() >= 2 && addr[0] == '+') {
            std::string formatted;
            formatted += addr[0]; // '+'
            size_t pos = 1;
            // Country code (1-3 digits)
            int ccLen = 1;
            if (addr.size() > 3 && (addr[1] == '1' || addr[1] == '7')) ccLen = 1;
            else if (addr.size() > 12) ccLen = 2;
            else if (addr.size() > 13) ccLen = 3;
            else ccLen = 1;
            for (int i = 0; i < ccLen && pos < addr.size(); i++, pos++) {
                formatted += addr[pos];
            }
            formatted += ' ';
            // Remaining digits in groups of 3
            int count = 0;
            while (pos < addr.size()) {
                if (count == 3) { formatted += ' '; count = 0; }
                if (addr[pos] >= '0' && addr[pos] <= '9') {
                    formatted += addr[pos];
                    count++;
                }
                pos++;
            }
            return formatted;
        }
        return addr;
    }

    return pid.address;
}

// ---- isThreePidBound ----
// Original Kotlin: IdentityService.kt getShareStatus() - checks lookup results + pending bindings

bool isThreePidBound(const ThreePid& pid, const std::string& mxid) {
    // Original Kotlin: If lookupResult contains threePid, SharedState is SHARED
    // In C++ utility: checks basic consistency - pid has a bound flag or has MXID in lookup
    if (pid.bound) return true;
    if (mxid.empty()) return false;
    // Check if the 3PID has a valid session (sid) - indicates binding activity
    if (!pid.sid.empty()) return true; // has been registered with identity server
    return false;
}

// ---- hashThreePidAddress ----
// Original Kotlin: Sha256Converter.kt convertToSha256() +
//   IdentityBulkLookupTask.kt getHashedAddresses()
// Format: SHA-256(lowercase(address) + " " + medium + " " + pepper)
// Returns base64url-no-padding encoded hash

std::string hashThreePidAddress(const std::string& address,
                                const std::string& medium,
                                const std::string& pepper) {
    // Original Kotlin:
    //   threePid.value.lowercase(Locale.ROOT) + " " + threePid.toMedium() + " " + pepper
    //   sha256Converter.convertToSha256(str) -> .toBase64NoPadding() -> base64ToBase64Url()
    std::string addrLower = address;
    std::transform(addrLower.begin(), addrLower.end(), addrLower.begin(),
                   [](unsigned char c) { return std::tolower(c); });

    std::string input = addrLower + " " + medium + " " + pepper;
    std::string hashRaw = sha256Raw(input);
    return base64UrlNoPad(hashRaw);
}

// ---- buildHashedLookupRequest (free function) ----
// Original Kotlin: IdentityBulkLookupTask lookUpInternal() ->
//   IdentityLookUpParams(hashedAddresses, ALGORITHM_SHA256, pepper)

std::string buildHashedLookupRequest(const std::vector<std::string>& hashedAddresses,
                                     const std::string& algorithm,
                                     const std::string& pepper) {
    // Original Kotlin: POST /_matrix/identity/v2/lookup
    // {"addresses":["hash1","hash2"],"algorithm":"sha256","pepper":"salt123"}
    std::ostringstream os;
    os << "{\"addresses\":[";
    for (size_t i = 0; i < hashedAddresses.size(); i++) {
        if (i > 0) os << ",";
        os << "\"" << esc(hashedAddresses[i]) << "\"";
    }
    os << "],\"algorithm\":\"" << esc(algorithm)
       << "\",\"pepper\":\"" << esc(pepper) << "\"}";
    return os.str();
}

} // namespace progressive
