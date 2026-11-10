#pragma once
#include <string>
#include <vector>
#include <memory>
#include <functional>
#include <chrono>
#include <nlohmann/json.hpp>

namespace matrix_sdk {

using json = nlohmann::json;

// ── Authentication ──

struct LoginRequest {
    std::string type = "m.login.password";
    std::string identifierType = "m.id.user";
    std::string user;
    std::string password;
    std::string token;
    std::string deviceId;
    std::string initialDeviceDisplayName;
};

struct LoginResponse {
    std::string userId;
    std::string accessToken;
    std::string deviceId;
    std::string homeServer;
    bool wellKnown = false;
};

struct RegisterRequest {
    std::string username;
    std::string password;
    std::string authType = "m.login.dummy";
    std::string initialDeviceDisplayName;
    bool inhibitLogin = false;
};

struct RegisterResponse {
    std::string userId;
    std::string accessToken;
    std::string deviceId;
    std::string homeServer;
    bool success = false;
};

// ── Client Configuration ──

struct ClientConfig {
    std::string homeserverUrl;
    std::string identityServerUrl;
    std::string appName = "PCP";
    std::string appVersion = "0.3.0";
    std::string deviceName;
    std::string databasePath;
    bool autoJoinOnInvite = false;
    bool useCompression = true;
    size_t maxFileUploadSize = 100 * 1024 * 1024; // 100 MiB
    int maxSyncTimeout = 30000; // ms
};

// ── Matrix Client (main API surface) ──

class MatrixClient {
public:
    explicit MatrixClient(const ClientConfig& config);
    ~MatrixClient();

    // Lifecycle
    bool start();
    void stop();
    bool isRunning() const { return m_running; }

    // Server discovery
    struct WellKnown {
        std::string homeserver;
        std::string identityServer;
        bool valid = false;
    };
    WellKnown discoverWellKnown(const std::string& domain);

    // Authentication
    LoginResponse login(const LoginRequest& req);
    RegisterResponse registerUser(const RegisterRequest& req);
    bool logout();
    bool refreshToken();

    // Session
    const std::string& userId() const { return m_userId; }
    const std::string& accessToken() const { return m_accessToken; }
    const std::string& deviceId() const { return m_deviceId; }

    // Sync
    void startSync(const std::string& since = "", int timeout = 30000,
                   const std::string& filter = "");
    void stopSync();
    void setSyncCallback(std::function<void(const json&)> cb);

    // Room operations
    std::string createRoom(const std::string& name, bool isDM = false,
                           const std::vector<std::string>& invite = {});
    bool joinRoom(const std::string& roomIdOrAlias);
    bool leaveRoom(const std::string& roomId);
    bool inviteUser(const std::string& roomId, const std::string& userId);
    bool kickUser(const std::string& roomId, const std::string& userId,
                  const std::string& reason = "");

    // Messages
    std::string sendMessage(const std::string& roomId, const std::string& body,
                            const std::string& msgType = "m.text");
    std::string sendFormattedMessage(const std::string& roomId,
                                      const std::string& body,
                                      const std::string& formattedBody,
                                      const std::string& format = "org.matrix.custom.html");
    bool redactEvent(const std::string& roomId, const std::string& eventId,
                     const std::string& reason = "");
    bool editMessage(const std::string& roomId, const std::string& eventId,
                     const std::string& newBody);

    // Media
    std::string uploadMedia(const std::string& localPath,
                            const std::string& contentType = "");
    std::string getMediaUrl(const std::string& mxcUri);
    bool downloadMedia(const std::string& mxcUri, const std::string& localPath);

    // User profile
    json getUserProfile(const std::string& userId);
    bool setDisplayName(const std::string& name);
    bool setAvatarUrl(const std::string& mxcUri);

    // Room state
    json getRoomState(const std::string& roomId);
    bool setRoomName(const std::string& roomId, const std::string& name);
    bool setRoomTopic(const std::string& roomId, const std::string& topic);
    json getRoomMembers(const std::string& roomId);

    // Room history
    json getRoomMessages(const std::string& roomId, const std::string& from = "",
                         int limit = 20, const std::string& dir = "b");
    json getContextForEvent(const std::string& roomId,
                            const std::string& eventId, int limit = 10);

    // Typing notifications
    bool sendTyping(const std::string& roomId, bool typing, int timeout = 30000);

    // Read receipts
    bool sendReadReceipt(const std::string& roomId, const std::string& eventId);

    // Presence
    bool setPresence(const std::string& presence, const std::string& statusMsg = "");

    // Search
    json searchMessages(const std::string& searchTerm,
                        const std::string& roomId = "",
                        int limit = 10);

    // Directory
    json getPublicRooms(const std::string& server = "", int limit = 50);

    // Account data
    bool setAccountData(const std::string& type, const json& data);
    json getAccountData(const std::string& type);

    // Notifications
    int getNotificationCount();
    json getPushRules();
    bool setPushRule(const std::string& scope, const std::string& kind,
                     const std::string& ruleId, const json& actions);

    // Device management
    json getDevices();
    bool deleteDevice(const std::string& deviceId);
    bool updateDeviceDisplayName(const std::string& deviceId,
                                  const std::string& name);

    // Content repository
    json getContentConfig();

    // End-to-end encryption (E2EE)
    bool uploadDeviceKeys(const json& deviceKeys);
    json claimOneTimeKeys(const std::string& userId, const std::string& deviceId,
                          const std::string& algorithm = "m.olm.v1.curve25519-aes-sha2");
    bool sendToDevice(const std::string& eventType, const std::string& txnId,
                      const json& messages);
    json getKeysChanges(const std::string& from, const std::string& to);

    // Callbacks
    void onEvent(std::function<void(const std::string& roomId, const json& event)> cb) {
        m_eventCallback = std::move(cb);
    }
    void onError(std::function<void(const std::string& error)> cb) {
        m_errorCallback = std::move(cb);
    }
    void onTokenRefresh(std::function<void(const std::string& newToken)> cb) {
        m_tokenRefreshCallback = std::move(cb);
    }

private:
    ClientConfig m_config;
    std::string m_userId;
    std::string m_accessToken;
    std::string m_deviceId;
    std::string m_nextBatch;
    bool m_running = false;

    // HTTP helpers
    json apiGet(const std::string& path, const std::map<std::string, std::string>& params = {});
    json apiPost(const std::string& path, const json& body);
    json apiPut(const std::string& path, const json& body);
    json apiDelete(const std::string& path);

    std::string buildUrl(const std::string& path,
                         const std::map<std::string, std::string>& params = {});
    json handleResponse(const std::string& response, int statusCode);

    // Callbacks
    std::function<void(const std::string&, const json&)> m_eventCallback;
    std::function<void(const std::string&)> m_errorCallback;
    std::function<void(const std::string&)> m_tokenRefreshCallback;
};

} // namespace matrix_sdk
