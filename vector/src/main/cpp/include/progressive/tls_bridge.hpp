#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// ==== JNI TLS Bridge — Native → Java SSLSocket ====
//
// Uses Android's javax.net.ssl.SSLSocket for HTTPS.
// Calls from C++ http_client → JNI → Java TLS socket → back.
// Opt-in via Labs: SETTINGS_LABS_NATIVE_HTTP

// Initialize the TLS bridge. Must be called once with JNIEnv.
// Stores JavaVM reference for thread-safe callbacks.
bool tlsBridgeInit(void* jniEnv);

// Execute a TLS request synchronously.
// host: matrix.example.org (no scheme, no path)
// port: 443
// request: full HTTP/1.1 request string (headers + body)
// timeoutMs: connection + read timeout
// Returns: full HTTP response (status line + headers + body)
//
// Thread-safe: attaches JNI to current thread if not already attached.
std::string tlsBridgeRequest(
    const std::string& host,
    int port,
    const std::string& request,
    int timeoutMs = 30000
);

// Check if the TLS bridge is available (JNI initialized).
bool tlsBridgeAvailable();

} // namespace progressive
