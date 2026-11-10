#pragma once
#include <string>
#include <vector>
#include <cstdint>

namespace pcp {

struct SmartReplySuggestion {
    std::string text;           // suggested reply text
    double confidence = 0.0;    // 0.0-1.0
    std::string category;       // "reply", "reaction", "question", "acknowledgment"
};

struct SmartReactionSuggestion {
    std::string emoji;          // e.g. "👍", "❤️", "😂"
    std::string label;          // e.g. "like", "love", "laugh"
    double confidence = 0.0;
};

struct MessageContext {
    std::string lastMessage;        // most recent message to reply to
    std::string lastSenderName;     // who sent it
    std::vector<std::string> recentMessages;  // last 5 messages for context
    std::string roomName;
    bool isDirect = false;
    bool isEncrypted = false;
};

// Build LLM prompt from message context for smart replies
std::string buildSmartReplyPrompt(const MessageContext& ctx);

// Build LLM prompt for smart reactions
std::string buildSmartReactionPrompt(const MessageContext& ctx);

// Parse LLM response into smart reply suggestions
std::vector<SmartReplySuggestion> parseSmartReplyResponse(const std::string& llmResponse);

// Parse LLM response into smart reaction suggestions
std::vector<SmartReactionSuggestion> parseSmartReactionResponse(const std::string& llmResponse);

// Get default quick replies when LLM is not available
std::vector<SmartReplySuggestion> getDefaultReplies(const MessageContext& ctx);

// Get default quick reactions (no LLM needed)
std::vector<SmartReactionSuggestion> getDefaultReactions();

// Pattern-based quick replies (no LLM, works offline)
std::vector<SmartReplySuggestion> getPatternReplies(const std::string& message);

// Check if message is a question
bool isQuestion(const std::string& text);

// Check if message is an acknowledgment (thanks, ok, etc.)
bool isAcknowledgment(const std::string& text);

} // namespace pcp
