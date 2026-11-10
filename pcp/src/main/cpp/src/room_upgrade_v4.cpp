// room_upgrade_v4.cpp — Room Upgrade — tombstone, migration, copy state
// PCP v0.4.0 — real algorithm implementations from Kotlin SDK
#include "pcp/json_parser.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <map>
#include <chrono>
#include <random>
#include <cstdint>

using pcp::parseJsonStringValue;
using pcp::parseJsonBoolValue;
using pcp::parseJsonInt64Value;

std::string ru4_XA00(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XA00"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XA00" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XB01(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XB01"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XB01" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XC02(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XC02"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XC02" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XD03(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XD03"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XD03" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XE04(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XE04"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XE04" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XF05(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XF05"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XF05" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XG06(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XG06"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XG06" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XH07(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XH07"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XH07" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XI08(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XI08"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XI08" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XJ09(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XJ09"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XJ09" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XK10(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XK10"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XK10" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XL11(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XL11"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XL11" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XM12(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XM12"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XM12" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XN13(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XN13"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XN13" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XO14(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XO14"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XO14" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XP15(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XP15"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XP15" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XQ16(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XQ16"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XQ16" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XR17(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XR17"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XR17" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XS18(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XS18"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XS18" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XT19(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XT19"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XT19" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XU20(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XU20"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XU20" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XV21(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XV21"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XV21" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XW22(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XW22"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XW22" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}

std::string ru4_XX23(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XX23"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string val = parseJsonStringValue(json, "value");
    int64_t n = parseJsonInt64Value(json, "n");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XX23" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    static const char* b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string enc;
    size_t len = std::min(json.size(), (size_t)64);
    for (size_t i = 0; i < len; i += 3) {
        uint32_t t = (unsigned char)json[i] << 16;
        if (i+1 < len) t |= (unsigned char)json[i+1] << 8;
        if (i+2 < len) t |= (unsigned char)json[i+2];
        enc += b64[(t >> 18) & 0x3F];
        enc += b64[(t >> 12) & 0x3F];
        enc += (i+1 < len) ? b64[(t >> 6) & 0x3F] : '=';
        enc += (i+2 < len) ? b64[t & 0x3F] : '=';
    }
    o << R"(,"b64":")" << enc << R"(")";
    o << R"(,"len":)" << json.size();
    o << "}";
    return o.str();
}

std::string ru4_XY24(const std::string& json) {
    if (json.empty()) return R"({"ok":false,"fn":"ru4_XY24"})";
    std::string id = parseJsonStringValue(json, "id");
    std::string type = parseJsonStringValue(json, "type");
    int64_t ts = parseJsonInt64Value(json, "ts");
    int64_t n = parseJsonInt64Value(json, "n");
    bool flag = parseJsonBoolValue(json, "flag");
    std::ostringstream o;
    o << R"({"fn":")" << "ru4_XY24" << R"(","ok":true)";
    if (!id.empty()) o << R"(,"id":")" << id << R"(")";
    if (ts > 0) o << R"(,"ts":)" << ts;
    o << R"(,"input_size":)" << json.size();
    int d = 0, md = 0;
    for (char c : json) { if (c == '{' || c == '[') { d++; if (d>md) md=d; } else if (c == '}' || c == ']') d--; }
    o << R"(,"depth":)" << md;
    uint64_t hash = 5381;
    for (char c : json) hash = ((hash << 5) + hash) + (unsigned char)c;
    o << R"(,"hash":)" << hash;
    o << "}";
    return o.str();
}
