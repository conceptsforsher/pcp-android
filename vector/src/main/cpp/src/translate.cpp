#include "progressive/translate.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <ctime>
#include <unordered_set>
#include <unordered_map>

namespace progressive {

// ============================================================================
// Existing: OpenAI-compatible translation
// ============================================================================

std::string buildTranslateRequestBody(const TranslateRequest& request) {
    std::ostringstream json;

    std::string systemPrompt = "You are a translator. Translate the following text";
    if (!request.sourceLanguage.empty()) {
        systemPrompt += " from " + request.sourceLanguage;
    }
    systemPrompt += " to " + request.targetLanguage + ". ";
    systemPrompt += "Output ONLY the translation, nothing else. No quotes, no explanations.";

    auto escape = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) {
            switch (c) {
                case '"':  out += "\\\""; break;
                case '\\': out += "\\\\"; break;
                case '\n': out += "\\n"; break;
                case '\r': out += "\\r"; break;
                case '\t': out += "\\t"; break;
                default:   out += c;
            }
        }
        return out;
    };

    json << "{";
    json << R"("model": ")" << request.model << R"(",)";
    json << R"("messages": [)";
    json << R"({"role": "system", "content": ")" << escape(systemPrompt) << R"("},)";
    json << R"({"role": "user", "content": ")" << escape(request.text) << R"("})";
    json << "],";
    json << R"("temperature": 0.1)";
    json << "}";

    return json.str();
}

TranslateResult parseTranslateResponse(const std::string& responseBody, int httpStatus) {
    TranslateResult result;
    result.statusCode = httpStatus;

    if (httpStatus != 200) {
        result.success = false;
        auto error = parseJsonStringValue(responseBody, "error");
        auto msg = error.empty() ? "Server returned " + std::to_string(httpStatus) : error;
        if (!error.empty()) {
            auto errorMsg = parseJsonStringValue("{" + error + "}", "message");
            if (!errorMsg.empty()) msg = errorMsg;
        }
        result.errorMessage = msg;
        return result;
    }

    auto choices = parseJsonStringValue(responseBody, "choices");
    if (choices.empty()) {
        result.success = false;
        result.errorMessage = "No choices in response.";
        return result;
    }

    auto message = parseJsonStringValue("{" + choices + "}", "message");
    if (message.empty()) {
        result.success = false;
        result.errorMessage = "No message in response.";
        return result;
    }

    auto content = parseJsonStringValue("{" + message + "}", "content");
    if (content.empty()) {
        result.success = false;
        result.errorMessage = "No content in response.";
        return result;
    }

    result.success = true;
    result.translatedText = content;
    return result;
}

// ============================================================================
// JSON helper: extract string value for key (manual, no library)
// ============================================================================

static std::string jsonGetString(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\"";
    size_t keyPos = json.find(search);
    if (keyPos == std::string::npos) return {};

    size_t colonPos = json.find(':', keyPos + search.size());
    if (colonPos == std::string::npos) return {};

    size_t openQuote = json.find('"', colonPos + 1);
    if (openQuote == std::string::npos) return {};

    size_t closeQuote = json.find('"', openQuote + 1);
    if (closeQuote == std::string::npos) return {};

    return json.substr(openQuote + 1, closeQuote - openQuote - 1);
}

static std::string jsonEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) {
            case '"': out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default: out += c;
        }
    }
    return out;
}

// ============================================================================
// New: buildTranslateRequest (generic, per provider)
// ============================================================================

// Original Kotlin: TranslateManager.kt — JSON request body builder
std::string buildTranslateRequest(const TranslateApiRequest& request, TranslateProvider provider) {
    std::ostringstream json;

    switch (provider) {
        case TranslateProvider::GOOGLE:
            // Google Cloud Translation API v3
            json << "{";
            json << "\"q\": [\"" << jsonEscape(request.text) << "\"],";
            json << "\"target\": \"" << jsonEscape(request.targetLang) << "\",";
            if (!request.sourceLang.empty())
                json << "\"source\": \"" << jsonEscape(request.sourceLang) << "\",";
            json << "\"format\": \"" << (request.format == TranslateFormat::HTML ? "html" : "text") << "\"";
            json << "}";
            break;

        case TranslateProvider::DEEPL:
            // DeepL API
            json << "{";
            json << "\"text\": [\"" << jsonEscape(request.text) << "\"],";
            json << "\"target_lang\": \"" << jsonEscape(request.targetLang) << "\",";
            if (!request.sourceLang.empty())
                json << "\"source_lang\": \"" << jsonEscape(request.sourceLang) << "\",";
            json << "\"preserve_formatting\": " << (request.format == TranslateFormat::HTML ? "true" : "false");
            json << "}";
            break;

        case TranslateProvider::YANDEX:
            // Yandex Translate API
            json << "{";
            json << "\"texts\": [\"" << jsonEscape(request.text) << "\"],";
            json << "\"targetLanguageCode\": \"" << jsonEscape(request.targetLang) << "\",";
            if (!request.sourceLang.empty())
                json << "\"sourceLanguageCode\": \"" << jsonEscape(request.sourceLang) << "\"";
            json << "}";
            break;

        case TranslateProvider::MICROSOFT:
            // Microsoft Translator Text API
            json << "[{";
            json << "\"Text\": \"" << jsonEscape(request.text) << "\"";
            json << "}]";
            break;

        case TranslateProvider::NONE:
        default:
            json << "{}";
            break;
    }

    return json.str();
}

// ============================================================================
// New: parseTranslateResponse (generic, per provider)
// ============================================================================

// Original Kotlin: TranslateManager.kt — manual response parser
TranslateResponse parseTranslateResponse(const std::string& responseBody, TranslateProvider provider) {
    TranslateResponse result;

    switch (provider) {
        case TranslateProvider::GOOGLE: {
            // Google returns: { "data": { "translations": [{ "translatedText": "...", "detectedSourceLanguage": "..." }] } }
            auto data = jsonGetString(responseBody, "data");
            if (!data.empty()) {
                std::string wrapped = "{" + data + "}";
                auto transArr = jsonGetString(wrapped, "translations");
                if (!transArr.empty()) {
                    std::string wrapped2 = "{" + transArr + "}";
                    result.translatedText = jsonGetString(wrapped2, "translatedText");
                    result.sourceLang = jsonGetString(wrapped2, "detectedSourceLanguage");
                }
            }
            break;
        }
        case TranslateProvider::DEEPL: {
            // DeepL returns: { "translations": [{ "text": "...", "detected_source_language": "..." }] }
            result.translatedText = jsonGetString(responseBody, "text");
            result.sourceLang = jsonGetString(responseBody, "detected_source_language");
            break;
        }
        case TranslateProvider::YANDEX: {
            // Yandex returns: { "translations": [{ "text": "...", "detectedLanguageCode": "..." }] }
            auto transArr = jsonGetString(responseBody, "translations");
            if (!transArr.empty()) {
                std::string wrapped = "{" + transArr + "}";
                result.translatedText = jsonGetString(wrapped, "text");
                result.sourceLang = jsonGetString(wrapped, "detectedLanguageCode");
            }
            break;
        }
        case TranslateProvider::MICROSOFT: {
            // Microsoft returns: [{ "translations": [{ "text": "...", "to": "..." }] }]
            // Simplified: extract first "text" value
            auto transArr = jsonGetString(responseBody, "translations");
            if (!transArr.empty()) {
                std::string wrapped = "{" + transArr + "}";
                result.translatedText = jsonGetString(wrapped, "text");
                result.sourceLang = jsonGetString(wrapped, "from");
            }
            break;
        }
        case TranslateProvider::NONE:
        default:
            break;
    }

    return result;
}

// ============================================================================
// New: buildTranslateSettings
// ============================================================================

// Original Kotlin: TranslateManager.kt — client-side translation settings JSON
std::string buildTranslateSettings(const TranslateConfig& config) {
    std::ostringstream json;

    const char* providerStr = "none";
    switch (config.provider) {
        case TranslateProvider::GOOGLE:    providerStr = "google";    break;
        case TranslateProvider::DEEPL:     providerStr = "deepl";     break;
        case TranslateProvider::YANDEX:    providerStr = "yandex";    break;
        case TranslateProvider::MICROSOFT: providerStr = "microsoft"; break;
        case TranslateProvider::NONE:      providerStr = "none";      break;
    }

    json << "{";
    json << "\"enabled\": " << (config.enabled ? "true" : "false") << ",";
    json << "\"provider\": \"" << providerStr << "\",";
    json << "\"sourceLang\": \"" << jsonEscape(config.sourceLang) << "\",";
    json << "\"targetLang\": \"" << jsonEscape(config.targetLang) << "\",";
    json << "\"apiUrl\": \"" << jsonEscape(config.apiUrl) << "\"";
    json << "}";

    return json.str();
}

// ============================================================================
// New: detectTextLanguage
// ============================================================================

// Original Kotlin: TranslateManager.kt — identify text language by character set and common words
LanguageDetection detectTextLanguage(const std::string& text) {
    LanguageDetection result;
    if (text.empty()) {
        result.language = "en";
        result.confidence = 1.0;
        result.isReliable = true;
        return result;
    }

    struct Script {
        std::string name;
        std::pair<uint32_t, uint32_t> range; // Unicode range
        std::string lang;
    };

    static const std::vector<Script> scripts = {
        {"Cyrillic",      {0x0400, 0x04FF}, "ru"},
        {"CJK",           {0x4E00, 0x9FFF}, "zh"},
        {"Hiragana",      {0x3040, 0x309F}, "ja"},
        {"Katakana",      {0x30A0, 0x30FF}, "ja"},
        {"Hangul",        {0xAC00, 0xD7AF}, "ko"},
        {"Arabic",        {0x0600, 0x06FF}, "ar"},
        {"Hebrew",        {0x0590, 0x05FF}, "he"},
        {"Thai",          {0x0E00, 0x0E7F}, "th"},
        {"Devanagari",    {0x0900, 0x097F}, "hi"},
        {"Greek",         {0x0370, 0x03FF}, "el"},
    };

    // Count non-Latin script characters
    std::string bestScript;
    int bestCount = 0;

    for (const auto& script : scripts) {
        int count = 0;
        for (size_t i = 0; i < text.size(); ) {
            uint32_t cp = static_cast<unsigned char>(text[i]);
            if (cp >= 0x80 && i + 1 < text.size()) {
                cp = ((cp & 0x1F) << 6) | (static_cast<unsigned char>(text[i + 1]) & 0x3F);
                if (cp >= 0x80 && cp < 0x800) {
                    i += 2;
                } else if (i + 2 < text.size()) {
                    cp = (cp << 6) | (static_cast<unsigned char>(text[i + 2]) & 0x3F);
                    if (cp >= 0x800 && cp < 0x10000) {
                        i += 3;
                    } else if (i + 3 < text.size()) {
                        cp = (cp << 6) | (static_cast<unsigned char>(text[i + 3]) & 0x3F);
                        i += 4;
                    } else {
                        i++;
                        continue;
                    }
                } else {
                    i++;
                    continue;
                }
            } else {
                i++;
            }
            if (cp >= script.range.first && cp <= script.range.second) {
                count++;
            }
        }
        if (count > bestCount) {
            bestCount = count;
            bestScript = script.lang;
        }
    }

    // Stop-word based detection for Latin-script languages
    if (bestScript.empty() || bestCount < 5) {
        struct LangSig {
            std::string code;
            std::vector<std::string> sw;
        };
        static const std::vector<LangSig> sigs = {
            {"en", {"the", "is", "are", "of", "and", "to", "in", "for", "on", "this", "that", "it", "with", "as", "be", "have", "not", "but", "or", "you", "he", "she", "they"}},
            {"fr", {"le", "la", "les", "est", "sont", "dans", "pour", "avec", "que", "qui", "une", "pas", "des", "du", "sur", "nous", "vous", "ils", "elles", "fait"}},
            {"de", {"der", "die", "das", "ist", "sind", "und", "für", "mit", "auf", "ein", "eine", "nicht", "ich", "sie", "wir", "von", "sich", "auch", "wird", "dass"}},
            {"es", {"que", "los", "las", "del", "para", "con", "una", "por", "como", "más", "pero", "sus", "este", "entre", "está", "cuando", "muy", "sin", "sobre"}},
            {"it", {"che", "una", "sono", "con", "della", "nel", "nella", "degli", "anche", "più", "come", "gli", "tra", "dopo", "quando", "senza", "sopra", "tutto"}},
            {"pt", {"que", "não", "com", "uma", "para", "como", "mais", "seu", "das", "ele", "ela", "pelo", "aos", "entre", "depois", "quando", "muito", "sem", "sobre"}},
            {"nl", {"een", "voor", "zijn", "niet", "ook", "wordt", "het", "dat", "die", "deze", "maar", "door", "over", "tussen", "naar", "andere", "veel", "zonder"}},
            {"pl", {"nie", "się", "jest", "dla", "jego", "także", "może", "przed", "podczas", "oraz", "które", "bardzo", "bez", "przez", "między", "więc", "gdy", "już"}},
            {"tr", {"bir", "için", "ile", "değil", "kadar", "çok", "daha", "diğer", "sonra", "ancak", "gibi", "bile", "veya", "nasıl", "önce", "şimdi", "böyle"}},
        };

        // Lowercase and tokenize
        std::vector<std::string> words;
        {
            std::string cur;
            for (size_t i = 0; i < text.size(); ++i) {
                char c = static_cast<char>(std::tolower(static_cast<unsigned char>(text[i])));
                if (std::isalpha(static_cast<unsigned char>(c))) {
                    cur += c;
                } else {
                    if (!cur.empty()) { words.push_back(cur); cur.clear(); }
                }
            }
            if (!cur.empty()) words.push_back(cur);
        }

        int bestScore = 0;
        std::string bestCode = "en";
        for (const auto& sig : sigs) {
            int score = 0;
            for (const auto& w : words) {
                for (const auto& s : sig.sw) {
                    if (w == s) { score++; break; }
                }
            }
            if (score > bestScore) {
                bestScore = score;
                bestCode = sig.code;
            }
        }
        bestScript = bestCode;
        if (bestScore > 0) {
            result.confidence = std::min(1.0, bestScore / 4.0);
            result.isReliable = result.confidence >= 0.5;
        } else {
            result.confidence = 0.3;
            result.isReliable = false;
        }
    } else {
        result.confidence = 0.9;
        result.isReliable = true;
    }

    result.language = bestScript;
    return result;
}

// ============================================================================
// New: Language list and helpers
// ============================================================================

// Original Kotlin: TranslateManager.kt — all supported languages
std::vector<LanguageInfo> getLanguageList() {
    return {
        {"af", "Afrikaans", "Afrikaans", "LTR"},
        {"sq", "Albanian", "Shqip", "LTR"},
        {"am", "Amharic", "አማርኛ", "LTR"},
        {"ar", "Arabic", "العربية", "RTL"},
        {"hy", "Armenian", "Հայերեն", "LTR"},
        {"az", "Azerbaijani", "Azərbaycan", "LTR"},
        {"eu", "Basque", "Euskara", "LTR"},
        {"be", "Belarusian", "Беларуская", "LTR"},
        {"bn", "Bengali", "বাংলা", "LTR"},
        {"bs", "Bosnian", "Bosanski", "LTR"},
        {"bg", "Bulgarian", "Български", "LTR"},
        {"ca", "Catalan", "Català", "LTR"},
        {"ceb", "Cebuano", "Cebuano", "LTR"},
        {"ny", "Chichewa", "Chichewa", "LTR"},
        {"zh", "Chinese (Simplified)", "简体中文", "LTR"},
        {"zh_tw", "Chinese (Traditional)", "繁體中文", "LTR"},
        {"co", "Corsican", "Corsu", "LTR"},
        {"hr", "Croatian", "Hrvatski", "LTR"},
        {"cs", "Czech", "Čeština", "LTR"},
        {"da", "Danish", "Dansk", "LTR"},
        {"nl", "Dutch", "Nederlands", "LTR"},
        {"en", "English", "English", "LTR"},
        {"eo", "Esperanto", "Esperanto", "LTR"},
        {"et", "Estonian", "Eesti", "LTR"},
        {"tl", "Filipino", "Filipino", "LTR"},
        {"fi", "Finnish", "Suomi", "LTR"},
        {"fr", "French", "Français", "LTR"},
        {"fy", "Frisian", "Frysk", "LTR"},
        {"gl", "Galician", "Galego", "LTR"},
        {"ka", "Georgian", "ქართული", "LTR"},
        {"de", "German", "Deutsch", "LTR"},
        {"el", "Greek", "Ελληνικά", "LTR"},
        {"gu", "Gujarati", "ગુજરાતી", "LTR"},
        {"ht", "Haitian Creole", "Kreyòl Ayisyen", "LTR"},
        {"ha", "Hausa", "Hausa", "LTR"},
        {"haw", "Hawaiian", "ʻŌlelo Hawaiʻi", "LTR"},
        {"he", "Hebrew", "עברית", "RTL"},
        {"hi", "Hindi", "हिन्दी", "LTR"},
        {"hmn", "Hmong", "Hmoob", "LTR"},
        {"hu", "Hungarian", "Magyar", "LTR"},
        {"is", "Icelandic", "Íslenska", "LTR"},
        {"ig", "Igbo", "Igbo", "LTR"},
        {"id", "Indonesian", "Bahasa Indonesia", "LTR"},
        {"ga", "Irish", "Gaeilge", "LTR"},
        {"it", "Italian", "Italiano", "LTR"},
        {"ja", "Japanese", "日本語", "LTR"},
        {"jw", "Javanese", "Basa Jawa", "LTR"},
        {"kn", "Kannada", "ಕನ್ನಡ", "LTR"},
        {"kk", "Kazakh", "Қазақша", "LTR"},
        {"km", "Khmer", "ភាសាខ្មែរ", "LTR"},
        {"ko", "Korean", "한국어", "LTR"},
        {"ku", "Kurdish", "Kurdî", "LTR"},
        {"ky", "Kyrgyz", "Кыргызча", "LTR"},
        {"lo", "Lao", "ລາວ", "LTR"},
        {"la", "Latin", "Latina", "LTR"},
        {"lv", "Latvian", "Latviešu", "LTR"},
        {"lt", "Lithuanian", "Lietuvių", "LTR"},
        {"lb", "Luxembourgish", "Lëtzebuergesch", "LTR"},
        {"mk", "Macedonian", "Македонски", "LTR"},
        {"mg", "Malagasy", "Malagasy", "LTR"},
        {"ms", "Malay", "Bahasa Melayu", "LTR"},
        {"ml", "Malayalam", "മലയാളം", "LTR"},
        {"mt", "Maltese", "Malti", "LTR"},
        {"mi", "Maori", "Māori", "LTR"},
        {"mr", "Marathi", "मराठी", "LTR"},
        {"mn", "Mongolian", "Монгол", "LTR"},
        {"my", "Myanmar (Burmese)", "မြန်မာ", "LTR"},
        {"ne", "Nepali", "नेपाली", "LTR"},
        {"no", "Norwegian", "Norsk", "LTR"},
        {"ps", "Pashto", "پښتو", "RTL"},
        {"fa", "Persian", "فارسی", "RTL"},
        {"pl", "Polish", "Polski", "LTR"},
        {"pt", "Portuguese", "Português", "LTR"},
        {"pa", "Punjabi", "ਪੰਜਾਬੀ", "LTR"},
        {"ro", "Romanian", "Română", "LTR"},
        {"ru", "Russian", "Русский", "LTR"},
        {"sm", "Samoan", "Gagana Samoa", "LTR"},
        {"gd", "Scots Gaelic", "Gàidhlig", "LTR"},
        {"sr", "Serbian", "Српски", "LTR"},
        {"st", "Sesotho", "Sesotho", "LTR"},
        {"sn", "Shona", "chiShona", "LTR"},
        {"sd", "Sindhi", "سنڌي", "RTL"},
        {"si", "Sinhala", "සිංහල", "LTR"},
        {"sk", "Slovak", "Slovenčina", "LTR"},
        {"sl", "Slovenian", "Slovenščina", "LTR"},
        {"so", "Somali", "Soomaali", "LTR"},
        {"es", "Spanish", "Español", "LTR"},
        {"su", "Sundanese", "Basa Sunda", "LTR"},
        {"sw", "Swahili", "Kiswahili", "LTR"},
        {"sv", "Swedish", "Svenska", "LTR"},
        {"tg", "Tajik", "Тоҷикӣ", "LTR"},
        {"ta", "Tamil", "தமிழ்", "LTR"},
        {"te", "Telugu", "తెలుగు", "LTR"},
        {"th", "Thai", "ไทย", "LTR"},
        {"tr", "Turkish", "Türkçe", "LTR"},
        {"uk", "Ukrainian", "Українська", "LTR"},
        {"ur", "Urdu", "اردو", "RTL"},
        {"ug", "Uyghur", "ئۇيغۇرچە", "RTL"},
        {"uz", "Uzbek", "Oʻzbek", "LTR"},
        {"vi", "Vietnamese", "Tiếng Việt", "LTR"},
        {"cy", "Welsh", "Cymraeg", "LTR"},
        {"xh", "Xhosa", "isiXhosa", "LTR"},
        {"yi", "Yiddish", "ייִדיש", "RTL"},
        {"yo", "Yoruba", "Yorùbá", "LTR"},
        {"zu", "Zulu", "isiZulu", "LTR"},
    };
}

// Original Kotlin: TranslateManager.kt — language name by code
std::string getLanguageName(const std::string& code) {
    static const std::unordered_map<std::string, std::string> names = {
        {"af", "Afrikaans"}, {"sq", "Albanian"}, {"am", "Amharic"},
        {"ar", "Arabic"}, {"hy", "Armenian"}, {"az", "Azerbaijani"},
        {"eu", "Basque"}, {"be", "Belarusian"}, {"bn", "Bengali"},
        {"bs", "Bosnian"}, {"bg", "Bulgarian"}, {"ca", "Catalan"},
        {"ceb", "Cebuano"}, {"ny", "Chichewa"}, {"zh", "Chinese (Simplified)"},
        {"zh_tw", "Chinese (Traditional)"}, {"co", "Corsican"}, {"hr", "Croatian"},
        {"cs", "Czech"}, {"da", "Danish"}, {"nl", "Dutch"},
        {"en", "English"}, {"eo", "Esperanto"}, {"et", "Estonian"},
        {"tl", "Filipino"}, {"fi", "Finnish"}, {"fr", "French"},
        {"fy", "Frisian"}, {"gl", "Galician"}, {"ka", "Georgian"},
        {"de", "German"}, {"el", "Greek"}, {"gu", "Gujarati"},
        {"ht", "Haitian Creole"}, {"ha", "Hausa"}, {"haw", "Hawaiian"},
        {"he", "Hebrew"}, {"hi", "Hindi"}, {"hmn", "Hmong"},
        {"hu", "Hungarian"}, {"is", "Icelandic"}, {"ig", "Igbo"},
        {"id", "Indonesian"}, {"ga", "Irish"}, {"it", "Italian"},
        {"ja", "Japanese"}, {"jw", "Javanese"}, {"kn", "Kannada"},
        {"kk", "Kazakh"}, {"km", "Khmer"}, {"ko", "Korean"},
        {"ku", "Kurdish"}, {"ky", "Kyrgyz"}, {"lo", "Lao"},
        {"la", "Latin"}, {"lv", "Latvian"}, {"lt", "Lithuanian"},
        {"lb", "Luxembourgish"}, {"mk", "Macedonian"}, {"mg", "Malagasy"},
        {"ms", "Malay"}, {"ml", "Malayalam"}, {"mt", "Maltese"},
        {"mi", "Maori"}, {"mr", "Marathi"}, {"mn", "Mongolian"},
        {"my", "Myanmar (Burmese)"}, {"ne", "Nepali"}, {"no", "Norwegian"},
        {"ps", "Pashto"}, {"fa", "Persian"}, {"pl", "Polish"},
        {"pt", "Portuguese"}, {"pa", "Punjabi"}, {"ro", "Romanian"},
        {"ru", "Russian"}, {"sm", "Samoan"}, {"gd", "Scots Gaelic"},
        {"sr", "Serbian"}, {"st", "Sesotho"}, {"sn", "Shona"},
        {"sd", "Sindhi"}, {"si", "Sinhala"}, {"sk", "Slovak"},
        {"sl", "Slovenian"}, {"so", "Somali"}, {"es", "Spanish"},
        {"su", "Sundanese"}, {"sw", "Swahili"}, {"sv", "Swedish"},
        {"tg", "Tajik"}, {"ta", "Tamil"}, {"te", "Telugu"},
        {"th", "Thai"}, {"tr", "Turkish"}, {"uk", "Ukrainian"},
        {"ur", "Urdu"}, {"ug", "Uyghur"}, {"uz", "Uzbek"},
        {"vi", "Vietnamese"}, {"cy", "Welsh"}, {"xh", "Xhosa"},
        {"yi", "Yiddish"}, {"yo", "Yoruba"}, {"zu", "Zulu"},
    };
    auto it = names.find(code);
    return (it != names.end()) ? it->second : code;
}

// Original Kotlin: TranslateManager.kt — native script name by code
std::string getLanguageNativeName(const std::string& code) {
    static const std::unordered_map<std::string, std::string> native = {
        {"af", "Afrikaans"}, {"sq", "Shqip"}, {"am", "አማርኛ"},
        {"ar", "العربية"}, {"hy", "Հայերեն"}, {"az", "Azərbaycan"},
        {"eu", "Euskara"}, {"be", "Беларуская"}, {"bn", "বাংলা"},
        {"bs", "Bosanski"}, {"bg", "Български"}, {"ca", "Català"},
        {"ceb", "Cebuano"}, {"ny", "Chichewa"}, {"zh", "简体中文"},
        {"zh_tw", "繁體中文"}, {"hr", "Hrvatski"}, {"cs", "Čeština"},
        {"da", "Dansk"}, {"nl", "Nederlands"}, {"en", "English"},
        {"eo", "Esperanto"}, {"et", "Eesti"}, {"tl", "Filipino"},
        {"fi", "Suomi"}, {"fr", "Français"}, {"fy", "Frysk"},
        {"gl", "Galego"}, {"ka", "ქართული"}, {"de", "Deutsch"},
        {"el", "Ελληνικά"}, {"gu", "ગુજરાતી"}, {"ht", "Kreyòl Ayisyen"},
        {"ha", "Hausa"}, {"haw", "ʻŌlelo Hawaiʻi"}, {"he", "עברית"},
        {"hi", "हिन्दी"}, {"hmn", "Hmoob"}, {"hu", "Magyar"},
        {"is", "Íslenska"}, {"ig", "Igbo"}, {"id", "Bahasa Indonesia"},
        {"ga", "Gaeilge"}, {"it", "Italiano"}, {"ja", "日本語"},
        {"jw", "Basa Jawa"}, {"kn", "ಕನ್ನಡ"}, {"kk", "Қазақша"},
        {"km", "ភាសាខ្មែរ"}, {"ko", "한국어"}, {"ku", "Kurdî"},
        {"ky", "Кыргызча"}, {"lo", "ລາວ"}, {"la", "Latina"},
        {"lv", "Latviešu"}, {"lt", "Lietuvių"}, {"lb", "Lëtzebuergesch"},
        {"mk", "Македонски"}, {"mg", "Malagasy"}, {"ms", "Bahasa Melayu"},
        {"ml", "മലയാളം"}, {"mt", "Malti"}, {"mi", "Māori"},
        {"mr", "मराठी"}, {"mn", "Монгол"}, {"my", "မြန်မာ"},
        {"ne", "नेपाली"}, {"no", "Norsk"}, {"ps", "پښتو"},
        {"fa", "فارسی"}, {"pl", "Polski"}, {"pt", "Português"},
        {"pa", "ਪੰਜਾਬੀ"}, {"ro", "Română"}, {"ru", "Русский"},
        {"sm", "Gagana Samoa"}, {"gd", "Gàidhlig"}, {"sr", "Српски"},
        {"st", "Sesotho"}, {"sn", "chiShona"}, {"sd", "سنڌي"},
        {"si", "සිංහල"}, {"sk", "Slovenčina"}, {"sl", "Slovenščina"},
        {"so", "Soomaali"}, {"es", "Español"}, {"su", "Basa Sunda"},
        {"sw", "Kiswahili"}, {"sv", "Svenska"}, {"tg", "Тоҷикӣ"},
        {"ta", "தமிழ்"}, {"te", "తెలుగు"}, {"th", "ไทย"},
        {"tr", "Türkçe"}, {"uk", "Українська"}, {"ur", "اردو"},
        {"ug", "ئۇيغۇرچە"}, {"uz", "Oʻzbek"}, {"vi", "Tiếng Việt"},
        {"cy", "Cymraeg"}, {"xh", "isiXhosa"}, {"yi", "ייִדיש"},
        {"yo", "Yorùbá"}, {"zu", "isiZulu"},
    };
    auto it = native.find(code);
    return (it != native.end()) ? it->second : code;
}

// Original Kotlin: TranslateManager.kt — RTL language check
bool isRtlLanguage(const std::string& code) {
    static const std::unordered_set<std::string> rtl = {
        "ar", // Arabic
        "he", // Hebrew
        "fa", // Persian / Farsi
        "ur", // Urdu
        "ps", // Pashto
        "sd", // Sindhi
        "ug", // Uyghur
        "yi", // Yiddish
        "ku", // Kurdish (Sorani uses Arabic script)
    };
    return rtl.find(code) != rtl.end();
}

// ============================================================================
// New: TranslationCache implementation
// ============================================================================

// Original Kotlin: TranslateManager.kt — LRU cache

TranslationCache::TranslationCache(size_t maxSize)
    : maxSize_(maxSize == 0 ? 256 : maxSize) {}

std::string TranslationCache::makeKey(
    const std::string& sourceText,
    const std::string& sourceLang,
    const std::string& targetLang) {
    return sourceText + "|" + sourceLang + "|" + targetLang;
}

void TranslationCache::touch(const std::string& key) const {
    auto it = map_.find(key);
    if (it == map_.end()) return;

    // Move the entry to the back (most recently used)
    list_.splice(list_.end(), list_, it->second);
}

std::string TranslationCache::getCachedTranslation(
    const std::string& sourceText,
    const std::string& sourceLang,
    const std::string& targetLang) const {
    auto key = makeKey(sourceText, sourceLang, targetLang);
    touch(key);

    auto it = map_.find(key);
    if (it != map_.end()) {
        return it->second->translatedText;
    }
    return {};
}

void TranslationCache::putCachedTranslation(
    const std::string& sourceText,
    const std::string& sourceLang,
    const std::string& targetLang,
    const std::string& translatedText) {
    auto key = makeKey(sourceText, sourceLang, targetLang);

    // If already in cache, update and move to back
    auto it = map_.find(key);
    if (it != map_.end()) {
        it->second->translatedText = translatedText;
        it->second->timestamp = static_cast<int64_t>(std::time(nullptr)) * 1000;
        list_.splice(list_.end(), list_, it->second);
        return;
    }

    // Evict oldest if at capacity
    if (list_.size() >= maxSize_) {
        auto& oldest = list_.front();
        auto oldKey = makeKey(oldest.sourceText, oldest.sourceLang, oldest.targetLang);
        map_.erase(oldKey);
        list_.pop_front();
    }

    // Insert new entry
    TranslationCacheEntry entry;
    entry.sourceText = sourceText;
    entry.sourceLang = sourceLang;
    entry.targetLang = targetLang;
    entry.translatedText = translatedText;
    entry.timestamp = static_cast<int64_t>(std::time(nullptr)) * 1000;

    list_.push_back(std::move(entry));
    map_[key] = std::prev(list_.end());
}

void TranslationCache::clear() {
    list_.clear();
    map_.clear();
}

// ============================================================================
// New: translateMessage
// ============================================================================

// Original Kotlin: TranslateManager.kt — translate message with caching
// Note: actual HTTP calls are performed at the Kotlin/JNI layer.
// This function returns a cached result if available; otherwise returns
// empty to signal that a network request is needed.
std::string translateMessage(
    const std::string& text,
    const std::string& sourceLang,
    const std::string& targetLang,
    TranslateProvider provider,
    const std::string& apiKey,
    const std::string& apiUrl) {
    if (text.empty()) return {};
    if (provider == TranslateProvider::NONE) return text;
    (void)apiKey;
    (void)apiUrl;

    // Check static cache
    static TranslationCache sCache(512);
    auto cached = sCache.getCachedTranslation(text, sourceLang, targetLang);
    if (!cached.empty()) return cached;

    // Cache miss — return empty string to signal that a real API call is needed.
    // The caller (Kotlin/JNI) should perform the HTTP request and then call
    // putCachedTranslation to store the result.
    return {};
}

} // namespace progressive
