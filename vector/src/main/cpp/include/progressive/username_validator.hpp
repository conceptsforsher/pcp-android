#ifndef PROGRESSIVE_USERNAME_VALIDATOR_HPP
#define PROGRESSIVE_USERNAME_VALIDATOR_HPP

#include <string>

namespace progressive {

struct UsernameValidation {
    bool valid = false;
    std::string username;
    std::string sanitized;     // suggested fix
    std::string errorMessage;
    int length = 0;
};

// Validate a Matrix username (localpart before @server).
// Rules per spec: 1-255 chars, lowercase alphanumeric + _.-=/
// Must start with letter, must not be "admin", "support", etc.
UsernameValidation validateUsername(const std::string& username);

// Sanitize a username: lowercase, remove invalid chars, trim.
std::string sanitizeUsername(const std::string& raw);

// Check if username is reserved (admin, support, matrix, etc.).
bool isReservedUsername(const std::string& username);

// Generate a suggested username from display name.
// "Alice Johnson" → "alice_johnson", "alice.johnson", "alice-johnson"
std::string suggestUsername(const std::string& displayName, int maxAttempts = 5);

// Check if username is likely a bot (ends with "bot", "bridge", etc.).
bool isBotUsername(const std::string& username);

// Get the display name representation of a username.
// "alice_johnson" → "Alice Johnson"
std::string usernameToDisplayName(const std::string& username);

} // namespace progressive

#endif // PROGRESSIVE_USERNAME_VALIDATOR_HPP
