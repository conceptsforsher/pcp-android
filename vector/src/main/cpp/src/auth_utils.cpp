#include "progressive/auth_utils.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <chrono>

namespace progressive {

AuthFlow parseAuthFlow(const std::string& responseJson, int httpStatus) {
    AuthFlow flow;

    if (httpStatus != 401 && httpStatus != 403) return flow;

    flow.sessionId  = parseJsonStringValue(responseJson, "session");
    flow.errorMessage = parseJsonStringValue(responseJson, "error");

    // Parse required stages: "flows": [{"stages": ["m.login.password", ...]}]
    auto flows = parseJsonStringValue(responseJson, "flows");
    if (!flows.empty()) {
        // Take the first flow's stages
        std::string firstFlow = "{" + flows + "}";
        auto stages = parseJsonStringValue(firstFlow, "stages");
        if (!stages.empty()) {
            std::istringstream stream(stages);
            std::string stage;
            while (std::getline(stream, stage, ',')) {
                while (!stage.empty() && stage.front() == ' ') stage.erase(0, 1);
                while (!stage.empty() && stage.back() == ' ') stage.pop_back();
                if (!stage.empty()) flow.stages.push_back(stage);
            }
        }
    }

    flow.completed = flow.stages.empty();
    return flow;
}

std::string buildAuthStageBody(const std::string& sessionId, const std::string& type,
    const std::string& paramsJson) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"auth": {"session": ")" << esc(sessionId)
         << R"(", "type": ")" << esc(type) << R"(")";
    if (!paramsJson.empty() && paramsJson != "{}") {
        // Inject params into the auth object
        std::string pjson = paramsJson;
        if (pjson[0] == '{') pjson = pjson.substr(1);
        if (!pjson.empty() && pjson.back() == '}') pjson.pop_back();
        if (!pjson.empty()) json << "," << pjson;
    }
    json << "}}";
    return json.str();
}

bool requiresAdditionalAuth(const std::string& responseJson, int httpStatus) {
    return httpStatus == 401 || (httpStatus == 403 &&
        responseJson.find("\"session\"") != std::string::npos);
}

std::string parseAuthSessionId(const std::string& responseJson) {
    return parseJsonStringValue(responseJson, "session");
}

bool isAuthTypeSupported(const std::string& type, const std::vector<std::string>& flows) {
    for (const auto& f : flows) {
        if (f == type) return true;
    }
    return false;
}

std::string getNextAuthStage(const std::vector<std::string>& completed,
    const std::vector<std::string>& required) {
    for (const auto& r : required) {
        bool found = false;
        for (const auto& c : completed) {
            if (c == r) { found = true; break; }
        }
        if (!found) return r;
    }
    return ""; // all completed
}

CaptchaInfo parseCaptchaInfo(const std::string& responseJson) {
    CaptchaInfo info;

    auto params = parseJsonStringValue(responseJson, "params");
    if (params.empty()) return info;

    std::string wrapped = "{" + params + "}";
    info.publicKey = parseJsonStringValue(wrapped, "public_key");
    info.siteKey   = info.publicKey;

    info.required = !info.publicKey.empty();
    return info;
}

bool requiresCaptcha(const std::string& responseJson) {
    return responseJson.find("m.login.recaptcha") != std::string::npos ||
           responseJson.find("recaptcha") != std::string::npos;
}

std::string buildCaptchaResponse(const std::string& sessionId, const std::string& captchaToken) {
    return buildAuthStageBody(sessionId, "m.login.recaptcha",
        R"("response": ")" + captchaToken + R"(")");
}

TokenAuth parseTokenLogin(const std::string& url) {
    TokenAuth token;

    auto tokenPos = url.find("loginToken=");
    if (tokenPos == std::string::npos) return token;

    tokenPos += 11;
    auto end = url.find('&', tokenPos);
    if (end == std::string::npos) end = url.size();
    token.loginToken = url.substr(tokenPos, end - tokenPos);
    token.valid = !token.loginToken.empty();
    token.expiresInMs = 120000; // 2 minutes default

    return token;
}

std::string buildTokenLoginBody(const std::string& token, const std::string& deviceName) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << R"({"type": "m.login.token", "token": ")" << esc(token) << R"(")";
    if (!deviceName.empty())
        json << R"(,"initial_device_display_name": ")" << esc(deviceName) << R"(")";
    json << "}";
    return json.str();
}

bool isTokenExpired(const TokenAuth& token) {
    if (!token.valid) return true;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    // Assume token issued recently
    return false; // client can't know server-side expiry
}

RateLimit parseRateLimit(const std::string& responseJson, int httpStatus) {
    RateLimit limit;
    limit.isLimited = (httpStatus == 429);

    auto retryMs = parseJsonStringValue(responseJson, "retry_after_ms");
    if (!retryMs.empty()) limit.retryAfterMs = std::stoi(retryMs);
    else limit.retryAfterMs = 5000; // default 5 seconds

    limit.errorCode = parseJsonStringValue(responseJson, "errcode");
    return limit;
}

std::string formatRateLimitMessage(const RateLimit& limit) {
    if (!limit.isLimited) return "";
    int seconds = limit.retryAfterMs / 1000;
    if (seconds < 60) return "Rate limited. Try again in " + std::to_string(seconds) + " seconds.";
    return "Rate limited. Try again in " + std::to_string(seconds / 60) + " minutes.";
}

} // namespace progressive
