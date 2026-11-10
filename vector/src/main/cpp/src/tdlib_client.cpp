#include "pcp/tdlib_client.hpp"
#include "pcp/tdlib_bridge.hpp"
#include <android/log.h>
#include <cctype>
#include <sstream>
#include <cstring>

#define TAG "TdLibClient"

namespace pcp {

static std::string escapeJson(const std::string& s) {
    std::string out;
    out.reserve(s.size() + 16);
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n"; break;
            case '\r': out += "\\r"; break;
            case '\t': out += "\\t"; break;
            default:   out += c; break;
        }
    }
    return out;
}

static void writeJson(std::ostringstream& ss, const TlObject& obj);
static void writeField(std::ostringstream& ss, const TlField& f);

TlObject::TlObject(const std::string& json) {
    *this = fromJson(json);
}

std::string TlObject::optString(const std::string& key, const std::string& def) const {
    for (auto& f : fields) {
        if (f.first == key && f.second.isString) return f.second.value;
    }
    return def;
}

int TlObject::optInt(const std::string& key, int def) const {
    for (auto& f : fields) {
        if (f.first == key && f.second.isInt) return f.second.intValue;
    }
    return def;
}

long long TlObject::optLong(const std::string& key, long long def) const {
    for (auto& f : fields) {
        if (f.first == key) {
            if (f.second.isLong) return f.second.longValue;
            if (f.second.isInt) return (long long)f.second.intValue;
        }
    }
    return def;
}

double TlObject::optDouble(const std::string& key, double def) const {
    for (auto& f : fields) {
        if (f.first == key && f.second.isDouble) return f.second.doubleValue;
    }
    return def;
}

bool TlObject::optBool(const std::string& key, bool def) const {
    for (auto& f : fields) {
        if (f.first == key && f.second.isBool) return f.second.boolValue;
    }
    return def;
}

TlObject TlObject::optObject(const std::string& key) const {
    for (auto& f : fields) {
        if (f.first == key && f.second.isObject) {
            TlObject obj;
            obj.fields = f.second.objectFields;
            obj.type = key;
            return obj;
        }
    }
    return TlObject{};
}

std::vector<TlObject> TlObject::optArray(const std::string& key) const {
    for (auto& f : fields) {
        if (f.first == key && f.second.isArray) {
            std::vector<TlObject> result;
            for (auto& item : f.second.arrayValues) {
                if (item.isObject) {
                    TlObject obj;
                    obj.fields = item.objectFields;
                    result.push_back(obj);
                }
            }
            return result;
        }
    }
    return {};
}

bool TlObject::has(const std::string& key) const {
    for (auto& f : fields) {
        if (f.first == key) return true;
    }
    return false;
}

static void writeJson(std::ostringstream& ss, const TlObject& obj) {
    ss << "{";
    bool first = true;
    for (auto& f : obj.fields) {
        if (!first) ss << ",";
        first = false;
        ss << "\"" << escapeJson(f.first) << "\":";
        writeField(ss, f.second);
    }
    ss << "}";
}

static void writeField(std::ostringstream& ss, const TlField& f) {
    if (f.isNull) { ss << "null"; return; }
    if (f.isString) { ss << "\"" << escapeJson(f.value) << "\""; return; }
    if (f.isInt) { ss << f.intValue; return; }
    if (f.isLong) { ss << f.longValue; return; }
    if (f.isDouble) { ss << f.doubleValue; return; }
    if (f.isBool) { ss << (f.boolValue ? "true" : "false"); return; }
    if (f.isArray) {
        ss << "[";
        bool fst = true;
        for (auto& item : f.arrayValues) {
            if (!fst) ss << ",";
            fst = false;
            writeField(ss, item);
        }
        ss << "]";
        return;
    }
    if (f.isObject) {
        TlObject tmp;
        tmp.fields = f.objectFields;
        writeJson(ss, tmp);
        return;
    }
    ss << "null";
}

std::string TlObject::toJson() const {
    std::ostringstream ss;
    writeJson(ss, *this);
    return ss.str();
}

static void skipWhitespace(const char*& p) {
    while (*p && (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r')) p++;
}

static std::string parseString(const char*& p) {
    std::string result;
    if (*p != '"') return result;
    p++; // skip opening quote
    while (*p && *p != '"') {
        if (*p == '\\') {
            p++;
            if (*p == '"') result += '"';
            else if (*p == '\\') result += '\\';
            else if (*p == 'n') result += '\n';
            else if (*p == 'r') result += '\r';
            else if (*p == 't') result += '\t';
        } else {
            result += *p;
        }
        p++;
    }
    if (*p == '"') p++;
    return result;
}

static TlField parseValue(const char*& p);

static TlField parseObject(const char*& p) {
    TlField field;
    field.isObject = true;
    p++; // skip {

    while (*p) {
        skipWhitespace(p);
        if (*p == '}') { p++; break; }
        if (*p == ',') { p++; skipWhitespace(p); }

        std::string key = parseString(p);
        skipWhitespace(p);
        if (*p == ':') p++;
        skipWhitespace(p);

        TlField value = parseValue(p);
        value.key = key;
        field.objectFields.push_back({key, value});
    }
    return field;
}

static TlField parseArray(const char*& p) {
    TlField field;
    field.isArray = true;
    p++; // skip [

    while (*p) {
        skipWhitespace(p);
        if (*p == ']') { p++; break; }
        if (*p == ',') { p++; skipWhitespace(p); }
        field.arrayValues.push_back(parseValue(p));
    }
    return field;
}

static TlField parseValue(const char*& p) {
    skipWhitespace(p);
    TlField field;

    if (*p == '"') {
        field.isString = true;
        field.value = parseString(p);
    } else if (*p == '{') {
        return parseObject(p);
    } else if (*p == '[') {
        return parseArray(p);
    } else if (*p == 't' || *p == 'f') {
        field.isBool = true;
        field.boolValue = (*p == 't');
        while (*p && *p != ',' && *p != '}' && *p != ']' && !isspace(*p)) p++;
    } else if (*p == 'n') {
        field.isNull = true;
        while (*p && *p != ',' && *p != '}' && *p != ']' && !isspace(*p)) p++;
    } else if (*p == '-' || (*p >= '0' && *p <= '9')) {
        std::string num;
        bool hasDot = false;
        while (*p && (*p == '-' || *p == '.' || (*p >= '0' && *p <= '9') || *p == 'e' || *p == 'E' || *p == '+')) {
            if (*p == '.' || *p == 'e' || *p == 'E') hasDot = true;
            num += *p;
            p++;
        }
        if (hasDot) {
            field.isDouble = true;
            field.doubleValue = std::stod(num);
        } else {
            long long val = std::stoll(num);
            if (val > 2147483647LL || val < -2147483648LL) {
                field.isLong = true;
                field.longValue = val;
            } else {
                field.isInt = true;
                field.intValue = (int)val;
            }
        }
    } else {
        // unknown — skip
        while (*p && *p != ',' && *p != '}' && *p != ']' && !isspace(*p)) p++;
    }

    return field;
}

TlObject TlObject::fromJson(const std::string& json) {
    TlObject obj;
    const char* p = json.c_str();
    skipWhitespace(p);
    if (*p == '{') {
        TlField field = parseObject(p);
        obj.fields = field.objectFields;
        for (auto& f : obj.fields) {
            if (f.first == "@type" && f.second.isString) {
                obj.type = f.second.value;
            }
        }
    }
    return obj;
}

TdLibClient::TdLibClient(int apiId_, const std::string& apiHash_,
                         const std::string& dbDir, const std::string& filesDir)
    : apiId(apiId_), apiHash(apiHash_), databaseDir(dbDir), filesDir(filesDir) {
}

TdLibClient::~TdLibClient() {
    close();
}

TlObject TdLibClient::buildParams(int apiId, const std::string& apiHash,
                                  const std::string& dbDir, const std::string& filesDir) {
    TlObject params;
    TlField type, db, files, useDb, useChatDb, useMsgDb, useSecret;
    TlField apiField, hashField, lang, device, appVer, storage, ignore;

    type.key = "@type"; type.isString = true; type.value = "setTdlibParameters";
    db.key = "database_directory"; db.isString = true; db.value = dbDir;
    files.key = "files_directory"; files.isString = true; files.value = filesDir;
    useDb.key = "use_file_database"; useDb.isBool = true; useDb.boolValue = true;
    useChatDb.key = "use_chat_info_database"; useChatDb.isBool = true; useChatDb.boolValue = true;
    useMsgDb.key = "use_message_database"; useMsgDb.isBool = true; useMsgDb.boolValue = true;
    useSecret.key = "use_secret_chats"; useSecret.isBool = true; useSecret.boolValue = false;
    apiField.key = "api_id"; apiField.isInt = true; apiField.intValue = apiId;
    hashField.key = "api_hash"; hashField.isString = true; hashField.value = apiHash;
    lang.key = "system_language_code"; lang.isString = true; lang.value = "en";
    device.key = "device_model"; device.isString = true; device.value = "Android";
    appVer.key = "application_version"; appVer.isString = true; appVer.value = "1.0.0";
    storage.key = "enable_storage_optimizer"; storage.isBool = true; storage.boolValue = true;
    ignore.key = "ignore_file_names"; ignore.isBool = true; ignore.boolValue = false;

    params.type = "setTdlibParameters";
    params.fields = {
        {"@type", type}, {"database_directory", db}, {"files_directory", files},
        {"use_file_database", useDb}, {"use_chat_info_database", useChatDb},
        {"use_message_database", useMsgDb}, {"use_secret_chats", useSecret},
        {"api_id", apiField}, {"api_hash", hashField},
        {"system_language_code", lang}, {"device_model", device},
        {"application_version", appVer}, {"enable_storage_optimizer", storage},
        {"ignore_file_names", ignore}
    };
    return params;
}

void TdLibClient::initialize() {
#ifdef PCP_HAS_TDLIB
    clientPtr = td_json_client_create();
    if (!clientPtr) {
        __android_log_print(ANDROID_LOG_ERROR, TAG, "Failed to create TDLib client");
        return;
    }
    __android_log_print(ANDROID_LOG_INFO, TAG, "TDLib client created: %p", clientPtr);

    sendRequest(buildParams(apiId, apiHash, databaseDir, filesDir));
    startReceiveLoop();
#endif
}

void TdLibClient::close() {
    running = false;
    if (receiveThread.joinable()) {
        receiveThread.join();
    }
#ifdef PCP_HAS_TDLIB
    if (clientPtr) {
        td_json_client_destroy(clientPtr);
        clientPtr = nullptr;
    }
#endif
    ready = false;
}

void TdLibClient::sendRequest(const TlObject& request) {
#ifdef PCP_HAS_TDLIB
    if (!clientPtr) return;
    auto json = request.toJson();
    __android_log_print(ANDROID_LOG_DEBUG, TAG, "send: %s", json.substr(0, 200).c_str());
    td_json_client_send(clientPtr, json.c_str());
#endif
}

TlObject TdLibClient::execute(const TlObject& request) {
#ifdef PCP_HAS_TDLIB
    auto json = request.toJson();
    const char* result = td_json_client_execute(clientPtr, json.c_str());
    if (result && result[0]) {
        return TlObject::fromJson(std::string(result));
    }
#endif
    return TlObject{};
}

void TdLibClient::setAuthCallback(std::function<void(const std::string&, const TlObject&)> cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    authCallback = std::move(cb);
}

void TdLibClient::setConnectionCallback(std::function<void(const std::string&)> cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    connectionCallback = std::move(cb);
}

void TdLibClient::setUpdateCallback(std::function<void(const std::string&, const TlObject&)> cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    updateCallback = std::move(cb);
}

void TdLibClient::setResponseCallback(std::function<void(const std::string&, const TlObject&)> cb) {
    std::lock_guard<std::mutex> lock(callbackMutex);
    responseCallback = std::move(cb);
}

void TdLibClient::startReceiveLoop() {
    running = true;
    receiveThread = std::thread([this]() {
        while (running) {
#ifdef PCP_HAS_TDLIB
            const char* resp = td_json_client_receive(clientPtr, 1.0);
            if (resp && resp[0]) {
                TlObject json = TlObject::fromJson(std::string(resp));
                handleResponse(json);
            }
#else
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
#endif
            if (!running) break;
        }
    });
}

void TdLibClient::handleResponse(const TlObject& json) {
    std::string type;
    for (auto& f : json.fields) {
        if (f.first == "@type" && f.second.isString) {
            type = f.second.value;
            break;
        }
    }

    if (type == "updateAuthorizationState") {
        TlObject state = json.optObject("authorization_state");
        std::string stateType;
        for (auto& f : state.fields) {
            if (f.first == "@type" && f.second.isString) {
                stateType = f.second.value;
                break;
            }
        }

        if (stateType == "authorizationStateReady") {
            ready = true;
        } else if (stateType == "authorizationStateClosed") {
            ready = false;
        }

        std::lock_guard<std::mutex> lock(callbackMutex);
        if (authCallback) authCallback(stateType, state);
        return;
    }

    if (type == "updateConnectionState") {
        TlObject state = json.optObject("state");
        std::string connState;
        for (auto& f : state.fields) {
            if (f.first == "@type" && f.second.isString) {
                connState = f.second.value;
                break;
            }
        }
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (connectionCallback) connectionCallback(connState);
        return;
    }

    if (type.find("update") == 0) {
        if (type == "updateUserStatus") {
            __android_log_print(ANDROID_LOG_DEBUG, TAG, "User status update");
        }
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (updateCallback) updateCallback(type, json);
    } else {
        std::lock_guard<std::mutex> lock(callbackMutex);
        if (responseCallback) responseCallback(type, json);
    }
}

} // namespace pcp
