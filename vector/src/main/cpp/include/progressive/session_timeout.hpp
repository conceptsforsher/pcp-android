#ifndef PROGRESSIVE_SESSION_TIMEOUT_HPP
#define PROGRESSIVE_SESSION_TIMEOUT_HPP

#include <string>
#include <cstdint>

namespace progressive {

enum class LockMethod { None, Pin, Biometric, Password };

struct SessionPolicy {
    LockMethod lockMethod = LockMethod::None;
    int idleTimeoutMinutes = 5;      // lock after N minutes idle
    int maxSessionMinutes = 0;       // 0 = unlimited
    int maxFailedPinAttempts = 5;
    int pinLength = 4;               // 4-8
    bool lockOnBackground = true;    // lock when app goes to background
    bool requireBioOnFg = false;     // require biometric on foreground
};

struct SessionState {
    int64_t lastActivityMs = 0;      // last user interaction
    int64_t sessionStartMs = 0;      // when session started
    int failedAttempts = 0;
    bool isLocked = false;
    bool isBackground = false;
    std::string lockReason;
};

// Check if session should be locked based on policy and state.
bool shouldLock(const SessionPolicy& policy, const SessionState& state);

// Get the reason for locking (empty if should not lock).
std::string getLockReason(const SessionPolicy& policy, const SessionState& state);

// Compute seconds until next lock (0 = should lock now).
int secondsUntilLock(const SessionPolicy& policy, const SessionState& state);

// Validate a PIN code (length, digits only).
bool isValidPin(const std::string& pin, int minLen = 4, int maxLen = 8);

// Rate-limit feedback: how many attempts remaining.
int attemptsRemaining(const SessionPolicy& policy, const SessionState& state);

// Check if a grace period is active (e.g., 30s after background).
bool isGracePeriod(const SessionPolicy& policy, int gracePeriodSeconds = 30);

// Format idle time as human-readable: "2m 30s", "1h 5m".
std::string formatIdleTime(int64_t idleMs);

// Format session duration as human-readable.
std::string formatSessionDuration(int64_t startMs);

// Compute hash of a PIN for secure comparison (simple SHA-256 placeholder).
std::string hashPin(const std::string& pin, const std::string& salt = "");

// Verify PIN against stored hash.
bool verifyPin(const std::string& pin, const std::string& storedHash, const std::string& salt = "");

} // namespace progressive

#endif // PROGRESSIVE_SESSION_TIMEOUT_HPP
