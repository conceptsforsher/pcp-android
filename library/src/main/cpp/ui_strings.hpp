#pragma once

/**
 * ui_strings.hpp - Localized UI string system for PCP chat client.
 *
 * Gettext-style string lookup with support for 15+ languages, plural forms,
 * format strings, and fallback chains. Language files can be loaded from JSON
 * or from embedded defaults.
 */

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

namespace pcp {
namespace library {

/**
 * Supported language codes (ISO 639-1 + optional region).
 * At least 15 languages are supported with embedded defaults.
 */
enum class Language {
    EN,       // English
    ES,       // Spanish
    FR,       // French
    DE,       // German
    IT,       // Italian
    PT,       // Portuguese
    RU,       // Russian
    ZH,       // Chinese (Simplified)
    JA,       // Japanese
    KO,       // Korean
    AR,       // Arabic
    HI,       // Hindi
    NL,       // Dutch
    PL,       // Polish
    TR,       // Turkish
    UK,       // Ukrainian
    SV,       // Swedish
    CUSTOM    // Custom/unknown language loaded from external source
};

/**
 * Plural rule types matching common language families.
 * Used by getPlural() and formatPlural() to select the right form.
 */
enum class PluralRule {
    ONE_OTHER,           // English, Spanish, German, Italian, Dutch, Swedish
    ONE_FEW_OTHER,       // Russian, Ukrainian, Polish
    ONE_TWO_OTHER,       // Arabic
    ONE_OTHER_ZERO,      // Portuguese
    OTHER,               // Chinese, Japanese, Korean, Turkish
    ONE_MANY_OTHER       // Hindi
};

/**
 * Manages localized UI strings with gettext-style lookups.
 *
 * Supports at least 15 languages with embedded default translations
 * for common UI strings. External JSON language files can be loaded
 * to extend or override translations.
 *
 * Usage:
 *   UIStrings::instance().setLanguage(Language::DE);
 *   std::string cancel = UIStrings::instance().getString("cancel");
 *   std::string msg = UIStrings::instance().formatString("welcome", "Alice");
 */
class UIStrings {
public:
    /** Get the global singleton instance. */
    static UIStrings& instance();

    /**
     * Set the active language. All subsequent getString() calls
     * will return translations in this language.
     * Falls back to English if the requested language has no loaded data.
     */
    void setLanguage(Language lang);
    void setLanguage(const std::string& langCode);

    /**
     * Retrieve a localized string by key.
     * If the key is not found, returns the key itself as fallback.
     * Keys are typically lowercase underscore-separated (e.g. "btn_cancel").
     */
    std::string getString(const std::string& key) const;

    /**
     * Check whether a key has a translation in the current language.
     */
    bool hasString(const std::string& key) const;

    /**
     * Format a string with positional arguments.
     * Placeholders are {0}, {1}, {2}, etc.
     * Example: formatString("welcome_back", "Alice") -> "Welcome back, Alice!"
     */
    template<typename... Args>
    std::string formatString(const std::string& key, Args&&... args) const {
        std::string tmpl = getString(key);
        return formatImpl(tmpl, 0, std::forward<Args>(args)...);
    }

    /** Get the current language. */
    Language getLanguage() const;
    std::string getLanguageCode() const;

    /** Get the list of available language codes. */
    std::vector<std::string> getAvailableLanguages() const;

    /**
     * Load translations from a JSON string.
     * Expected format: { "lang_code": { "key": "value", ... }, ... }
     * Multiple languages can be loaded at once.
     */
    bool loadStrings(const std::string& jsonData);

    /**
     * Load translations for a single language from a flat JSON object.
     * Expected format: { "key": "value", ... }
     */
    bool loadStringsForLanguage(const std::string& langCode,
                                 const std::string& jsonData);

    /**
     * Get the correct plural form index for the given count.
     * Languages have different plural rules:
     *   English (PluralRule::ONE_OTHER):  0 if count==1, else 1
     *   Russian (PluralRule::ONE_FEW_OTHER): complex rules
     *   Chinese (PluralRule::OTHER):     always 0
     *   Arabic (PluralRule::ONE_TWO_OTHER): 6 forms
     */
    int getPluralIndex(int count) const;

    /**
     * Get a pluralized string. Keys are like "messages" with variants
     * "messages_0", "messages_1", "messages_2", etc.
     * Falls back to the base key if plural variants don't exist.
     */
    std::string getPlural(const std::string& key, int count) const;

    /**
     * Format a pluralized string with arguments.
     */
    template<typename... Args>
    std::string formatPlural(const std::string& key, int count, Args&&... args) const {
        std::string tmpl = getPlural(key, count);
        return formatPluralImpl(count, tmpl, 0, std::forward<Args>(args)...);
    }

    /** Get the plural rule for the current language. */
    PluralRule getPluralRule() const;

    /** Convert language enum to ISO 639-1 code. */
    static std::string languageToCode(Language lang);

    /** Convert ISO 639-1 code to language enum. Returns CUSTOM if unknown. */
    static Language codeToLanguage(const std::string& code);

    /** Get the native name of a language (e.g., "Deutsch" for German). */
    static std::string getLanguageNativeName(Language lang);

private:
    UIStrings();
    ~UIStrings() = default;
    UIStrings(const UIStrings&) = delete;
    UIStrings& operator=(const UIStrings&) = delete;

    void loadEmbeddedDefaults();

    // Format helper: recursive variadic expansion
    template<typename T, typename... Rest>
    std::string formatImpl(const std::string& tmpl, int index, T&& arg, Rest&&... rest) const {
        std::string placeholder = "{" + std::to_string(index) + "}";
        std::string result = tmpl;
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.size(), toString(arg));
            pos += toString(arg).size();
        }
        return formatImpl(result, index + 1, std::forward<Rest>(rest)...);
    }

    std::string formatImpl(const std::string& tmpl, int /*index*/) const {
        return tmpl;
    }

    template<typename T, typename... Rest>
    std::string formatPluralImpl(int count, const std::string& tmpl, int index,
                                   T&& arg, Rest&&... rest) const {
        (void)count; // count is used by callers; forwarded for potential override
        return formatImpl(tmpl, index, std::forward<T>(arg), std::forward<Rest>(rest)...);
    }

    std::string formatPluralImpl(int /*count*/, const std::string& tmpl, int /*index*/) const {
        return tmpl;
    }

    template<typename T>
    static std::string toString(const T& val) {
        if constexpr (std::is_same_v<T, std::string>) return val;
        else if constexpr (std::is_same_v<T, const char*>) return std::string(val);
        else if constexpr (std::is_floating_point_v<T>) return std::to_string(val);
        else return std::to_string(val);
    }

    Language currentLanguage_ = Language::EN;
    PluralRule currentPluralRule_ = PluralRule::ONE_OTHER;

    // lang_code -> (key -> value)
    std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations_;

    /** Get plural rule for a language code */
    static PluralRule getPluralRuleForLanguage(const std::string& langCode);
};

} // namespace library
} // namespace pcp
