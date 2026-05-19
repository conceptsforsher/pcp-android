#ifndef PROGRESSIVE_PASSWORD_VALIDATOR_HPP
#define PROGRESSIVE_PASSWORD_VALIDATOR_HPP

#include <string>

namespace progressive {

struct PasswordResult {
    bool valid = false;
    int strength = 0;           // 0-100
    std::string strengthLabel;  // "Weak", "Fair", "Good", "Strong"
    std::string feedback;       // human-readable suggestions
};

// Validate password strength with zxcvbn-like heuristics.
PasswordResult validatePassword(const std::string& password);

// Check minimum requirements: length >= 8, has upper, lower, digit.
bool meetsMinimumRequirements(const std::string& password);

// Compute entropy-based strength score (0-100).
int computePasswordStrength(const std::string& password);

// Get a human-readable strength label.
std::string getStrengthLabel(int strength);

// Check if password has been used before (common passwords list).
bool isCommonPassword(const std::string& password);

// Generate feedback for password improvement.
std::string generatePasswordFeedback(const std::string& password);

// Count character classes present (upper, lower, digit, special).
int countCharClasses(const std::string& password);

// Estimate crack time for a brute-force attack (seconds).
double estimateCrackTimeSeconds(const std::string& password);

// Format crack time as human-readable.
std::string formatCrackTime(double seconds);

} // namespace progressive

#endif // PROGRESSIVE_PASSWORD_VALIDATOR_HPP
