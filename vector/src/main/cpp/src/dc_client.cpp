#include "progressive/dc_client.hpp"

#ifdef PROGRESSIVE_HAS_DELTACHAT

#include "progressive/dc_bridge.hpp"
#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace progressive {
namespace deltachat {

static std::string toString(const char* str) {
    if (!str) return "";
    std::string result(str);
    return result;
}

static int64_t toInt64(const std::string& s) {
    try { return std::stoll(s); } catch (...) { return 0; }
}

static int32_t toInt32(const std::string& s) {
    try { return std::stoi(s); } catch (...) { return 0; }
}

static std::string extractStr(const std::string& json, const std::string& key) {
    std::string search = "\"" + key + "\":";
    auto pos = json.find(search);
    if (pos == std::string::npos) return "";
    pos += search.length();
    bool isString = (pos < json.size() && json[pos] == '"');
    if (isString) {
        pos++;
        auto end = json.find('"', pos);
        if (end == std::string::npos) return "";
        return json.substr(pos, end - pos);
    } else {
        while (pos < json.size() && json[pos] == ' ') pos++;
        auto end = std::min(static_cast<size_t>(json.find_first_of(", \t\n\r}", pos)), json.size());
        return json.substr(pos, end - pos);
    }
}

DcNativeClient::DcNativeClient() = default;

DcNativeClient::~DcNativeClient() {
    destroy();
}

bool DcNativeClient::init() {
    if (ctx_) return true;
    if (!load_dc_api(api_)) return false;
    ctx_ = api_.create("progressive-android", nullptr);
    if (!ctx_) return false;
    api_.open(ctx_);
    return true;
}

void DcNativeClient::destroy() {
    running_ = false;
    if (event_thread_.joinable()) {
        event_thread_.join();
    }
    if (ctx_) {
        api_.stop_io(ctx_);
        api_.close(ctx_);
        api_.unref(ctx_);
        ctx_ = nullptr;
    }
}

bool DcNativeClient::configure(const std::string& addr, const std::string& password) {
    if (!ctx_) return false;
    api_.set_config(ctx_, "addr", addr.c_str());
    api_.set_config(ctx_, "mail_pw", password.c_str());
    api_.configure(ctx_);
    return true;
}

bool DcNativeClient::isConfigured() const {
    if (!ctx_) return false;
    return api_.is_configured(ctx_) != 0;
}

std::string DcNativeClient::getInfo() const {
    if (!ctx_) return "{}";
    return toString(api_.get_info(ctx_));
}

int64_t DcNativeClient::sendTextMessage(int64_t chatId, const std::string& text) {
    if (!ctx_) return 0;
    return api_.send_text_msg(ctx_, chatId, text.c_str());
}

std::vector<DcChat> DcNativeClient::getChats(int flags, const std::string& query, int contactId) {
    std::vector<DcChat> result;
    if (!ctx_) return result;

    auto* list = api_.get_chatlist(ctx_, flags, query.empty() ? nullptr : query.c_str(), contactId);
    if (!list) return result;

    int count = api_.get_chatlist_cnt(list);
    for (int i = 0; i < count; i++) {
        int64_t chatId = api_.get_chatlist_id(list, i);
        auto chatJson = toString(api_.get_chat(ctx_, chatId));
        DcChat chat = parseChatJson(chatJson);
        if (chat.id == 0) chat.id = chatId;
        result.push_back(chat);
    }
    api_.chatlist_unref(list);
    return result;
}

std::vector<DcMessage> DcNativeClient::getMessages(int64_t chatId, int flags, int marker) {
    std::vector<DcMessage> result;
    if (!ctx_) return result;

    auto* array = api_.get_chat_msgs(ctx_, chatId, flags, marker);
    if (!array) return result;

    int count = api_.get_chat_msgs_cnt(array);
    for (int i = 0; i < count; i++) {
        int64_t msgId = api_.get_chat_msgs_id(array, i);
        auto json = toString(api_.get_msg(ctx_, msgId));
        DcMessage msg = parseMessageJson(json);
        if (msg.id == 0) msg.id = msgId;
        result.push_back(msg);
    }
    api_.array_unref(array);
    return result;
}

std::vector<DcContact> DcNativeClient::getContacts(int flags, const std::string& query) {
    std::vector<DcContact> result;
    if (!ctx_) return result;

    auto* array = api_.get_contacts(ctx_, flags, query.empty() ? nullptr : query.c_str());
    if (!array) return result;

    int count = api_.get_chat_msgs_cnt(array);
    for (int i = 0; i < count; i++) {
        int64_t contactId = api_.get_chat_msgs_id(array, i);
        auto json = toString(api_.get_contact(ctx_, contactId));
        result.push_back(parseContactJson(json));
    }
    api_.array_unref(array);
    return result;
}

DcContact DcNativeClient::getContact(int64_t contactId) {
    if (!ctx_) return {};
    auto json = toString(api_.get_contact(ctx_, contactId));
    return parseContactJson(json);
}

int64_t DcNativeClient::createContact(const std::string& name, const std::string& addr) {
    if (!ctx_) return 0;
    return api_.create_contact(ctx_, name.empty() ? nullptr : name.c_str(), addr.c_str());
}

int DcNativeClient::blockContact(int64_t contactId, int block) {
    if (!ctx_) return 0;
    return api_.block_contact(ctx_, contactId, block);
}

int64_t DcNativeClient::createGroupChat(int verified, const std::string& name) {
    if (!ctx_) return 0;
    return api_.create_group_chat(ctx_, verified, name.c_str());
}

int64_t DcNativeClient::addContactToChat(int64_t chatId, int64_t contactId) {
    if (!ctx_) return 0;
    return api_.add_contact_to_chat(ctx_, chatId, contactId);
}

int64_t DcNativeClient::removeContactFromChat(int64_t chatId, int64_t contactId) {
    if (!ctx_) return 0;
    return api_.remove_contact_from_chat(ctx_, chatId, contactId);
}

int DcNativeClient::setChatName(int64_t chatId, const std::string& name) {
    if (!ctx_) return 0;
    return api_.set_chat_name(ctx_, chatId, name.c_str());
}

void DcNativeClient::markSeenMsgs(const std::vector<int64_t>&) {
    if (!ctx_) return;
}

void DcNativeClient::deleteMsgs(const std::vector<int64_t>&) {
    if (!ctx_) return;
}

void DcNativeClient::deleteChat(int64_t chatId) {
    if (!ctx_) return;
    api_.delete_chat(ctx_, chatId);
}

void DcNativeClient::startIo() {
    if (!ctx_) return;
    api_.start_io(ctx_);
    running_ = true;
    event_thread_ = std::thread(&DcNativeClient::eventLoop, this);
}

void DcNativeClient::stopIo() {
    running_ = false;
    if (ctx_) api_.stop_io(ctx_);
    if (event_thread_.joinable()) event_thread_.join();
}

void DcNativeClient::maybeNetwork() {
    if (ctx_) api_.maybe_network(ctx_);
}

void DcNativeClient::setEventCallback(EventCallback cb) {
    event_callback_ = std::move(cb);
}

int DcNativeClient::getConnectivity() const {
    if (!ctx_) return 0;
    return api_.get_connectivity(ctx_);
}

std::string DcNativeClient::getSecureJoinQr(int64_t chatId) {
    if (!ctx_) return "";
    return toString(api_.get_securejoin_qr(ctx_, chatId));
}

int DcNativeClient::joinSecureJoin(const std::string& qr) {
    if (!ctx_) return 0;
    return api_.join_securejoin(ctx_, qr.c_str());
}

int DcNativeClient::checkQr(const std::string& qr) {
    if (!ctx_) return 0;
    return api_.check_qr(ctx_, qr.c_str());
}

std::string DcNativeClient::imex(int what, const std::string& param1, const std::string& param2) {
    if (!ctx_) return "";
    return toString(api_.imex(ctx_, what,
        param1.empty() ? nullptr : param1.c_str(),
        param2.empty() ? nullptr : param2.c_str()));
}

std::string DcNativeClient::initiateKeyTransfer() {
    if (!ctx_) return "";
    return toString(api_.initiate_key_transfer(ctx_));
}

void DcNativeClient::eventLoop() {
    while (running_) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        if (!ctx_ || !event_callback_) continue;

        auto* event = api_.get_next_event(ctx_);
        if (!event) continue;

        DcEvent ev;
        ev.id = api_.event_get_id(event);
        ev.data1 = api_.event_get_data1_int(event);
        ev.data2 = api_.event_get_data2_int(event);

        if (ev.id != 0) {
            event_callback_(ev);
        }
        api_.event_unref(event);
    }
}

DcChat DcNativeClient::parseChatJson(const std::string& json) const {
    DcChat chat;
    chat.id = toInt64(extractStr(json, "id"));
    chat.name = extractStr(json, "name");
    chat.type = toInt32(extractStr(json, "type"));
    chat.unread_count = toInt32(extractStr(json, "unread_count"));
    chat.is_protected = toInt32(extractStr(json, "is_protected"));
    chat.is_verified = toInt32(extractStr(json, "is_verified"));
    chat.profile_image = extractStr(json, "profile_image");
    chat.color = extractStr(json, "color");
    return chat;
}

DcMessage DcNativeClient::parseMessageJson(const std::string& json) const {
    DcMessage msg;
    msg.id = toInt64(extractStr(json, "id"));
    msg.chat_id = toInt64(extractStr(json, "chat_id"));
    msg.from_id = toInt64(extractStr(json, "from_id"));
    msg.view_type = toInt32(extractStr(json, "view_type"));
    msg.state = toInt32(extractStr(json, "state"));
    msg.is_info = toInt32(extractStr(json, "is_info"));
    msg.timestamp = toInt64(extractStr(json, "timestamp"));
    msg.text = extractStr(json, "text");
    msg.sender_name = extractStr(json, "override_sender_name");
    msg.is_outgoing = (msg.from_id == 1) ? 1 : 0;
    return msg;
}

DcContact DcNativeClient::parseContactJson(const std::string& json) const {
    DcContact contact;
    contact.id = toInt64(extractStr(json, "id"));
    contact.name = extractStr(json, "display_name");
    contact.address = extractStr(json, "address");
    contact.profile_image = extractStr(json, "profile_image");
    contact.color = extractStr(json, "color");
    return contact;
}

} // namespace deltachat
} // namespace progressive

#endif // PROGRESSIVE_HAS_DELTACHAT
