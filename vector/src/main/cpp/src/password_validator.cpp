#include "progressive/password_validator.hpp"
#include <sstream>
#include <cmath>
#include <cctype>
#include <algorithm>
#include <unordered_set>

namespace progressive {

PasswordResult validatePassword(const std::string& password) {
    PasswordResult result;

    if (password.empty()) {
        result.feedback = "Password cannot be empty.";
        return result;
    }

    if (!meetsMinimumRequirements(password)) {
        result.strength = 10;
        result.strengthLabel = "Weak";
        result.feedback = generatePasswordFeedback(password);
        return result;
    }

    if (isCommonPassword(password)) {
        result.strength = 5;
        result.strengthLabel = "Weak";
        result.feedback = "This password is too common.";
        return result;
    }

    result.strength = computePasswordStrength(password);
    result.strengthLabel = getStrengthLabel(result.strength);
    result.valid = result.strength >= 30;
    result.feedback = result.valid ? "" : generatePasswordFeedback(password);

    return result;
}

bool meetsMinimumRequirements(const std::string& password) {
    if (password.size() < 8) return false;

    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
    }
    return hasUpper && hasLower && hasDigit;
}

int computePasswordStrength(const std::string& password) {
    int len = static_cast<int>(password.size());
    int classes = countCharClasses(password);

    // Base score from length
    double score = len * 4.0;

    // Bonus for character classes
    score += (classes - 1) * 10.0;

    // Bonus for length > 12
    if (len > 12) score += (len - 12) * 3.0;

    // Penalty for repetition
    int repeats = 0;
    for (size_t i = 1; i < password.size(); ++i) {
        if (password[i] == password[i - 1]) repeats++;
    }
    score -= repeats * 2.0;

    // Penalty for sequential chars
    int sequential = 0;
    for (size_t i = 2; i < password.size(); ++i) {
        if (password[i] - password[i - 1] == 1 &&
            password[i - 1] - password[i - 2] == 1) {
            sequential++;
        }
    }
    score -= sequential * 5.0;

    return std::max(0, std::min(100, static_cast<int>(score)));
}

std::string getStrengthLabel(int strength) {
    if (strength >= 80) return "Strong";
    if (strength >= 60) return "Good";
    if (strength >= 30) return "Fair";
    return "Weak";
}

bool isCommonPassword(const std::string& password) {
    static const std::unordered_set<std::string> common = {
        "password", "12345678", "qwerty123", "admin123", "letmein123",
        "welcome1", "monkey123", "dragon123", "password1", "123456789",
        "qwertyuiop", "abc123456", "password123", "iloveyou1", "batman123"
    };
    auto lower = password;
    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
    return common.find(lower) != common.end();
}

std::string generatePasswordFeedback(const std::string& password) {
    std::ostringstream feedback;

    if (password.size() < 8) {
        feedback << "Use at least 8 characters. ";
    }

    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        if (std::ispunct(static_cast<unsigned char>(c))) hasSpecial = true;
    }

    if (!hasUpper) feedback << "Add uppercase letters. ";
    if (!hasLower) feedback << "Add lowercase letters. ";
    if (!hasDigit) feedback << "Add numbers. ";
    if (!hasSpecial) feedback << "Add special characters. ";

    if (password.size() < 12) {
        feedback << "Longer passwords are stronger. ";
    }

    return feedback.str();
}

int countCharClasses(const std::string& password) {
    bool hasUpper = false, hasLower = false, hasDigit = false, hasSpecial = false;
    for (char c : password) {
        if (std::isupper(static_cast<unsigned char>(c))) hasUpper = true;
        else if (std::islower(static_cast<unsigned char>(c))) hasLower = true;
        else if (std::isdigit(static_cast<unsigned char>(c))) hasDigit = true;
        else hasSpecial = true;
    }
    return (hasUpper ? 1 : 0) + (hasLower ? 1 : 0) +
           (hasDigit ? 1 : 0) + (hasSpecial ? 1 : 0);
}

double estimateCrackTimeSeconds(const std::string& password) {
    int classes = countCharClasses(password);
    int poolSize = 0;
    if (classes >= 1) poolSize += 26;  // lowercase
    if (classes >= 2) poolSize += 26;  // uppercase
    if (classes >= 3) poolSize += 10;  // digits
    if (classes >= 4) poolSize += 32;  // specials
    if (poolSize == 0) poolSize = 10;

    // Brute force: poolSize ^ length / guesses_per_second
    double combinations = std::pow(static_cast<double>(poolSize), password.size());
    double gps = 1e9; // 1 billion guesses per second
    return combinations / gps;
}

std::string formatCrackTime(double seconds) {
    if (seconds < 60) return "< 1 minute";
    if (seconds < 3600) return std::to_string(static_cast<int>(seconds / 60)) + " minutes";
    if (seconds < 86400) return std::to_string(static_cast<int>(seconds / 3600)) + " hours";
    if (seconds < 31536000) return std::to_string(static_cast<int>(seconds / 86400)) + " days";
    if (seconds < 31536000 * 100) return std::to_string(static_cast<int>(seconds / 31536000)) + " years";
    return "centuries";
}

} // namespace progressive
