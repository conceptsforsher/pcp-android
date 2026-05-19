#include "progressive/translate.hpp"
#include "progressive/json_parser.hpp"
#include <sstream>

namespace progressive {

std::string buildTranslateRequestBody(const TranslateRequest& request) {
    std::ostringstream json;

    // Build system prompt
    std::string systemPrompt = "You are a translator. Translate the following text";
    if (!request.sourceLanguage.empty()) {
        systemPrompt += " from " + request.sourceLanguage;
    }
    systemPrompt += " to " + request.targetLanguage + ". ";
    systemPrompt += "Output ONLY the translation, nothing else. No quotes, no explanations.";

    // Escape user text for JSON
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
        // Try to extract nested error message from OpenAI format
        if (!error.empty()) {
            auto errorMsg = parseJsonStringValue("{" + error + "}", "message");
            if (!errorMsg.empty()) msg = errorMsg;
        }
        result.errorMessage = msg;
        return result;
    }

    // Extract "choices" → [0] → "message" → "content"
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

} // namespace progressive
