#include "pcp/smart_reply.hpp"
#include <sstream>
#include <algorithm>

namespace pcp {

std::string buildSmartReplyPrompt(const MessageContext& ctx) {
    std::ostringstream os;
    os << "You are a helpful assistant suggesting quick replies. ";
    os << "Given the following conversation context, suggest 3 short, natural reply options. ";
    os << "Return ONLY a JSON array of strings, like [\"reply1\",\"reply2\",\"reply3\"]. ";
    if (!ctx.roomName.empty()) os << "Room: " << ctx.roomName << ". ";
    os << "Last message from " << ctx.lastSenderName << ": " << ctx.lastMessage << ". ";
    if (!ctx.recentMessages.empty()) {
        os << "Recent context: ";
        for (size_t i = 0; i < ctx.recentMessages.size() && i < 3; i++)
            os << "\"" << ctx.recentMessages[i] << "\" ";
    }
    os << "Suggest 3 short replies:";
    return os.str();
}

std::string buildSmartReactionPrompt(const MessageContext& ctx) {
    std::ostringstream os;
    os << "Suggest 3 emoji reactions for this message: \"" << ctx.lastMessage << "\". ";
    os << "Return ONLY JSON array of emoji strings.";
    return os.str();
}

std::vector<SmartReplySuggestion> parseSmartReplyResponse(const std::string& resp) {
    std::vector<SmartReplySuggestion> result;
    // Parse JSON array: ["reply1", "reply2", "reply3"]
    size_t pos = resp.find('[');
    if (pos == std::string::npos) return result;
    pos++;
    while (pos < resp.size()) {
        auto q1 = resp.find('"', pos);
        if (q1 == std::string::npos) break;
        auto q2 = resp.find('"', q1 + 1);
        if (q2 == std::string::npos) break;
        std::string text = resp.substr(q1 + 1, q2 - q1 - 1);
        if (!text.empty()) result.push_back({text, 1.0 - result.size() * 0.15, "reply"});
        pos = q2 + 1;
        if (result.size() >= 3) break;
    }
    return result;
}

std::vector<SmartReactionSuggestion> parseSmartReactionResponse(const std::string& resp) {
    std::vector<SmartReactionSuggestion> result;
    size_t pos = resp.find('[');
    if (pos == std::string::npos) return result;
    pos++;
    while (pos < resp.size()) {
        auto q1 = resp.find('"', pos);
        if (q1 == std::string::npos) break;
        auto q2 = resp.find('"', q1 + 1);
        if (q2 == std::string::npos) break;
        std::string emoji = resp.substr(q1 + 1, q2 - q1 - 1);
        if (!emoji.empty()) result.push_back({emoji, emoji, 1.0 - result.size() * 0.2});
        pos = q2 + 1;
        if (result.size() >= 3) break;
    }
    return result;
}

std::vector<SmartReplySuggestion> getDefaultReplies(const MessageContext& ctx) {
    std::vector<SmartReplySuggestion> replies;
    if (isQuestion(ctx.lastMessage)) {
        replies.push_back({"Yes", 0.9, "reply"});
        replies.push_back({"No", 0.8, "reply"});
        replies.push_back({"Not sure", 0.6, "reply"});
    } else if (isAcknowledgment(ctx.lastMessage)) {
        replies.push_back({"👍", 0.9, "reply"});
        replies.push_back({"😊", 0.8, "reply"});
    } else {
        replies.push_back({"OK", 0.7, "acknowledgment"});
        replies.push_back({"Thanks!", 0.6, "acknowledgment"});
        replies.push_back({"👍", 0.5, "reply"});
    }
    return replies;
}

std::vector<SmartReactionSuggestion> getDefaultReactions() {
    return {
        {"👍", "like", 1.0},
        {"❤️", "love", 0.9},
        {"😂", "laugh", 0.8},
        {"😮", "wow", 0.7},
        {"😢", "sad", 0.6},
        {"👏", "clap", 0.5}
    };
}

std::vector<SmartReplySuggestion> getPatternReplies(const std::string& msg) {
    std::vector<SmartReplySuggestion> replies;
    std::string lower;
    std::transform(msg.begin(), msg.end(), std::back_inserter(lower), ::tolower);
    
    if (lower.find("hello") != std::string::npos || lower.find("hi") == 0)
        replies = {{"Hi!", 0.9, "reply"}, {"Hello!", 0.8, "reply"}, {"Hey!", 0.7, "reply"}};
    else if (lower.find("how are you") != std::string::npos)
        replies = {{"Good, you?", 0.9, "question"}, {"Fine thanks", 0.8, "reply"}, {"👍", 0.7, "reply"}};
    else if (lower.find("thank") != std::string::npos)
        replies = {{"You're welcome!", 0.9, "acknowledgment"}, {"No problem", 0.8, "acknowledgment"}, {"😊", 0.7, "reply"}};
    else if (lower.find("bye") != std::string::npos)
        replies = {{"Bye!", 0.9, "reply"}, {"See you!", 0.8, "reply"}, {"👋", 0.7, "reply"}};
    
    return replies;
}

bool isQuestion(const std::string& text) {
    return text.find('?') != std::string::npos ||
           text.find("what") == 0 || text.find("how") == 0 ||
           text.find("why") == 0 || text.find("when") == 0 ||
           text.find("where") == 0 || text.find("who") == 0 ||
           text.find("can you") == 0 || text.find("could you") == 0;
}

bool isAcknowledgment(const std::string& text) {
    std::string lower;
    std::transform(text.begin(), text.end(), std::back_inserter(lower), ::tolower);
    return lower.find("ok") != std::string::npos ||
           lower.find("thanks") != std::string::npos ||
           lower.find("thank") != std::string::npos ||
           lower.find("sure") != std::string::npos ||
           lower.find("got it") != std::string::npos;
}

} // namespace pcp
