#pragma once
#define ANDROID_LOG_INFO 4
#define ANDROID_LOG_WARN 5
#define ANDROID_LOG_ERROR 6
#define ANDROID_LOG_DEBUG 3
#define ANDROID_LOG_VERBOSE 2
#define LOG_TAG "PCP"
#define LOGI(...) ((void)0)
#define LOGW(...) ((void)0)
#define LOGE(...) ((void)0)
#define LOGD(...) ((void)0)
#define LOGV(...) ((void)0)
#ifdef __cplusplus
extern "C" {
#endif
static inline int __android_log_print(int prio, const char* tag, const char* fmt, ...) { (void)prio; (void)tag; (void)fmt; return 0; }
static inline int __android_log_write(int prio, const char* tag, const char* text) { (void)prio; (void)tag; (void)text; return 0; }
#ifdef __cplusplus
}
#endif
