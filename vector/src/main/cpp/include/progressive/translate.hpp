#pragma once

#include <string>
#include <vector>
#include <list>
#include <unordered_map>
#include <cstdint>

namespace progressive {

// ---- Existing types ----

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

std::string buildTranslateRequestBody(const TranslateRequest& request);
TranslateResult parseTranslateResponse(const std::string& responseBody, int httpStatus);

// ---- New types and functions ----

// Original Kotlin: TranslateManager.kt — translation providers
enum class TranslateProvider {
    GOOGLE,
    DEEPL,
    YANDEX,
    MICROSOFT,
    NONE
};

// Original Kotlin: TranslateManager.kt — provider configuration
struct TranslateConfig {
    bool enabled = false;
    TranslateProvider provider = TranslateProvider::NONE;
    std::string sourceLang;  // "" = auto
    std::string targetLang;
    std::string apiKey;      // API key for the provider
    std::string apiUrl;      // Custom API endpoint
};

// Original Kotlin: TranslateManager.kt — text format for translation
enum class TranslateFormat {
    TEXT,
    HTML
};

// Original Kotlin: TranslateManager.kt — translation API request
struct TranslateApiRequest {
    std::string text;
    std::string sourceLang;  // "" = auto
    std::string targetLang;
    TranslateFormat format = TranslateFormat::TEXT;
    std::string model;       // for LLM-based providers
};

// Original Kotlin: TranslateManager.kt — translation response
struct TranslateResponse {
    std::string translatedText;
    std::string sourceLang;    // detected source language
    std::string targetLang;
    std::vector<std::string> alternatives; // alternative translations
};

// Original Kotlin: TranslateManager.kt — JSON request builder for translation APIs
std::string buildTranslateRequest(const TranslateApiRequest& request, TranslateProvider provider);

// Original Kotlin: TranslateManager.kt — manual JSON response parser
TranslateResponse parseTranslateResponse(const std::string& responseBody, TranslateProvider provider);

// Original Kotlin: TranslateManager.kt — build client-side translation settings JSON
std::string buildTranslateSettings(const TranslateConfig& config);

// Original Kotlin: TranslateManager.kt — language detection result
struct LanguageDetection {
    std::string language;     // e.g. "en", "ru"
    double confidence = 0.0;  // 0.0 to 1.0
    bool isReliable = false;  // confidence >= threshold
};

// Original Kotlin: TranslateManager.kt — detect language of text
LanguageDetection detectTextLanguage(const std::string& text);

// Original Kotlin: TranslateManager.kt — language info (code, name, native, direction)
struct LanguageInfo {
    std::string code;        // ISO-639-1, e.g. "en"
    std::string name;        // e.g. "English"
    std::string nativeName;  // e.g. "English", "Русский", "日本語"
    std::string direction;   // "LTR" or "RTL"

    [[nodiscard]] bool isRtl() const { return direction == "RTL"; }
};

// Original Kotlin: TranslateManager.kt — full list of supported languages
std::vector<LanguageInfo> getLanguageList();

// Original Kotlin: TranslateManager.kt — get language name from code (English)
std::string getLanguageName(const std::string& code);

// Original Kotlin: TranslateManager.kt — get native script name from code
std::string getLanguageNativeName(const std::string& code);

// Original Kotlin: TranslateManager.kt — check if language is right-to-left
bool isRtlLanguage(const std::string& code);

// Original Kotlin: TranslateManager.kt — translate a message with caching
// Returns translated text if successful, empty string otherwise.
std::string translateMessage(
    const std::string& text,
    const std::string& sourceLang,
    const std::string& targetLang,
    TranslateProvider provider,
    const std::string& apiKey,
    const std::string& apiUrl);

// Original Kotlin: TranslateManager.kt — LRU cache entry
struct TranslationCacheEntry {
    std::string sourceText;
    std::string sourceLang;
    std::string targetLang;
    std::string translatedText;
    int64_t timestamp = 0; // epoch millis
};

// Original Kotlin: TranslateManager.kt — simple LRU cache for translations
class TranslationCache {
public:
    explicit TranslationCache(size_t maxSize = 256);

    // Look up a cached translation. Returns empty string if not found.
    [[nodiscard]] std::string getCachedTranslation(
        const std::string& sourceText,
        const std::string& sourceLang,
        const std::string& targetLang) const;

    // Store a translation in the cache.
    void putCachedTranslation(
        const std::string& sourceText,
        const std::string& sourceLang,
        const std::string& targetLang,
        const std::string& translatedText);

    // Remove all entries.
    void clear();

    [[nodiscard]] size_t size() const { return list_.size(); }

private:
    size_t maxSize_;
    mutable std::list<TranslationCacheEntry> list_;
    // key = sourceText + "|" + sourceLang + "|" + targetLang
    mutable std::unordered_map<std::string, decltype(list_.begin())> map_;

    static std::string makeKey(
        const std::string& sourceText,
        const std::string& sourceLang,
        const std::string& targetLang);

    void touch(const std::string& key) const;
};

} // namespace progressive
