#include "progressive/composer_manager.hpp"
#include <sstream>
#include <regex>
#include <algorithm>

namespace progressive {

// ====== Enum ======

const char* canSendStatusToString(CanSendStatus status) {
    switch (status) {
        case CanSendStatus::ALLOWED: return "allowed";
        case CanSendStatus::NO_PERMISSION: return "no_permission";
        case CanSendStatus::UNSUPPORTED_E2E_ALGORITHM: return "unsupported_e2e";
    }
    return "allowed";
}

// ====== Text Format Detection ======

TextFormat detectTextFormat(const std::string& text) {
    TextFormat fmt;
    // Check bold: surrounded by **
    fmt.bold = text.find("**") != std::string::npos;
    // Check italic: single * or _ (but not double)
    size_t singleStar = 0;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '*' && (i == 0 || text[i-1] != '*') && (i+1 == text.size() || text[i+1] != '*')) singleStar++;
        if (text[i] == '_' && (i == 0 || text[i-1] != '_') && (i+1 == text.size() || text[i+1] != '_')) singleStar++;
    }
    fmt.italic = singleStar >= 2;
    // Check code: backtick
    fmt.code = text.find("`") != std::string::npos;
    // Check quote: lines starting with "> "
    fmt.quote = text.rfind("> ", 0) == 0;
    return fmt;
}

std::string applyFormat(const std::string& text, const TextFormat& format) {
    std::string result = text;
    if (format.bold) result = "**" + result + "**";
    if (format.italic) result = "*" + result + "*";
    if (format.code) result = "`" + result + "`";
    if (format.quote) result = "> " + result;
    return result;
}

std::string stripFormatting(const std::string& text) {
    std::string result;
    bool inBold = false, inItalic = false;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '*' && i+1 < text.size() && text[i+1] == '*') {
            inBold = !inBold;
            i++; // skip second *
            if (inBold) result += "**";
            else result += "**";
            continue;
        }
        if (text[i] == '*' || text[i] == '_') {
            inItalic = !inItalic;
            if (inItalic) result += text[i];
            else result += text[i];
            continue;
        }
        result += text[i];
    }
    return result;
}

// ====== @Mention ======

bool isAtMentionTrigger(const std::string& text, int cursorPos, bool& isRoom) {
    if (cursorPos < 1) return false;
    char c = text[static_cast<size_t>(cursorPos) - 1];
    if (c == '@') { isRoom = false; return true; }
    if (c == '#') { isRoom = true; return true; }
    return false;
}

std::string extractMentionQuery(const std::string& text, int cursorPos) {
    if (cursorPos < 1) return "";
    // Find the trigger character (@ or #) going backwards
    size_t start = static_cast<size_t>(cursorPos) - 1;
    while (start > 0) {
        char c = text[start];
        if (c == ' ' || c == '\n') { start++; break; }
        if (c == '@' || c == '#') { start++; break; }
        start--;
    }
    if (start >= text.size()) return "";
    return text.substr(start, static_cast<size_t>(cursorPos) - start);
}

std::string buildMentionPill(const std::string& displayName, const std::string& userId) {
    return displayName;
}

std::string buildRoomMentionPill(const std::string& roomAlias) {
    return roomAlias;
}

std::string replaceMention(const std::string& text, const MentionMatch& match) {
    std::string result = text;
    if (match.matchEndPos > match.matchStartPos && match.matchEndPos <= static_cast<int>(result.size())) {
        result.replace(match.matchStartPos, match.matchEndPos - match.matchStartPos, match.displayName);
    }
    return result;
}

// ====== Emoji Shortcode ======

static bool isEmojiShortcode(const std::string& text, size_t start) {
    if (text[start] != ':') return false;
    for (size_t i = start + 1; i < text.size(); i++) {
        if (text[i] == ':') return true;
        if (!isalnum(text[i]) && text[i] != '_' && text[i] != '-') return false;
    }
    return false;
}

std::vector<EmojiMatch> findEmojiShortcodes(const std::string& text) {
    std::vector<EmojiMatch> matches;
    size_t pos = 0;
    while ((pos = text.find(':', pos)) != std::string::npos) {
        size_t end = text.find(':', pos + 1);
        if (end != std::string::npos) {
            EmojiMatch m;
            m.shortcode = text.substr(pos, end - pos + 1);
            m.startPos = static_cast<int>(pos);
            m.endPos = static_cast<int>(end + 1);
            matches.push_back(m);
        }
        pos++;
    }
    return matches;
}

std::string replaceEmojiShortcode(const std::string& text, const EmojiMatch& match) {
    // Simple common emoji map
    static const std::unordered_map<std::string, std::string> emojiMap = {
        {":smile:", "😊"}, {":slight_smile:", "🙂"}, {":grin:", "😁"},
        {":heart:", "❤️"}, {":thumbsup:", "👍"}, {":thumbsdown:", "👎"},
        {":clap:", "👏"}, {":fire:", "🔥"}, {":rocket:", "🚀"},
        {":tada:", "🎉"}, {":cry:", "😢"}, {":sob:", "😭"},
        {":angry:", "😠"}, {":wink:", "😉"}, {":ok:", "👌"},
        {":100:", "💯"}, {":check:", "✅"}, {":x:", "❌"},
        {":+1:", "👍"}, {":-1:", "👎"}, {":wave:", "👋"},
    };

    auto it = emojiMap.find(match.shortcode);
    std::string replacement = (it != emojiMap.end()) ? it->second : match.shortcode;

    std::string result = text;
    if (match.endPos <= static_cast<int>(result.size())) {
        result.replace(match.startPos, match.endPos - match.startPos, replacement);
    }
    return result;
}

std::string autoReplaceEmojis(const std::string& text) {
    std::string result = text;
    auto matches = findEmojiShortcodes(text);
    // Replace in reverse order to preserve positions
    for (auto it = matches.rbegin(); it != matches.rend(); ++it) {
        result = replaceEmojiShortcode(result, *it);
    }
    return result;
}

// ====== Message Validation ======

MessageValidation validateMessage(const std::string& text, int maxLength) {
    MessageValidation v;
    v.currentLength = static_cast<int>(text.size());
    v.maxLength = maxLength;

    // Check empty (whitespace only)
    v.isEmpty = true;
    for (char c : text) {
        if (c != ' ' && c != '\t' && c != '\n' && c != '\r') {
            v.isEmpty = false;
            break;
        }
    }

    v.isTooLong = v.currentLength > maxLength;
    v.valid = !v.isEmpty && !v.isTooLong;

    if (v.isEmpty) v.errorMessage = "Message is empty";
    else if (v.isTooLong) {
        v.errorMessage = "Message too long (" + std::to_string(v.currentLength) +
                         " > " + std::to_string(maxLength) + ")";
    }

    return v;
}

// ====== Quote Formatting ======

std::string buildQuotedBody(const std::string& quotedText, const std::string& replyText,
                             const std::string& quotedSender) {
    std::ostringstream os;
    if (!quotedSender.empty()) os << "> <" << quotedSender << "> ";
    // Add "> " prefix to each line of quoted text
    std::string q = quotedText;
    size_t pos = 0;
    while (pos < q.size()) {
        auto nl = q.find('\n', pos);
        if (nl == std::string::npos) nl = q.size();
        os << "> " << q.substr(pos, nl - pos) << "\n";
        pos = nl + 1;
    }
    os << "\n" << replyText;
    return os.str();
}

std::string buildQuotedHtmlBody(const std::string& quotedHtml, const std::string& replyHtml,
                                 const std::string& quotedSender) {
    std::ostringstream os;
    os << "<mx-reply>";
    os << "<blockquote>";
    if (!quotedSender.empty()) os << "<a href=\"https://matrix.to/#/user/" << quotedSender << "\">" << quotedSender << "</a><br/>";
    os << quotedHtml;
    os << "</blockquote>";
    os << "</mx-reply>";
    os << replyHtml;
    return os.str();
}

// ====== ComposerManager ======

ComposerManager::ComposerManager() {}

void ComposerManager::enterRegularMode(bool fromSharing) {
    sendMode_.type = SendModeType::REGULAR;
    sendMode_.fromSharing = fromSharing;
    sendMode_.text.clear();
}

void ComposerManager::enterEditMode(const std::string& eventId) {
    sendMode_.type = SendModeType::EDIT;
    sendMode_.linkedEventId = eventId;
}

void ComposerManager::enterQuoteMode(const std::string& eventId) {
    sendMode_.type = SendModeType::QUOTE;
    sendMode_.linkedEventId = eventId;
}

void ComposerManager::enterReplyMode(const std::string& eventId) {
    sendMode_.type = SendModeType::REPLY;
    sendMode_.linkedEventId = eventId;
}

void ComposerManager::enterVoiceMode() {
    sendMode_.type = SendModeType::VOICE;
}

SendMode ComposerManager::getSendMode() const { return sendMode_; }
SendModeType ComposerManager::getSendModeType() const { return sendMode_.type; }

void ComposerManager::setText(const std::string& text) { composerText_ = text; sendMode_.text = text; }
std::string ComposerManager::getText() const { return composerText_; }
void ComposerManager::setFullScreen(bool fullScreen) { isFullScreen_ = fullScreen; }

std::string ComposerManager::insertMention(int cursorPos, const MentionMatch& match) {
    std::string text = composerText_;
    // Find the @ or # trigger going backwards
    int start = cursorPos;
    while (start > 0 && text[start-1] != ' ' && text[start-1] != '\n' && text[start-1] != '@' && text[start-1] != '#') start--;
    if (start > 0 && (text[start-1] == '@' || text[start-1] == '#')) start--;

    std::string replacement = match.displayName + " ";
    text.replace(start, cursorPos - start, replacement);
    return text;
}

bool ComposerManager::canSendMessage() const { return canSend_ == CanSendStatus::ALLOWED; }

MessageValidation ComposerManager::validateCurrentMessage() const {
    return validateMessage(composerText_, maxMessageLength_);
}

// ====== Text Formatting ======

std::string ComposerManager::applyBold(const std::string& text, int selStart, int selEnd) {
    if (selStart >= selEnd) return text;
    std::string result = text;
    result.insert(selEnd, "**");
    result.insert(selStart, "**");
    return result;
}

std::string ComposerManager::applyItalic(const std::string& text, int selStart, int selEnd) {
    if (selStart >= selEnd) return text;
    std::string result = text;
    result.insert(selEnd, "*");
    result.insert(selStart, "*");
    return result;
}

std::string ComposerManager::applyCode(const std::string& text, int selStart, int selEnd) {
    if (selStart >= selEnd) return text;
    std::string result = text;
    result.insert(selEnd, "`");
    result.insert(selStart, "`");
    return result;
}

std::string ComposerManager::applyQuote(const std::string& text, int selStart, int selEnd) {
    std::string result;
    // Add "> " prefix to each line in selection
    for (int i = 0; i < static_cast<int>(text.size()); i++) {
        if (i == selStart || (i > 0 && text[i-1] == '\n' && i >= selStart && i < selEnd)) {
            result += "> ";
        }
        result += text[i];
    }
    return result;
}

// ====== Serialization ======

std::string ComposerManager::stateToJson() const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"send_mode":)" << static_cast<int>(sendMode_.type)
       << R"(,"send_mode_name":")" << (sendMode_.type == SendModeType::REGULAR ? "regular" :
            sendMode_.type == SendModeType::QUOTE ? "quote" : sendMode_.type == SendModeType::EDIT ? "edit" :
            sendMode_.type == SendModeType::REPLY ? "reply" : "voice")
       << R"(","text":")" << esc(composerText_)
       << R"(","linked_event_id":")" << esc(sendMode_.linkedEventId)
       << R"(,"can_send":")" << canSendStatusToString(canSend_)
       << R"(,"is_fullscreen":)" << (isFullScreen_ ? "true" : "false")
       << R"(,"text_length":)" << static_cast<int>(composerText_.size())
       << "}";
    return os.str();
}

std::string ComposerManager::sendModeToJson(const SendMode& mode) const {
    auto esc = [](const std::string& s) -> std::string {
        std::string out;
        for (char c : s) { if (c == '"') out += "\\\""; else out += c; }
        return out;
    };

    std::ostringstream os;
    os << R"({"type":)" << static_cast<int>(mode.type)
       << R"(,"text":")" << esc(mode.text)
       << R"(,"linked_event_id":")" << esc(mode.linkedEventId)
       << R"(,"from_sharing":)" << (mode.fromSharing ? "true" : "false")
       << "}";
    return os.str();
}

} // namespace progressive
