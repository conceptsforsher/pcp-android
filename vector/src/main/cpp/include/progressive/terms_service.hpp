#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace progressive {

// Represents a localized terms/policy document from a service.
//
// Original Kotlin (TermsResponse.kt:28-79):
//   data class TermsResponse(@Json(name = "policies") val policies: JsonDict? = null)
//   fun getLocalizedTerms(userLanguage, defaultLanguage): List<LocalizedFlowDataLoginTerms>
//
// JSON format:
//   {"policies": {"terms_of_service": {"version": "1.2",
//       "en": {"name": "Terms of Service", "url": "https://..."},
//       "fr": {"name": "Conditions d'utilisation", "url": "https://..."}}}}

struct LocalizedTerms {
    std::string policyName;      // e.g. "terms_of_service", "privacy_policy"
    std::string localizedName;   // The name in the user's language
    std::string localizedUrl;    // The URL in the user's language
    std::string version;         // Policy version string
    bool accepted = false;       // Whether the user has already accepted
};

// Raw policies: policy_name → language → {name, url}
using PolicyMap = std::unordered_map<std::string, std::unordered_map<std::string, std::unordered_map<std::string, std::string>>>;

// Original Kotlin (TermsResponse.kt:28-32):
//   data class TermsResponse(
//       @Json(name = "policies") val policies: JsonDict? = null
//   )
struct TermsResponse {
    // policies: policy_name → { "version": ..., "en": {...}, "fr": {...} }
    PolicyMap policies;

    bool empty() const { return policies.empty(); }

    // Original Kotlin (TermsResponse.kt:34-52):
    //   fun getLocalizedTerms(userLanguage, defaultLanguage="en"):
    //       List<LocalizedFlowDataLoginTerms>
    std::vector<LocalizedTerms> getLocalizedTerms(
        const std::string& userLanguage,
        const std::string& defaultLanguage = "en"
    ) const;
};

// Original Kotlin (GetTermsResponse.kt:19-22):
//   data class GetTermsResponse(
//       val serverResponse: TermsResponse,
//       val alreadyAcceptedTermUrls: Set<String>
//   )
struct GetTermsResponse {
    TermsResponse serverResponse;
    std::vector<std::string> alreadyAcceptedUrls;
};

// Original Kotlin (AcceptTermsBody.kt:28-31):
//   data class AcceptTermsBody(
//       @Json(name = "user_accepts") val acceptedTermUrls: List<String>
//   )
struct AcceptTermsBody {
    std::vector<std::string> acceptedUrls;
};

// Parse TermsResponse JSON from GET /terms endpoint
TermsResponse parseTermsResponse(const std::string& json);

// Serialize TermsResponse to JSON
std::string termsResponseToJson(const TermsResponse& response);

// Serialize AcceptTermsBody to JSON for POST /agree
std::string acceptTermsBodyToJson(const AcceptTermsBody& body);

// Extract localized terms with accepted status
std::vector<LocalizedTerms> getLocalizedTermsWithStatus(
    const TermsResponse& response,
    const std::vector<std::string>& alreadyAcceptedUrls,
    const std::string& userLanguage,
    const std::string& defaultLanguage = "en"
);

} // namespace progressive
