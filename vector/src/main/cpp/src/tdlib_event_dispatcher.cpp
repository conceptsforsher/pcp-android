#include "pcp/tdlib_event_dispatcher.hpp"

namespace pcp {

void TdEventDispatcher::onMessageDeleted(int64_t chatId, const std::vector<int64_t>& ids) {
    MessageDeleted md{chatId, ids};
    dispatch(TdEvent::MESSAGE_DELETED, &md);
}

void TdEventDispatcher::onUserStatus(int64_t userId, UserStatus status) {
    UserStatusUpdate us{userId, status};
    dispatch(TdEvent::USER_STATUS, &us);
}

void TdEventDispatcher::onConnectionChanged(TdConnectionState state) {
    ConnectionUpdate cu{state};
    dispatch(TdEvent::CONNECTION_CHANGED, &cu);
}

void TdEventDispatcher::onAuthChanged(const std::string& type, const std::string& json) {
    AuthUpdate au{type, json};
    dispatch(TdEvent::AUTH_CHANGED, &au);
}

void TdEventDispatcher::onNewStory(const TdStory& story) {
    StoryUpdate su{story};
    dispatch(TdEvent::NEW_STORY, &su);
}

void TdEventDispatcher::onFileProgress(int fileId, int64_t size, int64_t downloaded, bool completed) {
    FileUpdate fu{fileId, size, downloaded, completed};
    dispatch(TdEvent::FILE_PROGRESS, &fu);
}

void TdEventDispatcher::onError(const TdError& error) {
    ErrorUpdate eu{error};
    dispatch(TdEvent::ERROR, &eu);
}

} // namespace pcp
