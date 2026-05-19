#include "progressive/identity_utils.hpp"
#include "progressive/matrix_patterns.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <ctime>

namespace progressive {

// ====== ResolvedId ======

ResolvedId resolveMatrixId(const std::string& input) {
    ResolvedId result;
    result.input = input;
    if (input.empty()) return result;

    // @user:server
    if (input[0] == '@' && isUserId(input)) {
        result.resolved = input;
        result.type = "user";
        result.valid = true;
        return result;
    }

    // !room:server
    if (input[0] == '!' && isRoomId(input)) {
        result.resolved = input;
        result.type = "room";
        result.valid = true;
        return result;
    }

    // #alias:server
    if (input[0] == '#' && isRoomAlias(input)) {
        result.resolved = input;
        result.type = "alias";
        result.valid = true;
        return result;
    }

    // $event
    if (input[0] == '$' && isEventId(input)) {
        result.resolved = input;
        result.type = "event";
        result.valid = true;
        return result;
    }

    // matrix.to URL
    if (isMatrixToPermalink(input)) {
        auto parsed = parseMatrixToPermalink(input);
        if (parsed.valid) {
            result.resolved = parsed.userId.empty() ? parsed.roomId : parsed.userId;
            result.type = parsed.type;
            result.valid = true;
            return result;
        }
    }

    // Email
    if (isEmail(input)) {
        result.resolved = input;
        result.type = "email";
        result.valid = true;
        return result;
    }

    // Phone
    if (isMsisdn(input)) {
        result.resolved = input;
        result.type = "phone";
        result.valid = true;
        return result;
    }

    return result;
}

IdentityThreePid parseThreePid(const std::string& input) {
    IdentityThreePid pid;
    if (input.empty()) return pid;

    if (isEmail(input)) {
        pid.medium = "email";
        pid.address = input;
        pid.valid = true;
        return pid;
    }

    if (isMsisdn(input)) {
        pid.medium = "msisdn";
        pid.address = input;
        pid.valid = true;
        return pid;
    }

    return pid;
}

bool isEmail(const std::string& input) {
    std::regex re(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
    return std::regex_match(input, re);
}

bool isMsisdn(const std::string& input) {
    // E.164: + followed by 7-15 digits
    std::regex re(R"(^\+\d{7,15}$)");
    return std::regex_match(input, re);
}

std::string formatThreePid(const IdentityThreePid& pid) {
    if (!pid.valid) return "";
    if (pid.medium == "email") return pid.address;
    if (pid.medium == "msisdn") return pid.address;
    return pid.address;
}

// ====== ThreePidMedium ======
// Original Kotlin: ThreePid.kt toMedium(), ThirdPartyIdentifier constants

const char* threePidMediumToString(ThreePidMedium medium) {
    return medium == ThreePidMedium::EMAIL ? "email" : "msisdn";
}

ThreePidMedium threePidMediumFromString(const std::string& s) {
    if (s == "email") return ThreePidMedium::EMAIL;
    if (s == "msisdn") return ThreePidMedium::MSISDN;
    return ThreePidMedium::EMAIL;
}

// ====== ThreePid (rich model) ======
// Original: IS_ThreePid::parse / IS_ThreePid::isEmail / IS_ThreePid::isMsisdn

ThreePid ThreePid::parse(const std::string& input) {
    ThreePid pid;
    pid.value = input;
    pid.address = input;
    if (isEmail(input)) {
        pid.medium = ThreePidMedium::EMAIL;
        pid.valid = true;
        return pid;
    }
    if (isMsisdn(input)) {
        pid.medium = ThreePidMedium::MSISDN;
        pid.valid = true;
        return pid;
    }
    return pid;
}

bool ThreePid::isEmail(const std::string& input) {
    auto at = input.find('@');
    return at != std::string::npos && at > 0 && at < input.size() - 1;
}

bool ThreePid::isMsisdn(const std::string& input) {
    if (!input.empty() && input[0] == '+') return true;
    for (char c : input) {
        if (c >= '0' && c <= '9') continue;
        if (c == ' ' || c == '-' || c == '(' || c == ')') continue;
        return false;
    }
    return input.size() >= 7;
}

// ====== IdentityServiceState ======

const char* identityServiceStateToString(IdentityServiceState state) {
    switch (state) {
        case IdentityServiceState::NOT_CONNECTED: return "NOT_CONNECTED";
        case IdentityServiceState::CONNECTING:    return "CONNECTING";
        case IdentityServiceState::CONNECTED:     return "CONNECTED";
        case IdentityServiceState::DISCONNECTED:  return "DISCONNECTED";
        case IdentityServiceState::TOKEN_EXPIRED: return "TOKEN_EXPIRED";
    }
    return "NOT_CONNECTED";
}

IdentityServiceState identityServiceStateFromString(const std::string& s) {
    if (s == "CONNECTING")    return IdentityServiceState::CONNECTING;
    if (s == "CONNECTED")     return IdentityServiceState::CONNECTED;
    if (s == "DISCONNECTED")  return IdentityServiceState::DISCONNECTED;
    if (s == "TOKEN_EXPIRED") return IdentityServiceState::TOKEN_EXPIRED;
    return IdentityServiceState::NOT_CONNECTED;
}

// ====== JSON helper (used by parsers) ======

namespace {
    std::string extractStr(const std::string& json, const std::string& key) {
        auto pp = json.find("\"" + key + "\"");
        if (pp == std::string::npos) return "";
        pp = json.find('"', pp + key.size() + 2);
        if (pp == std::string::npos) return "";
        pp++;
        size_t e = pp;
        while (e < json.size() && json[e] != '"') e++;
        return json.substr(pp, e - pp);
    }

    int64_t extractInt(const std::string& json, const std::string& key) {
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

    std::string escapeJson(const std::string& s) {
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
}

// ====== JSON Builders ======
// Original Kotlin: IdentityRequestTokenForEmailBody.kt,
//   IdentityRequestTokenForMsisdnBody.kt,
//   IdentityRequestOwnershipParams.kt

std::string buildThreePidRequest(const ThreePid& pid,
                                 const std::string& clientSecret,
                                 int sendAttempt,
                                 const std::string& countryCode) {
    // Original Kotlin: when (params.threePid) is Email -> email=..., is Msisdn -> phone_number=..., country=..., send_attempt, client_secret
    std::ostringstream os;
    os << "{\"client_secret\":\"" << escapeJson(clientSecret)
       << "\",\"send_attempt\":" << sendAttempt;

    if (pid.medium == ThreePidMedium::EMAIL) {
        // Original Kotlin: IdentityRequestTokenForEmailBody(email = params.threePid.email)
        os << ",\"email\":\"" << escapeJson(pid.address) << "\"";
    } else {
        // Original Kotlin: IdentityRequestTokenForMsisdnBody(phoneNumber = params.threePid.msisdn, countryCode = params.threePid.getCountryCode())
        os << ",\"phone_number\":\"" << escapeJson(pid.address) << "\"";
        os << ",\"country\":\"" << escapeJson(countryCode.empty() ? "US" : countryCode) << "\"";
    }

    os << "}";
    return os.str();
}

std::string buildThreePidValidationRequest(const std::string& sid,
                                           const std::string& clientSecret,
                                           const std::string& token) {
    // Original Kotlin: IdentityRequestOwnershipParams(clientSecret, sid, token)
    std::ostringstream os;
    os << "{\"client_secret\":\"" << escapeJson(clientSecret)
       << "\",\"sid\":\"" << escapeJson(sid)
       << "\",\"token\":\"" << escapeJson(token)
       << "\"}";
    return os.str();
}

std::string buildBulkLookupBody(const std::vector<ThreePid>& threepids) {
    // Original Kotlin: identity-bulk-lookup unhashed "threepids" array format
    // {"threepids":[["medium","address"],...]}
    std::ostringstream os;
    os << "{\"threepids\":[";
    for (size_t i = 0; i < threepids.size(); i++) {
        if (i > 0) os << ",";
        os << "[\"" << escapeJson(threePidMediumToString(threepids[i].medium))
           << "\",\"" << escapeJson(threepids[i].address) << "\"]";
    }
    os << "]}";
    return os.str();
}

// ====== Manual Parsers ======
// Original Kotlin: IdentityRequestTokenResponse.kt (sid),
//   IdentityBulkLookupTask.kt handleSuccess()

ThreePidValidationSession parseThreePidTokenResponse(const std::string& json) {
    // Original Kotlin: IdentityRequestTokenResponse(sid)
    ThreePidValidationSession session;
    session.sid = extractStr(json, "sid");
    if (!session.sid.empty()) {
        session.validatedAt = static_cast<int64_t>(std::time(nullptr)) * 1000;
    }
    return session;
}

std::vector<FoundThreePid> parseBulkLookupResponse(
    const std::string& json,
    const std::vector<ThreePid>& originalPids) {
    // Original Kotlin: IdentityBulkLookupTask handleSuccess -
    //   lookUpData.identityLookUpResponse.mappings (Map<String,String>) mapped back
    //   via hashedAddresses.indexOf()
    //
    // For unhashed lookup, the "mappings" object uses keys like "email+address"
    // For hashed lookup, keys are sha256 hashes

    std::vector<FoundThreePid> results;

    // Find "mappings"
    auto pos = json.find("\"mappings\"");
    if (pos == std::string::npos) return results;

    pos = json.find('{', pos);
    if (pos == std::string::npos) return results;
    pos++;

    // Iterate key-value pairs in mappings
    while (pos < json.size()) {
        while (pos < json.size() && (json[pos] == ' ' || json[pos] == ',' || json[pos] == '\n' || json[pos] == '\t')) pos++;
        if (pos >= json.size() || json[pos] == '}') break;

        if (json[pos] == '"') {
            pos++;
            size_t keyEnd = pos;
            while (keyEnd < json.size() && json[keyEnd] != '"') keyEnd++;
            std::string key = json.substr(pos, keyEnd - pos);

            // Find colon
            auto col = json.find(':', keyEnd);
            if (col == std::string::npos) break;
            // Find value string
            auto vStart = json.find('"', col);
            if (vStart == std::string::npos) break;
            vStart++;
            size_t vEnd = vStart;
            while (vEnd < json.size() && json[vEnd] != '"') vEnd++;
            std::string mxid = json.substr(vStart, vEnd - vStart);

            FoundThreePid found;
            found.mxid = mxid;

            // Try to match key back to original pid
            // For unhashed lookup, key format is "medium+address"
            // For hashed lookup, we need to hash each original and compare
            for (const auto& pid : originalPids) {
                std::string candidate = std::string(threePidMediumToString(pid.medium)) + "+" + pid.address;
                if (candidate == key) {
                    found.threePid = pid;
                    break;
                }
            }

            if (!found.mxid.empty()) {
                results.push_back(found);
            }

            pos = vEnd + 1;
        } else {
            pos++;
        }
    }

    return results;
}

// ====== Display Name Utilities ======

bool isAmbiguousName(const std::string& name1, const std::string& name2) {
    if (name1.empty() || name2.empty()) return false;
    auto n1 = name1;
    auto n2 = name2;
    std::transform(n1.begin(), n1.end(), n1.begin(), ::tolower);
    std::transform(n2.begin(), n2.end(), n2.begin(), ::tolower);
    return n1 == n2;
}

bool isValidDisplayName(const std::string& name) {
    if (name.empty() || name.size() > 100) return false;
    bool hasNonSpace = false;
    for (char c : name) {
        if (!std::isspace(static_cast<unsigned char>(c))) hasNonSpace = true;
    }
    return hasNonSpace;
}

std::string getIdentityInitials(const std::string& displayName, int maxChars) {
    std::string result;
    bool takeNext = true;
    for (char c : displayName) {
        if (takeNext && !std::isspace(static_cast<unsigned char>(c))) {
            result += std::toupper(static_cast<unsigned char>(c));
            takeNext = false;
            if (static_cast<int>(result.size()) >= maxChars) break;
        }
        if (std::isspace(static_cast<unsigned char>(c))) takeNext = true;
    }
    return result;
}

bool isCanonicalAlias(const std::string& alias, const std::string& expectedRoomId) {
    return !alias.empty() && expectedRoomId.find(alias) != std::string::npos;
}

std::string extractAliasLocalpart(const std::string& alias) {
    if (alias.empty() || alias[0] != '#') return {};
    auto colon = alias.find(':');
    if (colon == std::string::npos) return alias.substr(1);
    return alias.substr(1, colon - 1);
}

std::vector<std::string> suggestAliases(const std::string& roomName, int maxResults) {
    std::vector<std::string> aliases;

    auto name = roomName;
    std::transform(name.begin(), name.end(), name.begin(), ::tolower);
    for (char& c : name) {
        if (std::isspace(static_cast<unsigned char>(c)) || c == '.') c = '-';
    }
    std::string cleaned;
    for (char c : name) {
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '-' || c == '_') {
            cleaned += c;
        }
    }

    if (!cleaned.empty()) {
        aliases.push_back("#" + cleaned);
        if (static_cast<int>(aliases.size()) >= maxResults) return aliases;
        aliases.push_back("#" + cleaned + "-chat");
        if (static_cast<int>(aliases.size()) >= maxResults) return aliases;
        aliases.push_back("#" + cleaned + "-room");
    }

    return aliases;
}

std::string disambiguateName(const std::string& displayName, const std::string& mxid) {
    if (displayName.empty()) return mxid;
    return displayName + " (" + mxid + ")";
}

} // namespace progressive
