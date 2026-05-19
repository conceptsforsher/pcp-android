#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ================================================================
// Terms/Consent Manager — terms of service & consent tracking
//
// Faithful port from Element Android original sources:
//   TermsService.kt — getTerms, agreeToTerms, getHomeserverTerms,
//     ServiceType (IntegrationManager, IdentityService)
//   TermsResponse.kt — policies (JsonDict), getLocalizedTerms()
//   LocalizedFlowDataLoginTerms.kt — policyName, version,
//     localizedUrl, localizedName
//
// Matrix spec: M_TERMS_NOT_SIGNED error, GET /terms endpoint
//   Terms are required before registration/login on some servers.
//   User must agree to all required policies before proceeding.
//
// Covers:
//   1. Terms policy parsing (policies → localized terms)
//   2. Terms acceptance tracking
//   3. Service types (Integration Manager, Identity Service)
//   4. Language fallback (user lang → default "en")
//   5. Build agree request body
//   6. Terms required check
// ================================================================

// ---- Service Type ----
// Original: TermsService.ServiceType

enum class TermsServiceType {
    INTEGRATION_MANAGER = 0,
    IDENTITY_SERVICE = 1,
    HOME_SERVER = 2,
};

const char* termsServiceTypeToString(TermsServiceType type);

// ---- Localized Terms ----
// Original: LocalizedFlowDataLoginTerms (policyName, version, localizedUrl, localizedName)

struct LocalizedTerms {
    std::string policyName;        // e.g. "privacy_policy"
    std::string version;           // e.g. "1.2"
    std::string localizedUrl;      // e.g. "https://example.org/privacy/en"
    std::string localizedName;     // e.g. "Privacy Policy"
    bool valid = false;
};

// ---- Terms Response ----
// Original: TermsResponse.kt (policies JsonDict)

struct TermsResponse {
    std::vector<LocalizedTerms> policies;
    std::string rawJson;
    bool hasPolicies = false;
    int policyCount = 0;
};

// ---- Terms Agreement Request ----

struct TermsAgreementRequest {
    TermsServiceType serviceType = TermsServiceType::HOME_SERVER;
    std::string baseUrl;
    std::vector<std::string> agreedUrls;    // URLs of agreed policies
    std::string token;                      // Optional auth token
};

// ---- Terms Manager ----

class TermsManager {
public:
    TermsManager();

    // ====== Terms Parsing ======
    // Original: TermsResponse.getLocalizedTerms(userLanguage, defaultLanguage)

    // Parse the homeserver terms response.
    // Original: getHomeserverTerms(baseUrl) → TermsResponse
    TermsResponse parseTermsResponse(const std::string& json);

    // Get localized terms for a specific language.
    // Falls back to "en" if userLanguage not found.
    // Original: getLocalizedTerms(userLanguage, defaultLanguage="en")
    std::vector<LocalizedTerms> getLocalizedTerms(const TermsResponse& response,
                                                    const std::string& userLanguage = "en",
                                                    const std::string& defaultLanguage = "en");

    // ====== Terms Agreement ======
    // Original: agreeToTerms(serviceType, baseUrl, agreedUrls, token)

    // Build the terms agreement request body.
    // POST /_matrix/client/r0/terms
    std::string buildAgreeRequest(const TermsAgreementRequest& req);

    // Check if terms agreement was successful.
    bool parseAgreementResponse(const std::string& json);

    // ====== Terms Status ======

    // Check if terms are required (M_TERMS_NOT_SIGNED error).
    bool areTermsRequired(const std::string& errorJson);

    // Check if user has agreed to all required terms.
    bool hasAgreedToAll(const TermsResponse& response, const std::vector<std::string>& agreedUrls);

    // Get the list of policy URLs that still need agreement.
    std::vector<std::string> getPendingPolicies(const TermsResponse& response,
                                                  const std::vector<std::string>& agreedUrls);

    // ====== Display ======

    // Format terms for display.
    std::string formatTermsList(const std::vector<LocalizedTerms>& terms);

    // Format a single term for display.
    std::string formatSingleTerm(const LocalizedTerms& term);

    // ====== Serialization ======

    // Export localized terms as JSON.
    std::string termsToJson(const std::vector<LocalizedTerms>& terms);

    // Export terms response as JSON.
    std::string responseToJson(const TermsResponse& resp);

private:
    std::unordered_map<std::string, bool> agreedPolicies_; // policyUrl → agreed
};

struct AcceptTermsBody {
    std::vector<std::string> acceptedUrls;
};

inline std::string acceptTermsBodyToJson(const AcceptTermsBody& body) {
    std::string json = R"({"acceptedUrls":[)";
    for (size_t i = 0; i < body.acceptedUrls.size(); i++) {
        if (i > 0) json += ",";
        json += "\"" + body.acceptedUrls[i] + "\"";
    }
    json += "]}";
    return json;
}

} // namespace progressive
