#include "progressive/sso_utils.hpp"
#include "progressive/login_utils.hpp"
#include "progressive/json_parser.hpp"
#include "progressive/url_tools.hpp"
#include <sstream>
#include <regex>
#include <algorithm>
#include <chrono>

namespace progressive {

std::vector<SsoRedirect> parseSsoRedirects(const std::string& loginFlowsJson) {
    std::vector<SsoRedirect> redirects;

    // Look for identity_providers or sso flows
    size_t pos = 0;
    while (true) {
        pos = loginFlowsJson.find("\"identity_providers\"", pos);
        if (pos == std::string::npos) break;

        auto bracket = loginFlowsJson.find('[', pos);
        if (bracket == std::string::npos) break;

        auto end = loginFlowsJson.find(']', bracket);
        if (end == std::string::npos) break;

        std::string array = loginFlowsJson.substr(bracket + 1, end - bracket - 1);

        // Parse each provider object
        size_t ipos = 0;
        while (true) {
            ipos = array.find("\"id\"", ipos);
            if (ipos == std::string::npos) break;

            auto objStart = array.rfind('{', ipos);
            if (objStart == std::string::npos) break;

            int depth = 0;
            auto objEnd = objStart;
            while (objEnd < array.size()) {
                if (array[objEnd] == '{') ++depth;
                else if (array[objEnd] == '}') --depth;
                if (depth == 0) break;
                ++objEnd;
            }
            if (objEnd >= array.size()) break;

            std::string obj = array.substr(objStart, objEnd - objStart + 1);

            SsoRedirect redirect;
            redirect.provider = parseJsonStringValue(obj, "id");
            redirect.brand = parseJsonStringValue(obj, "name");
            if (redirect.brand.empty()) redirect.brand = getSsoProviderBrand(redirect.provider);

            redirect.valid = !redirect.provider.empty();
            if (redirect.valid) redirects.push_back(redirect);

            ipos = objEnd + 1;
        }

        pos = end + 1;
    }

    return redirects;
}

LoginToken parseLoginToken(const std::string& callbackUrl) {
    LoginToken token;

    // Matrix SSO callback: https://.../#/login?loginToken=xxx
    auto tokenPos = callbackUrl.find("loginToken=");
    if (tokenPos == std::string::npos) {
        // Try element callback format
        tokenPos = callbackUrl.find("login_token=");
    }

    if (tokenPos != std::string::npos) {
        tokenPos += (callbackUrl[tokenPos + 5] == '=' ? 12 : 11);
        auto end = callbackUrl.find('&', tokenPos);
        if (end == std::string::npos) end = callbackUrl.size();
        token.token = callbackUrl.substr(tokenPos, end - tokenPos);
        token.loginToken = token.token;
        token.valid = true;
        token.expiresAtMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count() + 120000;
    }

    return token;
}

bool isSsoCallbackUrl(const std::string& url) {
    return url.find("loginToken=") != std::string::npos ||
           url.find("login_token=") != std::string::npos;
}

std::string buildSsoLoginUrl(const std::string& baseUrl, const std::string& redirectUrl) {
    std::string url = baseUrl;
    if (url.find('?') == std::string::npos) url += '?';
    else url += '&';
    url += "redirectUrl=" + urlEncode(redirectUrl);
    return url;
}

std::string extractSsoProvider(const std::string& idpId) {
    auto lastDot = idpId.rfind('.');
    if (lastDot != std::string::npos) return idpId.substr(lastDot + 1);
    return idpId;
}

std::string getSsoProviderBrand(const std::string& provider) {
    auto lower = provider;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);

    if (lower == "google") return "Google";
    if (lower == "github") return "GitHub";
    if (lower == "apple") return "Apple";
    if (lower == "facebook") return "Facebook";
    if (lower == "gitlab") return "GitLab";
    if (lower == "twitter" || lower == "x") return "X (Twitter)";
    if (lower == "microsoft") return "Microsoft";
    if (lower == "saml") return "SAML";
    if (lower == "oidc") return "OpenID Connect";
    if (lower == "cas") return "CAS";
    if (lower == "ldap") return "LDAP";
    if (lower == "keycloak") return "Keycloak";
    return provider;
}

bool isLoginTokenExpired(const LoginToken& token) {
    if (!token.valid) return true;
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return now >= token.expiresAtMs;
}

std::string ssoProvidersToJson(const std::vector<SsoRedirect>& providers) {
    auto esc = [](const std::string& s) -> std::string {
        std::string out; for (char c : s) { if (c == '"') out += "\\\""; else out += c; } return out;
    };
    std::ostringstream json;
    json << "[";
    for (size_t i = 0; i < providers.size(); ++i) {
        if (i > 0) json << ",";
        json << R"({"id": ")" << esc(providers[i].provider) << R"(")";
        json << R"(,"brand": ")" << esc(providers[i].brand) << R"(")";
        json << "}";
    }
    json << "]";
    return json.str();
}

HomeserverUrl validateHomeserverUrl(const std::string& input) {
    HomeserverUrl result;
    result.rawUrl = input;
    if (input.empty()) {
        result.errorMessage = "URL cannot be empty.";
        return result;
    }

    // Auto-add https:// if missing
    std::string url = input;
    if (url.find("://") == std::string::npos) {
        url = "https://" + url;
    }

    auto parsed = parseUrlParts(url);
    if (!parsed.valid) {
        result.errorMessage = "Invalid URL format.";
        return result;
    }

    result.sanitizedUrl = parsed.protocol + "://" + parsed.host;
    if (!parsed.port.empty() && parsed.port != "443" && parsed.port != "80") {
        result.sanitizedUrl += ":" + parsed.port;
    }
    result.serverName = parsed.host;
    result.isHttps = parsed.protocol == "https";
    result.port = parsed.port.empty() ? (result.isHttps ? 443 : 80) : std::stoi(parsed.port);
    result.valid = true;

    return result;
}

bool isHomeserverUrl(const std::string& url) {
    return url.find("matrix") != std::string::npos ||
           url.find("_matrix") != std::string::npos;
}

} // namespace progressive
