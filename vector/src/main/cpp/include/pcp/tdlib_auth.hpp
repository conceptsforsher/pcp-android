#ifndef PCP_TDLIB_AUTH_HPP
#define PCP_TDLIB_AUTH_HPP

#include "pcp/tdlib_client.hpp"
#include <functional>
#include <string>

namespace pcp {

enum class AuthStep {
    WAITING_PHONE,
    WAITING_CODE,
    WAITING_PASSWORD,
    READY,
    CLOSED,
    ERROR
};

struct AuthCodeInfo {
    std::string type;
    std::string codeInfoJson;
    std::string nextType;
    int timeout = 0;
};

using AuthStateCallback = std::function<void(AuthStep step, AuthCodeInfo info, std::string hint)>;

class TdLibAuth {
public:
    explicit TdLibAuth(TdLibClient& client);
    ~TdLibAuth();

    void setStateCallback(AuthStateCallback cb);
    AuthStep currentStep() const { return step; }
    bool isReady() const { return step == AuthStep::READY; }

    void sendPhone(const std::string& phone);
    void sendCode(const std::string& code);
    void sendPassword(const std::string& password);
    void requestQrCode();
    void logout();

    void handleAuthState(const std::string& type, const TlObject& state);

private:
    TdLibClient& client;
    AuthStep step = AuthStep::WAITING_PHONE;
    AuthCodeInfo lastCodeInfo;
    AuthStateCallback stateCallback;
};

} // namespace pcp
#endif
