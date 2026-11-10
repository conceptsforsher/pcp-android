#ifndef PCP_TDLIB_TYPES_HPP
#define PCP_TDLIB_TYPES_HPP

#include <string>
#include <vector>
#include <cstdint>
#include <optional>

namespace pcp {

// === User ===
enum class UserStatus { ONLINE, OFFLINE, RECENTLY, LAST_WEEK, LAST_MONTH, EMPTY };

struct TdUser {
    int64_t id = 0;
    std::string firstName;
    std::string lastName;
    std::string username;
    std::string phoneNumber;
    std::string bio;
    bool isPremium = false;
    bool isVerified = false;
    bool isBot = false;
    bool isContact = false;
    bool isMutualContact = false;
    UserStatus status = UserStatus::EMPTY;
    int wasOnline = 0;
    int64_t emojiStatusId = 0;
};

// === Chat ===
struct TdChat {
    int64_t id = 0;
    std::string title;
    std::string type;  // "private", "group", "supergroup", "channel", "secret"
    int unreadCount = 0;
    int unreadMentionCount = 0;
    int64_t lastMessageId = 0;
    std::string lastMessageText;
    int64_t lastMessageTime = 0;
    bool isPinned = false;
    bool isMarkedUnread = false;
    int64_t order = 0;
    std::string draftText;
    int muteFor = 0;
};

// === Message Content Types ===
enum class ContentType {
    TEXT, PHOTO, VIDEO, VOICE, VIDEO_NOTE, ANIMATION,
    STICKER, DOCUMENT, POLL, LOCATION, CONTACT, DICE, GAME, CALL, SYSTEM, UNKNOWN
};

struct PhotoSize {
    std::string type; // "s", "m", "x", "y", "w", "a", "b", "c", "d", "i"
    int width = 0, height = 0;
    int fileId = 0;
    int64_t fileSize = 0;
};

struct TdPhoto {
    bool hasStickers = false;
    std::vector<PhotoSize> sizes;
};

struct TdVoice {
    int duration = 0;
    int fileId = 0;
    int64_t fileSize = 0;
    bool isListened = false;
};

struct TdVideo {
    int duration = 0;
    int width = 0, height = 0;
    int fileId = 0;
    int64_t fileSize = 0;
    bool supportsStreaming = false;
};

struct TdDocument {
    std::string fileName;
    std::string mimeType;
    int fileId = 0;
    int64_t fileSize = 0;
};

struct TdSticker {
    int64_t setId = 0;
    int width = 0, height = 0;
    std::string emoji;
    int fileId = 0;
    int64_t fileSize = 0;
    bool isAnimated = false;
    bool isVideo = false;
};

struct PollOption {
    std::string text;
    int voterCount = 0;
    int votePercentage = 0;
    bool isChosen = false;
};

struct TdPoll {
    int64_t id = 0;
    std::string question;
    std::vector<PollOption> options;
    int totalVoterCount = 0;
    bool isAnonymous = true;
    bool isQuiz = false;
    bool isClosed = false;
    bool allowMultipleAnswers = false;
};

struct TdLocation {
    double latitude = 0.0;
    double longitude = 0.0;
    int livePeriod = 0;
    int heading = 0;
};

struct TdContact {
    std::string firstName, lastName, phoneNumber;
    int64_t userId = 0;
};

struct TdDice {
    std::string emoji;
    int value = 0;
};

struct TdWebPage {
    std::string url, displayUrl, title, description, siteName;
    bool hasInstantView = false;
};

struct TdForwardInfo {
    int64_t fromChatId = 0;
    int64_t fromMessageId = 0;
    std::string origin; // "user", "chat", "channel", "hidden"
    std::string senderName;
    int64_t senderUserId = 0;
};

struct TdCallInfo {
    int id = 0;
    int64_t userId = 0;
    bool isOutgoing = false;
    bool isVideo = false;
    int duration = 0;
    std::string discardReason;
};

// === Message ===
struct TdMessage {
    int64_t id = 0;
    int64_t chatId = 0;
    int64_t senderId = 0;
    std::string senderName;
    int date = 0;
    int editDate = 0;
    bool isOutgoing = false;
    bool isChannelPost = false;
    ContentType contentType = ContentType::UNKNOWN;
    std::string text;
    std::string caption;

    // Optional rich content
    std::optional<TdPhoto> photo;
    std::optional<TdVoice> voice;
    std::optional<TdVideo> video;
    std::optional<TdDocument> document;
    std::optional<TdSticker> sticker;
    std::optional<TdPoll> poll;
    std::optional<TdLocation> location;
    std::optional<TdContact> contact;
    std::optional<TdDice> dice;
    std::optional<TdWebPage> webPage;
    std::optional<TdCallInfo> call;
    std::optional<TdForwardInfo> forwardInfo;

    // Thread
    int64_t replyToMessageId = 0;
    int64_t threadId = 0;
    int64_t viaBotUserId = 0;

    // Views
    int viewCount = 0;
    int forwardCount = 0;
};

// === Story ===
struct TdStory {
    int64_t id = 0;
    int64_t senderChatId = 0;
    std::string senderName;
    int date = 0;
    bool isBeingViewed = false;
    bool isPinned = false;
    ContentType contentType = ContentType::UNKNOWN;
    std::string caption;
    std::optional<TdPhoto> photo;
    std::optional<TdVideo> video;
    int viewCount = 0;
    int reactionCount = 0;
};

// === Chat Position ===
struct TdChatPosition {
    std::string list; // "main", "archive"
    int64_t order = 0;
    bool isPinned = false;
};

// === Session Info ===
struct TdSession {
    int64_t id = 0;
    bool isCurrent = false;
    std::string appName, appVersion, deviceModel, platform, ip, country;
    int logInDate = 0;
    int lastActiveDate = 0;
};

// === Notification Settings ===
struct TdNotificationSettings {
    bool useDefaultMute = true;
    int muteFor = 0;
    bool useDefaultSound = true;
    int64_t soundId = 0;
    bool showPreview = true;
    bool disablePinned = false;
    bool disableMention = false;
};

// === Proxy ===
enum class ProxyType { SOCKS5, HTTP, MTPROTO };

struct TdProxy {
    int id = 0;
    std::string server;
    int port = 0;
    ProxyType type = ProxyType::SOCKS5;
    bool isEnabled = false;
    int64_t lastPing = 0;
};

// === Error ===
struct TdError {
    int code = 0;
    std::string message;
};

// === Connection State ===
enum class TdConnectionState {
    DISCONNECTED, CONNECTING, CONNECTED, UPDATING, WAITING_NETWORK
};

} // namespace pcp
#endif
