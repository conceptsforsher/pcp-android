#include "pcp/tdlib_connection.hpp"
#include <android/log.h>
#include <ctime>

#define TAG "TdConnection"

namespace pcp {

void TdConnectionManager::onStateChanged(TdConnectionState s) {
    auto prev = state.exchange(s);
    if (s == TdConnectionState::CONNECTED) {
        lastConnected = std::time(nullptr);
        __android_log_print(ANDROID_LOG_INFO, TAG, "Connected to Telegram");
    } else if (s == TdConnectionState::DISCONNECTED || s == TdConnectionState::WAITING_NETWORK) {
        lastDisconnected = std::time(nullptr);
        __android_log_print(ANDROID_LOG_WARN, TAG, "Disconnected from Telegram");
    }
    if (callback && prev != s) callback(s);
}

void TdConnectionManager::onNetworkAvailable() {
    __android_log_print(ANDROID_LOG_INFO, TAG, "Network available");
}

void TdConnectionManager::onNetworkLost() {
    __android_log_print(ANDROID_LOG_WARN, TAG, "Network lost");
    onStateChanged(TdConnectionState::WAITING_NETWORK);
}

int64_t TdConnectionManager::disconnectDuration() const {
    if (lastDisconnected == 0) return 0;
    return std::time(nullptr) - lastDisconnected;
}

} // namespace pcp
