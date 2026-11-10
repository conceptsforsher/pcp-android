#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

/* ── TDLib JSON C API stub ──────────────────────────────────────────
   Provides td_json_client_* symbols so pcp_native.so links
   and the Telegram auth/messaging UI works without a real TDLib .so.

   Simulated:
   - Full auth flow (phone → wait_code → send_code → ready)
   - Fake user profile, chat list with 3 sample chats
   - Fake messages in each chat
   - All API calls return valid JSON responses

   To replace with real TDLib:
     1. Delete this file (tdlib_stub.c)
     2. Build libtdjson.so for armeabi-v7a (see TDLib docs)
     3. Place it in jniLibs/ or add to CMake target_link_libraries
   ─────────────────────────────────────────────────────────────────── */

typedef struct {
    int step;         /* 0=phone, 1=code, 2=ready, 3=closed */
    int has_updates;  /* number of pending updates */
    int msg_id;       /* fake message ID counter */
    int chat_id;      /* fake chat ID counter */
    pthread_mutex_t lock;
    char *staged_response;
} td_client_t;

static td_client_t *client_new(void) {
    td_client_t *c = calloc(1, sizeof(*c));
    pthread_mutex_init(&c->lock, NULL);
    c->step = 0;
    c->msg_id = 1;
    c->chat_id = 1000;
    return c;
}

static void client_free(td_client_t *c) {
    if (!c) return;
    pthread_mutex_destroy(&c->lock);
    free(c->staged_response);
    free(c);
}

static void set_response(td_client_t *c, const char *r) {
    pthread_mutex_lock(&c->lock);
    free(c->staged_response);
    c->staged_response = r ? strdup(r) : NULL;
    pthread_mutex_unlock(&c->lock);
}

void *td_json_client_create(void) {
    return client_new();
}

void td_json_client_send(void *client, const char *request) {
    if (!client || !request) return;
    td_client_t *c = (td_client_t *)client;

    /* Minimal JSON decision: look for @type in the request */
    if (strstr(request, "setTdlibParameters")) {
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateWaitPhoneNumber\"}}");
        c->has_updates = 1;
    } else if (strstr(request, "setAuthenticationPhoneNumber")) {
        c->step = 1;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{"
            "\"@type\":\"authorizationStateWaitCode\","
            "\"code_info\":{"
            "\"@type\":\"authenticationCodeInfo\","
            "\"phone_number\":\"+1234567890\","
            "\"type\":{\"@type\":\"authenticationCodeTypeSms\",\"length\":5},"
            "\"next_type\":null,"
            "\"timeout\":60}}}");
    } else if (strstr(request, "checkAuthenticationCode")) {
        c->step = 2;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateReady\"}}");
        /* Also stage a ready user */
        c->has_updates = 2;
    } else if (strstr(request, "checkAuthenticationPassword")) {
        c->step = 2;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateReady\"}}");
    } else if (strstr(request, "logOut")) {
        c->step = 3;
        set_response(c,
            "{\"@type\":\"updateAuthorizationState\","
            "\"authorization_state\":{\"@type\":\"authorizationStateClosed\"}}");
    } else if (strstr(request, "getMe")) {
        /* Return a fake user after auth is ready */
        if (c->step >= 2) {
            set_response(c,
                "{\"@type\":\"user\",\"id\":777000,\"first_name\":\"Telegram\","
                "\"last_name\":\"\",\"username\":\"telegram\",\"phone_number\":\"+1234567890\","
                "\"type\":{\"@type\":\"userTypeRegular\"},"
                "\"profile_photo\":null,\"is_verified\":false,\"is_premium\":false}");
        }
    } else if (strstr(request, "loadChats") || strstr(request, "getChats")) {
        /* Return 3 fake chats */
        set_response(c,
            "{\"@type\":\"chats\",\"total_count\":3,\"chat_ids\":[1001,1002,1003]}");
        c->has_updates = 3;  // will generate 3 updateNewChat events
    } else if (strstr(request, "getChatHistory")) {
        /* Return fake version and my_id */
        if (strstr(request, "\"version\"")) {
            set_response(c,
                "{\"@type\":\"optionValueString\",\"value\":\"1.8.46\"}");
        } else if (strstr(request, "\"my_id\"")) {
            set_response(c,
                "{\"@type\":\"optionValueInteger\",\"value\":777000}");
        } else {
            set_response(c, "{\"@type\":\"ok\"}");
        }
    } else {
        /* Generic ok response */
        set_response(c, "{\"@type\":\"ok\"}");
    }
}

const char *td_json_client_receive(void *client, double timeout) {
    if (!client) return NULL;
    td_client_t *c = (td_client_t *)client;

    pthread_mutex_lock(&c->lock);
    char *r = c->staged_response;
    c->staged_response = NULL;

    /* Generate events after auth is ready */
    if (!r && c->step >= 2 && c->has_updates > 0) {
        int update_num = c->has_updates;
        c->has_updates--;
        if (update_num >= 1 && update_num <= 3) {
            /* updateNewChat for fake chats */
            const char *chats[] = {
                "{\"@type\":\"updateNewChat\",\"chat\":{"
                "\"@type\":\"chat\",\"id\":1001,"
                "\"title\":\"PCP News\","
                "\"type\":{\"@type\":\"chatTypeSupergroup\",\"is_channel\":true},"
                "\"last_message\":{"
                "\"@type\":\"message\",\"id\":1,\"chat_id\":1001,\"date\":1000001,"
                "\"content\":{"
                "\"@type\":\"messageText\","
                "\"text\":{\"@type\":\"formattedText\",\"text\":\"Welcome to PCP!\"}}},"
                "\"unread_count\":1}}",

                "{\"@type\":\"updateNewChat\",\"chat\":{"
                "\"@type\":\"chat\",\"id\":1002,"
                "\"title\":\"Alice (Dev)\","
                "\"type\":{\"@type\":\"chatTypePrivate\",\"user_id\":1001},"
                "\"last_message\":{"
                "\"@type\":\"message\",\"id\":2,\"chat_id\":1002,\"date\":1000002,"
                "\"content\":{"
                "\"@type\":\"messageText\","
                "\"text\":{\"@type\":\"formattedText\",\"text\":\"Hey! How is the Telegram port going?\"}}},"
                "\"unread_count\":3}}",

                "{\"@type\":\"updateNewChat\",\"chat\":{"
                "\"@type\":\"chat\",\"id\":1003,"
                "\"title\":\"Telegram Integration\","
                "\"type\":{\"@type\":\"chatTypeBasicGroup\"},"
                "\"last_message\":{"
                "\"@type\":\"message\",\"id\":3,\"chat_id\":1003,\"date\":1000003,"
                "\"content\":{"
                "\"@type\":\"messageText\","
                "\"text\":{\"@type\":\"formattedText\",\"text\":\"TDLib stub is working! Multi-protocol MVP.\"}}},"
                "\"unread_count\":0}}"
            };
            r = strdup(chats[update_num - 1]);
        }
    }
    pthread_mutex_unlock(&c->lock);
    return r;
}

const char *td_json_client_execute(void *client, const char *request) {
    if (!client || !request) return NULL;
    /* For ping — return ok */
    return "{\"@type\":\"ok\"}";
}

void td_json_client_destroy(void *client) {
    client_free((td_client_t *)client);
}
