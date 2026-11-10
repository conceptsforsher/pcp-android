#ifndef PCP_TDLIB_EVENT_DISPATCHER_HPP
#define PCP_TDLIB_EVENT_DISPATCHER_HPP

#include "pcp/tdlib_types.hpp"
#include "pcp/tdlib_client.hpp"
#include <functional>
#include <mutex>
#include <vector>

namespace pcp {

struct ChatListUpdate { std::vector<int64_t> chatIds; };
struct ChatUpdate { TdChat chat; };
struct MessageUpdate { TdMessage message; };
struct MessageDeleted { int64_t chatId; std::vector<int64_t> messageIds; };
struct UserStatusUpdate { int64_t userId; UserStatus status; };
struct ConnectionUpdate { TdConnectionState state; };
struct AuthUpdate { std::string stateType; std::string stateJson; };
struct StoryUpdate { TdStory story; };
struct FileUpdate { int fileId; int64_t size; int64_t downloaded; bool completed; };
struct ErrorUpdate { TdError error; };

using EventCallback = std::function<void(int eventType, const void* data)>;

enum class TdEvent {
    CHAT_LIST_CHANGED = 1,
    NEW_MESSAGE = 2,
    MESSAGE_DELETED = 3,
    CHAT_UPDATED = 4,
    USER_STATUS = 5,
    CONNECTION_CHANGED = 6,
    AUTH_CHANGED = 7,
    NEW_STORY = 8,
    FILE_PROGRESS = 9,
    ERROR = 10,
};

class TdEventDispatcher {
public:
    TdEventDispatcher() = default;

    void subscribe(EventCallback cb) {
        std::lock_guard<std::mutex> lock(mtx);
        callbacks.push_back(std::move(cb));
    }

    void dispatch(TdEvent type, const void* data) {
        std::lock_guard<std::mutex> lock(mtx);
        for (auto& cb : callbacks) {
            cb(static_cast<int>(type), data);
        }
    }

    // Typed dispatch helpers
    void onNewMessage(const TdMessage& msg) { dispatch(TdEvent::NEW_MESSAGE, &msg); }
    void onChatListChanged() { dispatch(TdEvent::CHAT_LIST_CHANGED, nullptr); }
    void onChatUpdated(const TdChat& chat) { dispatch(TdEvent::CHAT_UPDATED, &chat); }
    void onMessageDeleted(int64_t chatId, const std::vector<int64_t>& ids);
    void onUserStatus(int64_t userId, UserStatus status);
    void onConnectionChanged(TdConnectionState state);
    void onAuthChanged(const std::string& type, const std::string& json);
    void onNewStory(const TdStory& story);
    void onFileProgress(int fileId, int64_t size, int64_t downloaded, bool completed);
    void onError(const TdError& error);

private:
    std::mutex mtx;
    std::vector<EventCallback> callbacks;
};

} // namespace pcp
#endif
