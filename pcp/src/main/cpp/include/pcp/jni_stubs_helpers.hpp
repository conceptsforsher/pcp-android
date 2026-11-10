/**
 * Shared helpers for jni_stubs_part*.cpp files.
 * Include this header instead of duplicating helper functions.
 */
#pragma once
#include <jni.h>
#include <string>
#include <sstream>
#include "pcp/string_utils.hpp"

static std::string j2s(JNIEnv* env, jstring s) {
    if (!s) return "";
    const char* c = env->GetStringUTFChars(s, nullptr);
    std::string r(c);
    env->ReleaseStringUTFChars(s, c);
    return r;
}

static std::string jsonObj(const std::string& pairs) { return "{" + pairs + "}"; }

static std::string jsonPair(const std::string& k, const std::string& v) {
    return "\"" + pcp::escapeJson(k) + "\":\"" + pcp::escapeJson(v) + "\"";
}

static std::string jsonPair(const std::string& k, bool v) {
    return "\"" + pcp::escapeJson(k) + "\":" + (v ? "true" : "false");
}

static std::string jsonPair(const std::string& k, int64_t v) {
    return "\"" + pcp::escapeJson(k) + "\":" + std::to_string(v);
}

static std::string jsonArr(const std::string& items) { return "[" + items + "]"; }
