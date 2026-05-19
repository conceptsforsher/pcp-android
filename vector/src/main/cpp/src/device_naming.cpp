#include "progressive/device_naming.hpp"
#include <sstream>
#include <cctype>
#include <algorithm>

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

} // namespace progressive
