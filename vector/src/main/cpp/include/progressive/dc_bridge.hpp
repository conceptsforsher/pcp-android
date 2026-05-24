#pragma once

#ifdef PROGRESSIVE_HAS_DELTACHAT

#include <string>
#include <functional>

namespace progressive {
namespace deltachat {

using dc_context_t = void;
using dc_chat_t = void;
using dc_chatlist_t = void;
using dc_msg_t = void;
using dc_contact_t = void;
using dc_array_t = void;
using dc_event_t = void;
using dc_lot_t = void;

struct DcApi {
    dc_context_t* (*create)(const char* os_name, const char* dbfile);
    int (*open)(dc_context_t* ctx);
    int (*close)(dc_context_t* ctx);
    void (*unref)(dc_context_t* ctx);
    int (*is_open)(dc_context_t* ctx);

    int (*configure)(dc_context_t* ctx);
    int (*is_configured)(dc_context_t* ctx);
    char* (*get_info)(dc_context_t* ctx);

    int (*set_config)(dc_context_t* ctx, const char* key, const char* value);
    char* (*get_config)(dc_context_t* ctx, const char* key);

    dc_chatlist_t* (*get_chatlist)(dc_context_t* ctx, int flags, const char* query, int contact_id);
    int (*get_chatlist_cnt)(dc_chatlist_t* list);
    int64_t (*get_chatlist_id)(dc_chatlist_t* list, int index);
    char* (*get_chatlist_summary)(dc_chatlist_t* list, int index, dc_chat_t* chat);
    void (*chatlist_unref)(dc_chatlist_t* list);

    char* (*get_chat)(dc_context_t* ctx, int64_t chat_id);
    char* (*get_msg)(dc_context_t* ctx, int64_t msg_id);

    int64_t (*send_text_msg)(dc_context_t* ctx, int64_t chat_id, const char* text);
    int64_t (*send_msg)(dc_context_t* ctx, int64_t chat_id, dc_msg_t* msg);

    dc_array_t* (*get_chat_msgs)(dc_context_t* ctx, int64_t chat_id, int flags, int marker);
    int (*get_chat_msgs_cnt)(dc_array_t* array);
    int64_t (*get_chat_msgs_id)(dc_array_t* array, int index);
    void (*array_unref)(dc_array_t* array);

    dc_msg_t* (*msg_new)(dc_context_t* ctx, int view_type);
    void (*msg_unref)(dc_msg_t* msg);
    void (*msg_set_text)(dc_msg_t* msg, const char* text);

    void (*markseen_msgs)(dc_context_t* ctx, dc_array_t* msgs);
    void (*delete_msgs)(dc_context_t* ctx, dc_array_t* msgs);
    void (*delete_chat)(dc_context_t* ctx, int64_t chat_id);

    dc_event_t* (*get_next_event)(dc_context_t* ctx);
    int (*event_get_id)(dc_event_t* event);
    int64_t (*event_get_data1_int)(dc_event_t* event);
    int64_t (*event_get_data2_int)(dc_event_t* event);
    char* (*event_get_data2_str)(dc_event_t* event);
    void (*event_unref)(dc_event_t* event);

    void (*start_io)(dc_context_t* ctx);
    void (*stop_io)(dc_context_t* ctx);

    dc_array_t* (*get_contacts)(dc_context_t* ctx, int flags, const char* query);
    int (*lookup_contact_id_by_addr)(dc_context_t* ctx, const char* addr);
    char* (*get_contact)(dc_context_t* ctx, int64_t contact_id);
    char* (*get_contact_encrinfo)(dc_context_t* ctx, int64_t contact_id);

    int64_t (*create_contact)(dc_context_t* ctx, const char* name, const char* addr);
    int (*block_contact)(dc_context_t* ctx, int64_t contact_id, int block);

    int64_t (*create_group_chat)(dc_context_t* ctx, int verified, const char* name);
    int64_t (*add_contact_to_chat)(dc_context_t* ctx, int64_t chat_id, int64_t contact_id);
    int64_t (*remove_contact_from_chat)(dc_context_t* ctx, int64_t chat_id, int64_t contact_id);
    int (*set_chat_name)(dc_context_t* ctx, int64_t chat_id, const char* name);
    int (*set_chat_profile_image)(dc_context_t* ctx, int64_t chat_id, const char* image);

    char* (*get_fresh_msgs)(dc_context_t* ctx);
    int (*get_fresh_msg_cnt)(dc_context_t* ctx, int64_t chat_id);

    char* (*get_securejoin_qr)(dc_context_t* ctx, int64_t chat_id);
    int (*join_securejoin)(dc_context_t* ctx, const char* qr);

    char* (*imex)(dc_context_t* ctx, int what, const char* param1, const char* param2);
    char* (*initiate_key_transfer)(dc_context_t* ctx);

    int (*get_connectivity)(dc_context_t* ctx);
    int (*get_connectivity_html)(dc_context_t* ctx);

    void (*maybe_network)(dc_context_t* ctx);

    int (*check_qr)(dc_context_t* ctx, const char* qr);
};

bool load_dc_api(DcApi& api);

} // namespace deltachat
} // namespace progressive

#endif // PROGRESSIVE_HAS_DELTACHAT
