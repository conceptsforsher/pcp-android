#ifndef PROGRESSIVE_MASQUERADE_HPP
#define PROGRESSIVE_MASQUERADE_HPP

#include <string>

namespace progressive {

struct MasqueradeConfig {
    bool enabled = false;
    std::string appName;       // e.g. "Calculator" or "Notes"
    std::string iconAlias;     // e.g. "calculator" or "notes"
    bool useDefaultIcon = true; // use system-provided mask icon
};

// Validate app name: 1-30 chars, no offensive words
bool isValidMasqueradeName(const std::string& name);

// Get the Android component alias for masquerading
// e.g. "im.vector.app.features.MainActivity" → "im.vector.app.features.MasqueradeCalculator"
std::string buildMasqueradeAlias(const std::string& baseAlias, const std::string& iconName);

// Generate a list of suggested masquerade names
// (Calculator, Notes, Settings, Clock, etc.)
std::string getSuggestedNames();

// Validate icon alias name (alphanumeric, 1-20 chars)
bool isValidIconAlias(const std::string& alias);

} // namespace progressive

#endif // PROGRESSIVE_MASQUERADE_HPP
