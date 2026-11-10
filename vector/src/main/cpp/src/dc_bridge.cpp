#include "pcp/dc_bridge.hpp"

#ifdef PCP_HAS_DELTACHAT

#include <dlfcn.h>
#include <cstring>

namespace pcp {
namespace deltachat {

bool load_dc_api(DcApi& api) {
    void* handle = dlopen("libdeltachat.so", RTLD_NOW);
    if (!handle) {
        handle = dlopen("libdeltachat.so.0", RTLD_NOW);
    }
    if (!handle) {
        return false;
    }

#define DC_LOAD(fn) \
    do { \
        api.fn = reinterpret_cast<decltype(api.fn)>(dlsym(handle, #fn)); \
        if (!api.fn) { dlclose(handle); return false; } \
    } while(0)

    DC_LOAD(dc_context_new);
    DC_LOAD(dc_open);
    DC_LOAD(dc_close);
    DC_LOAD(dc_context_unref);
    DC_LOAD(dc_is_open);
    DC_LOAD(dc_configure);
    DC_LOAD(dc_is_configured);
    DC_LOAD(dc_get_info);
    DC_LOAD(dc_set_config);
    DC_LOAD(dc_get_config);
    DC_LOAD(dc_get_chatlist);
    DC_LOAD(dc_get_chatlist_cnt);
    DC_LOAD(dc_get_chatlist_id);
    DC_LOAD(dc_get_chatlist_summary);
    DC_LOAD(dc_chatlist_unref);
    DC_LOAD(dc_get_chat);
    DC_LOAD(dc_get_msg);
    DC_LOAD(dc_send_text_msg);
    DC_LOAD(dc_send_msg);
    DC_LOAD(dc_get_chat_msgs);
    DC_LOAD(dc_get_chat_msgs_cnt);
    DC_LOAD(dc_get_chat_msgs_id);
    DC_LOAD(dc_array_unref);
    DC_LOAD(dc_msg_new);
    DC_LOAD(dc_msg_unref);
    DC_LOAD(dc_msg_set_text);
    DC_LOAD(dc_markseen_msgs);
    DC_LOAD(dc_delete_msgs);
    DC_LOAD(dc_delete_chat);
    DC_LOAD(dc_get_next_event);
    DC_LOAD(dc_event_get_id);
    DC_LOAD(dc_event_get_data1_int);
    DC_LOAD(dc_event_get_data2_int);
    DC_LOAD(dc_event_get_data2_str);
    DC_LOAD(dc_event_unref);
    DC_LOAD(dc_start_io);
    DC_LOAD(dc_stop_io);
    DC_LOAD(dc_get_contacts);
    DC_LOAD(dc_lookup_contact_id_by_addr);
    DC_LOAD(dc_get_contact);
    DC_LOAD(dc_get_contact_encrinfo);
    DC_LOAD(dc_create_contact);
    DC_LOAD(dc_block_contact);
    DC_LOAD(dc_create_group_chat);
    DC_LOAD(dc_add_contact_to_chat);
    DC_LOAD(dc_remove_contact_from_chat);
    DC_LOAD(dc_set_chat_name);
    DC_LOAD(dc_set_chat_profile_image);
    DC_LOAD(dc_get_fresh_msgs);
    DC_LOAD(dc_get_fresh_msg_cnt);
    DC_LOAD(dc_get_securejoin_qr);
    DC_LOAD(dc_join_securejoin);
    DC_LOAD(dc_imex);
    DC_LOAD(dc_initiate_key_transfer);
    DC_LOAD(dc_get_connectivity);
    DC_LOAD(dc_get_connectivity_html);
    DC_LOAD(dc_maybe_network);
    DC_LOAD(dc_check_qr);

#undef DC_LOAD
    return true;
}

} // namespace deltachat
} // namespace pcp

#endif // PCP_HAS_DELTACHAT
