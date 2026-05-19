#include "progressive/masquerade.hpp"
#include <sstream>
#include <regex>
#include <cctype>
#include <algorithm>

namespace progressive {

bool isValidMasqueradeName(const std::string& name) {
    if (name.empty() || name.size() > 30) return false;
    // Must start with letter or digit
    if (!std::isalnum(static_cast<unsigned char>(name[0]))) return false;
    // Allow letters, digits, spaces, hyphens
    for (char c : name) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != ' ' && c != '-') {
            return false;
        }
    }
    return true;
}

bool isValidIconAlias(const std::string& alias) {
    if (alias.empty() || alias.size() > 20) return false;
    for (char c : alias) {
        if (!std::isalnum(static_cast<unsigned char>(c)) && c != '_') {
            return false;
        }
    }
    return true;
}

std::string buildMasqueradeAlias(const std::string& baseAlias, const std::string& iconName) {
    // Format: basePackage.Masquerade<IconName>
    auto dot = baseAlias.rfind('.');
    if (dot == std::string::npos) return baseAlias + ".Masquerade" + iconName;

    auto pkg = baseAlias.substr(0, dot);
    return pkg + ".Masquerade" + iconName;
}

std::string getSuggestedNames() {
    // Return comma-separated suggested app names
    return "Calculator,Notes,Clock,Calendar,Weather,Files,Settings,Camera,"
           "Gallery,Music,Podcasts,Recorder,Voice Memos,Utilities,"
           "Scheduler,Alarm,Reminders,Planner,Organizer";
}

} // namespace progressive
