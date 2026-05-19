#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {

// ================================================================
// Message Composer Manager — rich text editing & send pipeline
//
// Faithful port from Element Android original sources:
//   MessageComposerViewState.kt — SendMode (Regular/Quote/Edit/Reply/Voice),
//     CanSendStatus (Allowed/NoPermission/UnSupportedE2eAlgorithm)
//   MessageComposerAction.kt — SendMessage, EnterEditMode, EnterQuoteMode,
//     EnterReplyMode, EnterRegularMode, UserIsTyping, OnTextChanged,
//     InsertUserDisplayName, SetFullScreen
//   SendService.kt — sendTextMessage, sendFormattedTextMessage
//
// Covers:
//   1. Send mode management (regular/quote/edit/reply/voice)
//   2. Text formatting (bold, italic, code, quote prefix)
//   3. @mention auto-completion detection
//   4. MXID mention formatting
//   5. Emoji shortcode → unicode
//   6. Message validation (empty, too long)
//   7. Send permission checks
// ================================================================

// ---- Send Mode ----
// Original: SendMode sealed interface

enum class SendModeType {
    REGULAR = 0,
    QUOTE = 1,
    EDIT = 2,
    REPLY = 3,
    VOICE = 4,
};

// ---- Can Send Status ----
// Original: CanSendStatus

enum class CanSendStatus {
    ALLOWED = 0,
    NO_PERMISSION = 1,               // Power level too low
    UNSUPPORTED_E2E_ALGORITHM = 2,   // Can't encrypt
};

const char* canSendStatusToString(CanSendStatus status);

// ---- Send Mode State ----

struct SendMode {
    SendModeType type = SendModeType::REGULAR;
    std::string text;                 // Current composer text
    std::string linkedEventId;        // For Quote/Edit/Reply — target event
    bool fromSharing = false;         // From external share intent
    bool valid = false;
};

// ---- Composer State ----
// Original: MessageComposerViewState

struct ComposerState {
    std::string roomId;
    bool isRoomError = false;
    CanSendStatus canSendMessage = CanSendStatus::ALLOWED;
    bool isSendButtonVisible = false;
    std::string rootThreadEventId;    // null = not in thread
    bool startsThread = false;
    SendMode sendMode;
    std::string text;                 // Current raw text
    bool isFullScreen = false;
    bool isVoiceRecording = false;

    bool isInThreadTimeline() const { return !rootThreadEventId.empty(); }
    bool canSend() const { return canSendMessage == CanSendStatus::ALLOWED; }
};

// ---- Text Formatting ----

struct TextFormat {
    bool bold = false;
    bool italic = false;
    bool code = false;
    bool quote = false;              // Lines prefixed with "> "
};

// Detect text formatting in a string.
TextFormat detectTextFormat(const std::string& text);

// Apply text formatting.
std::string applyFormat(const std::string& text, const TextFormat& format);

// Strip formatting for plain text send.
std::string stripFormatting(const std::string& text);

// ---- @Mention Auto-Complete ----

struct MentionMatch {
    std::string displayName;         // "Alice"
    std::string userId;              // "@alice:example.org"
    std::string avatarUrl;           // mxc://...
    std::string query;               // What the user typed after @
    int matchStartPos = 0;           // Position in text where @ starts
    int matchEndPos = 0;             // Position where match ends
    bool isRoomMention = false;      // #room-alias vs @user
};

// Check if the current text position is at a mention trigger.
bool isAtMentionTrigger(const std::string& text, int cursorPos, bool& isRoom);

// Extract the mention query (text after @ or # before cursor).
std::string extractMentionQuery(const std::string& text, int cursorPos);

// Build a mention pill text ("Alice").
std::string buildMentionPill(const std::string& displayName, const std::string& userId);

// Build a room mention pill text ("#room:example.org").
std::string buildRoomMentionPill(const std::string& roomAlias);

// Replace mention query with full pill in text.
std::string replaceMention(const std::string& text, const MentionMatch& match);

// ---- Emoji Shortcode ----

struct EmojiMatch {
    std::string shortcode;           // ":smile:"
    std::string unicode;             // "😊"
    int startPos = 0;
    int endPos = 0;
};

// Find emoji shortcodes in text (e.g. ":smile:").
std::vector<EmojiMatch> findEmojiShortcodes(const std::string& text);

// Replace emoji shortcode with unicode.
std::string replaceEmojiShortcode(const std::string& text, const EmojiMatch& match);

// Auto-replace all emoji shortcodes in text.
std::string autoReplaceEmojis(const std::string& text);

// ---- Message Validation ----

struct MessageValidation {
    bool valid = false;
    bool isEmpty = true;
    bool isTooLong = false;
    int maxLength = 65535;
    int currentLength = 0;
    std::string errorMessage;
};

// Validate a message before sending.
MessageValidation validateMessage(const std::string& text, int maxLength = 65535);

// ---- Quote Formatting ----

// Build a quoted message body (Matrix reply format with "> " prefix).
std::string buildQuotedBody(const std::string& quotedText, const std::string& replyText,
                             const std::string& quotedSender = "");

// Build quoted HTML body (Matrix rich reply format).
std::string buildQuotedHtmlBody(const std::string& quotedHtml, const std::string& replyHtml,
                                 const std::string& quotedSender = "");

// ---- Composer Manager ----

class ComposerManager {
public:
    ComposerManager();

    // ====== Send Mode ======
    // Original: EnterRegularMode/EnterEditMode/EnterQuoteMode/EnterReplyMode

    void enterRegularMode(bool fromSharing = false);
    void enterEditMode(const std::string& eventId);
    void enterQuoteMode(const std::string& eventId);
    void enterReplyMode(const std::string& eventId);
    void enterVoiceMode();

    SendMode getSendMode() const;
    SendModeType getSendModeType() const;

    // ====== Text Management ======
    // Original: OnTextChanged, InsertUserDisplayName

    void setText(const std::string& text);
    std::string getText() const;

    // Insert formatted user mention at cursor position.
    std::string insertMention(int cursorPos, const MentionMatch& match);

    // Set full screen mode.
    void setFullScreen(bool fullScreen);

    // ====== Text Formatting ======

    // Apply bold to selection.
    std::string applyBold(const std::string& text, int selStart, int selEnd);

    // Apply italic to selection.
    std::string applyItalic(const std::string& text, int selStart, int selEnd);

    // Apply code (backtick) to selection.
    std::string applyCode(const std::string& text, int selStart, int selEnd);

    // Toggle quote prefix on selected lines.
    std::string applyQuote(const std::string& text, int selStart, int selEnd);

    // ====== Validation ======

    // Check if user has send permission.
    bool canSendMessage() const;

    // Validate message content.
    MessageValidation validateCurrentMessage() const;

    // ====== Serialization ======

    std::string stateToJson() const;
    std::string sendModeToJson(const SendMode& mode) const;

private:
    SendMode sendMode_;
    std::string composerText_;
    bool isFullScreen_ = false;
    CanSendStatus canSend_ = CanSendStatus::ALLOWED;
    int maxMessageLength_ = 65535;
};

} // namespace progressive
