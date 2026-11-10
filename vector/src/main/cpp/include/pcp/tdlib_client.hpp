#ifndef PCP_TDLIB_CLIENT_HPP
#define PCP_TDLIB_CLIENT_HPP

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <mutex>
#include <queue>
#include <condition_variable>
#include <memory>

namespace pcp {

struct TlField {
    std::string key;
    std::string value;
    int intValue = 0;
    long long longValue = 0;
    double doubleValue = 0.0;
    bool boolValue = false;
    std::vector<TlField> arrayValues;
    std::vector<std::pair<std::string, TlField>> objectFields;
    bool isNull = false;
    bool isArray = false;
    bool isObject = false;
    bool isString = false;
    bool isInt = false;
    bool isLong = false;
    bool isDouble = false;
    bool isBool = false;
};

class TlObject {
public:
    std::string type;
    std::vector<std::pair<std::string, TlField>> fields;

    TlObject() = default;
    explicit TlObject(const std::string& json);

    std::string optString(const std::string& key, const std::string& def = "") const;
    int optInt(const std::string& key, int def = 0) const;
    long long optLong(const std::string& key, long long def = 0) const;
    double optDouble(const std::string& key, double def = 0.0) const;
    bool optBool(const std::string& key, bool def = false) const;
    TlObject optObject(const std::string& key) const;
    std::vector<TlObject> optArray(const std::string& key) const;
    bool has(const std::string& key) const;

    std::string toJson() const;
    static TlObject fromJson(const std::string& json);
};

using ResponseCallback = std::function<void(const TlObject& resp)>;

class TdLibClient {
public:
    TdLibClient(int apiId, const std::string& apiHash,
                const std::string& databaseDir, const std::string& filesDir);
    ~TdLibClient();

    TdLibClient(const TdLibClient&) = delete;
    TdLibClient& operator=(const TdLibClient&) = delete;

    void initialize();
    void close();

    void sendRequest(const TlObject& request);
    TlObject execute(const TlObject& request);

    void setAuthCallback(std::function<void(const std::string& type, const TlObject& state)> cb);
    void setConnectionCallback(std::function<void(const std::string& state)> cb);
    void setUpdateCallback(std::function<void(const std::string& type, const TlObject& data)> cb);
    void setResponseCallback(std::function<void(const std::string& type, const TlObject& data)> cb);

    bool isLoaded() const { return clientPtr != nullptr; }
    bool isReady() const { return ready; }
    std::string getUserId() const { return userId; }

private:
    void startReceiveLoop();
    void stopReceiveLoop();
    void handleResponse(const TlObject& json);

    void* clientPtr = nullptr;
    std::thread receiveThread;
    std::atomic<bool> running{false};
    std::atomic<bool> ready{false};

    int apiId;
    std::string apiHash;
    std::string databaseDir;
    std::string filesDir;
    std::string userId;

    std::function<void(const std::string&, const TlObject&)> authCallback;
    std::function<void(const std::string&)> connectionCallback;
    std::function<void(const std::string&, const TlObject&)> updateCallback;
    std::function<void(const std::string&, const TlObject&)> responseCallback;

    std::mutex sendMutex;
    std::mutex callbackMutex;

    static TlObject buildParams(int apiId, const std::string& apiHash,
                                const std::string& dbDir, const std::string& filesDir);
};

} // namespace pcp

#endif // PCP_TDLIB_CLIENT_HPP
