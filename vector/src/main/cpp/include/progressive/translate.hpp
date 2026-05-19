#ifndef PROGRESSIVE_TRANSLATE_HPP
#define PROGRESSIVE_TRANSLATE_HPP

#include <string>

namespace progressive {

struct TranslateRequest {
    std::string text;
    std::string sourceLanguage;  // "" for auto-detect
    std::string targetLanguage;  // e.g. "en", "ru"
    std::string apiEndpoint;     // e.g. "https://api.openai.com/v1/chat/completions"
    std::string apiToken;
    std::string model;           // e.g. "gpt-4o-mini"
};

struct TranslateResult {
    bool success = false;
    std::string translatedText;
    std::string errorMessage;
    int statusCode = 0;
};

// Build JSON request body for OpenAI-compatible translation
std::string buildTranslateRequestBody(const TranslateRequest& request);

// Parse the OpenAI-compatible response to extract translated text
TranslateResult parseTranslateResponse(const std::string& responseBody, int httpStatus);

} // namespace progressive

#endif // PROGRESSIVE_TRANSLATE_HPP
