#include "progressive/session_timeout.hpp"
#include <sstream>
#include <chrono>
#include <cctype>
#include <algorithm>

namespace progressive {

bool shouldLock(const SessionPolicy& policy, const SessionState& state) {
    if (state.isLocked) return false; // already locked

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    // Failed attempts exceeded
    if (policy.lockMethod != LockMethod::None &&
        state.failedAttempts >= policy.maxFailedPinAttempts) {
        return true;
    }

    // Background lock
    if (policy.lockOnBackground && state.isBackground) {
        return true;
    }

    // Idle timeout
    if (policy.idleTimeoutMinutes > 0 && state.lastActivityMs > 0) {
        int64_t idleMs = now - state.lastActivityMs;
        if (idleMs >= policy.idleTimeoutMinutes * 60 * 1000LL) {
            return true;
        }
    }

    // Max session duration
    if (policy.maxSessionMinutes > 0 && state.sessionStartMs > 0) {
        int64_t sessionMs = now - state.sessionStartMs;
        if (sessionMs >= policy.maxSessionMinutes * 60 * 1000LL) {
            return true;
        }
    }

    return false;
}

std::string getLockReason(const SessionPolicy& policy, const SessionState& state) {
    if (state.isLocked) return "Already locked";

    if (state.failedAttempts >= policy.maxFailedPinAttempts) {
        return "Max PIN attempts exceeded";
    }

    if (policy.lockOnBackground && state.isBackground) {
        return "App moved to background";
    }

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    if (policy.idleTimeoutMinutes > 0 && state.lastActivityMs > 0) {
        int64_t idleMs = now - state.lastActivityMs;
        if (idleMs >= policy.idleTimeoutMinutes * 60 * 1000LL) {
            return "Idle timeout exceeded";
        }
    }

    if (policy.maxSessionMinutes > 0 && state.sessionStartMs > 0) {
        int64_t sessionMs = now - state.sessionStartMs;
        if (sessionMs >= policy.maxSessionMinutes * 60 * 1000LL) {
            return "Max session duration exceeded";
        }
    }

    return "";
}

int secondsUntilLock(const SessionPolicy& policy, const SessionState& state) {
    if (shouldLock(policy, state)) return 0;

    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();

    int minSeconds = INT32_MAX;

    if (policy.idleTimeoutMinutes > 0 && state.lastActivityMs > 0) {
        int64_t idleMs = now - state.lastActivityMs;
        int64_t remainingMs = policy.idleTimeoutMinutes * 60 * 1000LL - idleMs;
        if (remainingMs > 0) {
            minSeconds = std::min(minSeconds, static_cast<int>(remainingMs / 1000));
        }
    }

    if (policy.maxSessionMinutes > 0 && state.sessionStartMs > 0) {
        int64_t sessionMs = now - state.sessionStartMs;
        int64_t remainingMs = policy.maxSessionMinutes * 60 * 1000LL - sessionMs;
        if (remainingMs > 0) {
            minSeconds = std::min(minSeconds, static_cast<int>(remainingMs / 1000));
        }
    }

    return minSeconds == INT32_MAX ? -1 : minSeconds;
}

bool isValidPin(const std::string& pin, int minLen, int maxLen) {
    if (pin.size() < static_cast<size_t>(minLen) || pin.size() > static_cast<size_t>(maxLen))
        return false;
    return std::all_of(pin.begin(), pin.end(), ::isdigit);
}

int attemptsRemaining(const SessionPolicy& policy, const SessionState& state) {
    return std::max(0, policy.maxFailedPinAttempts - state.failedAttempts);
}

bool isGracePeriod(const SessionPolicy& policy, int gracePeriodSeconds) {
    // Grace period: don't lock immediately when going to background
    return true; // Placeholder — actual impl needs timing context
}

std::string formatIdleTime(int64_t idleMs) {
    if (idleMs < 0) return "0s";
    int64_t totalSec = idleMs / 1000;
    int hours = totalSec / 3600;
    int minutes = (totalSec % 3600) / 60;
    int seconds = totalSec % 60;

    std::ostringstream out;
    if (hours > 0) out << hours << "h ";
    if (minutes > 0 || hours > 0) out << minutes << "m ";
    out << seconds << "s";
    return out.str();
}

std::string formatSessionDuration(int64_t startMs) {
    auto now = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::system_clock::now().time_since_epoch()).count();
    return formatIdleTime(now - startMs);
}

std::string hashPin(const std::string& pin, const std::string& salt) {
    // Simple XOR-based hash (placeholder — use SHA-256 in production)
    std::string combined = salt + pin;
    std::string hash;
    hash.reserve(combined.size());
    for (size_t i = 0; i < combined.size(); ++i) {
        char c = combined[i] ^ (salt.empty() ? 0x55 : salt[i % salt.size()]);
        hash += c;
    }
    return hash;
}

bool verifyPin(const std::string& pin, const std::string& storedHash, const std::string& salt) {
    return hashPin(pin, salt) == storedHash;
}

} // namespace progressive
