#pragma once

#include <string>
#include <vector>
#include <cstdint>

namespace progressive {
namespace deltachat {

struct DcChat {
    int64_t id = 0;
    std::string name;
    int32_t type = 0;
    int64_t last_message_id = 0;
    int32_t unread_count = 0;
    int32_t is_protected = 0;
    int32_t is_verified = 0;
    std::string profile_image;
    std::string color;
};

struct DcMessage {
    int64_t id = 0;
    int64_t chat_id = 0;
    int64_t from_id = 0;
    int32_t view_type = 0;
    int32_t state = 0;
    int32_t is_info = 0;
    int64_t timestamp = 0;
    std::string text;
    std::string sender_name;
    int32_t is_outgoing = 0;
};

struct DcContact {
    int64_t id = 0;
    std::string name;
    std::string address;
    std::string profile_image;
    std::string color;
};

enum class DcEventType {
    NONE = 0,
    CONFIGURE_PROGRESS = 2041,
    IMEX_PROGRESS = 2051,
    IMEX_FILE_WRITTEN = 2052,
    SECUREJOIN_INVITER_PROGRESS = 2061,
    SECUREJOIN_JOINER_PROGRESS = 2062,
    CONNECTIVITY_CHANGED = 2100,
    CONTACTS_CHANGED = 2101,
    SELFAVATAR_CHANGED = 2102,
    INCOMING_MSG = 2105,
    MSGS_CHANGED = 2106,
    INCOMING_MSG_BUNCH = 2107,
    MSG_DELIVERED = 2110,
    MSG_FAILED = 2112,
    MSG_READ = 2115,
    CHAT_MODIFIED = 2120,
    CHAT_EPHEMERAL_TIMER_MODIFIED = 2121,
    CHATLIST_CHANGED = 2122,
    CHATLIST_ITEM_CHANGED = 2123,
    CONFIG_SYNCHRONIZED = 2130,
    ERROR = 4000,
    WARNING = 4001,
    INFO = 4002,
    ERROR_NETWORK = 4003,
    ERROR_SELF_NOT_IN_GROUP = 4010
};

struct DcEvent {
    int32_t id = 0;
    int64_t data1 = 0;
    int64_t data2 = 0;
};

} // namespace deltachat
} // namespace progressive
