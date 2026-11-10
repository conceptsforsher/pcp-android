#include "pcp/tdlib_auth.hpp"
#include <android/log.h>
#define TAG "TdLibAuth"

namespace pcp {

static TlField str(const std::string& key, const std::string& val) {
    TlField f; f.key = key; f.isString = true; f.value = val; return f;
}

TdLibAuth::TdLibAuth(TdLibClient& c) : client(c) {
    client.setAuthCallback([this](const std::string& type, const TlObject& state) {
        handleAuthState(type, state);
    });
}

TdLibAuth::~TdLibAuth() {}

void TdLibAuth::setStateCallback(AuthStateCallback cb) {
    stateCallback = std::move(cb);
}

void TdLibAuth::sendPhone(const std::string& phone) {
    TlObject req;
    req.type = "setAuthenticationPhoneNumber";
    req.fields = {
        {"@type", str("@type", "setAuthenticationPhoneNumber")},
        {"phone_number", str("phone_number", phone)}
    };
    client.sendRequest(req);
}

void TdLibAuth::sendCode(const std::string& code) {
    TlObject req;
    req.type = "checkAuthenticationCode";
    req.fields = {
        {"@type", str("@type", "checkAuthenticationCode")},
        {"code", str("code", code)}
    };
    client.sendRequest(req);
}

void TdLibAuth::sendPassword(const std::string& password) {
    TlObject req;
    req.type = "checkAuthenticationPassword";
    req.fields = {
        {"@type", str("@type", "checkAuthenticationPassword")},
        {"password", str("password", password)}
    };
    client.sendRequest(req);
}

void TdLibAuth::requestQrCode() {
    TlObject req;
    req.type = "requestQrCodeAuthentication";
    req.fields = {
        {"@type", str("@type", "requestQrCodeAuthentication")},
    };
    client.sendRequest(req);
}

void TdLibAuth::logout() {
    TlObject req;
    req.type = "logOut";
    req.fields = {
        {"@type", str("@type", "logOut")},
    };
    client.sendRequest(req);
}

void TdLibAuth::handleAuthState(const std::string& type, const TlObject& state) {
    if (type == "authorizationStateWaitPhoneNumber") {
        step = AuthStep::WAITING_PHONE;
        if (stateCallback) stateCallback(step, lastCodeInfo, "Enter phone number");
    } else if (type == "authorizationStateWaitCode") {
        step = AuthStep::WAITING_CODE;
        TlObject codeInfo = state.optObject("code_info");
        lastCodeInfo.type = codeInfo.optString("type", "");
        lastCodeInfo.codeInfoJson = codeInfo.toJson();
        lastCodeInfo.nextType = codeInfo.optString("next_type", "");
        lastCodeInfo.timeout = codeInfo.optInt("timeout", 0);
        if (stateCallback) stateCallback(step, lastCodeInfo, "Enter code");
    } else if (type == "authorizationStateWaitPassword") {
        step = AuthStep::WAITING_PASSWORD;
        if (stateCallback) stateCallback(step, lastCodeInfo, "Enter password");
    } else if (type == "authorizationStateReady") {
        step = AuthStep::READY;
        if (stateCallback) stateCallback(step, lastCodeInfo, "Ready");
    } else if (type == "authorizationStateClosed") {
        step = AuthStep::CLOSED;
        if (stateCallback) stateCallback(step, lastCodeInfo, "Closed");
    }
}

} // namespace pcp
