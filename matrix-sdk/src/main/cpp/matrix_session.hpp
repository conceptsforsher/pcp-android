#pragma once
#include <string>
#include <vector>
#include <memory>
#include <nlohmann/json.hpp>
#include "pcp/event_models.hpp"
#include "pcp/sync_models.hpp"

namespace matrix_sdk {

using json = nlohmann::json;

struct SessionData {
    std::string userId;
    std::string accessToken;
    std::string deviceId;
    std::string homeserverUrl;
    std::string identityServerUrl;
    std::string refreshToken;
    bool isTokenLogin = false;
};

struct DeviceInfo {
    std::string deviceId;
    std::string displayName;
    std::string lastSeenIp;
    int64_t lastSeenTs = 0;
    bool verified = false;
};

class MatrixSession {
public:
    MatrixSession(const SessionData& data);
    ~MatrixSession();

    const std::string& userId() const { return m_data.userId; }
    const std::string& accessToken() const { return m_data.accessToken; }
    const std::string& deviceId() const { return m_data.deviceId; }

    void setAccessToken(const std::string& token) { m_data.accessToken = token; }
    void setRefreshToken(const std::string& token) { m_data.refreshToken = token; }

    bool saveToDisk(const std::string& path);
    static std::unique_ptr<MatrixSession> loadFromDisk(const std::string& path);

    std::vector<DeviceInfo> getDevices();
    bool deleteDevice(const std::string& deviceId);
    bool updateDeviceDisplayName(const std::string& deviceId, const std::string& name);

    void setPresence(const std::string& presence, const std::string& statusMsg = "");
    json getPresence(const std::string& userId);

    json getProfile(const std::string& userId);
    bool setDisplayName(const std::string& name);
    bool setAvatarUrl(const std::string& mxcUri);

    bool uploadDeviceKeys(const json& keys, const json& oneTimeKeys = {});
    json claimKeys(const std::vector<std::string>& userIds);

private:
    SessionData m_data;
    json m_deviceKeys;
};

} // namespace matrix_sdk
