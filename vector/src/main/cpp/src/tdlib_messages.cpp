#include "pcp/tdlib_messages.hpp"
#include <android/log.h>
#include <sstream>
#define TAG "TdLibMessages"

namespace pcp {

static TlField str(const std::string& key, const std::string& val) {
    TlField f; f.key = key; f.isString = true; f.value = val; return f;
}
static TlField num(const std::string& key, int val) {
    TlField f; f.key = key; f.isInt = true; f.intValue = val; return f;
}
static TlField big(const std::string& key, long long val) {
    TlField f; f.key = key; f.isLong = true; f.longValue = val; return f;
}
static TlField boo(const std::string& key, bool val) {
    TlField f; f.key = key; f.isBool = true; f.boolValue = val; return f;
}
static TlField dbl(const std::string& key, double val) {
    TlField f; f.key = key; f.isDouble = true; f.doubleValue = val; return f;
}
static TlField arr(const std::string& key, std::vector<TlField> vals) {
    TlField f; f.key = key; f.isArray = true; f.arrayValues = std::move(vals); return f;
}
static TlField obj(const std::string& key, std::vector<std::pair<std::string,TlField>> fields) {
    TlField f; f.key = key; f.isObject = true; f.objectFields = std::move(fields); return f;
}
static TlField nullField(const std::string& key) {
    TlField f; f.key = key; f.isNull = true; return f;
}

TdLibMessages::TdLibMessages(TdLibClient& c) : client(c) {}

TdLibMessages::~TdLibMessages() {}

void TdLibMessages::addProxy(const std::string& server, int port, bool enable, const std::string& proxyType,
                              const std::string& username, const std::string& password, const std::string& secret) {
    TlObject req;
    req.type = "addProxy";
    std::vector<std::pair<std::string,TlField>> typeFields;
    if (proxyType == "mtproto") {
        typeFields = {{"@type", str("@type", "proxyTypeMtproto")}, {"secret", str("secret", secret)}};
    } else if (proxyType == "http") {
        typeFields = {{"@type", str("@type", "proxyTypeHttp")}};
        if (!username.empty()) typeFields.push_back({"username", str("username", username)});
        if (!password.empty()) typeFields.push_back({"password", str("password", password)});
    } else {
        typeFields = {{"@type", str("@type", "proxyTypeSocks5")}};
        if (!username.empty()) typeFields.push_back({"username", str("username", username)});
        if (!password.empty()) typeFields.push_back({"password", str("password", password)});
    }
    req.fields = {
        {"@type", str("@type", "addProxy")},
        {"server", str("server", server)},
        {"port", num("port", port)},
        {"enable", boo("enable", enable)},
        {"type", obj("type", typeFields)}
    };
    client.sendRequest(req);
}

void TdLibMessages::editProxy(int proxyId, const std::string& server, int port, bool enable,
                               const std::string& proxyType) {
    TlObject req;
    req.type = "editProxy";
    std::vector<std::pair<std::string,TlField>> typeFields;
    if (proxyType == "mtproto") {
        typeFields = {{"@type", str("@type", "proxyTypeMtproto")}};
    } else if (proxyType == "http") {
        typeFields = {{"@type", str("@type", "proxyTypeHttp")}};
    } else {
        typeFields = {{"@type", str("@type", "proxyTypeSocks5")}};
    }
    req.fields = {
        {"@type", str("@type", "editProxy")},
        {"proxy_id", num("proxy_id", proxyId)},
        {"server", str("server", server)},
        {"port", num("port", port)},
        {"enable", boo("enable", enable)},
        {"type", obj("type", typeFields)}
    };
    client.sendRequest(req);
}

void TdLibMessages::enableProxy(int proxyId) {
    TlObject req;
    req.type = "enableProxy";
    req.fields = {{"@type", str("@type", "enableProxy")}, {"proxy_id", num("proxy_id", proxyId)}};
    client.sendRequest(req);
}

void TdLibMessages::disableProxy() {
    TlObject req;
    req.type = "disableProxy";
    req.fields = {{"@type", str("@type", "disableProxy")}};
    client.sendRequest(req);
}

void TdLibMessages::removeProxy(int proxyId) {
    TlObject req;
    req.type = "removeProxy";
    req.fields = {{"@type", str("@type", "removeProxy")}, {"proxy_id", num("proxy_id", proxyId)}};
    client.sendRequest(req);
}

void TdLibMessages::getProxies() {
    TlObject req;
    req.type = "getProxies";
    req.fields = {{"@type", str("@type", "getProxies")}};
    client.sendRequest(req);
}

void TdLibMessages::pingProxy(int proxyId) {
    TlObject req;
    req.type = "pingProxy";
    req.fields = {{"@type", str("@type", "pingProxy")}, {"proxy_id", num("proxy_id", proxyId)}};
    client.sendRequest(req);
}

void TdLibMessages::testProxy(const std::string& server, int port, const std::string& proxyType,
                               int dcId, double timeout) {
    TlObject req;
    req.type = "testProxy";
    std::vector<std::pair<std::string,TlField>> typeFields;
    if (proxyType == "mtproto") {
        typeFields = {{"@type", str("@type", "proxyTypeMtproto")}};
    } else if (proxyType == "http") {
        typeFields = {{"@type", str("@type", "proxyTypeHttp")}};
    } else {
        typeFields = {{"@type", str("@type", "proxyTypeSocks5")}};
    }
    req.fields = {
        {"@type", str("@type", "testProxy")},
        {"server", str("server", server)},
        {"port", num("port", port)},
        {"type", obj("type", typeFields)},
        {"dc_id", num("dc_id", dcId)},
        {"timeout", dbl("timeout", timeout)}
    };
    client.sendRequest(req);
}

TlObject TdLibMessages::makeFileLocal(const std::string& path) {
    TlObject f;
    f.type = "inputFileLocal";
    f.fields = {{"@type", str("@type", "inputFileLocal")}, {"path", str("path", path)}};
    return f;
}

TlObject TdLibMessages::makeFormattedText(const std::string& text) {
    TlObject ft;
    ft.type = "formattedText";
    ft.fields = {{"@type", str("@type", "formattedText")}, {"text", str("text", text)}};

    TlObject imt;
    imt.type = "inputMessageText";
    TlField textObj{"text", text}; textObj.isObject = true; textObj.objectFields = ft.fields;
    imt.fields = {{"@type", str("@type", "inputMessageText")}, {"text", textObj}};
    return imt;
}

void TdLibMessages::sendText(long long chatId, const std::string& text, long long threadId, long long replyTo) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageText")},
            {"text", obj("text", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", text)}
            })}
        })}
    };
    if (threadId > 0) req.fields.push_back({"message_thread_id", big("message_thread_id", threadId)});
    if (replyTo > 0) req.fields.push_back({"reply_to", obj("reply_to", {
        {"@type", str("@type", "inputMessageReplyTo")},
        {"message_id", big("message_id", replyTo)}
    })});
    client.sendRequest(req);
}

void TdLibMessages::sendPhoto(long long chatId, const std::string& path, const std::string& caption, long long threadId) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessagePhoto")},
            {"photo", obj("photo", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"caption", obj("caption", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", caption)}
            })}
        })}
    };
    if (threadId > 0) req.fields.push_back({"message_thread_id", big("message_thread_id", threadId)});
    client.sendRequest(req);
}

void TdLibMessages::sendVideo(long long chatId, const std::string& path, const std::string& caption,
                               int duration, int width, int height, long long threadId) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageVideo")},
            {"video", obj("video", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"duration", num("duration", duration)},
            {"width", num("width", width)},
            {"height", num("height", height)},
            {"supports_streaming", boo("supports_streaming", true)},
            {"caption", obj("caption", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", caption)}
            })}
        })}
    };
    if (threadId > 0) req.fields.push_back({"message_thread_id", big("message_thread_id", threadId)});
    client.sendRequest(req);
}

void TdLibMessages::sendVoice(long long chatId, const std::string& path, int duration) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageVoiceNote")},
            {"voice_note", obj("voice_note", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"duration", num("duration", duration)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendDocument(long long chatId, const std::string& path, const std::string& caption) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageDocument")},
            {"document", obj("document", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"caption", obj("caption", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", caption)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendSticker(long long chatId, int stickerFileId) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageSticker")},
            {"sticker", obj("sticker", {
                {"@type", str("@type", "inputFileId")},
                {"id", num("id", stickerFileId)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendDice(long long chatId, const std::string& emoji) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageDice")},
            {"emoji", str("emoji", emoji)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendLocation(long long chatId, double lat, double lon, int livePeriod) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageLocation")},
            {"location", obj("location", {
                {"@type", str("@type", "location")},
                {"latitude", dbl("latitude", lat)},
                {"longitude", dbl("longitude", lon)}
            })},
            {"live_period", num("live_period", livePeriod)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendPoll(long long chatId, const std::string& question,
                             const std::vector<std::string>& options, bool anonymous, bool quiz, int correctOption) {
    std::vector<TlField> optFields;
    for (auto& o : options) optFields.push_back(str("", o));

    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessagePoll")},
            {"question", str("question", question)},
            {"options", arr("options", std::move(optFields))},
            {"is_anonymous", boo("is_anonymous", anonymous)},
            {"type", obj("type", quiz ? std::vector<std::pair<std::string,TlField>>{
                {"@type", str("@type", "pollTypeQuiz")},
                {"correct_option_id", num("correct_option_id", correctOption)}
            } : std::vector<std::pair<std::string,TlField>>{
                {"@type", str("@type", "pollTypeRegular")},
                {"allow_multiple_answers", boo("allow_multiple_answers", false)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendContact(long long chatId, const std::string& phone,
                                const std::string& firstName, const std::string& lastName) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageContact")},
            {"contact", obj("contact", {
                {"@type", str("@type", "contact")},
                {"phone_number", str("phone_number", phone)},
                {"first_name", str("first_name", firstName)},
                {"last_name", str("last_name", lastName)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendAnimation(long long chatId, const std::string& path, const std::string& caption,
                                    int duration, int width, int height) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageAnimation")},
            {"animation", obj("animation", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"duration", num("duration", duration)},
            {"width", num("width", width)},
            {"height", num("height", height)},
            {"caption", obj("caption", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", caption)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendVideoNote(long long chatId, const std::string& path, int duration, int length) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageVideoNote")},
            {"video_note", obj("video_note", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"duration", num("duration", duration)},
            {"length", num("length", length)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendMessageAlbum(long long chatId, const std::vector<std::string>& filePaths,
                                     const std::vector<std::string>& captions, bool isVideo) {
    std::vector<TlField> contents;
    for (size_t i = 0; i < filePaths.size(); i++) {
        std::string caption = (i < captions.size()) ? captions[i] : "";
        std::string mediaField = isVideo ? "video" : "photo";
        std::string mediaType = isVideo ? "inputMessageVideo" : "inputMessagePhoto";
        TlField entry;
        entry.key = "";
        entry.isObject = true;
        entry.objectFields = {
            {"@type", str("@type", mediaType)},
            {mediaField, obj(mediaField, {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", filePaths[i])}
            })},
            {"caption", obj("caption", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", caption)}
            })}
        };
        contents.push_back(entry);
    }
    TlObject req;
    req.type = "sendMessageAlbum";
    req.fields = {
        {"@type", str("@type", "sendMessageAlbum")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_contents", arr("input_message_contents", std::move(contents))}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendAudio(long long chatId, const std::string& path, const std::string& title,
                               const std::string& performer, int duration) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageAudio")},
            {"audio", obj("audio", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", path)}
            })},
            {"title", str("title", title)},
            {"performer", str("performer", performer)},
            {"duration", num("duration", duration)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::getChatHistory(long long chatId, int limit, long long fromMessageId) {
    TlObject req;
    req.type = "getChatHistory";
    req.fields = {
        {"@type", str("@type", "getChatHistory")},
        {"chat_id", big("chat_id", chatId)},
        {"limit", num("limit", limit)},
        {"from_message_id", big("from_message_id", fromMessageId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::loadChats(int limit) {
    TlObject req;
    req.type = "loadChats";
    req.fields = {
        {"@type", str("@type", "loadChats")},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::openChat(long long chatId) {
    TlObject req;
    req.type = "openChat";
    req.fields = {{"@type", str("@type", "openChat")}, {"chat_id", big("chat_id", chatId)}};
    client.sendRequest(req);
}

void TdLibMessages::closeChat(long long chatId) {
    TlObject req;
    req.type = "closeChat";
    req.fields = {{"@type", str("@type", "closeChat")}, {"chat_id", big("chat_id", chatId)}};
    client.sendRequest(req);
}

void TdLibMessages::deleteMessages(long long chatId, const std::vector<long long>& msgIds, bool revoke) {
    std::vector<TlField> idFields;
    for (auto id : msgIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "deleteMessages";
    req.fields = {
        {"@type", str("@type", "deleteMessages")},
        {"chat_id", big("chat_id", chatId)},
        {"message_ids", arr("message_ids", std::move(idFields))},
        {"revoke", boo("revoke", revoke)}
    };
    client.sendRequest(req);
}

void TdLibMessages::editMessage(long long chatId, long long msgId, const std::string& newText) {
    TlObject req;
    req.type = "editMessageText";
    req.fields = {
        {"@type", str("@type", "editMessageText")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageText")},
            {"text", obj("text", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", newText)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::viewMessages(long long chatId, const std::vector<long long>& msgIds) {
    std::vector<TlField> idFields;
    for (auto id : msgIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "viewMessages";
    req.fields = {
        {"@type", str("@type", "viewMessages")},
        {"chat_id", big("chat_id", chatId)},
        {"message_ids", arr("message_ids", std::move(idFields))}
    };
    client.sendRequest(req);
}

void TdLibMessages::searchMessages(long long chatId, const std::string& query, int limit) {
    TlObject req;
    req.type = "searchChatMessages";
    req.fields = {
        {"@type", str("@type", "searchChatMessages")},
        {"chat_id", big("chat_id", chatId)},
        {"query", str("query", query)},
        {"limit", num("limit", limit)},
        {"filter", obj("filter", {{"@type", str("@type", "searchMessagesFilterEmpty")}})}
    };
    client.sendRequest(req);
}

void TdLibMessages::forwardMessages(long long toChatId, long long fromChatId,
                                     const std::vector<long long>& msgIds, bool sendCopy) {
    std::vector<TlField> idFields;
    for (auto id : msgIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "forwardMessages";
    req.fields = {
        {"@type", str("@type", "forwardMessages")},
        {"chat_id", big("chat_id", toChatId)},
        {"from_chat_id", big("from_chat_id", fromChatId)},
        {"message_ids", arr("message_ids", std::move(idFields))},
        {"send_copy", boo("send_copy", sendCopy)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setMessageReaction(long long chatId, long long msgId, const std::string& reaction) {
    TlObject react;
    react.type = "reactionTypeEmoji";
    react.fields = {{"@type", str("@type", "reactionTypeEmoji")}, {"emoji", str("emoji", reaction)}};

    TlObject req;
    req.type = "setMessageReaction";
    req.fields = {
        {"@type", str("@type", "setMessageReaction")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)}
    };
    if (!reaction.empty()) {
        std::vector<TlField> arrVals;
        TlField entry; entry.isObject = true; entry.objectFields = react.fields;
        arrVals.push_back(entry);
        req.fields.push_back({"reaction_types", arr("reaction_types", std::move(arrVals))});
    }
    client.sendRequest(req);
}

void TdLibMessages::removeMessageReaction(long long chatId, long long msgId, const std::string& reaction) {
    setMessageReaction(chatId, msgId, "");
}

void TdLibMessages::setPollVote(long long chatId, long long msgId, const std::vector<int>& optionIds) {
    std::vector<TlField> ids;
    for (auto id : optionIds) { TlField f; f.isInt = true; f.intValue = id; ids.push_back(f); }
    TlObject req;
    req.type = "setPollAnswer";
    req.fields = {
        {"@type", str("@type", "setPollAnswer")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)},
        {"option_ids", arr("option_ids", std::move(ids))}
    };
    client.sendRequest(req);
}

void TdLibMessages::stopPoll(long long chatId, long long msgId) {
    TlObject req;
    req.type = "stopPoll";
    req.fields = {
        {"@type", str("@type", "stopPoll")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::pinMessage(long long chatId, long long msgId, bool disableNotification) {
    TlObject req;
    req.type = "pinChatMessage";
    req.fields = {
        {"@type", str("@type", "pinChatMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)},
        {"disable_notification", boo("disable_notification", disableNotification)},
        {"only_for_self", boo("only_for_self", false)}
    };
    client.sendRequest(req);
}

void TdLibMessages::unpinMessage(long long chatId, long long msgId) {
    TlObject req;
    req.type = "unpinChatMessage";
    req.fields = {
        {"@type", str("@type", "unpinChatMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::unpinAllMessages(long long chatId) {
    TlObject req;
    req.type = "unpinAllChatMessages";
    req.fields = {{"@type", str("@type", "unpinAllChatMessages")}, {"chat_id", big("chat_id", chatId)}};
    client.sendRequest(req);
}

void TdLibMessages::getScheduledMessages(long long chatId) {
    TlObject req;
    req.type = "getScheduledMessages";
    req.fields = {{"@type", str("@type", "getScheduledMessages")}, {"chat_id", big("chat_id", chatId)}};
    client.sendRequest(req);
}

void TdLibMessages::sendScheduled(long long chatId, const std::string& text, int scheduleDate) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageText")},
            {"text", obj("text", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", text)}
            })}
        })},
        {"scheduling_state", obj("scheduling_state", {
            {"@type", str("@type", "messageSchedulingStateSendAtDate")},
            {"send_date", num("send_date", scheduleDate)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::getChatDraftMessage(long long chatId, long long threadId) {
    TlObject req;
    req.type = "getChatDraftMessage";
    req.fields = {
        {"@type", str("@type", "getChatDraftMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", threadId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setChatDraftMessage(long long chatId, long long threadId, const std::string& text) {
    TlObject draft;
    draft.type = "draftMessage";
    draft.fields = {
        {"@type", str("@type", "draftMessage")},
        {"reply_to_message_id", big("reply_to_message_id", 0)},
        {"date", num("date", 0)},
        {"input_message_text", obj("input_message_text", {
            {"@type", str("@type", "inputMessageText")},
            {"text", obj("text", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", text)}
            })}
        })}
    };

    TlObject req;
    req.type = "setChatDraftMessage";
    req.fields = {
        {"@type", str("@type", "setChatDraftMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", threadId)},
        {"draft_message", obj("draft_message", draft.fields)}
    };
    client.sendRequest(req);
}

void TdLibMessages::clearChatDraftMessage(long long chatId, long long threadId) {
    TlObject req;
    req.type = "setChatDraftMessage";
    req.fields = {
        {"@type", str("@type", "setChatDraftMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", threadId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendTextWithTtl(long long chatId, const std::string& text, int ttlSeconds) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageText")},
            {"text", obj("text", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", text)}
            })},
            {"ttl", num("ttl", ttlSeconds)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendTextSilent(long long chatId, const std::string& text, long long threadId) {
    TlObject req;
    req.type = "sendMessage";
    req.fields = {
        {"@type", str("@type", "sendMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"disable_notification", boo("disable_notification", true)},
        {"input_message_content", obj("input_message_content", {
            {"@type", str("@type", "inputMessageText")},
            {"text", obj("text", {
                {"@type", str("@type", "formattedText")},
                {"text", str("text", text)}
            })}
        })}
    };
    if (threadId > 0) req.fields.push_back({"message_thread_id", big("message_thread_id", threadId)});
    client.sendRequest(req);
}

void TdLibMessages::importContacts(const std::vector<std::pair<std::string, std::string>>& contacts) {
    std::vector<TlField> contactFields;
    for (auto& c : contacts) {
        TlField entry;
        entry.isObject = true;
        entry.objectFields = {
            {"@type", str("@type", "contact")},
            {"phone_number", str("phone_number", c.first)},
            {"first_name", str("first_name", c.second)},
            {"last_name", str("last_name", "")}
        };
        contactFields.push_back(entry);
    }

    TlObject req;
    req.type = "importContacts";
    req.fields = {
        {"@type", str("@type", "importContacts")},
        {"contacts", arr("contacts", std::move(contactFields))}
    };
    client.sendRequest(req);
}

void TdLibMessages::searchContacts(const std::string& query, int limit) {
    TlObject req;
    req.type = "searchContacts";
    req.fields = {
        {"@type", str("@type", "searchContacts")},
        {"query", str("query", query)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::removeContacts(const std::vector<long long>& userIds) {
    std::vector<TlField> idFields;
    for (auto id : userIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }

    TlObject req;
    req.type = "removeContacts";
    req.fields = {
        {"@type", str("@type", "removeContacts")},
        {"user_ids", arr("user_ids", std::move(idFields))}
    };
    client.sendRequest(req);
}

void TdLibMessages::getNotificationSettings() {
    TlObject req;
    req.type = "getNotificationSettings";
    req.fields = {{"@type", str("@type", "getNotificationSettings")}};
    client.sendRequest(req);
}

void TdLibMessages::setNotificationSettings(int muteFor, bool showPreview, long long soundId) {
    TlObject settings;
    settings.type = "notificationSettings";
    settings.fields = {
        {"@type", str("@type", "notificationSettings")},
        {"mute_for", num("mute_for", muteFor)},
        {"show_preview", boo("show_preview", showPreview)},
        {"sound_id", big("sound_id", soundId)}
    };

    TlObject req;
    req.type = "setNotificationSettings";
    req.fields = {
        {"@type", str("@type", "setNotificationSettings")},
        {"notification_settings", obj("notification_settings", settings.fields)}
    };
    client.sendRequest(req);
}

void TdLibMessages::downloadFile(int fileId, int priority, long long offset, long long limit, bool synchronous) {
    TlObject req;
    req.type = "downloadFile";
    req.fields = {
        {"@type", str("@type", "downloadFile")},
        {"file_id", num("file_id", fileId)},
        {"priority", num("priority", priority)},
        {"offset", big("offset", offset)},
        {"limit", big("limit", limit)},
        {"synchronous", boo("synchronous", synchronous)}
    };
    client.sendRequest(req);
}

void TdLibMessages::cancelDownloadFile(int fileId) {
    TlObject req;
    req.type = "cancelDownloadFile";
    req.fields = {
        {"@type", str("@type", "cancelDownloadFile")},
        {"file_id", num("file_id", fileId)},
        {"only_if_pending", boo("only_if_pending", false)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getFile(int fileId) {
    TlObject req;
    req.type = "getFile";
    req.fields = {{"@type", str("@type", "getFile")}, {"file_id", num("file_id", fileId)}};
    client.sendRequest(req);
}

void TdLibMessages::deleteFile(int fileId) {
    TlObject req;
    req.type = "deleteFile";
    req.fields = {{"@type", str("@type", "deleteFile")}, {"file_id", num("file_id", fileId)}};
    client.sendRequest(req);
}

void TdLibMessages::getMessageThread(long long chatId, long long msgId) {
    TlObject req;
    req.type = "getMessageThread";
    req.fields = {
        {"@type", str("@type", "getMessageThread")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getMessageThreadHistory(long long chatId, long long msgId, long long fromMsgId, int limit) {
    TlObject req;
    req.type = "getMessageThreadHistory";
    req.fields = {
        {"@type", str("@type", "getMessageThreadHistory")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)},
        {"from_message_id", big("from_message_id", fromMsgId)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendThreadReply(long long chatId, long long threadId, const std::string& text, long long replyTo) {
    sendText(chatId, text, threadId, replyTo);
}

void TdLibMessages::getForumTopics(long long chatId, const std::string& query, int limit) {
    TlObject req;
    req.type = "getForumTopics";
    req.fields = {
        {"@type", str("@type", "getForumTopics")},
        {"chat_id", big("chat_id", chatId)},
        {"query", str("query", query)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getForumTopic(long long chatId, long long topicId) {
    TlObject req;
    req.type = "getForumTopic";
    req.fields = {
        {"@type", str("@type", "getForumTopic")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", topicId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::createForumTopic(long long chatId, const std::string& name, int iconColor) {
    TlObject req;
    req.type = "createForumTopic";
    req.fields = {
        {"@type", str("@type", "createForumTopic")},
        {"chat_id", big("chat_id", chatId)},
        {"name", str("name", name)},
        {"icon", obj("icon", {
            {"@type", str("@type", "forumTopicIcon")},
            {"color", num("color", iconColor)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::deleteForumTopic(long long chatId, long long topicId) {
    TlObject req;
    req.type = "deleteForumTopic";
    req.fields = {
        {"@type", str("@type", "deleteForumTopic")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", topicId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::toggleForumTopicClosed(long long chatId, long long topicId, bool closed) {
    TlObject req;
    req.type = "toggleForumTopicIsClosed";
    req.fields = {
        {"@type", str("@type", "toggleForumTopicIsClosed")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", topicId)},
        {"is_closed", boo("is_closed", closed)}
    };
    client.sendRequest(req);
}

void TdLibMessages::toggleForumTopicPinned(long long chatId, long long topicId, bool pinned) {
    TlObject req;
    req.type = "toggleForumTopicIsPinned";
    req.fields = {
        {"@type", str("@type", "toggleForumTopicIsPinned")},
        {"chat_id", big("chat_id", chatId)},
        {"message_thread_id", big("message_thread_id", topicId)},
        {"is_pinned", boo("is_pinned", pinned)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getStickerSet(long long setId) {
    TlObject req;
    req.type = "getStickerSet";
    req.fields = {{"@type", str("@type", "getStickerSet")}, {"set_id", big("set_id", setId)}};
    client.sendRequest(req);
}

void TdLibMessages::searchStickerSet(const std::string& name) {
    TlObject req;
    req.type = "searchStickerSet";
    req.fields = {{"@type", str("@type", "searchStickerSet")}, {"name", str("name", name)}};
    client.sendRequest(req);
}

void TdLibMessages::getInstalledStickerSets() {
    TlObject req;
    req.type = "getInstalledStickerSets";
    req.fields = {
        {"@type", str("@type", "getInstalledStickerSets")},
        {"sticker_type", obj("sticker_type", {{"@type", str("@type", "stickerTypeRegular")}})}
    };
    client.sendRequest(req);
}

void TdLibMessages::getRecentStickers() {
    TlObject req;
    req.type = "getRecentStickers";
    req.fields = {{"@type", str("@type", "getRecentStickers")}, {"is_attached", boo("is_attached", false)}};
    client.sendRequest(req);
}

void TdLibMessages::searchStickers(const std::string& emoji, int limit) {
    TlObject req;
    req.type = "searchStickers";
    req.fields = {
        {"@type", str("@type", "searchStickers")},
        {"emoji", str("emoji", emoji)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getActiveStories(int limit) {
    TlObject req;
    req.type = "getActiveStories";
    req.fields = {{"@type", str("@type", "getActiveStories")}, {"limit", num("limit", limit)}};
    client.sendRequest(req);
}

void TdLibMessages::getStory(long long chatId, long long storyId) {
    TlObject req;
    req.type = "getStory";
    req.fields = {
        {"@type", str("@type", "getStory")},
        {"chat_id", big("chat_id", chatId)},
        {"story_id", big("story_id", storyId)},
        {"only_local", boo("only_local", false)}
    };
    client.sendRequest(req);
}

void TdLibMessages::openStory(long long chatId, long long storyId) {
    TlObject req;
    req.type = "openStory";
    req.fields = {
        {"@type", str("@type", "openStory")},
        {"story_sender_chat_id", big("story_sender_chat_id", chatId)},
        {"story_id", big("story_id", storyId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::closeStory(long long chatId, long long storyId) {
    TlObject req;
    req.type = "closeStory";
    req.fields = {
        {"@type", str("@type", "closeStory")},
        {"story_sender_chat_id", big("story_sender_chat_id", chatId)},
        {"story_id", big("story_id", storyId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setStoryReaction(long long chatId, long long storyId, const std::string& reaction) {
    TlObject reactType;
    reactType.type = "reactionTypeEmoji";
    reactType.fields = {{"@type", str("@type", "reactionTypeEmoji")}, {"emoji", str("emoji", reaction)}};

    TlObject req;
    req.type = "setStoryReaction";
    req.fields = {
        {"@type", str("@type", "setStoryReaction")},
        {"story_sender_chat_id", big("story_sender_chat_id", chatId)},
        {"story_id", big("story_id", storyId)},
        {"reaction_type", obj("reaction_type", reactType.fields)},
        {"update_recent_reactions", boo("update_recent_reactions", true)}
    };
    client.sendRequest(req);
}

void TdLibMessages::deleteStory(long long chatId, long long storyId) {
    TlObject req;
    req.type = "deleteStory";
    req.fields = {
        {"@type", str("@type", "deleteStory")},
        {"story_sender_chat_id", big("story_sender_chat_id", chatId)},
        {"story_id", big("story_id", storyId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendStory(const std::string& filePath, bool isVideo, const std::string& caption) {
    TlObject content;
    content.type = isVideo ? "inputStoryContentVideo" : "inputStoryContentPhoto";
    content.fields = {
        {"@type", str("@type", isVideo ? "inputStoryContentVideo" : "inputStoryContentPhoto")},
        {isVideo ? "video" : "photo", obj(isVideo ? "video" : "photo", {
            {"@type", str("@type", "inputFileLocal")},
            {"path", str("path", filePath)}
        })},
        {"added_sticker_file_ids", arr("added_sticker_file_ids", {})}
    };

    TlObject req;
    req.type = "sendStory";
    req.fields = {
        {"@type", str("@type", "sendStory")},
        {"content", obj("content", content.fields)},
        {"caption", obj("caption", {
            {"@type", str("@type", "formattedText")},
            {"text", str("text", caption)}
        })},
        {"privacy_settings", obj("privacy_settings", {
            {"@type", str("@type", "storyPrivacySettings")},
            {"visibility", obj("visibility", {{"@type", str("@type", "storyPrivacySettingsEveryone")}})},
            {"is_forwarding_allowed", boo("is_forwarding_allowed", true)},
            {"exception_user_ids", arr("exception_user_ids", {})}
        })},
        {"active_period", num("active_period", 86400)},
        {"is_pinned", boo("is_pinned", false)},
        {"protect_content", boo("protect_content", false)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getUser(long long userId) {
    TlObject req;
    req.type = "getUser";
    req.fields = {{"@type", str("@type", "getUser")}, {"user_id", big("user_id", userId)}};
    client.sendRequest(req);
}

void TdLibMessages::getUserFullInfo(long long userId) {
    TlObject req;
    req.type = "getUserFullInfo";
    req.fields = {{"@type", str("@type", "getUserFullInfo")}, {"user_id", big("user_id", userId)}};
    client.sendRequest(req);
}

void TdLibMessages::getMe() {
    TlObject req;
    req.type = "getMe";
    req.fields = {{"@type", str("@type", "getMe")}};
    client.sendRequest(req);
}

void TdLibMessages::setBio(const std::string& bio) {
    TlObject req;
    req.type = "setBio";
    req.fields = {{"@type", str("@type", "setBio")}, {"bio", str("bio", bio)}};
    client.sendRequest(req);
}

void TdLibMessages::setName(const std::string& firstName, const std::string& lastName) {
    TlObject req;
    req.type = "setName";
    req.fields = {
        {"@type", str("@type", "setName")},
        {"first_name", str("first_name", firstName)},
        {"last_name", str("last_name", lastName)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setUsername(const std::string& username) {
    TlObject req;
    req.type = "setUsername";
    req.fields = {{"@type", str("@type", "setUsername")}, {"username", str("username", username)}};
    client.sendRequest(req);
}

void TdLibMessages::setProfilePhoto(const std::string& photoPath) {
    TlObject req;
    req.type = "setProfilePhoto";
    req.fields = {
        {"@type", str("@type", "setProfilePhoto")},
        {"photo", obj("photo", {
            {"@type", str("@type", "inputChatPhotoStatic")},
            {"photo", obj("photo", {
                {"@type", str("@type", "inputFileLocal")},
                {"path", str("path", photoPath)}
            })}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::getUserProfilePhotos(long long userId, int offset, int limit) {
    TlObject req;
    req.type = "getUserProfilePhotos";
    req.fields = {
        {"@type", str("@type", "getUserProfilePhotos")},
        {"user_id", big("user_id", userId)},
        {"offset", num("offset", offset)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getActiveSessions() {
    TlObject req;
    req.type = "getActiveSessions";
    req.fields = {{"@type", str("@type", "getActiveSessions")}};
    client.sendRequest(req);
}

void TdLibMessages::terminateSession(long long sessionId) {
    TlObject req;
    req.type = "terminateSession";
    req.fields = {{"@type", str("@type", "terminateSession")}, {"session_id", big("session_id", sessionId)}};
    client.sendRequest(req);
}

void TdLibMessages::terminateAllOtherSessions() {
    TlObject req;
    req.type = "terminateAllOtherSessions";
    req.fields = {{"@type", str("@type", "terminateAllOtherSessions")}};
    client.sendRequest(req);
}

void TdLibMessages::getConnectedWebsites() {
    TlObject req;
    req.type = "getConnectedWebsites";
    req.fields = {{"@type", str("@type", "getConnectedWebsites")}};
    client.sendRequest(req);
}

void TdLibMessages::getGroupsInCommon(long long userId, long long offsetChatId) {
    TlObject req;
    req.type = "getGroupsInCommon";
    req.fields = {
        {"@type", str("@type", "getGroupsInCommon")},
        {"user_id", big("user_id", userId)},
        {"offset_chat_id", big("offset_chat_id", offsetChatId)},
        {"limit", num("limit", 100)}
    };
    client.sendRequest(req);
}

void TdLibMessages::blockUser(long long userId) {
    TlObject req;
    req.type = "blockMessageSenderFromReplies";
    req.fields = {
        {"@type", str("@type", "blockMessageSenderFromReplies")},
        {"message_id", big("message_id", 0)},
        {"delete_message", boo("delete_message", true)},
        {"delete_all_messages", boo("delete_all_messages", true)},
        {"report_spam", boo("report_spam", true)}
    };
    client.sendRequest(req);
}

void TdLibMessages::unblockUser(long long userId) {
    TlObject req;
    req.type = "unblockMessageSenderFromReplies";
    req.fields = {
        {"@type", str("@type", "unblockMessageSenderFromReplies")},
        {"message_id", big("message_id", 0)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getPremiumState() {
    TlObject req;
    req.type = "getPremiumState";
    req.fields = {{"@type", str("@type", "getPremiumState")}};
    client.sendRequest(req);
}

void TdLibMessages::setEmojiStatus(long long customEmojiId, int duration) {
    TlObject req;
    req.type = "setEmojiStatus";
    req.fields = {
        {"@type", str("@type", "setEmojiStatus")},
        {"emoji_status", obj("emoji_status", {
            {"@type", str("@type", "emojiStatus")},
            {"custom_emoji_id", big("custom_emoji_id", customEmojiId)}
        })}
    };
    if (duration > 0) req.fields.push_back({"duration", num("duration", duration)});
    client.sendRequest(req);
}

void TdLibMessages::clearEmojiStatus() {
    TlObject req;
    req.type = "setEmojiStatus";
    req.fields = {
        {"@type", str("@type", "setEmojiStatus")},
        {"emoji_status", obj("emoji_status", {
            {"@type", str("@type", "emojiStatusEmpty")}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::getDefaultEmojiStatuses() {
    TlObject req;
    req.type = "getDefaultEmojiStatuses";
    req.fields = {{"@type", str("@type", "getDefaultEmojiStatuses")}};
    client.sendRequest(req);
}

void TdLibMessages::getThemedEmojiStatuses() {
    TlObject req;
    req.type = "getThemedEmojiStatuses";
    req.fields = {{"@type", str("@type", "getThemedEmojiStatuses")}};
    client.sendRequest(req);
}

void TdLibMessages::getRecentEmojiStatuses() {
    TlObject req;
    req.type = "getRecentEmojiStatuses";
    req.fields = {{"@type", str("@type", "getRecentEmojiStatuses")}};
    client.sendRequest(req);
}

void TdLibMessages::setCloseFriends(const std::vector<long long>& userIds) {
    std::vector<TlField> idFields;
    for (auto id : userIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "setCloseFriends";
    req.fields = {
        {"@type", str("@type", "setCloseFriends")},
        {"user_ids", arr("user_ids", std::move(idFields))}
    };
    client.sendRequest(req);
}

void TdLibMessages::getCloseFriends() {
    TlObject req;
    req.type = "getCloseFriends";
    req.fields = {{"@type", str("@type", "getCloseFriends")}};
    client.sendRequest(req);
}

void TdLibMessages::createNewSecretChat(long long userId) {
    TlObject req;
    req.type = "createNewSecretChat";
    req.fields = {{"@type", str("@type", "createNewSecretChat")}, {"user_id", big("user_id", userId)}};
    client.sendRequest(req);
}

void TdLibMessages::openSecretChat(long long secretChatId) {
    TlObject req;
    req.type = "openSecretChat";
    req.fields = {{"@type", str("@type", "openSecretChat")}, {"secret_chat_id", big("secret_chat_id", secretChatId)}};
    client.sendRequest(req);
}

void TdLibMessages::closeSecretChat(long long secretChatId) {
    TlObject req;
    req.type = "closeSecretChat";
    req.fields = {{"@type", str("@type", "closeSecretChat")}, {"secret_chat_id", big("secret_chat_id", secretChatId)}};
    client.sendRequest(req);
}

void TdLibMessages::getSecretChat(long long secretChatId) {
    TlObject req;
    req.type = "getSecretChat";
    req.fields = {{"@type", str("@type", "getSecretChat")}, {"secret_chat_id", big("secret_chat_id", secretChatId)}};
    client.sendRequest(req);
}

void TdLibMessages::sendChatSetTtlMessage(long long chatId, int ttl) {
    TlObject req;
    req.type = "sendChatSetTtlMessage";
    req.fields = {
        {"@type", str("@type", "sendChatSetTtlMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"ttl", num("ttl", ttl)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setSecretChatTtl(long long secretChatId, int ttl) {
    TlObject req;
    req.type = "setSecretChatTtl";
    req.fields = {
        {"@type", str("@type", "setSecretChatTtl")},
        {"secret_chat_id", big("secret_chat_id", secretChatId)},
        {"ttl", num("ttl", ttl)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getBlockedUsers(int offset, int limit) {
    TlObject req;
    req.type = "getBlockedMessageSenders";
    req.fields = {
        {"@type", str("@type", "getBlockedMessageSenders")},
        {"offset", num("offset", offset)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getBackgrounds(bool dark) {
    TlObject req;
    req.type = "getBackgrounds";
    req.fields = {{"@type", str("@type", "getBackgrounds")}, {"for_dark_theme", boo("for_dark_theme", dark)}};
    client.sendRequest(req);
}

void TdLibMessages::getChatBackground(long long chatId) {
    TlObject req;
    req.type = "getChatBackground";
    req.fields = {{"@type", str("@type", "getChatBackground")}, {"chat_id", big("chat_id", chatId)}};
    client.sendRequest(req);
}

void TdLibMessages::setChatBackground(long long chatId, long long bgId) {
    TlObject req;
    req.type = "setChatBackground";
    req.fields = {
        {"@type", str("@type", "setChatBackground")},
        {"chat_id", big("chat_id", chatId)},
        {"background", obj("background", {
            {"@type", str("@type", "inputBackgroundPrevious")},
            {"background_id", big("background_id", bgId)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::deleteChatBackground(long long chatId) {
    TlObject req;
    req.type = "deleteChatBackground";
    req.fields = {
        {"@type", str("@type", "deleteChatBackground")},
        {"chat_id", big("chat_id", chatId)},
        {"restore_previous", boo("restore_previous", true)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setChatFolder(int folderId, const std::string& title,
                                   const std::vector<long long>& pinnedIds,
                                   const std::vector<long long>& includedIds,
                                   const std::vector<long long>& excludedIds) {
    auto toArr = [](const std::vector<long long>& ids) {
        std::vector<TlField> f; for (auto id : ids) { TlField tf; tf.isLong = true; tf.longValue = id; f.push_back(tf); } return f;
    };
    TlObject req;
    req.type = folderId > 0 ? "editChatFolder" : "createChatFolder";
    req.fields = {
        {"@type", str("@type", req.type)},
        {"folder", obj("folder", {
            {"@type", str("@type", "chatFolder")},
            {"title", str("title", title)},
            {"icon", obj("icon", {{"@type", str("@type", "chatFolderIcon")}, {"name", str("name", "Custom")}})},
            {"pinned_chat_ids", arr("pinned_chat_ids", toArr(pinnedIds))},
            {"included_chat_ids", arr("included_chat_ids", toArr(includedIds))},
            {"excluded_chat_ids", arr("excluded_chat_ids", toArr(excludedIds))},
            {"exclude_muted", boo("exclude_muted", false)},
            {"exclude_read", boo("exclude_read", false)},
            {"exclude_archived", boo("exclude_archived", true)},
            {"include_contacts", boo("include_contacts", true)},
            {"include_non_contacts", boo("include_non_contacts", true)},
            {"include_bots", boo("include_bots", true)},
            {"include_groups", boo("include_groups", true)},
            {"include_channels", boo("include_channels", true)}
        })}
    };
    if (folderId > 0) req.fields.push_back({"chat_folder_id", num("chat_folder_id", folderId)});
    client.sendRequest(req);
}

void TdLibMessages::getChatFolders() {
    TlObject req;
    req.type = "getRecommendedChatFolders";
    req.fields = {{"@type", str("@type", "getRecommendedChatFolders")}};
    client.sendRequest(req);
}

void TdLibMessages::deleteChatFolder(int folderId) {
    TlObject req;
    req.type = "deleteChatFolder";
    req.fields = {
        {"@type", str("@type", "deleteChatFolder")},
        {"chat_folder_id", num("chat_folder_id", folderId)},
        {"leave_chat_ids", arr("leave_chat_ids", {})}
    };
    client.sendRequest(req);
}

void TdLibMessages::getChatMember(long long chatId, long long userId) {
    TlObject req;
    req.type = "getChatMember";
    req.fields = {
        {"@type", str("@type", "getChatMember")},
        {"chat_id", big("chat_id", chatId)},
        {"member_id", obj("member_id", {
            {"@type", str("@type", "messageSenderUser")},
            {"user_id", big("user_id", userId)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::searchChatMembers(long long chatId, const std::string& query, int limit) {
    TlObject req;
    req.type = "searchChatMembers";
    req.fields = {
        {"@type", str("@type", "searchChatMembers")},
        {"chat_id", big("chat_id", chatId)},
        {"query", str("query", query)},
        {"limit", num("limit", limit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getChatAdministrators(long long chatId) {
    TlObject req;
    req.type = "getChatAdministrators";
    req.fields = {
        {"@type", str("@type", "getChatAdministrators")},
        {"chat_id", big("chat_id", chatId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::addChatMember(long long chatId, long long userId, int forwardLimit) {
    TlObject req;
    req.type = "addChatMember";
    req.fields = {
        {"@type", str("@type", "addChatMember")},
        {"chat_id", big("chat_id", chatId)},
        {"user_id", big("user_id", userId)},
        {"forward_limit", num("forward_limit", forwardLimit)}
    };
    client.sendRequest(req);
}

void TdLibMessages::joinChatByInviteLink(const std::string& inviteLink) {
    TlObject req;
    req.type = "joinChatByInviteLink";
    req.fields = {
        {"@type", str("@type", "joinChatByInviteLink")},
        {"invite_link", str("invite_link", inviteLink)}
    };
    client.sendRequest(req);
}

void TdLibMessages::checkChatInviteLink(const std::string& inviteLink) {
    TlObject req;
    req.type = "checkChatInviteLink";
    req.fields = {
        {"@type", str("@type", "checkChatInviteLink")},
        {"invite_link", str("invite_link", inviteLink)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getChatInviteLink(long long chatId, bool createsJoinRequest) {
    TlObject req;
    req.type = "getChatInviteLink";
    req.fields = {
        {"@type", str("@type", "getChatInviteLink")},
        {"chat_id", big("chat_id", chatId)},
        {"creates_join_request", boo("creates_join_request", createsJoinRequest)}
    };
    client.sendRequest(req);
}

void TdLibMessages::searchPublicChat(const std::string& username) {
    TlObject req;
    req.type = "searchPublicChat";
    req.fields = {
        {"@type", str("@type", "searchPublicChat")},
        {"username", str("username", username)}
    };
    client.sendRequest(req);
}

void TdLibMessages::searchPublicChats(const std::string& query) {
    TlObject req;
    req.type = "searchPublicChats";
    req.fields = {
        {"@type", str("@type", "searchPublicChats")},
        {"query", str("query", query)}
    };
    client.sendRequest(req);
}

void TdLibMessages::createNewBasicGroupChat(const std::vector<long long>& userIds, const std::string& title) {
    std::vector<TlField> idFields;
    for (auto id : userIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "createNewBasicGroupChat";
    req.fields = {
        {"@type", str("@type", "createNewBasicGroupChat")},
        {"user_ids", arr("user_ids", std::move(idFields))},
        {"title", str("title", title)}
    };
    client.sendRequest(req);
}

void TdLibMessages::createNewSupergroupChat(const std::string& title, bool isChannel, const std::string& description) {
    TlObject req;
    req.type = "createNewSupergroupChat";
    req.fields = {
        {"@type", str("@type", "createNewSupergroupChat")},
        {"title", str("title", title)},
        {"is_channel", boo("is_channel", isChannel)},
        {"description", str("description", description)}
    };
    client.sendRequest(req);
}

void TdLibMessages::setChatMemberStatus(long long chatId, long long userId, const std::string& status) {
    TlObject memberId;
    memberId.type = "messageSenderUser";
    memberId.fields = {{"@type", str("@type", "messageSenderUser")}, {"user_id", big("user_id", userId)}};

    TlObject req;
    req.type = "setChatMemberStatus";
    req.fields = {
        {"@type", str("@type", "setChatMemberStatus")},
        {"chat_id", big("chat_id", chatId)},
        {"member_id", obj("member_id", memberId.fields)},
        {"status", obj("status", {
            {"@type", str("@type", status)}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::reportChat(long long chatId, const std::vector<long long>& messageIds, const std::string& reason) {
    std::vector<TlField> idFields;
    for (auto id : messageIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "reportChat";
    req.fields = {
        {"@type", str("@type", "reportChat")},
        {"chat_id", big("chat_id", chatId)},
        {"message_ids", arr("message_ids", std::move(idFields))},
        {"reason", obj("reason", {{"@type", str("@type", reason)}})}
    };
    client.sendRequest(req);
}

void TdLibMessages::getBotCommands(long long botUserId, long long scopeChatId) {
    TlObject req;
    req.type = "getCommands";
    req.fields = {
        {"@type", str("@type", "getCommands")},
        {"scope", obj("scope", {
            {"@type", str("@type", "botCommandScopePeer")},
            {"peer", obj("peer", {
                {"@type", str("@type", "inputPeerUser")},
                {"user_id", big("user_id", botUserId)}
            })}
        })},
        {"language_code", str("language_code", "")}
    };
    client.sendRequest(req);
}

void TdLibMessages::getInlineQueryResults(long long botUserId, long long chatId, const std::string& query, const std::string& offset) {
    TlObject req;
    req.type = "getInlineQueryResults";
    req.fields = {
        {"@type", str("@type", "getInlineQueryResults")},
        {"bot_user_id", big("bot_user_id", botUserId)},
        {"chat_id", big("chat_id", chatId)},
        {"query", str("query", query)},
        {"offset", str("offset", offset)}
    };
    client.sendRequest(req);
}

void TdLibMessages::sendInlineQueryResult(long long chatId, long long queryId, const std::string& resultId, bool hideViaBot) {
    TlObject req;
    req.type = "sendInlineQueryResultMessage";
    req.fields = {
        {"@type", str("@type", "sendInlineQueryResultMessage")},
        {"chat_id", big("chat_id", chatId)},
        {"query_id", big("query_id", queryId)},
        {"result_id", str("result_id", resultId)},
        {"hide_via_bot", boo("hide_via_bot", hideViaBot)}
    };
    client.sendRequest(req);
}

void TdLibMessages::answerCallbackQuery(long long callbackQueryId, const std::string& text, bool showAlert) {
    TlObject req;
    req.type = "answerCallbackQuery";
    req.fields = {
        {"@type", str("@type", "answerCallbackQuery")},
        {"callback_query_id", big("callback_query_id", callbackQueryId)},
        {"text", str("text", text)},
        {"show_alert", boo("show_alert", showAlert)}
    };
    client.sendRequest(req);
}

void TdLibMessages::toggleBotIsPaused(long long botUserId, bool isPaused) {
    TlObject req;
    req.type = "toggleBotIsPaused";
    req.fields = {
        {"@type", str("@type", "toggleBotIsPaused")},
        {"bot_user_id", big("bot_user_id", botUserId)},
        {"is_paused", boo("is_paused", isPaused)}
    };
    client.sendRequest(req);
}

void TdLibMessages::canBotSendMessages(long long botUserId) {
    TlObject req;
    req.type = "canBotSendMessages";
    req.fields = {
        {"@type", str("@type", "canBotSendMessages")},
        {"bot_user_id", big("bot_user_id", botUserId)}
    };
    client.sendRequest(req);
}

MessageInfo TdLibMessages::parseMessageInfo(const TlObject& msg) {
    MessageInfo info;
    info.id = msg.optLong("id", 0);
    info.chatId = msg.optLong("chat_id", 0);
    info.date = msg.optInt("date", 0);
    info.isOutgoing = msg.optBool("is_outgoing", false);

    TlObject sender = msg.optObject("sender_id");
    info.senderId = sender.optString("user_id", sender.optString("chat_id", ""));

    TlObject content = msg.optObject("content");
    info.contentType = content.type;
    TlObject textObj = content.optObject("text");
    info.text = textObj.optString("text", content.optString("text", ""));

    if (info.text.empty()) {
        if (content.type.find("Photo") != std::string::npos) info.text = "[Photo]";
        else if (content.type.find("Video") != std::string::npos) info.text = "[Video]";
        else if (content.type.find("Voice") != std::string::npos) info.text = "[Voice]";
        else if (content.type.find("Sticker") != std::string::npos) info.text = "[Sticker]";
        else if (content.type.find("Document") != std::string::npos) info.text = "[Document]";
        else if (content.type.find("Poll") != std::string::npos) info.text = "[Poll]";
        else if (content.type.find("Location") != std::string::npos) info.text = "[Location]";
        else if (content.type.find("Dice") != std::string::npos) info.text = "[Dice]";
        else if (content.type.find("Animation") != std::string::npos) info.text = "[GIF]";
        else if (content.type.find("Contact") != std::string::npos) info.text = "[Contact]";
        else info.text = "[" + content.type + "]";
    }

    info.rawJson = msg.toJson();
    return info;
}

// --- Calls ---

void TdLibMessages::createCall(long long userId, bool isVideo) {
    std::vector<TlField> libVers = {str("", "3.0.0"), str("", "4.0.0"), str("", "5.0.0")};
    TlObject req;
    req.type = "createCall";
    req.fields = {
        {"@type", str("@type", "createCall")},
        {"user_id", big("user_id", userId)},
        {"protocol", obj("protocol", {
            {"@type", str("@type", "callProtocol")},
            {"udp_p2p", boo("udp_p2p", true)},
            {"udp_reflector", boo("udp_reflector", true)},
            {"min_layer", num("min_layer", 65)},
            {"max_layer", num("max_layer", 92)},
            {"library_versions", arr("library_versions", std::move(libVers))}
        })},
        {"is_video", boo("is_video", isVideo)}
    };
    client.sendRequest(req);
}

void TdLibMessages::acceptCall(int callId) {
    TlObject req;
    req.type = "acceptCall";
    req.fields = {
        {"@type", str("@type", "acceptCall")},
        {"call_id", num("call_id", callId)},
        {"protocol", obj("protocol", {
            {"@type", str("@type", "callProtocol")},
            {"udp_p2p", boo("udp_p2p", true)},
            {"udp_reflector", boo("udp_reflector", true)},
            {"min_layer", num("min_layer", 65)},
            {"max_layer", num("max_layer", 92)},
            {"library_versions", arr("library_versions", {str("", "3.0.0"), str("", "4.0.0"), str("", "5.0.0")})}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::discardCall(int callId, bool isDisconnected) {
    TlObject req;
    req.type = "discardCall";
    req.fields = {
        {"@type", str("@type", "discardCall")},
        {"call_id", num("call_id", callId)},
        {"is_disconnected", boo("is_disconnected", isDisconnected)},
        {"duration", num("duration", 0)},
        {"is_video", boo("is_video", false)},
        {"connection_id", big("connection_id", 0)}
    };
    client.sendRequest(req);
}

// --- Live Location ---

void TdLibMessages::editLiveLocation(long long chatId, long long msgId, double lat, double lon,
                                      int heading, int proximityAlertRadius) {
    TlObject req;
    req.type = "editMessageLiveLocation";
    req.fields = {
        {"@type", str("@type", "editMessageLiveLocation")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)},
        {"location", obj("location", {
            {"@type", str("@type", "location")},
            {"latitude", dbl("latitude", lat)},
            {"longitude", dbl("longitude", lon)}
        })},
        {"heading", num("heading", heading)},
        {"proximity_alert_radius", num("proximity_alert_radius", proximityAlertRadius)}
    };
    client.sendRequest(req);
}

void TdLibMessages::stopLiveLocation(long long chatId, long long msgId) {
    TlObject req;
    req.type = "editMessageLiveLocation";
    req.fields = {
        {"@type", str("@type", "editMessageLiveLocation")},
        {"chat_id", big("chat_id", chatId)},
        {"message_id", big("message_id", msgId)},
        {"location", nullField("location")}
    };
    client.sendRequest(req);
}

// --- Story Privacy & Stealth ---

void TdLibMessages::setStoryPrivacy(bool everyone, bool contacts, bool closeFriends,
                                     const std::vector<long long>& selectedUserIds) {
    std::string visType;
    if (everyone) visType = "storyPrivacySettingsEveryone";
    else if (contacts) visType = "storyPrivacySettingsContacts";
    else if (closeFriends) visType = "storyPrivacySettingsCloseFriends";
    else visType = "storyPrivacySettingsSelectedUsers";

    std::vector<TlField> userIdFields;
    for (auto id : selectedUserIds) { TlField f; f.isLong = true; f.longValue = id; userIdFields.push_back(f); }

    TlObject req;
    req.type = "setStoryPrivacySettings";
    req.fields = {
        {"@type", str("@type", "setStoryPrivacySettings")},
        {"privacy_settings", obj("privacy_settings", {
            {"@type", str("@type", "storyPrivacySettings")},
            {"visibility", obj("visibility", {{"@type", str("@type", visType)}})},
            {"is_forwarding_allowed", boo("is_forwarding_allowed", true)},
            {"exception_user_ids", arr("exception_user_ids", std::move(userIdFields))}
        })}
    };
    client.sendRequest(req);
}

void TdLibMessages::getStoryPrivacy() {
    TlObject req;
    req.type = "getStoryPrivacySettings";
    req.fields = {{"@type", str("@type", "getStoryPrivacySettings")}};
    client.sendRequest(req);
}

void TdLibMessages::activateStealthMode() {
    TlObject req;
    req.type = "activateStealthMode";
    req.fields = {{"@type", str("@type", "activateStealthMode")}};
    client.sendRequest(req);
}

void TdLibMessages::getChatsToSendStories() {
    TlObject req;
    req.type = "getChatsToSendStories";
    req.fields = {{"@type", str("@type", "getChatsToSendStories")}};
    client.sendRequest(req);
}

void TdLibMessages::canSendStoryToChat(long long chatId) {
    TlObject req;
    req.type = "canSendStoryToChat";
    req.fields = {
        {"@type", str("@type", "canSendStoryToChat")},
        {"chat_id", big("chat_id", chatId)}
    };
    client.sendRequest(req);
}

void TdLibMessages::getStoryNotificationExceptions() {
    TlObject req;
    req.type = "getStoryNotificationSettings";
    req.fields = {{"@type", str("@type", "getStoryNotificationSettings")}};
    client.sendRequest(req);
}

void TdLibMessages::setStoryNotificationExceptions(const std::vector<long long>& userIds, bool enabled) {
    std::vector<TlField> idFields;
    for (auto id : userIds) { TlField f; f.isLong = true; f.longValue = id; idFields.push_back(f); }
    TlObject req;
    req.type = "setStoryNotificationSettings";
    req.fields = {
        {"@type", str("@type", "setStoryNotificationSettings")},
        {"notification_settings", obj("notification_settings", {
            {"@type", str("@type", "storyNotificationSettings")},
            {"user_ids", arr("user_ids", std::move(idFields))},
            {"are_enabled", boo("are_enabled", enabled)}
        })}
    };
    client.sendRequest(req);
}

UserStatusInfo TdLibMessages::parseUserStatus(const TlObject& user) {
    UserStatusInfo info;
    info.userId = user.optLong("id", 0);
    info.isPremium = user.optBool("is_premium", false);

    TlObject status = user.optObject("status");
    info.type = status.type;

    if (status.type == "userStatusOnline") {
        info.isOnline = true;
    } else if (status.type == "userStatusOffline") {
        info.wasOnline = status.optInt("was_online", 0);
    } else if (status.type == "userStatusRecently") {
        info.wasOnline = -1;  // signal: recently
    } else if (status.type == "userStatusLastWeek") {
        info.wasOnline = -2;
    } else if (status.type == "userStatusLastMonth") {
        info.wasOnline = -3;
    }

    TlObject emoji = user.optObject("emoji_status");
    if (emoji.type == "emojiStatus") {
        info.emojiStatus = std::to_string(emoji.optLong("custom_emoji_id", 0));
    }

    return info;
}

} // namespace pcp
