#include "progressive/terms_service.hpp"
#include <algorithm>

namespace progressive {

// ==== LocalizedTerms Extraction ====
//
// Original Kotlin (TermsResponse.kt:34-52):
//   fun getLocalizedTerms(userLanguage, defaultLanguage = "en"):
//       List<LocalizedFlowDataLoginTerms> {
//       return policies?.map {
//           val tos = policies[it.key] as? Map<*, *>
//           ((tos[userLanguage] ?: tos[defaultLanguage]) as? Map<*, *>)?.let { termsMap ->
//               val name = termsMap[NAME] as? String
//               val url = termsMap[URL] as? String
//               LocalizedFlowDataLoginTerms(
//                   policyName = it.key,
//                   localizedUrl = url,
//                   localizedName = name,
//                   version = tos[VERSION] as? String
//               )
//           }
//       }?.filterNotNull().orEmpty()
//   }

std::vector<LocalizedTerms> TermsResponse::getLocalizedTerms(
    const std::string& userLanguage,
    const std::string& defaultLanguage
) const {
    std::vector<LocalizedTerms> result;

    for (const auto& policyPair : policies) {
        const auto& policyName = policyPair.first;
        const auto& langMap = policyPair.second;
        LocalizedTerms lt;
        lt.policyName = policyName;

        // Find version
        auto vit = langMap.find("version");
        if (vit != langMap.end()) {
            auto verIt = vit->second.begin();
            // Version is sometimes stored as string value directly
            // In the JSON: "version": "1.2" — this maps to {"version": {"": "1.2"}}
            // Actually, in raw JSON, version is a direct string, not nested
            // We handle this below in parsing
        }

        // Original Kotlin: tos[userLanguage] ?: tos[defaultLanguage]
        const auto* langData = [&]() -> const std::unordered_map<std::string, std::string>* {
            auto it = langMap.find(userLanguage);
            if (it != langMap.end()) return &it->second;
            it = langMap.find(defaultLanguage);
            if (it != langMap.end()) return &it->second;
            // Fallback: first non-version entry
            for (const auto& langEntry : langMap) {
                if (langEntry.first != "version") return &langEntry.second;
            }
            return nullptr;
        }();

        if (langData) {
            auto nameIt = langData->find("name");
            if (nameIt != langData->end()) lt.localizedName = nameIt->second;
            auto urlIt = langData->find("url");
            if (urlIt != langData->end()) lt.localizedUrl = urlIt->second;
        }

        // Version
        auto verIt = langMap.find("version");
        if (verIt != langMap.end()) {
            // Version is stored as "version": {"": "1.2"} from our parser
            // or could be a single key-value
            if (!verIt->second.empty()) {
                lt.version = verIt->second.begin()->second;
            }
        }

        result.push_back(lt);
    }

    return result;
}

// ==== JSON Parsing ====
//
// Manual brace-counting JSON parser for TermsResponse.
// JSON format:
//   {"policies": {
//       "terms_of_service": {
//           "version": "1.2",
//           "en": {"name": "Terms of Service", "url": "https://..."},
//           "fr": {"name": "Conditions", "url": "https://..."}
//       }
//   }}

static std::string extractJsonValue(const std::string& json, const std::string& key) {
    auto pos = json.find("\"" + key + "\"");
    if (pos == std::string::npos) return "";
    pos = json.find(':', pos);
    if (pos == std::string::npos) return "";
    pos++;
    // Skip whitespace
    while (pos < json.size() && (json[pos] == ' ' || json[pos] == '\t' || json[pos] == '\n')) pos++;
    if (pos >= json.size()) return "";

    if (json[pos] == '{') {
        // Extract brace-delimited object
        int depth = 1;
        size_t start = pos;
        pos++;
        while (pos < json.size() && depth > 0) {
            if (json[pos] == '{') depth++;
            else if (json[pos] == '}') depth--;
            pos++;
        }
        return json.substr(start, pos - start);
    }

    if (json[pos] == '"') {
        pos++;
        size_t end = pos;
        while (end < json.size() && json[end] != '"') {
            if (json[end] == '\\') end++;
            end++;
        }
        return json.substr(pos - 1, end - pos + 2); // include quotes
    }

    // Number or other — extract until comma/brace
    size_t end = pos;
    while (end < json.size() && json[end] != ',' && json[end] != '}') end++;
    return json.substr(pos, end - pos);
}

TermsResponse parseTermsResponse(const std::string& json) {
    // Original Kotlin: termsAPI.getTerms(url) returns TermsResponse
    TermsResponse response;

    // Find "policies" key and extract its object
    auto policiesObj = extractJsonValue(json, "policies");
    if (policiesObj.empty() || policiesObj[0] != '{') return response;

    // Parse each policy:
    // "policyName": { "version": "1.2", "en": {...}, "fr": {...} }
    size_t pos = 1; // skip leading '{'
    while (pos < policiesObj.size()) {
        // Skip whitespace
        while (pos < policiesObj.size() && (policiesObj[pos] == ' ' || policiesObj[pos] == '\t' || policiesObj[pos] == '\n' || policiesObj[pos] == ',')) pos++;
        if (pos >= policiesObj.size() || policiesObj[pos] == '}') break;

        // Extract policy name: "key"
        if (policiesObj[pos] == '"') {
            pos++;
            size_t nameStart = pos;
            while (pos < policiesObj.size() && policiesObj[pos] != '"') pos++;
            std::string policyName = policiesObj.substr(nameStart, pos - nameStart);
            pos++; // skip closing quote

            // Skip ':'
            while (pos < policiesObj.size() && policiesObj[pos] != ':') pos++;
            pos++; // skip ':'

            // Skip whitespace
            while (pos < policiesObj.size() && (policiesObj[pos] == ' ' || policiesObj[pos] == '\t' || policiesObj[pos] == '\n')) pos++;

            // Extract the policy object: { ... }
            if (pos < policiesObj.size() && policiesObj[pos] == '{') {
                int depth = 1;
                size_t objStart = pos;
                pos++;
                while (pos < policiesObj.size() && depth > 0) {
                    if (policiesObj[pos] == '{') depth++;
                    else if (policiesObj[pos] == '}') depth--;
                    pos++;
                }
                std::string policyObj = policiesObj.substr(objStart, pos - objStart);

                // Parse inside the policy object: "version", "en", "fr", etc.
                std::unordered_map<std::string, std::unordered_map<std::string, std::string>> langMap;
                size_t inner = 1;
                while (inner < policyObj.size()) {
                    while (inner < policyObj.size() && (policyObj[inner] == ' ' || policyObj[inner] == '\t' || policyObj[inner] == '\n' || policyObj[inner] == ',')) inner++;
                    if (inner >= policyObj.size() || policyObj[inner] == '}') break;

                    if (policyObj[inner] == '"') {
                        inner++;
                        size_t keyStart = inner;
                        while (inner < policyObj.size() && policyObj[inner] != '"') inner++;
                        std::string innerKey = policyObj.substr(keyStart, inner - keyStart);
                        inner++; // skip closing quote

                        // Skip ':'
                        while (inner < policyObj.size() && policyObj[inner] != ':') inner++;
                        inner++;

                        // Skip whitespace
                        while (inner < policyObj.size() && (policyObj[inner] == ' ' || policyObj[inner] == '\t' || policyObj[inner] == '\n')) inner++;

                        // Extract value
                        if (inner < policyObj.size()) {
                            if (policyObj[inner] == '{') {
                                // Nested object: {"name": "...", "url": "..."}
                                int nd = 1;
                                size_t noStart = inner;
                                inner++;
                                while (inner < policyObj.size() && nd > 0) {
                                    if (policyObj[inner] == '{') nd++;
                                    else if (policyObj[inner] == '}') nd--;
                                    inner++;
                                }
                                std::string nestedObj = policyObj.substr(noStart, inner - noStart);

                                // Parse name/url from nested object
                                std::unordered_map<std::string, std::string> props;
                                size_t np = 1;
                                while (np < nestedObj.size()) {
                                    while (np < nestedObj.size() && (nestedObj[np] == ' ' || nestedObj[np] == '\t' || nestedObj[np] == '\n' || nestedObj[np] == ',')) np++;
                                    if (np >= nestedObj.size() || nestedObj[np] == '}') break;
                                    if (nestedObj[np] == '"') {
                                        np++;
                                        size_t pkStart = np;
                                        while (np < nestedObj.size() && nestedObj[np] != '"') np++;
                                        std::string propKey = nestedObj.substr(pkStart, np - pkStart);
                                        np++;
                                        while (np < nestedObj.size() && nestedObj[np] != ':') np++;
                                        np++;
                                        while (np < nestedObj.size() && (nestedObj[np] == ' ' || nestedObj[np] == '\t')) np++;
                                        if (np < nestedObj.size() && nestedObj[np] == '"') {
                                            np++;
                                            size_t pvStart = np;
                                            while (np < nestedObj.size() && nestedObj[np] != '"') {
                                                if (nestedObj[np] == '\\') np++;
                                                np++;
                                            }
                                            std::string propVal = nestedObj.substr(pvStart, np - pvStart);
                                            np++;
                                            props[propKey] = propVal;
                                        }
                                    }
                                }
                                langMap[innerKey] = props;
                            } else if (policyObj[inner] == '"') {
                                // String value (e.g., "version": "1.2")
                                inner++;
                                size_t svStart = inner;
                                while (inner < policyObj.size() && policyObj[inner] != '"') {
                                    if (policyObj[inner] == '\\') inner++;
                                    inner++;
                                }
                                std::string strVal = policyObj.substr(svStart, inner - svStart);
                                inner++;
                                std::unordered_map<std::string, std::string> sv;
                                sv[""] = strVal;
                                langMap[innerKey] = sv;
                            } else {
                                // Number or other
                                size_t nvStart = inner;
                                while (inner < policyObj.size() && policyObj[inner] != ',' && policyObj[inner] != '}') inner++;
                                std::string numVal = policyObj.substr(nvStart, inner - nvStart);
                                std::unordered_map<std::string, std::string> nv;
                                nv[""] = numVal;
                                langMap[innerKey] = nv;
                            }
                        }
                    }
                }

                response.policies[policyName] = langMap;
            }
        }
    }

    return response;
}

std::string termsResponseToJson(const TermsResponse& response) {
    // Original Kotlin: Moshi serialization of TermsResponse
    std::string json = "{\"policies\":{";
    bool firstPolicy = true;
    for (const auto& policyPair : response.policies) {
        const auto& policyName = policyPair.first;
        const auto& langMap = policyPair.second;
        if (!firstPolicy) json += ",";
        firstPolicy = false;
        json += "\"" + policyName + "\":{";
        bool firstLang = true;
        for (const auto& langPair : langMap) {
            const auto& langKey = langPair.first;
            const auto& props = langPair.second;
            if (!firstLang) json += ",";
            firstLang = false;
            if (langKey == "version" && props.count("") > 0) {
                json += "\"version\":\"" + props.at("") + "\"";
            } else {
                json += "\"" + langKey + "\":{";
                bool firstProp = true;
                for (const auto& propPair : props) {
                    const auto& propKey = propPair.first;
                    const auto& propVal = propPair.second;
                    if (!firstProp) json += ",";
                    firstProp = false;
                    json += "\"" + propKey + "\":\"" + propVal + "\"";
                }
                json += "}";
            }
        }
        json += "}";
    }
    json += "}}";
    return json;
}

std::string acceptTermsBodyToJson(const AcceptTermsBody& body) {
    // Original Kotlin: Moshi serialization of AcceptTermsBody
    // JSON: {"user_accepts": ["url1", "url2"]}
    std::string json = "{\"user_accepts\":[";
    bool first = true;
    for (const auto& url : body.acceptedUrls) {
        if (!first) json += ",";
        first = false;
        json += "\"" + url + "\"";
    }
    json += "]}";
    return json;
}

std::vector<LocalizedTerms> getLocalizedTermsWithStatus(
    const TermsResponse& response,
    const std::vector<std::string>& alreadyAcceptedUrls,
    const std::string& userLanguage,
    const std::string& defaultLanguage
) {
    // Original Kotlin (DefaultTermsService.kt:38-42):
    //   val termsResponse = executeRequest { termsAPI.getTerms(url) }
    //   return GetTermsResponse(termsResponse, getAlreadyAcceptedTermUrlsFromAccountData())
    auto terms = response.getLocalizedTerms(userLanguage, defaultLanguage);

    // Mark accepted status
    for (auto& term : terms) {
        term.accepted = std::find(alreadyAcceptedUrls.begin(), alreadyAcceptedUrls.end(),
                                   term.localizedUrl) != alreadyAcceptedUrls.end();
    }

    return terms;
}

} // namespace progressive
