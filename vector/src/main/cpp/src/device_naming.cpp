#include "progressive/device_naming.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>
#include <chrono>

namespace progressive {

// ---- User Agent Builder ----
// Original Kotlin (ComputeUserAgentUseCase.kt:execute):
//   fun execute(flavorDescription: String): String {
//       val appPackageName = context.applicationContext.packageName
//       val pm = context.packageManager
//       val appName = tryOrNull { pm.getApplicationLabel(...) }?.takeIf {
//           it.matches("\\A\\p{ASCII}*\\z".toRegex())
//       } ?: appPackageName
//       val appVersion = tryOrNull { pm.getPackageInfoCompat(...).versionName } ?: FALLBACK_APP_VERSION
//       val deviceManufacturer = Build.MANUFACTURER
//       val deviceModel = Build.MODEL
//       val androidVersion = Build.VERSION.RELEASE
//       val deviceBuildId = Build.DISPLAY
//       val matrixSdkVersion = BuildConfig.SDK_VERSION
//
//       return buildString {
//           append(appName)
//           append("/")
//           append(appVersion)
//           append(" (")
//           append(deviceManufacturer)
//           append(" ")
//           append(deviceModel)
//           append("; ")
//           append("Android ")
//           append(androidVersion)
//           append("; ")
//           append(deviceBuildId)
//           append("; ")
//           append("Flavour ")
//           append(flavorDescription)
//           append("; ")
//           append("MatrixAndroidSdk2 ")
//           append(matrixSdkVersion)
//           append(")")
//       }
//   }

std::string buildUserAgent(
    const std::string& appName,
    const std::string& appVersion,
    const std::string& manufacturer,
    const std::string& model,
    const std::string& androidVersion,
    const std::string& buildId,
    const std::string& flavorDescription,
    const std::string& sdkVersion
) {
    // Sanitize all components to remove control characters
    auto name = sanitizeUserAgentComponent(appName);
    auto version = sanitizeUserAgentComponent(appVersion);
    auto mfr = sanitizeUserAgentComponent(manufacturer);
    auto mdl = sanitizeUserAgentComponent(model);
    auto av = sanitizeUserAgentComponent(androidVersion);
    auto bid = sanitizeUserAgentComponent(buildId);
    auto flav = sanitizeUserAgentComponent(flavorDescription);
    auto sdk = sanitizeUserAgentComponent(sdkVersion);

    // Fall back to package name if appName contains non-ASCII
    // Original Kotlin: it.matches("\\A\\p{ASCII}*\\z".toRegex()) ?: appPackageName
    if (!isAsciiOnly(name)) {
        name = "im.vector.app"; // default package name
    }

    // Format: "Element/1.5.0 (Xiaomi Mi 9T; Android 11; RKQ1.200826.002; Flavour GooglePlay; MatrixAndroidSdk2 1.5.0)"
    std::ostringstream ua;
    ua << name << "/" << version;
    ua << " (";
    ua << mfr << " " << mdl << "; ";
    ua << "Android " << av << "; ";
    ua << bid << "; ";
    ua << "Flavour " << flav << "; ";
    ua << "MatrixAndroidSdk2 " << sdk;
    ua << ")";
    return ua.str();
}

// ---- Device Display Name ----
// Original String resource: <string name="login_default_session_public_name">${app_name} Android</string>
// Used in: FtueAuthLoginFragment.kt:151
//   val initialDeviceName = getString(CommonStrings.login_default_session_public_name)
//
// Combined with model for descriptive session name: "Element Android (Pixel 7)"

std::string buildDeviceDisplayName(const std::string& appName, const std::string& deviceModel) {
    std::ostringstream out;
    out << appName << " Android";
    if (!deviceModel.empty()) {
        out << " (" << sanitizeUserAgentComponent(deviceModel) << ")";
    }
    return out.str();
}

// ---- ASCII Check ----
// Original Kotlin: it.matches("\\A\\p{ASCII}*\\z".toRegex())
// Returns true if all characters are in the ASCII range (0-127).

bool isAsciiOnly(const std::string& input) {
    for (unsigned char c : input) {
        if (c > 127) return false;
    }
    return true;
}

// ---- Short Device Name ----
// Removes manufacturer prefix if model already contains it.
// "Xiaomi Mi 9T" → "Mi 9T"
// "Google Pixel 7" → "Pixel 7"
// "samsung SM-G991B" → "SM-G991B"
// "Fairphone FP4" → "FP4"

std::string shortDeviceName(const std::string& manufacturer, const std::string& model) {
    std::string mfrLower = manufacturer;
    std::string mdlLower = model;
    for (char& c : mfrLower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    for (char& c : mdlLower) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));

    // If model already starts with manufacturer name, remove it
    if (mdlLower.find(mfrLower) == 0) {
        size_t skip = mfrLower.size();
        // Skip any space after manufacturer
        if (skip < model.size() && model[skip] == ' ') skip++;
        return model.substr(skip);
    }

    return model;
}

// ---- SDK Version Formatter ----
// Original Kotlin: "MatrixAndroidSdk2 ${matrixSdkVersion}"

std::string formatSdkVersion(const std::string& sdkVersion) {
    return "MatrixAndroidSdk2 " + sdkVersion;
}

// ---- Sanitizer ----
// Remove semicolons, parentheses, and control characters from user-agent components.
// These characters would break the user-agent format.

std::string sanitizeUserAgentComponent(const std::string& input) {
    std::string result;
    for (char c : input) {
        // Allow printable ASCII + space (but not ; ( ) \)
        if (c == ';' || c == '(' || c == ')' || c == '\\' || c == '\n' || c == '\r' || c == '\t') {
            result += ' ';
        } else if (static_cast<unsigned char>(c) >= 32 && static_cast<unsigned char>(c) <= 126) {
            result += c;
        }
        // Skip other control characters
    }

    // Trim
    while (!result.empty() && result.front() == ' ') result.erase(0, 1);
    while (!result.empty() && result.back() == ' ') result.pop_back();

    return result;
}

// ========================================================================
// Expanded: UserAgentInfo, parseUserAgent, formatUserAgent, guessDeviceType,
//           deviceIconName, isDeviceActive
// Ported from Kotlin user agent logic
// ========================================================================

// ---- Parse User Agent ----
// Original Kotlin: Reverse of ComputeUserAgentUseCase format
// Input: "Element/1.5.0 (Xiaomi Mi 9T; Android 11; RKQ1.200826.002; Flavour GooglePlay; MatrixAndroidSdk2 1.5.0)"
// Output: structured UserAgentInfo

UserAgentInfo parseUserAgent(const std::string& userAgent) {
    UserAgentInfo info;

    if (userAgent.empty()) return info;

    // Split into app info (before '(') and device info (inside '(' ... ')')
    auto parenOpen = userAgent.find('(');
    auto parenClose = userAgent.rfind(')');

    // Parse app name / version: "AppName/Version"
    std::string appPart;
    if (parenOpen != std::string::npos) {
        appPart = userAgent.substr(0, parenOpen);
    } else {
        appPart = userAgent;
    }

    // Remove trailing whitespace from appPart
    while (!appPart.empty() && appPart.back() == ' ') appPart.pop_back();

    auto slashPos = appPart.find('/');
    if (slashPos != std::string::npos) {
        info.appName = appPart.substr(0, slashPos);
        info.appVersion = appPart.substr(slashPos + 1);
    } else {
        info.appName = appPart;
    }

    // Parse device info inside parentheses
    if (parenOpen != std::string::npos && parenClose != std::string::npos && parenClose > parenOpen) {
        std::string devicePart = userAgent.substr(parenOpen + 1, parenClose - parenOpen - 1);

        // Split by "; " (semicolon-space)
        std::vector<std::string> segments;
        size_t start = 0;
        size_t end;
        while ((end = devicePart.find("; ", start)) != std::string::npos) {
            segments.push_back(devicePart.substr(start, end - start));
            start = end + 2;
        }
        if (start < devicePart.size()) {
            segments.push_back(devicePart.substr(start));
        }

        // Segment 0: "Manufacturer Model" or "Manufacturer Model (OS Version)"
        // Segment 1 onwards: "OS Version", "BuildId", "Flavour X", "MatrixAndroidSdk2 Y"
        for (size_t i = 0; i < segments.size(); ++i) {
            const auto& seg = segments[i];
            if (i == 0) {
                // First segment: manufacturer and model
                auto spacePos = seg.find(' ');
                if (spacePos != std::string::npos) {
                    info.manufacturer = seg.substr(0, spacePos);
                    info.model = seg.substr(spacePos + 1);
                } else {
                    info.manufacturer = seg;
                }
            } else if (seg.find("Android ") == 0) {
                info.osName = "Android";
                info.osVersion = seg.substr(8); // after "Android "
            } else if (seg.find("iOS ") == 0) {
                info.osName = "iOS";
                info.osVersion = seg.substr(4);
            } else if (seg.find("Linux ") == 0) {
                info.osName = "Linux";
                info.osVersion = seg.substr(6);
            } else if (seg.find("Windows ") == 0) {
                info.osName = "Windows";
                info.osVersion = seg.substr(8);
            } else if (seg.find("macOS ") == 0) {
                info.osName = "macOS";
                info.osVersion = seg.substr(6);
            } else if (seg.find("Flavour ") == 0) {
                info.flavor = seg.substr(8);
            } else if (seg.find("MatrixAndroidSdk2 ") == 0) {
                info.sdkVersion = seg.substr(18);
            } else if (info.osName.empty()) {
                // Fallback: assume it's the build ID
                info.buildId = seg;
            }
        }
    }

    return info;
}

// ---- Format User Agent for Display ----
// Original Kotlin: UI logic in device list / settings
// "Element/1.5.0 (Xiaomi Mi 9T; Android 11; ...)" → "Element Android 1.5.0"
// With device info if available

std::string formatUserAgent(const std::string& userAgent) {
    if (userAgent.empty()) return "Unknown device";

    auto info = parseUserAgent(userAgent);
    if (info.appName.empty() && info.manufacturer.empty()) return userAgent;

    std::ostringstream out;

    // App name first
    if (!info.appName.empty()) {
        out << info.appName;
        // Include OS name if available and not already part of app name
        if (!info.osName.empty()) {
            out << " " << info.osName;
        }
        if (!info.appVersion.empty()) {
            out << " " << info.appVersion;
        }
    }

    // Device info
    if (!info.manufacturer.empty() || !info.model.empty()) {
        if (!info.appName.empty()) out << " on ";
        if (!info.manufacturer.empty()) out << info.manufacturer;
        if (!info.model.empty()) {
            if (!info.manufacturer.empty()) out << " ";
            out << info.model;
        }
    }

    return out.str().empty() ? userAgent : out.str();
}

// ---- Guess Device Type from User Agent ----
// Original Kotlin: UI detection logic (spread across device list fragments)
// Detects mobile, desktop, web based on user agent patterns

std::string guessDeviceType(const std::string& userAgent) {
    if (userAgent.empty()) return "Unknown";

    auto lower = userAgent;
    std::transform(lower.begin(), lower.end(), lower.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });

    // Mobile patterns
    if (lower.find("android") != std::string::npos) return "Mobile";
    if (lower.find("iphone") != std::string::npos) return "Mobile";
    if (lower.find("ipad") != std::string::npos) return "Mobile";
    if (lower.find("ios") != std::string::npos) return "Mobile";
    if (lower.find("mobile") != std::string::npos) return "Mobile";
    if (lower.find("fdroid") != std::string::npos) return "Mobile";
    if (lower.find("riot") != std::string::npos) return "Mobile";
    if (lower.find("element") != std::string::npos) {
        // Element on Android or iOS is mobile
        if (lower.find("android") != std::string::npos || lower.find("ios") != std::string::npos) return "Mobile";
        // Element with no platform hint — check for electron (desktop) or web
        if (lower.find("electron") != std::string::npos) return "Desktop";
        if (lower.find("web") != std::string::npos) return "Web";
        // Default mobile if just "Element" (most common on mobile)
        return "Mobile";
    }

    // Desktop patterns
    if (lower.find("electron") != std::string::npos) return "Desktop";
    if (lower.find("desktop") != std::string::npos) return "Desktop";
    if (lower.find("linux") != std::string::npos && lower.find("electron") == std::string::npos) {
        // Non-electron Linux could be desktop or unknown — assume Desktop
        if (lower.find("android") == std::string::npos && lower.find("web") == std::string::npos) return "Desktop";
    }
    if (lower.find("windows") != std::string::npos) return "Desktop";
    if (lower.find("mac") != std::string::npos && lower.find("os x") != std::string::npos) return "Desktop";

    // Web patterns
    if (lower.find("mozilla") != std::string::npos && lower.find("electron") == std::string::npos) return "Web";
    if (lower.find("chrome") != std::string::npos && lower.find("electron") == std::string::npos) return "Web";
    if (lower.find("safari") != std::string::npos) return "Web";
    if (lower.find("firefox") != std::string::npos) return "Web";
    if (lower.find("edge") != std::string::npos) return "Web";
    if (lower.find("web") != std::string::npos) return "Web";
    if (lower.find("browser") != std::string::npos) return "Web";

    // Parse structured format and check OS
    auto info = parseUserAgent(userAgent);
    if (!info.osName.empty()) {
        auto osLower = info.osName;
        std::transform(osLower.begin(), osLower.end(), osLower.begin(),
                       [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
        if (osLower == "android" || osLower == "ios") return "Mobile";
        if (osLower == "linux" || osLower == "windows" || osLower == "macos") return "Desktop";
    }

    return "Unknown";
}

// ---- Device Icon Name Mapping ----
// Original Kotlin: icon resources in device list UI
// Maps device type to Android drawable resource name

std::string deviceIconName(const std::string& deviceType) {
    if (deviceType == "Mobile") return "ic_device_mobile";
    if (deviceType == "Desktop") return "ic_device_desktop";
    if (deviceType == "Web") return "ic_device_web";
    return "ic_device_unknown";
}

// ---- Is Device Active ----
// Original Kotlin: UI active/inactive status (7 day threshold)
// Active = seen within last 7 days (unlike inactive check which uses 90 days)

bool isDeviceActive(int64_t lastSeenMs, int64_t nowMs) {
    if (lastSeenMs <= 0) return false;
    if (nowMs <= 0) {
        nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }
    int64_t diffMs = nowMs - lastSeenMs;
    // Active if seen within the last 7 days
    return diffMs <= 7LL * 24 * 3600 * 1000;
}

} // namespace progressive
