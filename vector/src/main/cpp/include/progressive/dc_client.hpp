#pragma once

#ifdef PROGRESSIVE_HAS_DELTACHAT

#include "progressive/dc_bridge.hpp"
#include "progressive/dc_types.hpp"
#include <string>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>
#include <chrono>

namespace progressive {
namespace deltachat {

class DcNativeClient {
public:
    using EventCallback = std::function<void(const DcEvent&)>;

    DcNativeClient();
    ~DcNativeClient();

    bool init();
    void destroy();

    bool configure(const std::string& addr, const std::string& password);
    bool isConfigured() const;
    std::string getInfo() const;

    int64_t sendTextMessage(int64_t chatId, const std::string& text);

    std::vector<DcChat> getChats(int flags, const std::string& query, int contactId);
    std::vector<DcMessage> getMessages(int64_t chatId, int flags, int marker);
    std::vector<DcContact> getContacts(int flags, const std::string& query);
    DcContact getContact(int64_t contactId);

    int64_t createContact(const std::string& name, const std::string& addr);
    int blockContact(int64_t contactId, int block);

    int64_t createGroupChat(int verified, const std::string& name);
    int64_t addContactToChat(int64_t chatId, int64_t contactId);
    int64_t removeContactFromChat(int64_t chatId, int64_t contactId);
    int setChatName(int64_t chatId, const std::string& name);

    void markSeenMsgs(const std::vector<int64_t>& msgIds);
    void deleteMsgs(const std::vector<int64_t>& msgIds);
    void deleteChat(int64_t chatId);

    void startIo();
    void stopIo();
    void maybeNetwork();

    void setEventCallback(EventCallback cb);

    int getConnectivity() const;

    std::string getSecureJoinQr(int64_t chatId);
    int joinSecureJoin(const std::string& qr);
    int checkQr(const std::string& qr);

    std::string imex(int what, const std::string& param1, const std::string& param2);
    std::string initiateKeyTransfer();

private:
    void eventLoop();
    DcChat parseChatJson(const std::string& json) const;
    DcMessage parseMessageJson(const std::string& json) const;
    DcContact parseContactJson(const std::string& json) const;

    dc_context_t* ctx_ = nullptr;
    DcApi api_;
    std::atomic<bool> running_{false};
    std::thread event_thread_;
    EventCallback event_callback_;
    mutable std::mutex mutex_;
};

} // namespace deltachat
} // namespace progressive

#endif // PROGRESSIVE_HAS_DELTACHAT
