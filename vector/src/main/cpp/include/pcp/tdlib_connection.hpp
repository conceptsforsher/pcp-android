#ifndef PCP_TDLIB_CONNECTION_HPP
#define PCP_TDLIB_CONNECTION_HPP

#include "pcp/tdlib_types.hpp"
#include <functional>
#include <atomic>
#include <chrono>

namespace pcp {

using ConnectionCallback = std::function<void(TdConnectionState state)>;

class TdConnectionManager {
public:
    TdConnectionManager() = default;

    void setCallback(ConnectionCallback cb) { callback = std::move(cb); }

    void onStateChanged(TdConnectionState state);
    void onNetworkAvailable();
    void onNetworkLost();

    TdConnectionState currentState() const { return state.load(); }
    bool isConnected() const { return state == TdConnectionState::CONNECTED; }
    bool canSend() const { return state == TdConnectionState::CONNECTED || state == TdConnectionState::UPDATING; }

    int64_t lastConnectedTime() const { return lastConnected; }
    int64_t disconnectDuration() const;

private:
    std::atomic<TdConnectionState> state{TdConnectionState::DISCONNECTED};
    ConnectionCallback callback;
    int64_t lastConnected = 0;
    int64_t lastDisconnected = 0;
};

} // namespace pcp
#endif
