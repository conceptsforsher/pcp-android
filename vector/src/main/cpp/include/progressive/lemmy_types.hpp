#pragma once

#include <string>
#include <cstdint>

namespace progressive {

// ==== Lemmy API Types ====
//
// All API functions return raw JSON strings (std::string).
// Parsing is left to Kotlin/Java side via JNI or to the
// minimal json_parser helpers.
//
// Lemmy API version: v3
// Base path: /api/v3/

// Lemmy auth token (JWT)
struct LemmyAuth {
    std::string jwt;        // JWT token from login response
    int64_t userId = 0;     // local_user_id from login
    bool valid = false;
};

// Lemmy instance info
struct LemmyInstance {
    std::string url;        // e.g. "https://lemmy.ml"
    std::string version;    // instance software version
};

// Login credentials
struct LemmyLogin {
    std::string username;       // or email
    std::string password;
    std::string totp2faToken;   // optional TOTP token
};

// ==== Lemmy API Error ====

struct LemmyError {
    std::string error;          // Human-readable error from response body
    int httpStatus = 0;         // HTTP status code
    bool valid = false;

    bool isOk() const { return !valid; }
};

// Extract error from JSON response (Lemmy error format: {"error": "..."})
inline LemmyError parseLemmyError(const std::string& jsonBody, int httpStatus) {
    if (httpStatus >= 200 && httpStatus < 300) return {};
    LemmyError e;
    e.httpStatus = httpStatus;
    e.valid = true;
    e.error = jsonBody;
    return e;
}

} // namespace progressive
